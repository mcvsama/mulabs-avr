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

#ifndef MULABS_AVR__DEVICES__XMEGA_AU__BASIC_PIN_H__INCLUDED
#define MULABS_AVR__DEVICES__XMEGA_AU__BASIC_PIN_H__INCLUDED


namespace mulabs {
namespace avr {
namespace xmega_au {

template<class pMCU>
	class BasicPort;


template<class pMCU>
	class BasicPin
	{
		typedef pMCU			MCU;
		typedef BasicPort<MCU>	Port;

	  public:
		constexpr
		BasicPin (Port port, uint8_t pin_number);

		/**
		 * Set pin level to logic_value. True means high, false mans low.
		 */
		BasicPin const&
		operator= (bool logic_value) const;

		/**
		 * Return the pin number in the port.
		 */
		constexpr uint8_t
		pin_number() const;

		/**
		 * Return pin number for the PinSet object
		 * (taking into account port number as well).
		 */
		constexpr size_t
		absolute_pin_number() const;

		/**
		 * Return port.
		 */
		constexpr Port
		port() const;

		/**
		 * Configure pin as input.
		 */
		void
		configure_as_input() const;

		/**
		 * Configure pin as output.
		 */
		void
		configure_as_output() const;

		/**
		 * Set pin level to high.
		 */
		void
		set_high() const;

		/**
		 * Set pin level to low.
		 */
		void
		set_low() const;

		/**
		 * Toggle pin level.
		 */
		void
		toggle() const;

	  private:
		Port	_port;
		uint8_t	_pin_number;
	};

} // namespace xmega_au
} // namespace avr
} // namespace mulabs

#include "basic_pin_i.h"

#endif

