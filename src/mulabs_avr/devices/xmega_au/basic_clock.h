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

		enum class Oscillator: uint8_t
		{
			RC2MHz			= 0b000,
			RC32MHz			= 0b001,
			RC32kHz			= 0b010,
			XOSC			= 0b011,
			PLL				= 0b100,
		};

		enum class PrescalerADiv: uint8_t
		{
			Div1			= 0b00000 << 2,
			Div2			= 0b00001 << 2,
			Div4			= 0b00011 << 2,
			Div8			= 0b00101 << 2,
			Div16			= 0b00111 << 2,
			Div32			= 0b01001 << 2,
			Div64			= 0b01011 << 2,
			Div128			= 0b01101 << 2,
			Div256			= 0b01111 << 2,
			Div512			= 0b10001 << 2,
		};

		enum class PrescalerBCDiv: uint8_t
		{
			DivB1C1			= 0b00,
			DivB1C2			= 0b01,
			DivB4C1			= 0b10,
			DivB2C2			= 0b11,
		};

		enum class RTCSource: uint8_t
		{
			ULP				= 0b000 << 1,
			TOsc1024Hz		= 0b001 << 1,
			RCOsc1024Hz		= 0b010 << 1,
			TOsc32768Hz		= 0b101 << 1,
			RCOsc32768Hz	= 0b110 << 1,
			ExternalClock	= 0b111 << 1,
		};

		enum class USBClockSource: uint8_t
		{
			PLL				= 0b00 << 1,
			RC32MHz			= 0b01 << 1,
		};

		enum class USBClockPrescaler: uint8_t
		{
			Div1			= 0b000 << 3,
			Div2			= 0b001 << 3,
			Div4			= 0b010 << 3,
			Div8			= 0b011 << 3,
			Div16			= 0b100 << 3,
			Div32			= 0b101 << 3,
		};

		enum class PLLSource: uint8_t
		{
			RC2MHz			= 0b00 << 6,
			RC32MHz			= 0b10 << 6,
			XOSC			= 0b11 << 6,
		};

		enum class DFLL
		{
			RC32MHz,
			RC2MHz,
		};

		enum class DFLLCalibrationReference32M: uint8_t
		{
			RC32768Hz		= 0b00 << 1,
			XOSC32768Hz		= 0b01 << 1,
			USBSOF			= 0b10 << 1,
		};

		enum class DFLLCalibrationReference2M
		{
			RC32768Hz,
			TOSC32768Hz,
		};

		enum class XOSCFrequency: uint8_t
		{
			Range400kTo2MHz	= 0b00 << 6,
			Range2To9MHz	= 0b01 << 6,
			Range9To12MHz	= 0b10 << 6,
			Range12To16MHz	= 0b11 << 6,
		};

		/**
		 * When the external oscillator is used as the reference for a DFLL, only EXTCLK and 32KHZ can be selected.
		 */
		enum class XOSCType: uint8_t
		{
			ExternalClock	= 0b0000,
			TOSC32768Hz		= 0b0010,

			// This option should be used only when frequency stability at startup is not important for the application.
			// The option is not suitable for crystals.
			XTAL256CLK		= 0b0011,

			// This option is intended for use with ceramic resonators. It can also be used when the frequency stability
			// at startup is not important for the application.
			XTAL1kCLK		= 0b0111,

			XTAL16kCLK		= 0b1011,
		};

	  public:
		/**
		 * Enable selected oscillators.
		 */
		template<class ...Oscillators>
			static void
			enable (Oscillators...);

		/**
		 * Disable selected oscillators.
		 */
		template<class ...Oscillators>
			static void
			disable (Oscillators...);

		/**
		 * Return true if indicated oscillator is stable and ready to use.
		 */
		static bool
		is_ready (Oscillator);

		/**
		 * Wait for all listed oscillators to stabilize.
		 */
		template<class ...Oscillators>
			static void
			wait_for (Oscillators...);

		/**
		 * Select clock used for CPU.
		 * The clock must be enabled prior to selection (see enable_clock(...)).
		 * This function will wait until clock source is stable, before switching.
		 */
		static void
		set_cpu_clock (Oscillator);

		/**
		 * Lock system clock - prevent clock configuration changes
		 * until MCU reset.
		 */
		static void
		lock_system_clock();

		/**
		 * Select prescaler A division.
		 */
		static void
		set (PrescalerADiv);

		/**
		 * Select prescalers B and C division.
		 */
		static void
		set (PrescalerBCDiv);

		/**
		 * Select the clock source for the real-time counter.
		 */
		static void
		set (RTCSource);

		/**
		 * Enable/disable RTC counter.
		 */
		static void
		set_rtc_enabled (bool);

		/**
		 * Set USB clock prescaler.
		 */
		static void
		set (USBClockPrescaler);

		/**
		 * Set USB clock source.
		 */
		static void
		set (USBClockSource);

		/**
		 * Enable/disable USB clock.
		 */
		static void
		set_usb_enabled (bool);

		/**
		 * Set external crystal frequency range.
		 * Needed before starting XOSC oscillator.
		 */
		static void
		set (XOSCFrequency);

		/**
		 * Select external oscillator type and start-up time.
		 */
		static void
		set (XOSCType);

		/**
		 * Set low power mode for external 32 kHz oscillator.
		 */
		static void
		set_low_power_32k_osc (bool);

		/**
		 * Set high power mode for 0.4-16 MHz external oscillator.
		 */
		static void
		set_high_power_16M_xosc (bool);

		// TODO XOSCFAIL
		// TODO RC32KCAL

		/**
		 * Set source/reference clock for PLL.
		 */
		static void
		set (PLLSource);

		/**
		 * Enabling this will divide output frequency from PLL by 2.
		 */
		static void
		set_pll_output_divided_by_2 (bool set);

		/**
		 * Set PLL multiplication factor.
		 */
		template<uint8_t Factor>
			static void
			set_pll_multiplication_factor();

		/**
		 * Select oscillator calibration reference for 32 MHz RC oscillator.
		 */
		static void
		set (DFLLCalibrationReference32M);

		/**
		 * Select oscillator calibration reference for 2 MHz RC oscillator.
		 */
		static void
		set (DFLLCalibrationReference2M);

		/**
		 * Set frequency ratio between the oscillator and the reference clock,
		 * which is 1024 Hz (actual reference clock is 32768 divided by 32).
		 */
		static void
		set_dfll_multiplication_factor (DFLL, uint16_t factor);

		/**
		 * Get DFLL fine calibration value.
		 */
		static uint8_t
		dfll_fine_calibration_value (DFLL);

		/**
		 * Set DFLL fine calibration value (CALA register).
		 *
		 * Can be called only when DFLL is disabled.
		 */
		static void
		set_dfll_fine_calibration_value (DFLL, uint8_t cal_a);

		/**
		 * Get DFLL coarse calibration value.
		 */
		static uint8_t
		dfll_coarse_calibration_value (DFLL);

		/**
		 * Set DFLL coarse calibration value (CALB register).
		 *
		 * Can be called only when DFLL is disabled.
		 */
		static void
		set_dfll_coarse_calibration_value (DFLL, uint8_t cal_b);

		/**
		 * Enable/disable DFLL.
		 */
		static void
		set_dfll_enabled (DFLL, bool);

	  private:
		/**
		 * Helper for enable()/disable() of Oscillators.
		 * Collect list of oscillators and prepare value for the OSC_CTRL/OSC_STATUS registers.
		 */
		template<class ...Oscillators>
			static constexpr uint8_t
			make_oscillators_list (Oscillator oscillator, Oscillators ...oscillators)
			{
				return make_oscillators_list (oscillator) | make_oscillators_list (oscillators...);
			}

		/**
		 * Recursive stop-condition for make_oscillators_list().
		 */
		static constexpr uint8_t
		make_oscillators_list (Oscillator oscillator)
		{
			return get_oscillator_bit (oscillator);
		}

		/**
		 * Return oscillator bit for registers like OSC_CTRL or OSC_STATUS.
		 */
		static constexpr uint8_t
		get_oscillator_bit (Oscillator oscillator)
		{
			return 1u << static_cast<uint8_t> (oscillator);
		}
	};


