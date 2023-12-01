#pragma once

#include "stardust/types.hpp"

#include <bitset>
#include <memory>
#include <map>

namespace stardust
{
	class character;

	namespace target_rules
	{
		const target_rule_t self("1000"); // Can affect the originator.
		const target_rule_t pulse("0100"); // Can affect surrounding characters, but not the originator.
		const target_rule_t aura = self | pulse; // Can affect surrounding characters, including the originator.
		const target_rule_t friendly("0010"); // Can affect friendly characters, not including the originator.
		const target_rule_t enemy("0001"); // Can affect enemy characters.
		const target_rule_t all_other = friendly | enemy; // Can affect friendly and enemy characters.
		const target_rule_t all = self | all_other; // Can affect friendly and enemy characters as well as the originator.
	}

	/**
	 * \class ability_base
	 * \brief Base class for all gameplay abilities.
	 */
	class ability
	{
	public:
		using uptr = std::unique_ptr<ability>;
		using sptr = std::shared_ptr<ability>;
		using wptr = std::weak_ptr<ability>;

		ability(id_t id, target_rule_t target_rule);

		// FIXME: The class is not copyable, because it doesn't properly handle owner and detachment.
		ability &operator=(const ability &) = delete;
		ability(const ability &) = delete;

		virtual ~ability() = default;

		/**
		 * \brief Ability's ID.
		*/
		const id_t &get_id() const
		{
			return _id;
		}

		/**
		 * \brief Ability's targeting rules.
		*/
		target_rule_t get_target_rule() const
		{
			return _target_rule;
		}

		/**
		 * \brief Attaches the ability to a character. The caller is responsible for detachment.
		 * 
		 * Typically, there's no need to call this function manually. As soon as the ability is
		 * added to a character, the character will set itself as the owner.
		*/
		void set_owner(character &new_owner);

		/**
		 * \brief Detaches the ability from a character.
		 * 
		 * Typically, there's no need to call this function manually. character class is responsible
		 * for managing the owner of abilities.
		*/
		void reset_owner();

		/**
		 * \brief A character that owns the ability.
		*/
		character *get_owner() const
		{
			return _owner;
		}

		// TODO: Change the argument type to something generic, like entity.
		/**
		 * \brief Applies the effect of the ability to a target.
		*/
		virtual void activate(character &target) = 0;

		// TODO: Move this to a separate class for passive abilities.
		/**
		 * \brief Removes the effect of the ability from a target.
		*/
		virtual void deactivate(character &target) = 0;

	protected:
		/**
		 * \brief Handles the ability's attachment to a character.
		 * Called by set_owner() after the owner is set.
		 * 
		 * The default implementation does nothing. Override for custom logic.
		*/
		virtual void on_attached() {}
		/**
		 * \brief Handles the ability's detachment from a character.
		 * Called by reset_owner() before the owner is reset.
		 * 
		 * The default implementation does nothing. Override for custom logic.
		*/
		virtual void on_detached() {}

	private:
		id_t _id{};
		character *_owner{};
		target_rule_t _target_rule{};
	};

	/**
	 * \class stat_modifier_ability
	 * \brief Ability that modifies stats when it gets added to a character.
	 * 
	 * Applies modifiers to character stats that match the given ID's.
	 */
	class stat_modifier_ability : public ability
	{
	public:
		using modifiers = std::map<id_t, stat_value_t>;

		using ability::ability;

		void activate(character &target) override;
		void deactivate(character &target) override;

		/**
		 * \brief Modifiers that the ability applies.
		*/
		const modifiers &get_modifiers() const
		{
			return _modifiers;
		}

		/**
		 * \brief Sets modifiers that the ability applies.
		 * If the ability is already attached to a character, the new modifiers will be applied immediately.
		*/
		void set_modifiers(modifiers new_modifiers);

	protected:
		void on_attached() override;
		void on_detached() override;

	private:
		void apply_modifiers(character &target);
		void remove_modifiers(character &target);

		modifiers _modifiers;
	};
}
