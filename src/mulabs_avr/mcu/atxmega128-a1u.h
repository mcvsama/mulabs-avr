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

#ifndef MULABS_AVR__MCU__ATXMEGA128_A1U_H__INCLUDED
#define MULABS_AVR__MCU__ATXMEGA128_A1U_H__INCLUDED

// AVR:
#include <avr/sleep.h>
#include <avr/cpufunc.h>
#include <util/delay.h>

// Mulabs AVR:
#include <mulabs_avr/avr/avr_fixes.h>
#include <mulabs_avr/avr/basic_register8.h>
#include <mulabs_avr/avr/basic_register16.h>
#include <mulabs_avr/devices/common/common_basic_io.h>
#include <mulabs_avr/devices/common/common_basic_pin_set.h>
#include <mulabs_avr/devices/xmega_au/basic_clock.h>
#include <mulabs_avr/devices/xmega_au/basic_jtag.h>
#include <mulabs_avr/devices/xmega_au/basic_pin.h>
#include <mulabs_avr/devices/xmega_au/basic_port.h>
#include <mulabs_avr/devices/xmega_au/basic_timer_01.h>
#include <mulabs_avr/devices/xmega_au/basic_usart.h>
#include <mulabs_avr/devices/xmega_au/event_system.h>
#include <mulabs_avr/devices/xmega_au/interrupt_system.h>
#include <mulabs_avr/utility/array.h>
#include <mulabs_avr/utility/type_traits.h>
#include <mulabs_avr/mcu/atxmega128-a1u-ports.h>


namespace mulabs {
namespace avr {

class ATXMega128A1U
{
  public:
	static constexpr uint8_t kNumPorts	= 11;

	using MCU				= ATXMega128A1U;
	using Register8			= BasicRegister8;
	using Register16		= BasicRegister16;
	using PortIntegerType	= uint8_t;

	using Clock				= xmega_au::BasicClock<MCU>;
	using IO				= CommonBasicIO<MCU>;
	using Pin				= xmega_au::BasicPin<MCU>;
	using Port				= xmega_au::BasicPort<MCU>;
	using PinSet			= CommonBasicPinSet<MCU>;
	using JTAG				= xmega_au::BasicJTAG<MCU>;
	using Timer01			= xmega_au::BasicTimer01<MCU>;
	using USART				= xmega_au::BasicUSART<MCU>;
	using EventSystem		= xmega_au::EventSystem;
	using InterruptSystem	= xmega_au::InterruptSystem;

	static_assert (is_literal_type<ATXMega128A1U::Register8>::value, "Register8 must be a literal type");
	static_assert (is_literal_type<ATXMega128A1U::Register16>::value, "Register16 must be a literal type");
	static_assert (is_literal_type<ATXMega128A1U::Clock>::value, "Clock must be a literal type");
	static_assert (is_literal_type<ATXMega128A1U::IO>::value, "IO must be a literal type");
	static_assert (is_literal_type<ATXMega128A1U::Pin>::value, "Pin must be a literal type");
	static_assert (is_literal_type<ATXMega128A1U::PinSet>::value, "PinSet must be a literal type");
	static_assert (is_literal_type<ATXMega128A1U::Port>::value, "Port must be a literal type");

#define MULABS_DECLARE_PORT(member_name, avr_port_prefix, port_number) \
	static constexpr Port member_name { \
		port_number, \
		avr_port_prefix##_DIR, avr_port_prefix##_DIRSET, avr_port_prefix##_DIRCLR, avr_port_prefix##_DIRTGL, \
		avr_port_prefix##_OUT, avr_port_prefix##_OUTSET, avr_port_prefix##_OUTCLR, avr_port_prefix##_OUTTGL, \
		avr_port_prefix##_IN, \
		avr_port_prefix##_INTCTRL, avr_port_prefix##_INT0MASK, avr_port_prefix##_INT1MASK, avr_port_prefix##_INTFLAGS, \
		avr_port_prefix##_PIN0CTRL, avr_port_prefix##_PIN1CTRL, avr_port_prefix##_PIN2CTRL, avr_port_prefix##_PIN3CTRL, \
		avr_port_prefix##_PIN4CTRL, avr_port_prefix##_PIN5CTRL, avr_port_prefix##_PIN6CTRL, avr_port_prefix##_PIN7CTRL, \
	};

	MULABS_DECLARE_PORT(port_a, kPORTA, 0)
	MULABS_DECLARE_PORT(port_b, kPORTB, 1)
	MULABS_DECLARE_PORT(port_c, kPORTC, 2)
	MULABS_DECLARE_PORT(port_d, kPORTD, 3)
	MULABS_DECLARE_PORT(port_e, kPORTE, 4)
	MULABS_DECLARE_PORT(port_f, kPORTF, 5)
	MULABS_DECLARE_PORT(port_h, kPORTH, 6)
	MULABS_DECLARE_PORT(port_j, kPORTJ, 7)
	MULABS_DECLARE_PORT(port_k, kPORTK, 8)
	MULABS_DECLARE_PORT(port_q, kPORTQ, 9)
	MULABS_DECLARE_PORT(port_r, kPORTR, 10)

#undef MULABS_DECLARE_PORT