template<class M>
	template<class ...Oscillators>
		inline void
		BasicClock<M>::enable (Oscillators ...oscillators)
		{
			OSC_CTRL = OSC_CTRL | make_oscillators_list (oscillators...);
		}


template<class M>
	template<class ...Oscillators>
		inline void
		BasicClock<M>::disable (Oscillators ...oscillators)
		{
			OSC_CTRL = OSC_CTRL & ~make_oscillators_list (oscillators...);
		}


template<class M>
	inline bool
	BasicClock<M>::is_ready (Oscillator oscillator)
	{
		return OSC_STATUS & get_oscillator_bit (oscillator);
	}


template<class M>
	template<class ...Oscillators>
		inline void
		BasicClock<M>::wait_for (Oscillators ...oscillators)
		{
			uint8_t bits = make_oscillators_list (oscillators...);

			while ((OSC_STATUS & bits) != bits)
				continue;
		}


template<class M>
	inline void
	BasicClock<M>::set_cpu_clock (Oscillator oscillator)
	{
		// Wait for the oscillator to stabilize:
		wait_for (oscillator);
		// Select the clock source:
		MCU::disable_configuration_change_protection_for_register();
		CLK_CTRL = static_cast<uint8_t> (oscillator);
	}


template<class M>
	inline void
	BasicClock<M>::lock_system_clock()
	{
		MCU::disable_configuration_change_protection_for_register();
		CLK_LOCK = 0b00000001;
	}


