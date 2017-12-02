/* vim:ts=4
 *
 * Copyleft 2012…2014  Michał Gawron
 * Marduk Unix Labs, http://mulabs.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Visit http://www.gnu.org/licenses/gpl-3.0.html for more information on licensing.
 */

#ifndef MULABS_AVR__DEVICES__XMEGA_AU__INTERRUPT_SYSTEM_H__INCLUDED
#define MULABS_AVR__DEVICES__XMEGA_AU__INTERRUPT_SYSTEM_H__INCLUDED

namespace mulabs {
namespace avr {
namespace xmega_au {

class InterruptSystem
{
  public:
	enum class Level: uint8_t
	{
		Disabled	= 0b00,	// Interrupts disabled
		Low			= 0b01,	// Low level interrupt
		Medium		= 0b10,	// Medium-level interrupt
		High		= 0b11,	// High-level interrupt
	};

  public:
	/**
	 * Enable interrupts on selected levels.
	 */
	template<class ...Levels>
		static void
		enable (Levels...);

	/**
	 * Disable interrupts on selected levels.
	 */
	template<class ...Levels>
		static void
		disable (Levels...);

  private:
	/**
	 * Helper for enable()/disable() of Levels.
	 * Collect list of levels and prepare value for the PMIC_CTRL register.
	 */
	template<class ...Levels>
		static constexpr uint8_t
		make_levels_list (Level level, Levels ...levels)
		{
			uint8_t result = get_level_bit (level);

			if constexpr (sizeof... (levels) > 0)
				result |= make_levels_list (levels...);

			return result;
		}

	/**
	 * Return level bit for registers like PMIC_CTRL.
	 */
	static constexpr uint8_t
	get_level_bit (Level level)
	{
		return (1u << static_cast<uint8_t> (level)) >> 1;
	}
};


template<class ...Levels>
	inline void
	InterruptSystem::enable (Levels ...levels)
	{
		PMIC_CTRL = PMIC_CTRL | make_levels_list (levels...);
	}


template<class ...Levels>
	inline void
	InterruptSystem::disable (Levels ...levels)
	{
		PMIC_CTRL = PMIC_CTRL & ~make_levels_list (levels...);
	}

} // namespace xmega_au
} // namespace avr
} // namespace mulabs

#endif

