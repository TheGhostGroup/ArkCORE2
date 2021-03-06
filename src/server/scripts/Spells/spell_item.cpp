/*
 * Copyright (C) 2011-2012 ArkCORE2 <http://www.arkania.net/>
 * Copyright (C) 2010-2012 Project SkyFire <http://www.projectskyfire.org/> 
 * Copyright (C) 2006-2012 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 3 of the License, or (at your
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

/*
 * Scripts for spells with SPELLFAMILY_GENERIC spells used by items.
 * Ordered alphabetically using scriptname.
 * Scriptnames of files in this file should be prefixed with "spell_item_".
 */

#include "ScriptPCH.h"
#include "SkillDiscovery.h"

// Generic script for handling item dummy effects which trigger another spell.
class spell_item_trigger_spell : public SpellScriptLoader
{
private:
    uint32 _triggeredSpellId;

public:
    spell_item_trigger_spell(const char* name, uint32 triggeredSpellId) : SpellScriptLoader(name), _triggeredSpellId(triggeredSpellId) { }

    class spell_item_trigger_spell_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_item_trigger_spell_SpellScript)
    private:
        uint32 _triggeredSpellId;

    public:
        spell_item_trigger_spell_SpellScript(uint32 triggeredSpellId) : SpellScript(), _triggeredSpellId(triggeredSpellId) { }

        bool Validate(SpellInfo const* /*spellEntry*/)
        {
            if (!sSpellMgr->GetSpellInfo(_triggeredSpellId))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (Item* pItem = GetCastItem())
                GetCaster()->CastSpell(GetCaster(), _triggeredSpellId, true, pItem);
        }

        void Register()
        {
            OnEffectHit += SpellEffectFn(spell_item_trigger_spell_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_item_trigger_spell_SpellScript(_triggeredSpellId);
    }
};

// http://www.wowhead.com/item=6522 Deviate Fish
// 8063 Deviate Fish
enum eDeviateFishSpells
{
    SPELL_SLEEPY            = 8064,
    SPELL_INVIGORATE        = 8065,
    SPELL_SHRINK            = 8066,
    SPELL_PARTY_TIME        = 8067,
    SPELL_HEALTHY_SPIRIT    = 8068,
};

class spell_item_deviate_fish : public SpellScriptLoader
{
public:
    spell_item_deviate_fish() : SpellScriptLoader("spell_item_deviate_fish") { }

    class spell_item_deviate_fish_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_item_deviate_fish_SpellScript)
    public:
        bool Validate(SpellInfo const* /*spellEntry*/)
        {
            for (uint32 spellId = SPELL_SLEEPY; spellId <= SPELL_HEALTHY_SPIRIT; ++spellId)
                if (!sSpellMgr->GetSpellInfo(spellId))
                    return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* pCaster = GetCaster();
            if (pCaster->GetTypeId() != TYPEID_PLAYER)
                return;

            uint32 spellId = urand(SPELL_SLEEPY, SPELL_HEALTHY_SPIRIT);
            pCaster->CastSpell(pCaster, spellId, true, NULL);
        }

        void Register()
        {
            OnEffectHit += SpellEffectFn(spell_item_deviate_fish_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_item_deviate_fish_SpellScript();
    }
};

// http://www.wowhead.com/item=58149 Flask of Enhancement
// 79637 Flask of Enhancement
enum eFlaskOfEnhancementSpells
{
    SPELL_FLASK_OF_ENHANCEMENT_INT = 79640,
    SPELL_FLASK_OF_ENHANCEMENT_AGI = 79639,
    SPELL_FLASK_OF_ENHANCEMENT_STR = 79638,
};

class spell_item_flask_of_enhancement : public SpellScriptLoader
{
public:
    spell_item_flask_of_enhancement() : SpellScriptLoader("spell_item_flask_of_enhancement") { }