template<class M>
	inline void
	BasicClock<M>::set (PrescalerADiv prescaler)
	{
		MCU::disable_configuration_change_protection_for_register();
		CLK_PSCTRL = (CLK_PSCTRL & 0b10000011) | static_cast<uint8_t> (prescaler);
	}


template<class M>
	inline void
	BasicClock<M>::set (PrescalerBCDiv prescaler)
	{
		MCU::disable_configuration_change_protection_for_register();
		CLK_PSCTRL = (CLK_PSCTRL & 0b11111100) | static_cast<uint8_t> (prescaler);
	}


template<class M>
	inline void
	BasicClock<M>::set (RTCSource source)
	{
		CLK_RTCCTRL = (CLK_RTCCTRL & 0b11110001) | static_cast<uint8_t> (source);
	}


template<class M>
	inline void
	BasicClock<M>::set_rtc_enabled (bool enabled)
	{
		CLK_RTCCTRL = (CLK_RTCCTRL & 0b11111110) | (enabled ? 1 : 0);
	}


template<class M>
	inline void
	BasicClock<M>::set (USBClockPrescaler prescaler)
	{
		CLK_USBCTRL = (CLK_USBCTRL & 0b11000111) | static_cast<uint8_t> (prescaler);
	}


template<class M>
	inline void
	BasicClock<M>::set (USBClockSource source)
	{
		CLK_USBCTRL = (CLK_USBCTRL & 0b11111001) | static_cast<uint8_t> (source);
	}


template<class M>
	inline void
	BasicClock<M>::set_usb_enabled (bool enabled)
	{
		CLK_USBCTRL = (CLK_USBCTRL & 0b11111110) | (enabled ? 1 : 0);
	}


template<class M>
	inline void
	BasicClock<M>::set (XOSCFrequency range)
	{
		OSC_XOSCCTRL = (OSC_XOSCCTRL & ~0b11000000) | static_cast<uint8_t> (range);
	}


template<class M>
	inline void
	BasicClock<M>::set (XOSCType type)
	{
		OSC_XOSCCTRL = (OSC_XOSCCTRL & ~0b00001111) | static_cast<uint8_t> (type);
	}


template<class M>
	inline void
	BasicClock<M>::set_low_power_32k_osc (bool enabled)
	{
		OSC_XOSCCTRL = (OSC_XOSCCTRL & ~0b00100000) | (enabled ? 0b00100000 : 0);
	}


template<class M>
	inline void
	BasicClock<M>::set_high_power_16M_xosc (bool enabled)
	{
		OSC_XOSCCTRL = (OSC_XOSCCTRL & ~0b00010000) | (enabled ? 0b00010000 : 0);
	}


