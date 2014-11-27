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

#ifndef MULABS_AVR__MCU_TX61_H__INCLUDED
#define MULABS_AVR__MCU_TX61_H__INCLUDED

// AVR:
#include <avr/io.h>
#include <avr/sleep.h>

// Local:
#include "interrupts.h"
#include "port.h"
#include "pin.h"
#include "timer.h"
#include "adc10_tx61.h"
#include "eeprom.h"


// Fixing broken C AVR "API":
#define __ADC ADC
#undef ADC


namespace mulabs {
namespace avr {

class AtTinyX61
{
  public:
	enum class IntTrigger: uint8_t
	{
		LowLevel	= 0b00,
		AnyChange	= 0b01,
		FallingEdge	= 0b10,
		RisingEdge	= 0b11,
	};

	typedef uint8_t volatile& Register;

	struct Timer0Config
	{
		static constexpr Register	ctl_a		= { TCCR0A };
		static constexpr Register	ctl_b		= { TCCR0B };
		static constexpr Register	counter_l	= { TCNT0L };
		static constexpr Register	counter_h	= { TCNT0H };
		static constexpr Register	top_l		= { OCR0A };
		static constexpr Register	top_h		= { OCR0B };
		static constexpr Register	intr_mask	= { TIMSK };
		static constexpr Register	intr_flag	= { TIFR };
	};

	typedef Timer<Timer0Config>		Timer0;
	typedef ADC10_Tx61				ADC;
	typedef EEPROM256				EEPROM;
	typedef PortTemplate<Register>	Port;
	typedef PinTemplate<Register>	Pin;

  public:
	static constexpr Timer0	timer0		= { };
	static constexpr ADC	adc			= { };
	static constexpr EEPROM	eeprom		= { };
	static constexpr Port	a			= { DDRA, PINA, PORTA, PORTA };
	static constexpr Port	b			= { DDRB, PINB, PORTB, PORTB };
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
	// Aliases:
	static constexpr Pin	reset_pin	= b_7;
	static constexpr Pin	usi_scl		= b_0;
	static constexpr Pin	usi_sda		= b_2;
	static constexpr Pin	usi_mosi	= b_0;
	static constexpr Pin	usi_miso	= b_1;
	static constexpr Pin	usi_sck		= b_2;
	static constexpr Pin	ain_0		= a_6;
	static constexpr Pin	ain_1		= a_7;
	static constexpr Pin	adc_0		= a_0;
	static constexpr Pin	adc_1		= a_1;
	static constexpr Pin	adc_2		= a_2;
	static constexpr Pin	adc_3		= a_4;
	static constexpr Pin	adc_4		= a_5;
	static constexpr Pin	adc_5		= a_6;
	static constexpr Pin	adc_6		= a_7;
	static constexpr Pin	adc_7		= b_4;
	static constexpr Pin	adc_8		= b_5;
	static constexpr Pin	adc_9		= b_6;
	static constexpr Pin	adc_10		= b_7;
	static constexpr Pin	aref		= a_3;
	static constexpr Pin	int_0		= b_6;

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
			pin == adc_7 ? 7 :
			pin == adc_8 ? 8 :
			pin == adc_9 ? 9 :
			pin == adc_10 ? 10 : -1;
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
	 * TODO verify
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
	 * TODO verify
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
	 * TODO verify
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
	 * TODO verify
	 * Wake-up sources:
	 * - INT0, INT1, and INTPCn
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
	 * TODO verify
	 * Configure INT0 interrupt.
	 * Non-atomic.
	 */
	static void
	configure_int01 (IntTrigger trigger)
	{
		MCUCR = (MCUCR & 0b01111100) | static_cast<uint8_t> (trigger);
	}

	/**
	 * TODO verify
	 * Enable INT0.
	 */
	static void
	enable_int01()
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
		set_bit (GIMSK, PCIE0);
		set_bit (GIMSK, PCIE1);
	}

	/**
	 * Disable PCINT interrupts.
	 */
	static void
	general_disable_pcints()
	{
		clear_bit (GIMSK, PCIE0);
		clear_bit (GIMSK, PCIE1);
	}

	/**
	 * Configure PCINTs to be enabled. Rest will be disabled.
	 */
	static void
	set_enabled_pcints (uint16_t mask)
	{
		PCMSK0 = mask & 0xff;
		PCMSK1 = (mask >> 8) & 0xff;
	}

	/**
	 * Enable given PCINTs. Rest will be unaffected.
	 */
	static void
	enable_pcints (uint16_t mask)
	{
		PCMSK0 |= mask & 0xff;
		PCMSK1 |= (mask >> 8) & 0xff;
	}

	/**
	 * Disable given PCINTs. Rest will be unaffected.
	 */
	static void
	disable_pcints (uint16_t mask)
	{
		PCMSK0 &= ~(mask & 0xff);
		PCMSK1 &= ~((mask >> 8) & 0xff);
	}

	/**
	 * Return mask for set_enabled_pcints(), enable_pcints(), disable_pcints().
	 */
	static uint16_t
	pcint (uint8_t number)
	{
		return bitnum<uint16_t> (number);
	}

	/**
	 * TODO verify
	 * General pull-up enable.
	 */
	static void
	general_enable_pull_ups()
	{
		clear_bit (MCUCR, PUD);
	}

	/**
	 * TODO verify
	 * General pull-up disable. Disables pull-ups even if they're enabled individually.
	 * Does not change individual pull-up settings.
	 */
	static void
	general_disable_pull_ups()
	{
		set_bit (MCUCR, PUD);
	}
};


typedef AtTinyX61 MCU;

} // namespace avr
} // namespace mulabs

#endif

