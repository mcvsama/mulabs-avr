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

#ifndef MULABS_AVR__USI_H__INCLUDED
#define MULABS_AVR__USI_H__INCLUDED

// AVR:
#include <avr/io.h>

// Local:
#include "utility.h"
#include "interrupts.h"
#include "mcu.h"
#include "pin.h"


namespace mulabs {
namespace avr {

class USI
{
  public:
	enum class Mode: uint8_t
	{
		Disabled					= 0b00000000,
		ThreeWire					= 0b00010000,
		TwoWire						= 0b00100000,
		TwoWireSCLHoldOnOverflow	= 0b00110000,
	};

	enum class CounterClockSource: uint8_t
	{
		None						= 0b00000000,
		SoftwareClockStrobe			= 0b00000010,
		Timer0CompareMatch			= 0b00000100,
		ExternalRisingEdge			= 0b00001000,
		ExternalFallingEdge			= 0b00001100,
		SoftExternalRisingEdge		= 0b00001010,
		SoftExternalFallingEdge		= 0b00001110,
	};

  public:
	static constexpr MCU::Pin	scl		= MCU::usi_scl;
	static constexpr MCU::Pin	sda		= MCU::usi_sda;

  private:
	static constexpr uint8_t ModeMask				= 0b00110000;
	static constexpr uint8_t CounterMask			= 0b00001111;
	static constexpr uint8_t CounterClockSourceMask	= 0b00001110;

  public:
	/**
	 * Select USI working mode.
	 */
	static void
	select_mode (Mode mode)
	{
		USICR = (USICR & ~ModeMask) | static_cast<uint8_t> (mode);

		switch (mode)
		{
			case Mode::TwoWire:
			case Mode::TwoWireSCLHoldOnOverflow:
				USI::select_counter_clock_source (USI::CounterClockSource::ExternalRisingEdge);
				// TODO check ExternalFallingEdge

				// SCL as output for auto SCL hold down after interrupt:
				scl.configure_as_output();
				// SDA by default to input to prevent holding it down accidentally:
				sda.configure_as_input();

				scl.set_tri_state();
				sda.set_tri_state();

				set_start_condition_interrupt_enabled (true);
				set_counter_overflow_interrupt_enabled (false);
				configure_for_start_condition();

				// When mode is not Mode::Disabled, output pins work in
				// open-collector mode, so it's safe to pull them high:
				scl.set_high();
				sda.set_high();
				break;

			case Mode::Disabled:
			case Mode::ThreeWire:
				break;
		}
	}

	/**
	 * Configure for start condition.
	 */
	static void
	configure_for_start_condition()
	{
		USISR = 0b11110000;
	}

	/**
	 * Enable/disable USI start condition interrupt.
	 *
	 * When the interrupt occurs, the SCL is held down and user must call
	 * start_condition_handled() to release the SCL line.
	 */
	static void
	set_start_condition_interrupt_enabled (bool enabled)
	{
		if (enabled)
			set_bit (USICR, USISIE);
		else
			clear_bit (USICR, USISIE);
	}

	/**
	 * Return true if start condition has been detected by USI.
	 *
	 * If that happens, SCL line is held down until USISIF flag is cleared.
	 */
	static bool
	start_condition_detected()
	{
		return get_bit (USISR, USISIF);
	}

	/**
	 * Clear start-condition-detected flag to release the SCL line.
	 */
	static void
	start_condition_handled()
	{
		// Must write 1 to clear the USISIF flag:
		set_bit (USISR, USISIF);
	}

	/**
	 * Enable/disable counter overflow interrupt.
	 *
	 * When the interrupt occurs and the mode is TwoWireSCLHoldOnOverflow, the SCL is held down
	 * and user must call counter_overflow_handled() to release the SCL line.
	 */
	static void
	set_counter_overflow_interrupt_enabled (bool enabled)
	{
		if (enabled)
			set_bit (USICR, USIOIE);
		else
			clear_bit (USICR, USIOIE);
	}

	/**
	 * Clear counter-overflow flag to release the SCL line.
	 * Applies only in TwoWireSCLHoldOnOverflow mode.
	 */
	static void
	counter_overflow_handled()
	{
		// Must write 1 to clear the USIOIF flag:
		set_bit (USISR, USIOIF);
	}

	/**
	 * Return true if stop condition has been detected.
	 */
	static bool
	stop_condition_detected()
	{
		return get_bit (USISR, USIPF);
	}

	/**
	 * Clear the stop-condition-detected flag.
	 */
	static void
	stop_condition_handled()
	{
		// Must write 1 to clear the USIPF flag:
		set_bit (USISR, USIPF);
	}

	/**
	 * Return true if there's data output collision, that is
	 * the MSB of the output data register is different from
	 * the level on the SDA line.
	 */
	static bool
	collision_detected()
	{
		return get_bit (USISR, USIDC);
	}

	/**
	 * Return current value of the 4-bit USI counter value.
	 * Use set_counter_clock_source() to decide when the counter
	 * should be incremented.
	 */
	static uint8_t
	counter_value()
	{
		return USISR & CounterMask;
	}

	/**
	 * Set new 4-bit USI counter value.
	 */
	static void
	set_counter_value (uint8_t value)
	{
		USISR = (USISR & ~CounterMask) | (value & CounterMask);
	}

	/**
	 * Select clocking source for the 4-bit USI counter.
	 */
	static void
	select_counter_clock_source (CounterClockSource source)
	{
		USICR = (USICR & ~CounterClockSourceMask) | static_cast<uint8_t> (source);
	}

	/**
	 * Return data from USI.
	 * If SDA is in input mode, then LSB of this register immediately reflects SDA level.
	 * If SDA is in output mode, then SDA level immediately reflects MSB of this register.
	 */
	static uint8_t
	data()
	{
		return USIDR;
	}

	/**
	 * Set byte to transmit.
	 */
	static void
	set_data (uint8_t byte)
	{
		USIDR = byte;
	}
};

} // namespace avr
} // namespace mulabs

#endif

