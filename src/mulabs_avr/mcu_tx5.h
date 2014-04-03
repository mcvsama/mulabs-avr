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

#ifndef MULABS_AVR__MCU_TX5_H__INCLUDED
#define MULABS_AVR__MCU_TX5_H__INCLUDED

// AVR:
#include <avr/io.h>
#include <avr/sleep.h>

// Local:
#include "interrupts.h"
#include "port.h"
#include "pin.h"


namespace mulabs {
namespace avr {

class AtTinyX5
{
  public:
	enum class Int0Trigger: uint8_t
	{
		LowLevel	= 0b00,
		AnyChange	= 0b01,
		FallingEdge	= 0b10,
		RisingEdge	= 0b11,
	};

  public:
	static constexpr Port	b			= { DDRB, PINB, PORTB, PORTB };
	static constexpr Pin	b_0			= { b, 0 };
	static constexpr Pin	b_1			= { b, 1 };
	static constexpr Pin	b_2			= { b, 2 };
	static constexpr Pin	b_3			= { b, 3 };
	static constexpr Pin	b_4			= { b, 4 };
	static constexpr Pin	b_5			= { b, 5 };
	// Aliases:
	static constexpr Pin	reset_pin	= b_5;
	static constexpr Pin	usi_scl		= b_2;
	static constexpr Pin	usi_sda		= b_0;
	static constexpr Pin	ain_0		= b_0;
	static constexpr Pin	ain_1		= b_1;
	static constexpr Pin	adc_0		= b_5;
	static constexpr Pin	adc_1		= b_2;
	static constexpr Pin	adc_2		= b_4;
	static constexpr Pin	adc_3		= b_3;
	static constexpr Pin	pcint_0		= b_0;
	static constexpr Pin	pcint_1		= b_1;
	static constexpr Pin	pcint_2		= b_2;
	static constexpr Pin	pcint_3		= b_3;
	static constexpr Pin	pcint_4		= b_4;
	static constexpr Pin	pcint_5		= b_5;

  public:
	/**
	 * Sleep for given number of milliseconds.
	 */
	static void
	sleep_ms (int ms)
	{
		for (int z = 0; z < ms; ++z)
			_delay_ms (1);
	}

	/**
	 * Sleep for given number of milliseconds.
	 */
	template<int MS>
		static void
		sleep_ms()
		{
			_delay_ms (MS);
		}

	/**
	 * Sleep for given number of microseconds.
	 */
	static void
	sleep_us (int us)
	{
		for (int z = 0; z < us / 20; ++z)
			_delay_us (20);
	}

	/**
	 * Sleep for given number of microseconds.
	 */
	template<int US>
		static void
		sleep_us()
		{
			_delay_us (US);
		}

	/**
	 * \param	prescale_factor
	 * 			Prescale factor, valid values are powers of 2, in range [1..256].
	 */
	template<unsigned int scale_factor>
		static void
		set_clock_prescaler()
		{
			static_assert (scale_factor == 1 ||
						   scale_factor == 2 ||
						   scale_factor == 4 ||
						   scale_factor == 8 ||
						   scale_factor == 16 ||
						   scale_factor == 32 ||
						   scale_factor == 64 ||
						   scale_factor == 128 ||
						   scale_factor == 256, "scale_factor must be power of 2 [1..256]");

			CLKPR = 0b10000000;
			switch (scale_factor)
			{
				case 1:		CLKPR = 0b00000000; break;
				case 2:		CLKPR = 0b00000001; break;
				case 4:		CLKPR = 0b00000010; break;
				case 8:		CLKPR = 0b00000011; break;
				case 16:	CLKPR = 0b00000100; break;
				case 32:	CLKPR = 0b00000101; break;
				case 64:	CLKPR = 0b00000110; break;
				case 128:	CLKPR = 0b00000111; break;
				case 256:	CLKPR = 0b00001000; break;
			}
		}

	/**
	 * Wake-up sources:
	 * - all sources for power_adc() and power_down()
	 * - other I/O
	 */
	static void
	sleep_idle()
	{
		set_sleep_mode (SLEEP_MODE_IDLE);
		sleep_mode();
	}

	/**
	 * Wake-up sources:
	 * - all sources for power_down()
	 * - EEPROM ready
	 * - ADC
	 */
	static void
	sleep_adc()
	{
		set_sleep_mode (SLEEP_MODE_ADC);
		sleep_mode();
	}

	/**
	 * Wake-up sources:
	 * - INT0 and INTPCn
	 * - Watchdog timeout
	 * - USI start condition.
	 */
	static void
	power_down()
	{
		set_sleep_mode (SLEEP_MODE_PWR_DOWN);
		sleep_mode();
	}

	/**
	 * Configure INT0 interrupt.
	 * Non-atomic.
	 */
	static void
	configure_int0 (Int0Trigger trigger)
	{
		MCUCR = MCUCR & 0b01111100 | static_cast<uint8_t> (trigger);
	}

	/**
	 * Enable INT0.
	 */
	static void
	enable_int0()
	{
		set_bit (GIMSK, INT0);
	}

	/**
	 * Disable INT0.
	 */
	static void
	disable_int0()
	{
		clear_bit (GIMSK, INT0);
	}

	/**
	 * Enable PCINT interrupts.
	 */
	static void
	general_enable_pcints()
	{
		set_bit (GIMSK, PCIE);
	}

	/**
	 * Disable PCINT interrupts.
	 */
	static void
	general_disable_pcints()
	{
		clear_bit (GIMSK, PCIE);
	}

	/**
	 * Configure PCINTs to be enabled. Rest will be disabled.
	 */
	static void
	set_enabled_pcints (uint8_t mask)
	{
		PCMSK = mask;
	}

	/**
	 * Enable given PCINTs. Rest will be unaffected.
	 */
	static void
	enable_pcints (uint8_t mask)
	{
		PCMSK |= mask;
	}

	/**
	 * Disable given PCINTs. Rest will be unaffected.
	 */
	static void
	disable_pcints (uint8_t mask)
	{
		PCMSK &= ~mask;
	}

	/**
	 * Return mask for set_enabled_pcints(), enable_pcints(), disable_pcints().
	 */
	static uint8_t
	pcint (uint8_t number)
	{
		return bitnum<uint8_t> (number);
	}

	/**
	 * General pull-up enable.
	 */
	static void
	general_enable_pull_ups()
	{
		clear_bit (MCUCR, PUD);
	}

	/**
	 * General pull-up disable. Disables pull-ups even if they're enabled individually.
	 * Does not change individual pull-up settings.
	 */
	static void
	general_disable_pull_ups()
	{
		set_bit (MCUCR, PUD);
	}
};


typedef AtTinyX5 MCU;

} // namespace avr
} // namespace mulabs

#endif

