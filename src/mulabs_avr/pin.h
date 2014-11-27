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

#ifndef MULABS_AVR__PIN_H__INCLUDED
#define MULABS_AVR__PIN_H__INCLUDED

// AVR:
#include <avr/io.h>

// Local:
#include "port.h"
#include "utility.h"


namespace mulabs {
namespace avr {

template<class pRegister>
	class PinTemplate
	{
	  public:
		typedef pRegister				Register;
		typedef PortTemplate<Register>	Port;

	  public:
		constexpr
		PinTemplate (Port port, uint8_t pin):
			_port (port),
			_pin (pin)
		{ }

		constexpr uint8_t
		pin() const
		{
			return _pin;
		}

		constexpr uint8_t
		bit() const
		{
			return bitnum<uint8_t> (_pin);
		}

		constexpr bool
		operator== (PinTemplate const& other)
		{
			return _port == other._port && _pin == other._pin;
		}

#if defined(__AVR_ATtiny10__) || \
	defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) || \
	defined(__AVR_ATtiny261__) || defined(__AVR_ATtiny461__) || defined(__AVR_ATtiny861__)
		/**
		 * Enable/disable digital input buffer on this pin.
		 * Pins that are used only for analog inputs can disable input buffer.
		 * By default it's enabled.
		 * FIXME Currenty only DIDR0 is supported.
		 */
		void
		set_digital_enabled (bool enabled) const noexcept
		{
			if (enabled)
				clear_bit (DIDR0, bitnum<uint8_t> (_pin));
			else
				set_bit (DIDR0, bitnum<uint8_t> (_pin));
		}
#endif

		/**
		 * Configure pin as output.
		 */
		void
		configure_as_output() const noexcept
		{
			set_bit (_port.dir(), _pin);
		}

		/**
		 * Configure pin as input.
		 *
		 * See notes for InputPin::read().
		 */
		void
		configure_as_input() const noexcept
		{
			clear_bit (_port.dir(), _pin);
		}

		/**
		 * Read input state.
		 *
		 * Note: After configuring pin as input pin, wait at least one cycle
		 * before attempting to read its value. See ATtiny25 documentation,
		 * "10.2.4 Reading the Pin Value".
		 */
		bool
		read() const noexcept
		{
			return get_bit (_port.in(), _pin);
		}

		/**
		 * Alias for read().
		 */
		bool
		operator()() const noexcept
		{
			return read();
		}

		/**
		 * Flip pin (low -> high, high -> low).
		 */
		void
		flip() const noexcept
		{
			// Special function of the IN register:
			set_bit (_port.in(), _pin);
		}

		/**
		 * Set pin to high.
		 */
		void
		set_high() const noexcept
		{
			set_bit (_port.out(), _pin);
		}

		/**
		 * Set pin to low.
		 */
		void
		set_low() const noexcept
		{
			clear_bit (_port.out(), _pin);
		}

		/**
		 * Set pin to given value.
		 */
		void
		set (bool value) const noexcept
		{
			if (value)
				set_high();
			else
				set_low();
		}

		/**
		 * Enable pull-up resistor.
		 */
		void
		pull_up() const noexcept
		{
			set_bit (_port.pue(), _pin);
		}

		/**
		 * Disable pull-up resistor.
		 */
		void
		set_tri_state() const noexcept
		{
			clear_bit (_port.pue(), _pin);
		}

	  private:
		Port const		_port;
		uint8_t const	_pin;
	};

} // namespace avr
} // namespace mulabs

#endif