    class spell_item_flask_of_enhancement_SpellScript : public SpellScript
    {
    public:
        PrepareSpellScript(spell_item_flask_of_enhancement_SpellScript)
        bool Validate(SpellInfo const* /*spellEntry*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_FLASK_OF_ENHANCEMENT_INT) || !sSpellMgr->GetSpellInfo(SPELL_FLASK_OF_ENHANCEMENT_AGI) || !sSpellMgr->GetSpellInfo(SPELL_FLASK_OF_ENHANCEMENT_STR))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* pCaster = GetCaster();
            if (pCaster->GetTypeId() != TYPEID_PLAYER)
                return;

            uint32 spellId;
            if (pCaster->GetStat(STAT_INTELLECT) >= pCaster->GetStat(STAT_AGILITY) && pCaster->GetStat(STAT_INTELLECT) >= pCaster->GetStat(STAT_STRENGTH))
                spellId = SPELL_FLASK_OF_ENHANCEMENT_INT;
            else if(pCaster->GetStat(STAT_AGILITY) >= pCaster->GetStat(STAT_INTELLECT) && pCaster->GetStat(STAT_AGILITY) >= pCaster->GetStat(STAT_STRENGTH))
                spellId = SPELL_FLASK_OF_ENHANCEMENT_AGI;
            else
                spellId = SPELL_FLASK_OF_ENHANCEMENT_STR;

            pCaster->CastSpell(pCaster, spellId, true, NULL);
        }

        void Register()
        {
            OnEffectHit += SpellEffectFn(spell_item_flask_of_enhancement_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_item_flask_of_enhancement_SpellScript();
    }
};

// http://www.wowhead.com/item=47499 Flask of the North
// 67019 Flask of the North
enum eFlaskOfTheNorthSpells
{
    SPELL_FLASK_OF_THE_NORTH_SP = 67016,
    SPELL_FLASK_OF_THE_NORTH_AP = 67017,
    SPELL_FLASK_OF_THE_NORTH_STR = 67018,
};

class spell_item_flask_of_the_north : public SpellScriptLoader
{
public:
    spell_item_flask_of_the_north() : SpellScriptLoader("spell_item_flask_of_the_north") { }

    class spell_item_flask_of_the_north_SpellScript : public SpellScript
    {
    public:
        PrepareSpellScript(spell_item_flask_of_the_north_SpellScript)
        bool Validate(SpellInfo const* /*spellEntry*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_FLASK_OF_THE_NORTH_SP))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_FLASK_OF_THE_NORTH_AP))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_FLASK_OF_THE_NORTH_STR))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* pCaster = GetCaster();
            if (pCaster->GetTypeId() != TYPEID_PLAYER)
                return;

            std::vector<uint32> possibleSpells;
            switch (pCaster->getClass())
            {
                case CLASS_WARLOCK:
                case CLASS_MAGE:
                case CLASS_PRIEST:
                    possibleSpells.push_back(SPELL_FLASK_OF_THE_NORTH_SP);
                    break;
                case CLASS_DEATH_KNIGHT:
                case CLASS_WARRIOR:
                    possibleSpells.push_back(SPELL_FLASK_OF_THE_NORTH_STR);
                    break;
                case CLASS_ROGUE:
                case CLASS_HUNTER:
                    possibleSpells.push_back(SPELL_FLASK_OF_THE_NORTH_AP);
                    break;
                case CLASS_DRUID:
                case CLASS_PALADIN:
                    possibleSpells.push_back(SPELL_FLASK_OF_THE_NORTH_SP);
                    possibleSpells.push_back(SPELL_FLASK_OF_THE_NORTH_STR);
                    break;
                case CLASS_SHAMAN:
                    possibleSpells.push_back(SPELL_FLASK_OF_THE_NORTH_SP);
                    possibleSpells.push_back(SPELL_FLASK_OF_THE_NORTH_AP);
                    break;
            }

            pCaster->CastSpell(pCaster, possibleSpells[irand(0, (possibleSpells.size() - 1))], true, NULL);
        }

        void Register()
        {
            OnEffectHit += SpellEffectFn(spell_item_flask_of_the_north_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_item_flask_of_the_north_SpellScript();
    }
};

// http://www.wowhead.com/item=62237 Adventurer's Journal
// 86939 Read Journal
enum eAdventurersJournal
{
    SPELL_LEARNING_BY_EXAMPLE   = 86963,
    SPELL_TERRAIN_EXPERT        = 86972,
    SPELL_DEMON_SLAYER          = 86974,
    SPELL_DRAGONSBANE           = 86975,
    SPELL_FIRE_AND_WATER        = 86976,
    SPELL_KNEECAPPER            = 86977,
    SPELL_DEADLY_DRIVE          = 86980,
    SPELL_MECHANICAL_APTITUDE   = 86982,
    SPELL_28_CHAPTERS_LATER     = 86983,
    SPELL_LIVING_FOREVER        = 86988,
    SPELL_THE_GREAT_HUNT        = 86992,
};

class spell_item_adventurers_journal : public SpellScriptLoader
{
public:
    spell_item_adventurers_journal() : SpellScriptLoader("spell_item_adventurers_journal") { }

    class spell_item_adventurers_journal_SpellScript : public SpellScript
    {
    public:
        PrepareSpellScript(spell_item_adventurers_journal_SpellScript)
        bool Load(SpellInfo const* /*spellEntry*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_LEARNING_BY_EXAMPLE)
                || !sSpellMgr->GetSpellInfo(SPELL_TERRAIN_EXPERT)
                || !sSpellMgr->GetSpellInfo(SPELL_DEMON_SLAYER)
                || !sSpellMgr->GetSpellInfo(SPELL_DRAGONSBANE)
                || !sSpellMgr->GetSpellInfo(SPELL_FIRE_AND_WATER)
                || !sSpellMgr->GetSpellInfo(SPELL_KNEECAPPER)
                || !sSpellMgr->GetSpellInfo(SPELL_DEADLY_DRIVE)
                || !sSpellMgr->GetSpellInfo(SPELL_MECHANICAL_APTITUDE)
                || !sSpellMgr->GetSpellInfo(SPELL_28_CHAPTERS_LATER)
                || !sSpellMgr->GetSpellInfo(SPELL_LIVING_FOREVER)
                || !sSpellMgr->GetSpellInfo(SPELL_THE_GREAT_HUNT))
                return false;
            return true;
        }

        void HandleScriptEffect(SpellEffIndex /*effIndex*/)
        {
            Unit* pCaster = GetCaster();
            if (pCaster->GetTypeId() != TYPEID_PLAYER)
                return;

            uint32 spellId = SPELL_THE_GREAT_HUNT;
            switch(urand(1,11))
            {
                case 1:     spellId = SPELL_LEARNING_BY_EXAMPLE;break;
                case 2:     spellId = SPELL_TERRAIN_EXPERT;     break;
                case 3:     spellId = SPELL_DEMON_SLAYER;       break;
                case 4:     spellId = SPELL_DRAGONSBANE;        break;
                case 5:     spellId = SPELL_FIRE_AND_WATER;     break;
                case 6:     spellId = SPELL_KNEECAPPER;         break;
                case 7:     spellId = SPELL_DEADLY_DRIVE;       break;
                case 8:     spellId = SPELL_MECHANICAL_APTITUDE;break;
                case 9:     spellId = SPELL_28_CHAPTERS_LATER;  break;
                case 10:    spellId = SPELL_LIVING_FOREVER;     break;                
            }
            pCaster->CastSpell(pCaster, spellId, true, NULL);
        }

        void Register()
        {
            OnEffectHit += SpellEffectFn(spell_item_adventurers_journal_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_item_adventurers_journal_SpellScript();
    }
};

// http://www.wowhead.com/item=10645 Gnomish Death Ray
// 13280 Gnomish Death Ray
enum eGnomishDeathRay
{
    SPELL_GNOMISH_DEATH_RAY_DUMMY_CASTER_SELF_DAMAGE = 13493,
    SPELL_GNOMISH_DEATH_RAY_TARGET_DAMAGE = 13279,
    SPELL_GNOMISH_DEATH_RAY_DUMMY_CASTER = 13280,
};

class spell_item_gnomish_death_ray : public SpellScriptLoader
{
public:
    spell_item_gnomish_death_ray() : SpellScriptLoader("spell_item_gnomish_death_ray") { }

    class spell_item_gnomish_death_ray_SpellScript : public SpellScript
    {
    public:

        PrepareSpellScript(spell_item_gnomish_death_ray_SpellScript)

