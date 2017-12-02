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

#ifndef MULABS_AVR__MCU__ATXMEGA32_U4_H__INCLUDED
#define MULABS_AVR__MCU__ATXMEGA32_U4_H__INCLUDED

// AVR:
#include <avr/sleep.h>
#include <avr/cpufunc.h>
#include <util/delay.h>

// Mulabs AVR:
#include <mulabs_avr/avr/avr_fixes.h>
#include <mulabs_avr/avr/basic_register8.h>
#include <mulabs_avr/avr/basic_register16.h>
#include <mulabs_avr/devices/common/common_basic_io.h>
#include <mulabs_avr/devices/common/common_basic_pin.h>
#include <mulabs_avr/devices/common/common_basic_pin_set.h>
#include <mulabs_avr/devices/mega32u4/basic_port.h>
#include <mulabs_avr/devices/mega32u4/basic_jtag.h>
#include <mulabs_avr/utility/array.h>
#include <mulabs_avr/utility/type_traits.h>
#include <mulabs_avr/mcu/atmega32-u4-registers.h>


namespace mulabs {
namespace avr {

class ATMega32U4
{
  public:
	static constexpr uint8_t kNumPorts	= 5;

	using MCU				= ATMega32U4;
	using Register8			= BasicRegister8;
	using Register16		= BasicRegister16;
	using PortIntegerType	= uint8_t;

	using IO				= CommonBasicIO<MCU>;
	using Pin				= CommonBasicPin<MCU>;
	using Port				= mega32u4::BasicPort<MCU>;
	using PinSet			= CommonBasicPinSet<MCU>;
	using JTAG				= mega32u4::BasicJTAG<MCU>;

	static_assert (is_literal_type<Register8>::value, "Register8 must be a literal type");
	static_assert (is_literal_type<Register16>::value, "Register16 must be a literal type");
	static_assert (is_literal_type<IO>::value, "IO must be a literal type");
	static_assert (is_literal_type<Pin>::value, "Pin must be a literal type");
	static_assert (is_literal_type<PinSet>::value, "PinSet must be a literal type");
	static_assert (is_literal_type<Port>::value, "Port must be a literal type");

#define MULABS_DECLARE_PORT(member_name, avr_port_prefix, port_number) \
	static constexpr Port member_name { \
		port_number, \
		avr_port_prefix##_PIN, avr_port_prefix##_DDR, avr_port_prefix##_PORT, \
	};

	MULABS_DECLARE_PORT(port_b, kPORTB, 0)
	MULABS_DECLARE_PORT(port_c, kPORTC, 1)
	MULABS_DECLARE_PORT(port_d, kPORTD, 2)
	MULABS_DECLARE_PORT(port_e, kPORTE, 3)
	MULABS_DECLARE_PORT(port_f, kPORTF, 4)

#undef MULABS_DECLARE_PORT

	static constexpr Port	ports_index[kNumPorts] = { port_b, port_c, port_d, port_e, port_f };

  public:
	/**
	 * Execute single no-operation instruction.
	 */
	static void
	nop() noexcept
	{
		_NOP();
	}

	/**
	 * Sleep for given number of milliseconds.
	 */
	static void
	sleep_ms (int ms) noexcept
	{
		for (int z = 0; z < ms; ++z)
			_delay_ms (1);
	}

	/**
	 * Sleep for given number of milliseconds.
	 */
	template<int MS>
		static void
		sleep_ms() noexcept
		{
			_delay_ms (MS);
		}

	/**
	 * Sleep for given number of microseconds.
	 */
	static void
	sleep_us (int us) noexcept
	{
		for (int z = 0; z < us / 20; ++z)
			_delay_us (20);
	}

	/**
	 * Sleep for given number of microseconds.
	 */
	template<int US>
		static void
		sleep_us() noexcept
		{
			_delay_us (US);
		}
};


constexpr uint8_t ATMega32U4::kNumPorts;
constexpr ATMega32U4::Port ATMega32U4::port_b;
constexpr ATMega32U4::Port ATMega32U4::port_c;
constexpr ATMega32U4::Port ATMega32U4::port_d;
constexpr ATMega32U4::Port ATMega32U4::port_e;
constexpr ATMega32U4::Port ATMega32U4::port_f;

} // namespace avr
} // namespace mulabs

#endif

