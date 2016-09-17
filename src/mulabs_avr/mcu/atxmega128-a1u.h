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
#include <mulabs_avr/devices/xmega_au/basic_clock.h>
#include <mulabs_avr/devices/xmega_au/basic_io.h>
#include <mulabs_avr/devices/xmega_au/basic_pin.h>
#include <mulabs_avr/devices/xmega_au/basic_pin_set.h>
#include <mulabs_avr/devices/xmega_au/basic_port.h>
#include <mulabs_avr/utility/array.h>
#include <mulabs_avr/utility/type_traits.h>
#include <mulabs_avr/mcu/atxmega128-a1u-ports.h>


namespace mulabs {
namespace avr {

class ATXMega128A1U
{
  public:
	static constexpr uint8_t kNumPorts	= 11;

	typedef ATXMega128A1U						MCU;
	typedef BasicRegister8						Register8;
	typedef BasicRegister16						Register16;
	typedef uint8_t								PortIntegerType;
	typedef Array<PortIntegerType, kNumPorts>	Pins;

	typedef xmega_au::BasicClock<MCU>			Clock;
	typedef xmega_au::BasicIO<MCU>				IO;
	typedef xmega_au::BasicPin<MCU>				Pin;
	typedef xmega_au::BasicPort<MCU>			Port;
	typedef xmega_au::BasicPinSet<MCU>			PinSet;

	static_assert (is_literal_type<Register8>::value, "Register8 must be a literal type");
	static_assert (is_literal_type<Register16>::value, "Register16 must be a literal type");
	static_assert (is_literal_type<Clock>::value, "Clock must be a literal type");
	static_assert (is_literal_type<IO>::value, "IO must be a literal type");
	static_assert (is_literal_type<Pin>::value, "Pin must be a literal type");
	static_assert (is_literal_type<PinSet>::value, "PinSet must be a literal type");
	static_assert (is_literal_type<Port>::value, "Port must be a literal type");
	static_assert (is_literal_type<Pins>::value, "Pins must be a literal type");

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