        bool Validate(SpellInfo const* /*spellEntry*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_GNOMISH_DEATH_RAY_DUMMY_CASTER_SELF_DAMAGE))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_GNOMISH_DEATH_RAY_TARGET_DAMAGE))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_GNOMISH_DEATH_RAY_DUMMY_CASTER))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* caster = GetCaster();
            Unit* target = caster->getVictim();

            if (target)
                caster->CastSpell(target, SPELL_GNOMISH_DEATH_RAY_TARGET_DAMAGE, true);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_item_gnomish_death_ray_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_item_gnomish_death_ray_SpellScript();
    }
};

// http://www.wowhead.com/item=27388 Mr. Pinchy
// 33060 Make a Wish
enum eMakeAWish
{
    SPELL_MR_PINCHYS_BLESSING       = 33053,
    SPELL_SUMMON_MIGHTY_MR_PINCHY   = 33057,
    SPELL_SUMMON_FURIOUS_MR_PINCHY  = 33059,
    SPELL_TINY_MAGICAL_CRAWDAD      = 33062,
    SPELL_MR_PINCHYS_GIFT           = 33064,
};

class spell_item_make_a_wish : public SpellScriptLoader
{
public:
    spell_item_make_a_wish() : SpellScriptLoader("spell_item_make_a_wish") { }

    class spell_item_make_a_wish_SpellScript : public SpellScript
    {
    public:
        PrepareSpellScript(spell_item_make_a_wish_SpellScript)
        bool Validate(SpellInfo const* /*spellEntry*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_MR_PINCHYS_BLESSING))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_SUMMON_MIGHTY_MR_PINCHY))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_SUMMON_FURIOUS_MR_PINCHY))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_TINY_MAGICAL_CRAWDAD))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_MR_PINCHYS_GIFT))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* pCaster = GetCaster();
            if (pCaster->GetTypeId() != TYPEID_PLAYER)
                return;

            uint32 spellId = SPELL_MR_PINCHYS_GIFT;
            switch (urand(1, 5))
            {
                case 1: spellId = SPELL_MR_PINCHYS_BLESSING; break;
                case 2: spellId = SPELL_SUMMON_MIGHTY_MR_PINCHY; break;
                case 3: spellId = SPELL_SUMMON_FURIOUS_MR_PINCHY; break;
                case 4: spellId = SPELL_TINY_MAGICAL_CRAWDAD; break;
            }
            pCaster->CastSpell(pCaster, spellId, true, NULL);
        }

        void Register()
        {
            OnEffectHit += SpellEffectFn(spell_item_make_a_wish_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_item_make_a_wish_SpellScript();
    }
};

// http://www.wowhead.com/item=32686 Mingo's Fortune Giblets
// 40802 Mingo's Fortune Generator
class spell_item_mingos_fortune_generator : public SpellScriptLoader
{
public:
    spell_item_mingos_fortune_generator() : SpellScriptLoader("spell_item_mingos_fortune_generator") { }

    class spell_item_mingos_fortune_generator_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_item_mingos_fortune_generator_SpellScript)
        void HandleDummy(SpellEffIndex effIndex)
        {
            // Selecting one from Bloodstained Fortune item
            uint32 newitemid;
            switch (urand(1, 20))
            {
                case 1:  newitemid = 32688; break;
                case 2:  newitemid = 32689; break;
                case 3:  newitemid = 32690; break;
                case 4:  newitemid = 32691; break;
                case 5:  newitemid = 32692; break;
                case 6:  newitemid = 32693; break;
                case 7:  newitemid = 32700; break;
                case 8:  newitemid = 32701; break;
                case 9:  newitemid = 32702; break;
                case 10: newitemid = 32703; break;
                case 11: newitemid = 32704; break;
                case 12: newitemid = 32705; break;
                case 13: newitemid = 32706; break;
                case 14: newitemid = 32707; break;
                case 15: newitemid = 32708; break;
                case 16: newitemid = 32709; break;
                case 17: newitemid = 32710; break;
                case 18: newitemid = 32711; break;
                case 19: newitemid = 32712; break;
                case 20: newitemid = 32713; break;
                default:
                    return;
            }

            CreateItem(effIndex, newitemid);
        }

        void Register()
        {
            OnEffectHit += SpellEffectFn(spell_item_mingos_fortune_generator_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_item_mingos_fortune_generator_SpellScript();
    }
};

// http://www.wowhead.com/item=10720 Gnomish Net-o-Matic Projector
// 13120 Net-o-Matic
enum eNetOMaticSpells
{
    SPELL_NET_O_MATIC_TRIGGERED1 = 16566,
    SPELL_NET_O_MATIC_TRIGGERED2 = 13119,
    SPELL_NET_O_MATIC_TRIGGERED3 = 13099,
};

class spell_item_net_o_matic : public SpellScriptLoader
{
public:
    spell_item_net_o_matic() : SpellScriptLoader("spell_item_net_o_matic") { }

    class spell_item_net_o_matic_SpellScript : public SpellScript
    {
    public:
        PrepareSpellScript(spell_item_net_o_matic_SpellScript)
        bool Validate(SpellInfo const* /*spellEntry*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_NET_O_MATIC_TRIGGERED1))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_NET_O_MATIC_TRIGGERED2))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_NET_O_MATIC_TRIGGERED3))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (Unit* target = GetHitUnit())
            {
                uint32 spellId = SPELL_NET_O_MATIC_TRIGGERED3;
                uint32 roll = urand(0, 99);
                if (roll < 2)                            // 2% for 30 sec self root (off-like chance unknown)
                    spellId = SPELL_NET_O_MATIC_TRIGGERED1;
                else if (roll < 4)                       // 2% for 20 sec root, charge to target (off-like chance unknown)
                    spellId = SPELL_NET_O_MATIC_TRIGGERED2;

                GetCaster()->CastSpell(target, spellId, true, NULL);
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_item_net_o_matic_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_item_net_o_matic_SpellScript();
    }
};

// http://www.wowhead.com/item=8529 Noggenfogger Elixir
// 16589 Noggenfogger Elixir
enum eNoggenfoggerElixirSpells
{
    SPELL_NOGGENFOGGER_ELIXIR_TRIGGERED1 = 16595,
    SPELL_NOGGENFOGGER_ELIXIR_TRIGGERED2 = 16593,
    SPELL_NOGGENFOGGER_ELIXIR_TRIGGERED3 = 16591,
};

class spell_item_noggenfogger_elixir : public SpellScriptLoader
{
public:
    spell_item_noggenfogger_elixir() : SpellScriptLoader("spell_item_noggenfogger_elixir") { }

    class spell_item_noggenfogger_elixir_SpellScript : public SpellScript
    {
    public:
        PrepareSpellScript(spell_item_noggenfogger_elixir_SpellScript)
        bool Validate(SpellInfo const* /*spellEntry*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_NOGGENFOGGER_ELIXIR_TRIGGERED1))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_NOGGENFOGGER_ELIXIR_TRIGGERED2))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_NOGGENFOGGER_ELIXIR_TRIGGERED3))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* pCaster = GetCaster();
            if (pCaster->GetTypeId() != TYPEID_PLAYER)
                return;

            uint32 spellId = SPELL_NOGGENFOGGER_ELIXIR_TRIGGERED3;
            switch (urand(1, 3))
            {
                case 1: spellId = SPELL_NOGGENFOGGER_ELIXIR_TRIGGERED1; break;
                case 2: spellId = SPELL_NOGGENFOGGER_ELIXIR_TRIGGERED2; break;
            }

            pCaster->CastSpell(pCaster, spellId, true, NULL);
        }

        void Register()
        {
            OnEffectHit += SpellEffectFn(spell_item_noggenfogger_elixir_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_item_noggenfogger_elixir_SpellScript();
    }
};

// http://www.wowhead.com/item=6657 Savory Deviate Delight
// 8213 Savory Deviate Delight
enum eSavoryDeviateDelight
{
    SPELL_FLIP_OUT_MALE     = 8219,
    SPELL_FLIP_OUT_FEMALE   = 8220,
    SPELL_YAAARRRR_MALE     = 8221,
    SPELL_YAAARRRR_FEMALE   = 8222,
};

class spell_item_savory_deviate_delight : public SpellScriptLoader
{
public:
    spell_item_savory_deviate_delight() : SpellScriptLoader("spell_item_savory_deviate_delight") { }

    class spell_item_savory_deviate_delight_SpellScript : public SpellScript
    {
    public:
        PrepareSpellScript(spell_item_savory_deviate_delight_SpellScript)
        bool Validate(SpellInfo const* /*spellEntry*/)
        {
            for (uint32 spellId = SPELL_FLIP_OUT_MALE; spellId <= SPELL_YAAARRRR_FEMALE; ++spellId)
                if (!sSpellMgr->GetSpellInfo(spellId))
                    return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* pCaster = GetCaster();
            if (pCaster->GetTypeId() != TYPEID_PLAYER)
                return;

            uint32 spellId = 0;
            switch (urand(1, 2))
            {
                // Flip Out - ninja
                case 1: spellId = (pCaster->getGender() == GENDER_MALE ? SPELL_FLIP_OUT_MALE : SPELL_FLIP_OUT_FEMALE); break;
                // Yaaarrrr - pirate
                case 2: spellId = (pCaster->getGender() == GENDER_MALE ? SPELL_YAAARRRR_MALE : SPELL_YAAARRRR_FEMALE); break;
            }
            pCaster->CastSpell(pCaster, spellId, true, NULL);
        }

