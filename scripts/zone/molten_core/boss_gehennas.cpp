/* Copyright (C) 2006 - 2008 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/* ScriptData
SDName: Boss_Gehennas
SD%Complete: 100
SDComment: Adds MC NYI
EndScriptData */

#include "../../sc_defines.h"



#define SPELL_SHADOWBOLT            19729
#define SPELL_RAINOFFIRE            19717
#define SPELL_GEHENNASCURSE         19716

struct MANGOS_DLL_DECL boss_gehennasAI : public ScriptedAI
{
    boss_gehennasAI(Creature *c) : ScriptedAI(c) {EnterEvadeMode();}
    uint32 ShadowBolt_Timer;
    uint32 RainOfFire_Timer;
    uint32 GehennasCurse_Timer;
    bool InCombat;

    void EnterEvadeMode()
    {
        ShadowBolt_Timer = 6000;
        RainOfFire_Timer = 10000;
        GehennasCurse_Timer = 12000;
        InCombat = false;

        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop();
        DoGoHome();
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISARM, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_INTERRUPT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_INTERRUPT_CAST, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SILENCE, true);       
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CONFUSED, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR , true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DAZE, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SLEEP, true);
        m_creature->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_BANISH, true);

    }

    void AttackStart(Unit *who)
    {
        if (!who)
            return;

        if (who->isTargetableForAttack() && who!= m_creature)
        {
            //Begin melee attack if we are within range
            DoStartMeleeAttack(who);
            InCombat = true;
        }
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!who || m_creature->getVictim())
            return;

        if (who->isTargetableForAttack() && who->isInAccessablePlaceFor(m_creature) && m_creature->IsHostileTo(who))
        {
            float attackRadius = m_creature->GetAttackDistance(who);
            if (m_creature->IsWithinDistInMap(who, attackRadius) && m_creature->GetDistanceZ(who) <= CREATURE_Z_ATTACK_RANGE && m_creature->IsWithinLOSInMap(who))
            {
                if(who->HasStealthAura())
                    who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);

                DoStartMeleeAttack(who);
                InCombat = true;

            }
        }
    }

    void UpdateAI(const uint32 diff)
    {
        //Return since we have no target
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        //ShadowBolt_Timer
        if (ShadowBolt_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_SHADOWBOLT);

            //7 seconds until we should cast this agian
            ShadowBolt_Timer = 7000;
        }else ShadowBolt_Timer -= diff;

        //RainOfFire_Timer
        if (RainOfFire_Timer < diff)
        {
            //Cast
            Unit* target = NULL;

            target = SelectUnit(SELECT_TARGET_RANDOM,0);
            if (target)
                DoCast(target,SPELL_RAINOFFIRE);

            //4-12 seconds until we should cast this agian
            RainOfFire_Timer = 4000 + rand()%8000;
        }else RainOfFire_Timer -= diff;

        //GehennasCurse_Timer
        if (GehennasCurse_Timer < diff)
        {
            //Cast
            DoCast(m_creature->getVictim(),SPELL_GEHENNASCURSE);

            //22-30 seconds until we should cast this agian
            GehennasCurse_Timer = 22000 + rand()%8000;;
        }else GehennasCurse_Timer -= diff;

        DoMeleeAttackIfReady();
    }
}; 
CreatureAI* GetAI_boss_gehennas(Creature *_Creature)
{
    return new boss_gehennasAI (_Creature);
}


void AddSC_boss_gehennas()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name="boss_gehennas";
    newscript->GetAI = GetAI_boss_gehennas;
    m_scripts[nrscripts++] = newscript;
}
