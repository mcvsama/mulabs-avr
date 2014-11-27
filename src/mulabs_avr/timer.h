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

#ifndef MULABS_AVR__TIMER_H__INCLUDED
#define MULABS_AVR__TIMER_H__INCLUDED

// AVR:
#include <avr/io.h>


namespace mulabs {
namespace avr {

template<class pConfig>
	class Timer
	{
	  public:
		typedef pConfig Config;

		enum class Mode: uint8_t
		{
			Normal8Bit			= 0b00000000,
			CTC8Bit				= 0b00000001,
			Normal16Bit			= 0b10000000,
			InputCapture8Bit	= 0b01000000,
			InputCapture16Bit	= 0b11000000,
		};

		enum class ExternalClockSource: uint8_t
		{
			FallingEdge			= 0b110,
			RisingEdge			= 0b111,
		};

	  private:
		static constexpr uint8_t ModeMask			= 0b11000001;
		static constexpr uint8_t PrescalerMask 		= 0b00000111;

	  public:
		/**
		 * Set mode.
		 */
		static void
		set_mode (Mode mode)
		{
			Config::ctl_a = (Config::ctl_a & ~ModeMask) | static_cast<uint8_t> (mode);
		}

		/**
		 * Set prescaler option.
		 * The input is CPU clock, and the output clocks the timer.
		 *
		 * \param	prescale_factor
		 * 			CPU clock division factor to drive the timer module.
		 * 			Valid values are: 8, 64, 256, 1024.
		 */
		template<unsigned int scale_factor>
			static void
			set_clock_prescaler() noexcept
			{
				static_assert (scale_factor == 1 ||
							   scale_factor == 8 ||
							   scale_factor == 64 ||
							   scale_factor == 256 ||
							   scale_factor == 1024, "scale_factor must be 1, 8, 64, 256 or 1024");

				switch (scale_factor)
				{
					case 1:		Config::ctl_b = (Config::ctl_b & ~PrescalerMask) | 0b001; break;
					case 8:		Config::ctl_b = (Config::ctl_b & ~PrescalerMask) | 0b010; break;
					case 64:	Config::ctl_b = (Config::ctl_b & ~PrescalerMask) | 0b011; break;
					case 256:	Config::ctl_b = (Config::ctl_b & ~PrescalerMask) | 0b100; break;
					case 1024:	Config::ctl_b = (Config::ctl_b & ~PrescalerMask) | 0b101; break;
				}
			}

		/**
		 * Stop timer completely from running.
		 * To reenable use set_external_clock_source() or set_clock_prescaler().
		 */
		static void
		stop_timer()
		{
			Config::ctl_b = (Config::ctl_b & ~PrescalerMask) | 0b000;
		}

		/**
		 * Set timer clock source to external.
		 */
		static void
		set_external_clock_source (ExternalClockSource source)
		{
			Config::ctl_b = (Config::ctl_b & ~PrescalerMask) | static_cast<uint8_t> (source);
		}

		/**
		 * Put timer on hold, so that configuration can be safely changed.
		 */
		static void
		set_on_hold (bool hold)
		{
			if (hold)
				set_bit (Config::ctl_b, TSM);
			else
				clear_bit (Config::ctl_b, TSM);
		}

		/**
		 * Reset the prescaler, so it starts counting from 0.
		 */
		static void
		reset_prescaler()
		{
			set_bit (Config::ctl_b, PSR0);
		}

		/**
		 * Set current 8-bit timer value.
		 *
		 * Use this when timer runs in 8-bit mode.
		 * This will temporarily disable compare matching (if used), see datasheet.
		 */
		static void
		set_value_8bit (uint8_t value)
		{
			Config::counter_l = value;
		}

		/**
		 * Read current 8-bit timer value.
		 * This will temporarily disable compare matching (if used), see datasheet.
		 */
		static uint8_t
		value_8bit()
		{
			return Config::counter_l;
		}

		/**
		 * Set current 16-bit timer value.
		 *
		 * Use this when timer runs in 16-bit mode.
		 * Interrupts should be disabled if atomicity is needed.
		 */
		static void
		set_value_16bit (uint16_t value)
		{
			Config::counter_h = (value >> 8) & 0xff;
			Config::counter_l = value & 0xff;
		}

		/**
		 * Read current 16-bit timer value.
		 *
		 * Use this when timer runs in 16-bit mode.
		 * Interrupts should be disabled if atomicity is needed.
		 */
		static uint16_t
		value_16bit()
		{
			uint16_t low = Config::counter_l;
			return low | static_cast<uint16_t> (Config::counter_h) << 8;
		}

		/**
		 * Set top value for the counter, that is value at which
		 * counter restarts.
		 *
		 * Use this when timer runs in 8-bit mode.
		 */
		static void
		set_top_value_8bit (uint8_t value)
		{
			Config::top_l = value;
		}

		/**
		 * Read top value for the counter.
		 *
		 * Use this when timer runs in 8-bit mode.
		 */
		static uint8_t
		top_value_8bit()
		{
			return Config::top_l;
		}

		/**
		 * Set top value for the counter, that is value at which
		 * counter restarts.
		 *
		 * Use this when timer runs in 16-bit mode.
		 * Interrupts should be disabled if atomicity is needed.
		 */
		static void
		set_top_value_16bit (uint8_t value)
		{
			Config::top_l = value & 0xff;
			Config::top_h = (value >> 8) & 0xff;
		}

		/**
		 * Read top value for the counter.
		 *
		 * Use this when timer runs in 16-bit mode.
		 * Interrupts should be disabled if atomicity is needed.
		 */
		static void
		top_value_16bit()
		{
			uint16_t low = Config::top_l;
			return low | static_cast<uint16_t> (Config::top_h) << 8;
		}

		/**
		 * Enable/disable overflow interrupt.
		 */
		static void
		set_overflow_interrupt_enabled (bool enabled)
		{
			if (enabled)
				set_bit (Config::intr_mask, TOIE0);
			else
				clear_bit (Config::intr_mask, TOIE0);
		}

		/**
		 * Enable/disable compare-match-A interrupt.
		 * Use also for 16-bit mode.
		 */
		static void
		set_compare_a_interrupt_enabled (bool enabled)
		{
			if (enabled)
				set_bit (Config::intr_mask, OCIE0A);
			else
				clear_bit (Config::intr_mask, OCIE0A);
		}

		/**
		 * Enable/disable compare-match-B interrupt.
		 * Use also for 16-bit mode.
		 */
		static void
		set_compare_b_interrupt_enabled (bool enabled)
		{
			if (enabled)
				set_bit (Config::intr_mask, OCIE0B);
			else
				clear_bit (Config::intr_mask, OCIE0B);
		}
	};

} // namespace avr
} // namespace mulabs

#endif