        void Register()
        {
            OnEffectHit += SpellEffectFn(spell_item_savory_deviate_delight_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_item_savory_deviate_delight_SpellScript();
    }
};

// http://www.wowhead.com/item=7734 Six Demon Bag
// 14537 Six Demon Bag
enum eSixDemonBagSpells
{
    SPELL_FROSTBOLT                 = 11538,
    SPELL_POLYMORPH                 = 14621,
    SPELL_SUMMON_FELHOUND_MINION    = 14642,
    SPELL_FIREBALL                  = 15662,
    SPELL_CHAIN_LIGHTNING           = 21179,
    SPELL_ENVELOPING_WINDS          = 25189,
};

class spell_item_six_demon_bag : public SpellScriptLoader
{
public:
    spell_item_six_demon_bag() : SpellScriptLoader("spell_item_six_demon_bag") { }

    class spell_item_six_demon_bag_SpellScript : public SpellScript
    {
    public:
        PrepareSpellScript(spell_item_six_demon_bag_SpellScript)
        bool Validate(SpellInfo const* /*spellEntry*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_FROSTBOLT))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_POLYMORPH))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_SUMMON_FELHOUND_MINION))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_FIREBALL))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_CHAIN_LIGHTNING))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_ENVELOPING_WINDS))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (Unit* target = GetHitUnit())
            {
                Unit* pCaster = GetCaster();

                uint32 spellId = 0;
                uint32 rand = urand(0, 99);
                if (rand < 25)                      // Fireball (25% chance)
                    spellId = SPELL_FIREBALL;
                else if (rand < 50)                 // Frostball (25% chance)
                    spellId = SPELL_FROSTBOLT;
                else if (rand < 70)                 // Chain Lighting (20% chance)
                    spellId = SPELL_CHAIN_LIGHTNING;
                else if (rand < 80)                 // Polymorph (10% chance)
                {
                    spellId = SPELL_POLYMORPH;
                    if (urand(0, 100) <= 30)        // 30% chance to self-cast
                        target = pCaster;
                }
                else if (rand < 95)                 // Enveloping Winds (15% chance)
                    spellId = SPELL_ENVELOPING_WINDS;
                else                                // Summon Felhund minion (5% chance)
                {
                    spellId = SPELL_SUMMON_FELHOUND_MINION;
                    target = pCaster;
                }

                pCaster->CastSpell(target, spellId, true, GetCastItem());
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_item_six_demon_bag_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_item_six_demon_bag_SpellScript();
    }
};

// http://www.wowhead.com/item=44012 Underbelly Elixir
// 59640 Underbelly Elixir
enum eUnderbellyElixirSpells
{
    SPELL_UNDERBELLY_ELIXIR_TRIGGERED1 = 59645,
    SPELL_UNDERBELLY_ELIXIR_TRIGGERED2 = 59831,
    SPELL_UNDERBELLY_ELIXIR_TRIGGERED3 = 59843,
};

class spell_item_underbelly_elixir : public SpellScriptLoader
{
public:
    spell_item_underbelly_elixir() : SpellScriptLoader("spell_item_underbelly_elixir") { }

    class spell_item_underbelly_elixir_SpellScript : public SpellScript
    {
    public:
        PrepareSpellScript(spell_item_underbelly_elixir_SpellScript)
        bool Validate(SpellInfo const* /*spellEntry*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_UNDERBELLY_ELIXIR_TRIGGERED1))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_UNDERBELLY_ELIXIR_TRIGGERED2))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_UNDERBELLY_ELIXIR_TRIGGERED3))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            Unit* pCaster = GetCaster();
            if (pCaster->GetTypeId() != TYPEID_PLAYER)
                return;

            uint32 spellId = SPELL_UNDERBELLY_ELIXIR_TRIGGERED3;
            switch (urand(1, 3))
            {
                case 1: spellId = SPELL_UNDERBELLY_ELIXIR_TRIGGERED1; break;
                case 2: spellId = SPELL_UNDERBELLY_ELIXIR_TRIGGERED2; break;
            }
            pCaster->CastSpell(pCaster, spellId, true, NULL);
        }

        void Register()
        {
            OnEffectHit += SpellEffectFn(spell_item_underbelly_elixir_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_item_underbelly_elixir_SpellScript();
    }
};

enum eShadowmourneVisuals
{
    SPELL_SHADOWMOURNE_VISUAL_LOW       = 72521,
    SPELL_SHADOWMOURNE_VISUAL_HIGH      = 72523,
    SPELL_SHADOWMOURNE_CHAOS_BANE_BUFF  = 73422,
};

class spell_item_shadowmourne : public SpellScriptLoader
{
public:
    spell_item_shadowmourne() : SpellScriptLoader("spell_item_shadowmourne") { }

    class spell_item_shadowmourne_AuraScript : public AuraScript
    {
    public:
        PrepareAuraScript(spell_item_shadowmourne_AuraScript)
        spell_item_shadowmourne_AuraScript() : AuraScript() { }

        bool Validate(SpellInfo const* /*spellEntry*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_SHADOWMOURNE_VISUAL_LOW))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_SHADOWMOURNE_VISUAL_HIGH))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_SHADOWMOURNE_CHAOS_BANE_BUFF))
                return false;
            return true;
        }

        void OnStackChange(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();

            switch (GetStackAmount())
            {
                case 1:
                    target->CastSpell(target, SPELL_SHADOWMOURNE_VISUAL_LOW, true);
                    break;
                case 6:
                    target->RemoveAurasDueToSpell(SPELL_SHADOWMOURNE_VISUAL_LOW);
                    target->CastSpell(target, SPELL_SHADOWMOURNE_VISUAL_HIGH, true);
                    break;
                case 10:
                    target->RemoveAurasDueToSpell(SPELL_SHADOWMOURNE_VISUAL_HIGH);
                    target->CastSpell(target, SPELL_SHADOWMOURNE_CHAOS_BANE_BUFF, true);
                    break;
            }
        }

        void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
        {
            Unit* target = GetTarget();
            target->RemoveAurasDueToSpell(SPELL_SHADOWMOURNE_VISUAL_LOW);
            target->RemoveAurasDueToSpell(SPELL_SHADOWMOURNE_VISUAL_HIGH);
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(spell_item_shadowmourne_AuraScript::OnStackChange, EFFECT_0, SPELL_AURA_MOD_STAT, AuraEffectHandleModes(AURA_EFFECT_HANDLE_REAL | AURA_EFFECT_HANDLE_REAPPLY));
            AfterEffectRemove += AuraEffectRemoveFn(spell_item_shadowmourne_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_STAT, AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new spell_item_shadowmourne_AuraScript();
    }
};

// http://www.wowhead.com/item=46725 Red Rider Air Rifle
// 67533 Shoot Air Rifle
enum AirRifleSpells
{
    SPELL_AIR_RIFLE_HOLD_VISUAL = 65582,
    SPELL_AIR_RIFLE_SHOOT       = 67532,
    SPELL_AIR_RIFLE_SHOOT_SELF  = 65577,
};

class spell_item_red_rider_air_rifle : public SpellScriptLoader
{
    public:
        spell_item_red_rider_air_rifle() : SpellScriptLoader("spell_item_red_rider_air_rifle") { }

        class spell_item_red_rider_air_rifle_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_red_rider_air_rifle_SpellScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_AIR_RIFLE_HOLD_VISUAL))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_AIR_RIFLE_SHOOT))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_AIR_RIFLE_SHOOT_SELF))
                    return false;
                return true;
            }

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                if (!GetHitUnit())
                    return;

                GetCaster()->CastSpell(GetCaster(), SPELL_AIR_RIFLE_HOLD_VISUAL, true);
                // needed because this spell shares GCD with its triggered spells (which must not be cast with triggered flag)
                if (Player* player = GetCaster()->ToPlayer())
                    player->GetGlobalCooldownMgr().CancelGlobalCooldown(GetSpellInfo());
                if (urand(0, 4))
                    GetCaster()->CastSpell(GetHitUnit(), SPELL_AIR_RIFLE_SHOOT, false);
                else
                    GetCaster()->CastSpell(GetCaster(), SPELL_AIR_RIFLE_SHOOT_SELF, false);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_red_rider_air_rifle_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_red_rider_air_rifle_SpellScript();
        }
};

enum eGenericData
{
    SPELL_ARCANITE_DRAGONLING           = 19804,
    SPELL_BATTLE_CHICKEN                = 13166,
    SPELL_MECHANICAL_DRAGONLING         = 4073,
    SPELL_MITHRIL_MECHANICAL_DRAGONLING = 12749,
};

enum CreateHeartCandy
{
    ITEM_HEART_CANDY_1 = 21818,
    ITEM_HEART_CANDY_2 = 21817,
    ITEM_HEART_CANDY_3 = 21821,
    ITEM_HEART_CANDY_4 = 21819,
    ITEM_HEART_CANDY_5 = 21816,
    ITEM_HEART_CANDY_6 = 21823,
    ITEM_HEART_CANDY_7 = 21822,
    ITEM_HEART_CANDY_8 = 21820,
};

class spell_item_create_heart_candy : public SpellScriptLoader
{
    public:
        spell_item_create_heart_candy() : SpellScriptLoader("spell_item_create_heart_candy") { }

        class spell_item_create_heart_candy_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_create_heart_candy_SpellScript);

            void HandleScript(SpellEffIndex effIndex)
            {
                PreventHitDefaultEffect(effIndex);
                if (!GetHitUnit() || !GetHitUnit()->ToPlayer())
                    return;

                Player* target = GetHitUnit()->ToPlayer();

                static const uint32 items[] = {ITEM_HEART_CANDY_1, ITEM_HEART_CANDY_2, ITEM_HEART_CANDY_3, ITEM_HEART_CANDY_4, ITEM_HEART_CANDY_5, ITEM_HEART_CANDY_6, ITEM_HEART_CANDY_7, ITEM_HEART_CANDY_8};

                target->AddItem(items[urand(0, 7)], 1);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_create_heart_candy_SpellScript::HandleScript, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_create_heart_candy_SpellScript();
        }
};

// http://www.wowhead.com/item=45912 Book of Glyph Mastery
// 64323 Book of Glyph Mastery
class spell_item_book_of_glyph_mastery : public SpellScriptLoader
{
public:
    spell_item_book_of_glyph_mastery() : SpellScriptLoader("spell_item_book_of_glyph_mastery") {}

    class spell_item_book_of_glyph_mastery_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_item_book_of_glyph_mastery_SpellScript);

        bool Load()
        {
            return GetCaster()->GetTypeId() == TYPEID_PLAYER;
        }

        SpellCastResult CheckRequirement()
        {
            if (HasDiscoveredAllSpells(GetSpellInfo()->Id, GetCaster()->ToPlayer()))
            {
                SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_LEARNED_EVERYTHING);
                return SPELL_FAILED_CUSTOM_ERROR;
            }

            return SPELL_CAST_OK;
        }

        void Register()
        {
            OnCheckCast += SpellCheckCastFn(spell_item_book_of_glyph_mastery_SpellScript::CheckRequirement);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_item_book_of_glyph_mastery_SpellScript();
    }
};

// http://www.wowhead.com/item=39253 Gift of the Harvester
// 52481 Gift of the Harvester
enum GiftOfTheHarvester
{
    NPC_GHOUL   = 28845,
    MAX_GHOULS  = 5,
};

class spell_item_gift_of_the_harvester : public SpellScriptLoader
{
    public:
        spell_item_gift_of_the_harvester() : SpellScriptLoader("spell_item_gift_of_the_harvester") {}

        class spell_item_gift_of_the_harvester_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_gift_of_the_harvester_SpellScript);

            SpellCastResult CheckRequirement()
            {
                std::list<Creature*> ghouls;
                GetCaster()->GetAllMinionsByEntry(ghouls, NPC_GHOUL);
                if (ghouls.size() >= MAX_GHOULS)
                {
                    SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_TOO_MANY_GHOULS);
                    return SPELL_FAILED_CUSTOM_ERROR;
                }

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_item_gift_of_the_harvester_SpellScript::CheckRequirement);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_gift_of_the_harvester_SpellScript();
        }
};

// http://www.wowhead.com/item=34920 Map of the Geyser Fields
// 45853 Survey Sinkholes
enum Sinkholes
{
    NPC_SOUTH_SINKHOLE      = 25664,
    NPC_NORTHEAST_SINKHOLE  = 25665,
    NPC_NORTHWEST_SINKHOLE  = 25666,
};

class spell_item_map_of_the_geyser_fields : public SpellScriptLoader
{
    public:
        spell_item_map_of_the_geyser_fields() : SpellScriptLoader("spell_item_map_of_the_geyser_fields") {}

        class spell_item_map_of_the_geyser_fields_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_map_of_the_geyser_fields_SpellScript);

            SpellCastResult CheckSinkholes()
            {
                Unit* caster = GetCaster();
                if (caster->FindNearestCreature(NPC_SOUTH_SINKHOLE, 30.0f, true)     ||
                    caster->FindNearestCreature(NPC_NORTHEAST_SINKHOLE, 30.0f, true) ||
                    caster->FindNearestCreature(NPC_NORTHWEST_SINKHOLE, 30.0f, true))
                    return SPELL_CAST_OK;

                SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_MUST_BE_CLOSE_TO_SINKHOLE);
                return SPELL_FAILED_CUSTOM_ERROR;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_item_map_of_the_geyser_fields_SpellScript::CheckSinkholes);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_map_of_the_geyser_fields_SpellScript();
        }
};

enum VanquishedClutchesSpells
{
    SPELL_CRUSHER       = 64982,
    SPELL_CONSTRICTOR   = 64983,
    SPELL_CORRUPTOR     = 64984,
};

class spell_item_vanquished_clutches : public SpellScriptLoader
{
    public:
        spell_item_vanquished_clutches() : SpellScriptLoader("spell_item_vanquished_clutches") { }

        class spell_item_vanquished_clutches_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_vanquished_clutches_SpellScript);

            bool Validate(SpellInfo const* /*spellEntry*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_CRUSHER))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_CONSTRICTOR))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_CORRUPTOR))
                    return false;
                return true;
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                uint32 spellId = RAND(SPELL_CRUSHER, SPELL_CONSTRICTOR, SPELL_CORRUPTOR);
                GetCaster()->CastSpell(GetCaster(), spellId, true);
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_item_vanquished_clutches_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_vanquished_clutches_SpellScript();
        }
};

enum AshbringerSounds
{
    SOUND_ASHBRINGER_1  = 8906,                             // "I was pure once"
    SOUND_ASHBRINGER_2  = 8907,                             // "Fought for righteousness"
    SOUND_ASHBRINGER_3  = 8908,                             // "I was once called Ashbringer"
    SOUND_ASHBRINGER_4  = 8920,                             // "Betrayed by my order"
    SOUND_ASHBRINGER_5  = 8921,                             // "Destroyed by Kel'Thuzad"
    SOUND_ASHBRINGER_6  = 8922,                             // "Made to serve"
    SOUND_ASHBRINGER_7  = 8923,                             // "My son watched me die"
    SOUND_ASHBRINGER_8  = 8924,                             // "Crusades fed his rage"
    SOUND_ASHBRINGER_9  = 8925,                             // "Truth is unknown to him"
    SOUND_ASHBRINGER_10 = 8926,                             // "Scarlet Crusade  is pure no longer"
    SOUND_ASHBRINGER_11 = 8927,                             // "Balnazzar's crusade corrupted my son"
    SOUND_ASHBRINGER_12 = 8928                              // "Kill them all!"
};

class spell_item_ashbringer : public SpellScriptLoader
{
public:
    spell_item_ashbringer() : SpellScriptLoader("spell_item_ashbringer") {}

    class spell_item_ashbringer_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_item_ashbringer_SpellScript);

        bool Load()
        {
            return GetCaster()->GetTypeId() == TYPEID_PLAYER;
        }

        void OnDummyEffect(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);

            Player* player = GetCaster()->ToPlayer();
            uint32 sound_id = RAND( SOUND_ASHBRINGER_1, SOUND_ASHBRINGER_2, SOUND_ASHBRINGER_3, SOUND_ASHBRINGER_4, SOUND_ASHBRINGER_5, SOUND_ASHBRINGER_6,
                SOUND_ASHBRINGER_7, SOUND_ASHBRINGER_8, SOUND_ASHBRINGER_9, SOUND_ASHBRINGER_10, SOUND_ASHBRINGER_11, SOUND_ASHBRINGER_12 );

            // Ashbringer's effect (spellID 28441) re-triggers every 5 seconds, with a chance of making it say one of the above 12 sounds
            if (urand(0, 60) < 1)
                player->PlayDirectSound(sound_id, player);
        }

        void Register()
        {
            OnEffectHit += SpellEffectFn(spell_item_ashbringer_SpellScript::OnDummyEffect, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_item_ashbringer_SpellScript();
    }
};

enum MagicEater
{
    SPELL_WILD_MAGIC                             = 58891,
    SPELL_WELL_FED_1                             = 57288,
    SPELL_WELL_FED_2                             = 57139,
    SPELL_WELL_FED_3                             = 57111,
    SPELL_WELL_FED_4                             = 57286,
    SPELL_WELL_FED_5                             = 57291,
};

class spell_magic_eater_food : public SpellScriptLoader
{
    public:
        spell_magic_eater_food() : SpellScriptLoader("spell_magic_eater_food")
        {
        }

        class spell_magic_eater_food_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_magic_eater_food_AuraScript);

            void HandleTriggerSpell(AuraEffect const* /*aurEff*/)
            {
                PreventDefaultAction();
                Unit* target = GetTarget();

                switch (urand(0, 5))
                {
                    case 0:
                        target->CastSpell(target, SPELL_WILD_MAGIC, true);
                        break;
                    case 1:
                        target->CastSpell(target, SPELL_WELL_FED_1, true);
                        break;
                    case 2:
                        target->CastSpell(target, SPELL_WELL_FED_2, true);
                        break;
                    case 3:
                        target->CastSpell(target, SPELL_WELL_FED_3, true);
                        break;
                    case 4:
                        target->CastSpell(target, SPELL_WELL_FED_4, true);
                        break;
                    case 5:
                        target->CastSpell(target, SPELL_WELL_FED_5, true);
                        break;
                    }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_magic_eater_food_AuraScript::HandleTriggerSpell, EFFECT_1, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_magic_eater_food_AuraScript();
        }
};

enum Refocus
{
    SPELL_AIMED_SHOT    = 19434,
    SPELL_MULTISHOT     = 2643,
    SPELL_VOLLEY        = 42243,
};

class spell_item_refocus : public SpellScriptLoader
{
    public:
        spell_item_refocus() : SpellScriptLoader("spell_item_refocus") { }

