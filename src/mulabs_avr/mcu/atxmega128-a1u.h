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
	static_assert (is_literal_type<ATXMega128A1U::PinSet>::value, "PinSet must be a literal type");
	static_assert (is_literal_type<ATXMega128A1U::JTAG>::value, "JTAG must be a literal type");
	static_assert (is_literal_type<ATXMega128A1U::Timer01>::value, "Timer01 must be a literal type");
	static_assert (is_literal_type<ATXMega128A1U::USART>::value, "USART must be a literal type");
	static_assert (is_literal_type<ATXMega128A1U::USB>::value, "USB must be a literal type");
	static_assert (is_literal_type<ATXMega128A1U::EventSystem>::value, "EventSystem must be a literal type");
	static_assert (is_literal_type<ATXMega128A1U::InterruptSystem>::value, "InterruptSystem must be a literal type");

	static constexpr Port		port_a		{ 0x0600, 0 };
	static constexpr Port		port_b		{ 0x0620, 1 };
	static constexpr Port		port_c		{ 0x0640, 2 };
	static constexpr Port		port_d		{ 0x0660, 3 };
	static constexpr Port		port_e		{ 0x0680, 4 };
	static constexpr Port		port_f		{ 0x06A0, 5 };
	static constexpr Port		port_h		{ 0x06E0, 6 };
	static constexpr Port		port_j		{ 0x0700, 7 };
	static constexpr Port		port_k		{ 0x0720, 8 };
	static constexpr Port		port_q		{ 0x07C0, 9 };
	static constexpr Port		port_r		{ 0x07E0, 10 };

	static constexpr Port	ports_index[kNumPorts] = { port_a, port_b, port_c, port_d, port_e,
													   port_f, port_h, port_j, port_k, port_q, port_r };

	static constexpr Timer01	timer_c0	{ 0x0800 };
	static constexpr Timer01	timer_c1	{ 0x0840 };
	static constexpr Timer01	timer_d0	{ 0x0900 };
	static constexpr Timer01	timer_d1	{ 0x0940 };
	static constexpr Timer01	timer_e0	{ 0x0a00 };
	static constexpr Timer01	timer_e1	{ 0x0a40 };
	static constexpr Timer01	timer_f0	{ 0x0b00 };
	static constexpr Timer01	timer_f1	{ 0x0b40 };

	static constexpr USART		usart_c0	{ 0X08a0 };
	static constexpr USART		usart_c1	{ 0X08b0 };
	static constexpr USART		usart_d0	{ 0X09a0 };
	static constexpr USART		usart_d1	{ 0X09b0 };
	static constexpr USART		usart_e0	{ 0X0aa0 };
	static constexpr USART		usart_e1	{ 0X0ab0 };
	static constexpr USART		usart_f0	{ 0X0ba0 };
	static constexpr USART		usart_f1	{ 0X0bb0 };

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

