#include "stardust/character.hpp"

#include <algorithm>
#include <cassert>
#include <numeric>

namespace stardust
{
	character::~character()
	{
		for (auto &ability : _passive_abilities)
		{
			ability->reset_owner();
		}
	}

	stat_value_t character::get_stat(const id_t &id) const
	{
		const auto [begin, end] = _modifiers.equal_range(id);
		return std::accumulate(begin, end, get_base_stat(id),
			[](const stat_value_t &value, const auto &stat_modifier_pair) -> stat_value_t
			{ return value + stat_modifier_pair.second.value; });
	}

	bool character::has_stat_modifier(const id_t &stat_id, const id_t &modifier_id) const
	{
		const auto [begin, end] = _modifiers.equal_range(stat_id);
		return std::find_if(begin, end, [&modifier_id](const auto &stat_modifier_pair)
			{ return stat_modifier_pair.second.id == modifier_id; }) != end;
	}

	character::modifiers character::get_stat_modifiers(const id_t &stat_id) const
	{
		modifiers stat_modifiers;
		const auto [begin, end] = _modifiers.equal_range(stat_id);
		std::transform(begin, end, std::back_inserter(stat_modifiers), [](const auto &stat_modifier_pair)
			{ return stat_modifier_pair.second; });

		return stat_modifiers;
	}

	void character::add_stat_modifier(const id_t &stat_id, modifier_info modifier)
	{
		_modifiers.emplace(stat_id, std::move(modifier));
	}

	void character::remove_stat_modifier(const id_t &stat_id, const id_t &modifier_id)
	{
		const auto [begin, end] = _modifiers.equal_range(stat_id);

		for (auto it = begin; it != end; ++it)
		{
			if (it->second.id == modifier_id)
			{
				_modifiers.erase(it);
			}
		}
	}

	void character::set_passive_abilities(abilities new_abilities)
	{
		_passive_abilities = std::move(new_abilities);

		for (const auto &ability : _passive_abilities)
		{
			assert(ability != nullptr);
			ability->set_owner(*this);
		}
	}
}