        class spell_item_refocus_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_item_refocus_SpellScript);

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Player* caster = GetCaster()->ToPlayer();

                if (!caster || caster->getClass() != CLASS_HUNTER)
                    return;

                if (caster->HasSpellCooldown(SPELL_AIMED_SHOT))
                    caster->RemoveSpellCooldown(SPELL_AIMED_SHOT, true);

                if (caster->HasSpellCooldown(SPELL_MULTISHOT))
                    caster->RemoveSpellCooldown(SPELL_MULTISHOT, true);

                if (caster->HasSpellCooldown(SPELL_VOLLEY))
                    caster->RemoveSpellCooldown(SPELL_VOLLEY, true);
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_item_refocus_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_item_refocus_SpellScript();
        }
};

class spell_item_muisek_vessel : public SpellScriptLoader
{
public:
    spell_item_muisek_vessel() : SpellScriptLoader("spell_item_muisek_vessel") { }

    class spell_item_muisek_vessel_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_item_muisek_vessel_SpellScript);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (Creature* target = GetHitCreature())
                if (target->isDead())
                    target->ForcedDespawn();
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_item_muisek_vessel_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_item_muisek_vessel_SpellScript();
    }
};

enum PurifyHelboarMeat
{
    SPELL_SUMMON_PURIFIED_HELBOAR_MEAT      = 29277,
    SPELL_SUMMON_TOXIC_HELBOAR_MEAT         = 29278,
};

class spell_item_purify_helboar_meat : public SpellScriptLoader
{
public:
    spell_item_purify_helboar_meat() : SpellScriptLoader("spell_item_purify_helboar_meat") { }