template<class M>
	inline void
	BasicClock<M>::set (PLLSource source)
	{
		OSC_PLLCTRL = (OSC_PLLCTRL & 0b00111111) | static_cast<uint8_t> (source);
	}


template<class M>
	inline void
	BasicClock<M>::set_pll_output_divided_by_2 (bool set)
	{
		OSC_PLLCTRL = (OSC_PLLCTRL & 0b11011111) | (set ? 0b00100000 : 0);
	}


template<class M>
	template<uint8_t Factor>
		inline void
		BasicClock<M>::set_pll_multiplication_factor()
		{
			static_assert (Factor >= 1 && Factor <= 31);

			OSC_PLLCTRL = (OSC_PLLCTRL & 0b11100000) | Factor;
		}


template<class M>
	inline void
	BasicClock<M>::set (DFLLCalibrationReference32M reference)
	{
		OSC_DFLLCTRL = (OSC_DFLLCTRL & 0b11111001) | static_cast<uint8_t> (reference);
	}


template<class M>
	inline void
	BasicClock<M>::set (DFLLCalibrationReference2M reference)
	{
		switch (reference)
		{
			case DFLLCalibrationReference2M::RC32768Hz:
				OSC_DFLLCTRL &= ~0b1;
				break;

			case DFLLCalibrationReference2M::TOSC32768Hz:
				OSC_DFLLCTRL |= 0b1;
				break;
		}
	}


template<class M>
	inline void
	BasicClock<M>::set_dfll_multiplication_factor (DFLL dfll, uint16_t factor)
	{
		uint8_t lsb = (factor >> 0) & 0xff;
		uint8_t msb = (factor >> 8) & 0xff;

		switch (dfll)
		{
			case DFLL::RC32MHz:
				DFLLRC32M_COMP1 = lsb;
				DFLLRC32M_COMP2 = msb;
				break;

			case DFLL::RC2MHz:
				DFLLRC2M_COMP1 = lsb;
				DFLLRC2M_COMP2 = msb;
				break;
		}
	}


template<class M>
	inline uint8_t
	BasicClock<M>::dfll_fine_calibration_value (DFLL dfll)
	{
		switch (dfll)
		{
			case DFLL::RC32MHz:
				return DFLLRC32M_CALA;

			case DFLL::RC2MHz:
				return DFLLRC2M_CALA;
		}
	}


template<class M>
	inline void
	BasicClock<M>::set_dfll_fine_calibration_value (DFLL dfll, uint8_t cal_a)
	{
		switch (dfll)
		{
			case DFLL::RC32MHz:
				DFLLRC32M_CALA = cal_a;
				break;

			case DFLL::RC2MHz:
				DFLLRC2M_CALA = cal_a;
				break;
		}
	}


template<class M>
	inline uint8_t
	BasicClock<M>::dfll_coarse_calibration_value (DFLL dfll)
	{
		switch (dfll)
		{
			case DFLL::RC32MHz:
				return DFLLRC32M_CALB;

			case DFLL::RC2MHz:
				return DFLLRC2M_CALB;
		}
	}


template<class M>
	inline void
	BasicClock<M>::set_dfll_coarse_calibration_value (DFLL dfll, uint8_t cal_b)
	{
		switch (dfll)
		{
			case DFLL::RC32MHz:
				DFLLRC32M_CALB = cal_b;
				break;

			case DFLL::RC2MHz:
				DFLLRC2M_CALB = cal_b;
				break;
		}
	}


template<class M>
	inline void
	BasicClock<M>::set_dfll_enabled (DFLL dfll, bool enabled)
	{
		switch (dfll)
		{
			case DFLL::RC32MHz:
				DFLLRC32M_CTRL = (DFLLRC32M_CTRL & 0b11111110) | (enabled ? 1 : 0);
				break;

			case DFLL::RC2MHz:
				DFLLRC2M_CTRL = (DFLLRC2M_CTRL & 0b11111110) | (enabled ? 1 : 0);
				break;
		}
	}

} // namespace xmega_au
} // namespace avr
} // namespace mulabs

#endif

