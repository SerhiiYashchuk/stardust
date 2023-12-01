#pragma once

#include "stardust/abilities.hpp"
#include "stardust/types.hpp"

#include <cstdint>
#include <initializer_list>
#include <iterator>
#include <map>
#include <memory>
#include <vector>

namespace stardust
{
	/**
	 * \class character
	 * \brief Game character with stats and abilities.
	 */
	class character
	{
	public:
		using abilities = std::vector<ability::sptr>;
		using stats = std::map<id_t, stat_value_t>;
		using modifiers = std::vector<modifier_info>;

		~character();

		/**
		 * \brief Sets character stats in the form of (ID, base value).
		*/
		void set_stats(stats new_stats)
		{
			_stats = std::move(new_stats);
		}

		/**
		 * \brief Character's stats.
		*/
		const stats &get_stats() const
		{
			return _stats;
		}

		/**
		 * \brief Checks if the character has a stat with the given ID.
		*/
		bool has_stat(const id_t &id) const
		{
			return _stats.contains(id);
		}

		/**
		 * \brief Base value of a character's stat.
		 * Throws an exception, if there's no stat with such ID.
		*/
		stat_value_t get_base_stat(const id_t &id) const
		{
			return _stats.at(id);
		}

		/**
		 * \brief Current value of a character's stat (with all modifiers applied).
		 * Throws an exception, if there's no stat with such ID.
		*/
		stat_value_t get_stat(const id_t &id) const;

		/**
		 * \brief Checks if there's a particular modifier for a stat.
		*/
		bool has_stat_modifier(const id_t &stat_id, const id_t &modifier_id) const;

		/**
		 * \brief Set of modifiers, applied to a stat.
		 * 
		 * Modifiers are not limited to character stats. They may also affect abilities and other things.
		 * It is for the user to decide what a modifier is applied to.
		*/
		modifiers get_stat_modifiers(const id_t &stat_id) const;

		/**
		 * \brief Adds a new modifier for a stat.
		*/
		void add_stat_modifier(const id_t &stat_id, modifier_info modifier);

		/**
		 * \brief Removes a modifier for a stat.
		*/
		void remove_stat_modifier(const id_t &stat_id, const id_t &modifier_id);

		/**
		 * \brief Character's passive abilities.
		 */
		const abilities &get_passive_abilities() const
		{
			return _passive_abilities;
		}

		/**
		 * \brief Sets passive abilities for the character.
		*/
		void set_passive_abilities(abilities new_abilities);

	private:
		using categorized_modifiers = std::multimap<id_t, modifier_info>;

		stats _stats;
		categorized_modifiers _modifiers;
		abilities _passive_abilities; // always-on effects
	};
}
