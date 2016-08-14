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

#ifndef MULABS_AVR__DEVICES__ADC10_TX5_H__INCLUDED
#define MULABS_AVR__DEVICES__ADC10_TX5_H__INCLUDED

// Standard:
#include <stdint.h>

// AVR:
#include <avr/io.h>
#include <avr/cpufunc.h>

// Local:
#include "utility.h"


namespace mulabs {
namespace avr {

class ADC10_Tx5
{
  public:
	enum class Reference: uint8_t
	{
		Vcc						= 0b00000000,
		PB0Pin					= 0b01000000,
		Int1V1					= 0b10000000,
		Int2V56NoCapacitor		= 0b10010000,
		Int2V56WithCapacitor	= 0b11010000,
	};

	enum class AutoTriggerSource: uint8_t
	{
		FreeRunning				= 0b000,
		AnalogComparator		= 0b001,
		Interrupt0				= 0b010,
		Timer0_MatchA			= 0b011,
		Timer0_Overflow			= 0b100,
		Timer0_MatchB			= 0b101,
		PinChangeInterrupt		= 0b110,
	};

  private:
	static constexpr uint8_t ADMUXReferenceMask			= 0b11010000;
	static constexpr uint8_t ADMUXInputMask				= 0b00001111;
	static constexpr uint8_t ADCSRAScaleMask			= 0b00000111;
	static constexpr uint8_t ADCSRBTriggerSourceMask	= 0b00000111;

  public:
	/**
	 * Enable/disable converter hardware.
	 * Remember to disable any pull-up resistors on ADC channels you want to use.
	 */
	static void
	set_enabled (bool enabled) noexcept
	{
		if (enabled)
			set_bit (ADCSRA, ADEN);
		else
			clear_bit (ADCSRA, ADEN);
	}

	/**
	 * Enable/disable free-running mode.
	 */
	static void
	set_free_running (bool free_running) noexcept
	{
		if (free_running)
			set_bit (ADCSRA, 5);
		else
			clear_bit (ADCSRA, 5);
	}

	/**
	 * Enable conversion interrupt.
	 */
	static void
	set_interrupt_enabled (bool enabled) noexcept
	{
		if (enabled)
			set_bit (ADCSRA, ADIF);
		else
			clear_bit (ADCSRA, ADIF);
	}

	/**
	 * Enable/disable auto-triggering.
	 */
	static void
	set_auto_triggering (bool enabled) noexcept
	{
		if (enabled)
			set_bit (ADCSRA, ADATE);
		else
			clear_bit (ADCSRA, ADATE);
	}

	/**
	 * Select source of auto-triggering.
	 */
	static void
	select_auto_trigger_source (AutoTriggerSource source) noexcept
	{
		ADCSRB = (ADCSRB & ~ADCSRBTriggerSourceMask) | static_cast<uint8_t> (source);
	}

	/**
	 * Select conversion input (0 == ADC0, 1 == ADC1…).
	 * Won't have any effect until ADC is enabled, see set_enabled().
	 */
	static void
	select_input (uint8_t input) noexcept
	{
		ADMUX = (ADMUX & ~ADMUXInputMask) | (input & 0x0f);
		_NOP();
	}

	/**
	 * Set prescaler option.
	 * The input is CPU clock, and the output clocks the ADC module.
	 * For good resolution, the ADC module should be clocked at 50…200 kHz.
	 *
	 * \param	prescale_factor
	 * 			CPU clock division factor to drive ADC module.
	 * 			Valid values are powers of 2, in range [1..256].
	 */
	template<unsigned int scale_factor>
		static void
		set_clock_prescaler() noexcept
		{
			static_assert (scale_factor == 2 ||
						   scale_factor == 4 ||
						   scale_factor == 8 ||
						   scale_factor == 16 ||
						   scale_factor == 32 ||
						   scale_factor == 64 ||
						   scale_factor == 128, "scale_factor must be power of 2 [2..128]");

			ADCSRA &= ~ADCSRAScaleMask;
			switch (scale_factor)
			{
				case 2:		ADCSRA |= 0b00000001; break;
				case 4:		ADCSRA |= 0b00000010; break;
				case 8:		ADCSRA |= 0b00000011; break;
				case 16:	ADCSRA |= 0b00000100; break;
				case 32:	ADCSRA |= 0b00000101; break;
				case 64:	ADCSRA |= 0b00000110; break;
				case 128:	ADCSRA |= 0b00000111; break;
			}
		}

	/**
	 * Select voltage reference for the converter.
	 * Won't have any effect until ADC is enabled, see set_enabled().
	 */
	static void
	select_reference (Reference reference) noexcept
	{
		ADMUX = (ADMUX & ~ADMUXReferenceMask) | static_cast<uint8_t> (reference);
	}

	/**
	 * Start conversion and return immediately.
	 * You can check if the result is ready with
	 * ready() method.
	 */
	static void
	sample() noexcept
	{
		set_bit (ADCSRA, ADSC);
	}

	/**
	 * Read data from ADC registers.
	 */
	static uint16_t
	read() noexcept
	{
		return ADCL + ADCH * 256;
	}

	/**
	 * Read data as float converted to voltage.
	 */
	static float
	read_relative() noexcept
	{
		return read() / 1024.0f;
	}

	/**
	 * Wait until conversion is complete
	 * and then return data from ADC registers.
	 */
	static uint16_t
	wait_and_read() noexcept
	{
		while (!ready())
		{ }

		return read();
	}

	/**
	 * Wait until conversion is complete
	 * and then return relative value.
	 */
	static float
	wait_and_read_relative() noexcept
	{
		return wait_and_read() / 1024.0f;
	}

	/**
	 * Start conversion, wait for completion
	 * and return data from ADC registers.
	 */
	static uint16_t
	sample_and_read() noexcept
	{
		sample();
		return wait_and_read();
	}

	/**
	 * Start conversion, wait for completion
	 * and return relative value.
	 */
	static float
	sample_and_read_relative() noexcept
	{
		return sample_and_read() / 1024.0f;
	}

	/**
	 * Return true if ADC conversion is done.
	 */
	static bool
	ready() noexcept
	{
		return !get_bit (ADCSRA, ADSC);
	}

	/**
	 * Convert result to voltage.
	 */
	static float
	convert_to_voltage (uint16_t value, float reference_voltage) noexcept
	{
		return value * reference_voltage / 1024.0f;
	}
};

} // namespace avr
} // namespace mulabs

#endif