    class spell_item_purify_helboar_meat_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_item_purify_helboar_meat_SpellScript);

        bool Load()
        {
            return GetCaster()->GetTypeId() == TYPEID_PLAYER;
        }

        bool Validate(SpellInfo const* /*spell*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_SUMMON_PURIFIED_HELBOAR_MEAT) ||  !sSpellMgr->GetSpellInfo(SPELL_SUMMON_TOXIC_HELBOAR_MEAT))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /* effIndex */)
        {
            Unit* caster = GetCaster();
            caster->CastSpell(caster, roll_chance_i(50) ? SPELL_SUMMON_PURIFIED_HELBOAR_MEAT : SPELL_SUMMON_TOXIC_HELBOAR_MEAT, true, NULL);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_item_purify_helboar_meat_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_item_purify_helboar_meat_SpellScript();
    }
};

enum SocretharsStone
{
    SPELL_SOCRETHAR_TO_SEAT     = 35743,
    SPELL_SOCRETHAR_FROM_SEAT   = 35744,
};

class spell_item_socrethars_stone : public SpellScriptLoader
{
public:
    spell_item_socrethars_stone() : SpellScriptLoader("spell_item_socrethars_stone") { }

    class spell_item_socrethars_stone_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_item_socrethars_stone_SpellScript);

        bool Load()
        {
            return (GetCaster()->GetAreaId() == 3900 || GetCaster()->GetAreaId() == 3742);
        }
        bool Validate(SpellInfo const* /*spell*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_SOCRETHAR_TO_SEAT) || !sSpellMgr->GetSpellInfo(SPELL_SOCRETHAR_FROM_SEAT))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /* effIndex */)
        {
            Unit* caster = GetCaster();
            switch (caster->GetAreaId())
            {
            case 3900:
                caster->CastSpell(caster, SPELL_SOCRETHAR_TO_SEAT, true);
                break;
            case 3742:
                caster->CastSpell(caster, SPELL_SOCRETHAR_FROM_SEAT, true);
                break;
            default:
                return;
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_item_socrethars_stone_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_item_socrethars_stone_SpellScript();
    }
};

