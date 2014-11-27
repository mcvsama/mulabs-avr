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

#ifndef MULABS_AVR__MCU_M32_H__INCLUDED
#define MULABS_AVR__MCU_M32_H__INCLUDED

// AVR:
#include <avr/io.h>
#include <avr/sleep.h>

// Local:
#include "avr8.h"


// Fixing broken C AVR "API":
#define __ADC ADC
#undef ADC


namespace mulabs {
namespace avr {

class AtMega32: public AVR8
{
  public:
	enum class IntTrigger: uint8_t
	{
		LowLevel		= 0b00,
		AnyChange		= 0b01,
		FallingEdge		= 0b10,
		RisingEdge		= 0b11,
	};

  public:
	static constexpr Port	a			= { DDRA, PINA, PORTA, PORTA };
	static constexpr Port	b			= { DDRB, PINB, PORTB, PORTB };
	static constexpr Port	c			= { DDRC, PINC, PORTC, PORTC };
	static constexpr Port	d			= { DDRD, PIND, PORTD, PORTD };
	static constexpr Pin	a_0			= { a, 0 };
	static constexpr Pin	a_1			= { a, 1 };
	static constexpr Pin	a_2			= { a, 2 };
	static constexpr Pin	a_3			= { a, 3 };
	static constexpr Pin	a_4			= { a, 4 };
	static constexpr Pin	a_5			= { a, 5 };
	static constexpr Pin	a_6			= { a, 6 };
	static constexpr Pin	a_7			= { a, 7 };
	static constexpr Pin	b_0			= { b, 0 };
	static constexpr Pin	b_1			= { b, 1 };
	static constexpr Pin	b_2			= { b, 2 };
	static constexpr Pin	b_3			= { b, 3 };
	static constexpr Pin	b_4			= { b, 4 };
	static constexpr Pin	b_5			= { b, 5 };
	static constexpr Pin	b_6			= { b, 6 };
	static constexpr Pin	b_7			= { b, 7 };
	static constexpr Pin	c_0			= { c, 0 };
	static constexpr Pin	c_1			= { c, 1 };
	static constexpr Pin	c_2			= { c, 2 };
	static constexpr Pin	c_3			= { c, 3 };
	static constexpr Pin	c_4			= { c, 4 };
	static constexpr Pin	c_5			= { c, 5 };
	static constexpr Pin	c_6			= { c, 6 };
	static constexpr Pin	c_7			= { c, 7 };
	static constexpr Pin	d_0			= { d, 0 };
	static constexpr Pin	d_1			= { d, 1 };
	static constexpr Pin	d_2			= { d, 2 };
	static constexpr Pin	d_3			= { d, 3 };
	static constexpr Pin	d_4			= { d, 4 };
	static constexpr Pin	d_5			= { d, 5 };
	static constexpr Pin	d_6			= { d, 6 };
	static constexpr Pin	d_7			= { d, 7 };
	// Aliases:
	static constexpr Pin	int_0		= d_2;
	static constexpr Pin	int_1		= d_3;
	static constexpr Pin	int_2		= b_2;
	static constexpr Pin	adc_0		= a_0;
	static constexpr Pin	adc_1		= a_1;
	static constexpr Pin	adc_2		= a_2;
	static constexpr Pin	adc_3		= a_4;
	static constexpr Pin	adc_4		= a_5;
	static constexpr Pin	adc_5		= a_6;
	static constexpr Pin	adc_6		= a_7;
	static constexpr Pin	adc_7		= b_4;
	static constexpr Pin	timer_0		= b_0;
	static constexpr Pin	timer_1		= b_1;
	static constexpr Pin	ain_0		= b_2;
	static constexpr Pin	ain_1		= b_3;
	static constexpr Pin	oc_0		= b_3;
	static constexpr Pin	usi_scl		= c_0;
	static constexpr Pin	usi_sda		= c_1;
	static constexpr Pin	usi_ss		= b_4;
	static constexpr Pin	usi_mosi	= b_5;
	static constexpr Pin	usi_miso	= b_6;
	static constexpr Pin	usi_sck		= b_7;
	static constexpr Pin	timer_osc_2	= c_7;
	static constexpr Pin	timer_osc_1	= c_6;
	static constexpr Pin	jtag_tdi	= c_5;
	static constexpr Pin	jtag_tdo	= c_4;
	static constexpr Pin	jtag_tms	= c_3;
	static constexpr Pin	jtag_tck	= c_2;
	static constexpr Pin	oc_2		= d_7;
	static constexpr Pin	icp_1		= d_6;
	static constexpr Pin	oc_1_a		= d_5;
	static constexpr Pin	oc_1_b		= d_4;
	static constexpr Pin	usart_txd	= d_1;
	static constexpr Pin	usart_rxd	= d_0;

  public:
	static constexpr uint8_t
	get_adc_number (Pin const& pin)
	{
		return
			pin == adc_0 ? 0 :
			pin == adc_1 ? 1 :
			pin == adc_2 ? 2 :
			pin == adc_3 ? 3 :
			pin == adc_4 ? 4 :
			pin == adc_5 ? 5 :
			pin == adc_6 ? 6 :
			pin == adc_7 ? 7 : -1;
	}

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
	 * General pull-up enable.
	 */
	static void
	general_enable_pull_ups()
	{
		clear_bit (SFIOR, PUD);
	}

	/**
	 * General pull-up disable. Disables pull-ups even if they're enabled individually.
	 * Does not change individual pull-up settings.
	 */
	static void
	general_disable_pull_ups()
	{
		set_bit (SFIOR, PUD);
	}

	/**
	 * Configure INT0 interrupt.
	 * Non-atomic.
	 */
	static void
	configure_int0 (IntTrigger trigger)
	{
		MCUCR = (MCUCR & 0b11111100) | static_cast<uint8_t> (trigger);
	}

	/**
	 * Configure INT1 interrupt.
	 * Non-atomic.
	 */
	static void
	configure_int1 (IntTrigger trigger)
	{
		MCUCR = (MCUCR & 0b11110011) | (static_cast<uint8_t> (trigger) << 2);
	}

	/**
	 * Enable/disable INT0.
	 */
	static void
	set_int0_enabled (bool enabled)
	{
		set_bit_value (GICR, INT0, enabled);
	}

	/**
	 * Enable/disable INT1.
	 */
	static void
	set_int1_enabled (bool enabled)
	{
		set_bit_value (GICR, INT1, enabled);
	}

	/**
	 * Enable/disable INT2.
	 */
	static void
	set_int2_enabled (bool enabled)
	{
		set_bit_value (GICR, INT2, enabled);
	}
};


typedef AtMega32 MCU;

} // namespace avr
} // namespace mulabs

#endif

