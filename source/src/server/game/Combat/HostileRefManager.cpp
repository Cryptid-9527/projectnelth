/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "HostileRefManager.h"
#include "ThreatManager.h"
#include "Unit.h"
#include "DBCStructure.h"
#include "SpellMgr.h"
#include "SpellInfo.h"

HostileRefManager::~HostileRefManager()
{
    deleteReferences();
}

//=================================================
// send threat to all my hateres for the victim
// The victim is hated than by them as well
// use for buffs and healing threat functionality

void HostileRefManager::threatAssist(Unit* victim, float baseThreat, SpellInfo const* threatSpell)
{
    if (!victim)
        return;

    HostileReference* ref = getFirst();
    float threat = ThreatCalcHelper::calcThreat(victim, iOwner, baseThreat, (threatSpell ? threatSpell->GetSchoolMask() : SPELL_SCHOOL_MASK_NORMAL), threatSpell);
    threat /= getSize();
    while (ref)
    {
        if (auto s = ref->getSource())
        if (ThreatCalcHelper::isValidProcess(victim, s->getOwner(), threatSpell))
            s->doAddThreat(victim, threat);

        ref = ref->next();
    }
}

//=================================================

void HostileRefManager::addTempThreat(float threat, bool apply)
{
    HostileReference* ref = getFirst();

    while (ref)
    {
        if (apply)
        {
            if (ref->getTempThreatModifier() == 0.0f)
                ref->addTempThreat(threat);
        }
        else
            ref->resetTempThreat();

        ref = ref->next();
    }
}

//=================================================

void HostileRefManager::addThreatPercent(int32 percent)
{
    HostileReference* ref = getFirst();
    while (ref)
    {
        ref->addThreatPercent(percent);
        ref = ref->next();
    }
}

//=================================================
// The online / offline status is given to the method. The calculation has to be done before

void HostileRefManager::setOnlineOfflineState(bool isOnline)
{
    HostileReference* ref = getFirst();
    while (ref)
    {
        ref->setOnlineOfflineState(isOnline);
        ref = ref->next();
    }
}

//=================================================
// The online / offline status is calculated and set

void HostileRefManager::updateThreatTables()
{
    HostileReference* ref = getFirst();
    while (ref)
    {
        ref->updateOnlineStatus();
        ref = ref->next();
    }
}

//=================================================
// The references are not needed anymore
// tell the source to remove them from the list and free the mem

void HostileRefManager::deleteReferences()
{
    HostileReference* ref = getFirst();
    while (ref)
    {
        HostileReference* nextRef = ref->next();
        ref->removeReference();
        delete ref;
        ref = nextRef;
    }
}

//=================================================
// delete one reference, defined by faction

void HostileRefManager::deleteNewlyInvalidReferences()
{
    HostileReference* ref = getFirst();
    while (ref)
    {
        HostileReference* nextRef = ref->next();
        if (auto s = ref->getSource())
            if (auto their_o = s->getOwner())
                if (auto my_o = getOwner())
                    if (!my_o->IsHostileTo(their_o))
                    {
                        SeverFrom(ref, their_o);
                        break;
                    }
        ref = nextRef;
    }
}

void HostileRefManager::SeverFrom(HostileReference* ref, Unit* their_o)
{
    if (!ref)
        return;
    if (!their_o)
        return;

    if (auto my_o = getOwner())
    {
        deleteReference(their_o);
        their_o->getHostileRefManager().deleteReference(my_o);

        if (their_o->HasUnitState(UNIT_STATE_CASTING) || my_o->HasUnitState(UNIT_STATE_CASTING))
        {
            their_o->_removeAttacker(my_o);
            my_o->_removeAttacker(their_o);
        }


        my_o->getThreatManager().setDirty(true);
        their_o->getThreatManager().setDirty(true);
    }
}

void HostileRefManager::deleteFarAwayReferencesOnAreaUpdate()
{
    HostileReference* ref = getFirst();
    if (auto my_o = getOwner())
    if (auto m = my_o->GetMap())
    while (ref)
    {
        HostileReference* nextRef = ref->next();
        if (auto s = ref->getSource())
            if (auto their_o = s->getOwner())
                if (auto c = their_o->ToCreature())
                        if (my_o->GetDistance(*their_o) > (m->Instanceable() ? 100.f : MAX_AGGRO_RADIUS))
                        {
                            SeverFrom(ref, their_o);
                            break;
                        }
        ref = nextRef;
    }
}

void HostileRefManager::deleteReferencesForFaction(uint32 faction)
{
    HostileReference* ref = getFirst();
    while (ref)
    {
        HostileReference* nextRef = ref->next();
        if (auto s = ref->getSource())
            if (auto their_o = s->getOwner())
                if (auto my_o = getOwner())
                if (auto templ = their_o->getFactionTemplateEntry())
        if (templ->faction == faction)
        {
            ref->removeReference();
            delete ref;
        }
        ref = nextRef;
    }
}

//=================================================
// delete one reference, defined by Unit

void HostileRefManager::deleteReference(Unit* creature)
{
    HostileReference* ref = getFirst();
    while (ref)
    {
        HostileReference* nextRef = ref->next();
        if (auto s = ref->getSource())
            if (auto their_o = s->getOwner())
                if (auto my_o = getOwner())
                    if (their_o == creature)
        {
            ref->removeReference();
            delete ref;
            break;
        }
        ref = nextRef;
    }
}

//=================================================
// set state for one reference, defined by Unit

void HostileRefManager::setOnlineOfflineState(Unit* creature, bool isOnline)
{
    HostileReference* ref = getFirst();
    while (ref)
    {
        HostileReference* nextRef = ref->next();
        if (auto s = ref->getSource())
        if (auto their_o = s->getOwner())
        if (auto my_o = getOwner())
        if (their_o == creature)
        {
            ref->setOnlineOfflineState(isOnline);
            break;
        }
        ref = nextRef;
    }
}

//=================================================

void HostileRefManager::UpdateVisibility()
{
    HostileReference* ref = getFirst();
    while (ref)
    {
        HostileReference* nextRef = ref->next();

        if (auto s          = ref->getSource())
        if (auto their_o    = s->getOwner())
        if (auto my_o       = getOwner())
        if (!their_o->canSeeOrDetect(my_o))
        {
            nextRef = ref->next();
            ref->removeReference();
            delete ref;
        }
        ref = nextRef;
    }
}