enum ImpaleLeviroth
{
    NPC_LEVIROTH                = 26452,
    SPELL_LEVIROTH_SELF_IMPALE  = 49882,
};

class spell_item_impale_leviroth : public SpellScriptLoader
{
public:
    spell_item_impale_leviroth() : SpellScriptLoader("spell_item_impale_leviroth") { }

    class spell_item_impale_leviroth_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_item_impale_leviroth_SpellScript);

        bool Validate(SpellInfo const* /*spell*/)
        {
            if (!sObjectMgr->GetCreatureTemplate(NPC_LEVIROTH))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /* effIndex */)
        {
            if (Unit* target = GetHitCreature())
                if (target->GetEntry() == NPC_LEVIROTH && !target->HealthBelowPct(95))
                    target->CastSpell(target, SPELL_LEVIROTH_SELF_IMPALE, true);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_item_impale_leviroth_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_item_impale_leviroth_SpellScript();
    }
};

enum NitroBoots
{
    SPELL_NITRO_BOOTS_SUCCESS       = 54861,
    SPELL_NITRO_BOOTS_BACKFIRE      = 46014,
};

class spell_item_nitro_boots : public SpellScriptLoader
{
public:
    spell_item_nitro_boots() : SpellScriptLoader("spell_item_nitro_boots") { }

