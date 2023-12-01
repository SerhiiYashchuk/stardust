#include "stardust/abilities.hpp"
#include "stardust/character.hpp"

#include <cassert>

namespace
{
	constexpr std::size_t target_self_bit = 0;
	constexpr std::size_t target_pulse_bit = 1;
	constexpr std::size_t target_friendly_bit = 2;
	constexpr std::size_t target_enemy_bit = 3;
}

namespace stardust
{
	// Base ability

	ability::ability(id_t id, target_rule_t target_rule)
		: _id(std::move(id)), _target_rule(target_rule)
	{
		assert(_target_rule.none());
	}

	void ability::set_owner(character &new_owner)
	{
		if (_owner != &new_owner)
		{
			reset_owner();
		}

		_owner = &new_owner;
		on_attached();
	}

	void ability::reset_owner()
	{
		if (_owner != nullptr)
		{
			on_detached();
			_owner = nullptr;
		}
	}

	// Stat modifier ability

	void stat_modifier_ability::activate(character &target)
	{
		// TODO: Check if the target is valid.
		apply_modifiers(target);
	}

	void stat_modifier_ability::deactivate(character &target)
	{
		// TODO: Check if the target is valid.
		remove_modifiers(target);
	}

	void stat_modifier_ability::set_modifiers(modifiers new_modifiers)
	{
		// FIXME: The old modifiers, if any, that were applied are not updated.
		_modifiers = std::move(new_modifiers);
	}

	void stat_modifier_ability::on_attached()
	{
		if (get_target_rule().test(target_self_bit))
		{
			activate(*get_owner());
		}
	}

	void stat_modifier_ability::on_detached()
	{
		if (get_target_rule().test(target_self_bit))
		{
			deactivate(*get_owner());
		}
	}

	void stat_modifier_ability::apply_modifiers(character &target)
	{
		for (const auto &[stat_id, modifier_value] : _modifiers)
		{
			target.add_stat_modifier(stat_id, {modifier_value, get_id(), this});
		}
	}

	void stat_modifier_ability::remove_modifiers(character &target)
	{
		for (const auto &[stat_id, modifier_value] : _modifiers)
		{
			target.remove_stat_modifier(stat_id, get_id());
		}
	}
}
