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

#ifndef MULABS_AVR__DEVICES__XMEGA_AU__BASIC_CLOCK_H__INCLUDED
#define MULABS_AVR__DEVICES__XMEGA_AU__BASIC_CLOCK_H__INCLUDED


namespace mulabs {
namespace avr {
namespace xmega_au {

template<class pMCU>
	class BasicClock
	{
	  public:
		using MCU = pMCU;

		enum class ClockSource: uint8_t
		{
			RC2MHz		= 0b000,
			RC32MHz		= 0b001,
			RC32KHz		= 0b010,
			XOSC		= 0b011,
			PLL			= 0b100,
		};

		enum ClockSourceFlags: uint8_t
		{
			RC2MHz		= bitnum<uint8_t> (0),
			RC32MHz		= bitnum<uint8_t> (1),
			RC32KHz		= bitnum<uint8_t> (2),
			XOSC		= bitnum<uint8_t> (3),
			PLL			= bitnum<uint8_t> (4),
		};

	  public:
		/**
		 * Enable selected clock sources (multiple bits may be set at once).
		 */
		static void
		enable_clock (ClockSourceFlags);

		/**
		 * Disable selected clock souces (multiple bits may be set at once).
		 */
		static void
		disable_clock (ClockSourceFlags);

		/**
		 * Select clock used for CPU.
		 * The clock must be enabled prior to selection (see enable_clock(...)).
		 * This function will wait until clock source is stable, before switching.
		 */
		static void
		select_clock_for_cpu (ClockSource);

		/**
		 * Lock system clock - prevent clock configuration changes
		 * until MCU reset.
		 */
		static void
		lock_system_clock();
	};


template<class M>
	inline void
	BasicClock<M>::enable_clock (ClockSourceFlags flags)
	{
		OSC_CTRL |= flags;
	}


template<class M>
	inline void
	BasicClock<M>::disable_clock (ClockSourceFlags flags)
	{
		OSC_CTRL &= ~flags;
	}


template<class M>
	inline void
	BasicClock<M>::select_clock_for_cpu (ClockSource clock_source)
	{
		// Wait for the oscillator to stabilize:
		while (!(OSC_STATUS & bitnum<uint8_t> (static_cast<uint8_t> (clock_source))))
			continue;
		// Select the clock source:
		MCU::disable_configuration_change_protection_for_register();
		CLK_CTRL = static_cast<uint8_t> (clock_source);
	}


template<class M>
	inline void
	BasicClock<M>::lock_system_clock()
	{
		MCU::disable_configuration_change_protection_for_register();
		CLK_LOCK = 0b00000001;
	}

} // namespace xmega_au
} // namespace avr
} // namespace mulabs

#endif