    class spell_item_nitro_boots_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_item_nitro_boots_SpellScript);

        bool Load()
        {
            if (!GetCastItem())
                return false;
            return true;
        }

        bool Validate(SpellInfo const* /*spell*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_NITRO_BOOTS_SUCCESS) || !sSpellMgr->GetSpellInfo(SPELL_NITRO_BOOTS_BACKFIRE))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /* effIndex */)
        {
            Unit* caster = GetCaster();
            caster->CastSpell(caster, roll_chance_i(95) ? SPELL_NITRO_BOOTS_SUCCESS : SPELL_NITRO_BOOTS_BACKFIRE, true, GetCastItem());
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_item_nitro_boots_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_item_nitro_boots_SpellScript();
    }
};

enum RocketBoots
{
    SPELL_ROCKET_BOOTS_PROC      = 30452,
};

class spell_item_rocket_boots : public SpellScriptLoader
{
public:
    spell_item_rocket_boots() : SpellScriptLoader("spell_item_rocket_boots") { }

    class spell_item_rocket_boots_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_item_rocket_boots_SpellScript);

        bool Load()
        {
            return GetCaster()->GetTypeId() == TYPEID_PLAYER;
        }

        bool Validate(SpellInfo const* /*spell*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_ROCKET_BOOTS_PROC))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /* effIndex */)
        {
            Player* caster = GetCaster()->ToPlayer();
            if (Battleground* bg = caster->GetBattleground())
                bg->EventPlayerDroppedFlag(caster);

            caster->RemoveSpellCooldown(SPELL_ROCKET_BOOTS_PROC);
            caster->CastSpell(caster, SPELL_ROCKET_BOOTS_PROC, true, NULL);
        }

        SpellCastResult CheckCast()
        {
            if (GetCaster()->IsInWater())
                return SPELL_FAILED_ONLY_ABOVEWATER;
            return SPELL_CAST_OK;
        }

        void Register()
        {
            OnCheckCast += SpellCheckCastFn(spell_item_rocket_boots_SpellScript::CheckCast);
            OnEffectHitTarget += SpellEffectFn(spell_item_rocket_boots_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_item_rocket_boots_SpellScript();
    }
};

enum TeachLanguage
{
    SPELL_LEARN_GNOMISH_BINARY      = 50242,
    SPELL_LEARN_GOBLIN_BINARY       = 50246,
};

class spell_item_teach_language : public SpellScriptLoader
{
public:
    spell_item_teach_language() : SpellScriptLoader("spell_item_teach_language") { }

    class spell_item_teach_language_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_item_teach_language_SpellScript);

        bool Load()
        {
            return GetCaster()->GetTypeId() == TYPEID_PLAYER;
        }

        bool Validate(SpellInfo const* /*spell*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_LEARN_GNOMISH_BINARY) || !sSpellMgr->GetSpellInfo(SPELL_LEARN_GOBLIN_BINARY))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /* effIndex */)
        {
            Player* caster = GetCaster()->ToPlayer();

            if (roll_chance_i(34))
                caster->CastSpell(caster,caster->GetTeam() == ALLIANCE ? SPELL_LEARN_GNOMISH_BINARY : SPELL_LEARN_GOBLIN_BINARY, true);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(spell_item_teach_language_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_item_teach_language_SpellScript();
    }
};

void AddSC_item_spell_scripts()
{
    // 23074 Arcanite Dragonling
    new spell_item_trigger_spell("spell_item_arcanite_dragonling", SPELL_ARCANITE_DRAGONLING);
    // 23133 Gnomish Battle Chicken
    new spell_item_trigger_spell("spell_item_gnomish_battle_chicken", SPELL_BATTLE_CHICKEN);
    // 23076 Mechanical Dragonling
    new spell_item_trigger_spell("spell_item_mechanical_dragonling", SPELL_MECHANICAL_DRAGONLING);
    // 23075 Mithril Mechanical Dragonling
    new spell_item_trigger_spell("spell_item_mithril_mechanical_dragonling", SPELL_MITHRIL_MECHANICAL_DRAGONLING);

    new spell_item_deviate_fish();
    new spell_item_adventurers_journal();
    new spell_item_flask_of_enhancement();
    new spell_item_flask_of_the_north();
    new spell_item_gnomish_death_ray();
    new spell_item_make_a_wish();
    new spell_item_mingos_fortune_generator();
    new spell_item_net_o_matic();
    new spell_item_noggenfogger_elixir();
    new spell_item_savory_deviate_delight();
    new spell_item_six_demon_bag();
    new spell_item_underbelly_elixir();
    new spell_item_shadowmourne();
    new spell_item_red_rider_air_rifle();

    new spell_item_create_heart_candy();
    new spell_item_book_of_glyph_mastery();
    new spell_item_gift_of_the_harvester();
    new spell_item_map_of_the_geyser_fields();
    new spell_item_vanquished_clutches();

    new spell_item_ashbringer();
    new spell_magic_eater_food();
    new spell_item_refocus();
    new spell_item_muisek_vessel();
    new spell_item_purify_helboar_meat();
    new spell_item_socrethars_stone();
    new spell_item_impale_leviroth();
    new spell_item_nitro_boots();
    new spell_item_rocket_boots();
    new spell_item_teach_language();
}
