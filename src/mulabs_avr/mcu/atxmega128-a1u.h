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

// Mulabs AVR:
#include <mulabs_avr/avr/avr_fixes.h>
#include <mulabs_avr/avr/basic_register8.h>
#include <mulabs_avr/avr/basic_register16.h>
#include <mulabs_avr/devices/xmega_au/basic_clock.h>
#include <mulabs_avr/devices/xmega_au/basic_io.h>
#include <mulabs_avr/devices/xmega_au/basic_pin.h>
#include <mulabs_avr/devices/xmega_au/basic_port.h>


namespace mulabs {
namespace avr {

class ATXMega128A1U
{
  public:
	static constexpr uint8_t kNumPorts	= 14;

	typedef ATXMega128A1U				MCU;
	typedef BasicRegister8				Register8;
	typedef BasicRegister16				Register16;
	typedef uint8_t						PortIntegerType;
	typedef PortIntegerType				Pins[kNumPorts];

	typedef xmega_au::BasicClock<MCU>	Clock;
	typedef xmega_au::BasicIO<MCU>		IO;
	typedef xmega_au::BasicPin<MCU>		Pin;
	typedef xmega_au::BasicPort<MCU>	Port;

#define MULABS_DECLARE_PORT(member_name, avr_port_prefix) \
	static constexpr Port member_name { \
		0, \
		MULABS_REG (avr_port_prefix##_DIR), MULABS_REG (avr_port_prefix##_DIRSET), MULABS_REG (avr_port_prefix##_DIRCLR), MULABS_REG (avr_port_prefix##_DIRTGL), \
		MULABS_REG (avr_port_prefix##_OUT), MULABS_REG (avr_port_prefix##_OUTSET), MULABS_REG (avr_port_prefix##_OUTCLR), MULABS_REG (avr_port_prefix##_OUTTGL), \
		MULABS_REG (avr_port_prefix##_IN), \
		MULABS_REG (avr_port_prefix##_INTCTRL), MULABS_REG (avr_port_prefix##_INT0MASK), MULABS_REG (avr_port_prefix##_INT1MASK), MULABS_REG (avr_port_prefix##_INTFLAGS), \
		MULABS_REG (avr_port_prefix##_PIN0CTRL), MULABS_REG (avr_port_prefix##_PIN1CTRL), MULABS_REG (avr_port_prefix##_PIN2CTRL), MULABS_REG (avr_port_prefix##_PIN3CTRL), \
		MULABS_REG (avr_port_prefix##_PIN4CTRL), MULABS_REG (avr_port_prefix##_PIN5CTRL), MULABS_REG (avr_port_prefix##_PIN6CTRL), MULABS_REG (avr_port_prefix##_PIN7CTRL), \
	};

	MULABS_DECLARE_PORT(port_a, PORTA)
	MULABS_DECLARE_PORT(port_b, PORTB)
	MULABS_DECLARE_PORT(port_c, PORTC)
	MULABS_DECLARE_PORT(port_d, PORTD)
	MULABS_DECLARE_PORT(port_e, PORTE)
	MULABS_DECLARE_PORT(port_f, PORTF)

#undef MULABS_DECLARE_PORT

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