	static constexpr Port	ports_index[kNumPorts] = { port_a, port_b, port_c, port_d, port_e,
													   port_f, port_h, port_j, port_k, port_q, port_r };

#define MULABS_DECLARE_TIMER_01(member_name, t) \
	static constexpr Timer01 member_name { \
		t##_CTRLA, t##_CTRLB, t##_CTRLC, t##_CTRLD, t##_CTRLE, \
		t##_INTCTRLA, t##_INTCTRLB, \
		t##_CTRLFCLR, t##_CTRLFSET, \
		t##_CTRLGCLR, t##_CTRLGSET, \
		t##_INTFLAGS, t##_TEMP, \
		t##_CNT, t##_CNTL, t##_CNTH, \
		t##_PER, t##_PERL, t##_PERH, t##_PERBUF, t##_PERBUFL, t##_PERBUFH, \
		t##_CCA, t##_CCAL, t##_CCAH, t##_CCABUF, t##_CCABUFL, t##_CCABUFH, \
		t##_CCB, t##_CCBL, t##_CCBH, t##_CCBBUF, t##_CCBBUFL, t##_CCBBUFH, \
		t##_CCC, t##_CCCL, t##_CCCH, t##_CCCBUF, t##_CCCBUFL, t##_CCCBUFH, \
		t##_CCD, t##_CCDL, t##_CCDH, t##_CCDBUF, t##_CCDBUFL, t##_CCDBUFH \
	};

	MULABS_DECLARE_TIMER_01(timer_c0, kTCC0)
	MULABS_DECLARE_TIMER_01(timer_c1, kTCC1)
	MULABS_DECLARE_TIMER_01(timer_d0, kTCD0)
	MULABS_DECLARE_TIMER_01(timer_d1, kTCD1)
	MULABS_DECLARE_TIMER_01(timer_e0, kTCE0)
	MULABS_DECLARE_TIMER_01(timer_e1, kTCE1)
	MULABS_DECLARE_TIMER_01(timer_f0, kTCF0)
	MULABS_DECLARE_TIMER_01(timer_f1, kTCF1)

#undef MULABS_DECLARE_TIMER_01

#define MULABS_DECLARE_USART(member_name, u) \
	static constexpr USART member_name { \
		u##_DATA, u##_STATUS, \
		u##_CTRLA, u##_CTRLB, u##_CTRLC, \
		u##_BAUDCTRLA, u##_BAUDCTRLB \
	};

	MULABS_DECLARE_USART(usart_c0, kUSARTC0)
	MULABS_DECLARE_USART(usart_c1, kUSARTC1)
	MULABS_DECLARE_USART(usart_d0, kUSARTD0)
	MULABS_DECLARE_USART(usart_d1, kUSARTD1)
	MULABS_DECLARE_USART(usart_e0, kUSARTE0)
	MULABS_DECLARE_USART(usart_e1, kUSARTE1)
	MULABS_DECLARE_USART(usart_f0, kUSARTF0)
	MULABS_DECLARE_USART(usart_f1, kUSARTF1)

#undef MULABS_DECLARE_USART

  public:
	/**
	 * Execute single no-operation instruction.
	 */
	static void
	nop()
	{
		_NOP();
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
	 * Disable configuration change protection for writing to
	 * a protected register for maximum period of 4 instructions.
	 * Interrupts are disabled during those 4 instructions.
	 */
	static void
	disable_configuration_change_protection_for_register();

	/**
	 * Disable configuration change protection for changing
	 * SPM/LPM for a maximum period of 4 instructions.
	 * Interrupts are disabled during those 4 instructions.
	 */
	static void
	disable_configuration_change_protection_for_spmlpm();
};


constexpr uint8_t ATXMega128A1U::kNumPorts;
constexpr ATXMega128A1U::Port ATXMega128A1U::port_a;
constexpr ATXMega128A1U::Port ATXMega128A1U::port_b;
constexpr ATXMega128A1U::Port ATXMega128A1U::port_c;
constexpr ATXMega128A1U::Port ATXMega128A1U::port_d;
constexpr ATXMega128A1U::Port ATXMega128A1U::port_e;
constexpr ATXMega128A1U::Port ATXMega128A1U::port_f;


inline void
ATXMega128A1U::disable_configuration_change_protection_for_register()
{
	CCP = 0xd8;
}


inline void
ATXMega128A1U::disable_configuration_change_protection_for_spmlpm()
{
	CCP = 0x9d;
}

} // namespace avr
} // namespace mulabs

#endif

