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

#ifndef MULABS_AVR__DEVICES__XMEGA_AU__BASIC_PIN_I_H__INCLUDED
#define MULABS_AVR__DEVICES__XMEGA_AU__BASIC_PIN_I_H__INCLUDED

#include "basic_port.h"


namespace mulabs {
namespace avr {
namespace xmega_au {

template<class M>
	constexpr
	BasicPin<M>::BasicPin (Port port, uint8_t pin_number):
		_port (port),
		_pin_number (pin_number)
	{
	}


template<class M>
	inline void
	BasicPin<M>::configure_as_output() const
	{
		_port.pin_configure_as_output (_pin_number);
	}


template<class M>
	inline void
	BasicPin<M>::configure_as_input() const
	{
		_port.pin_configure_as_input (_pin_number);
	}


template<class M>
	inline BasicPin<M> const&
	BasicPin<M>::operator= (bool logic_value) const
	{
		_port.pin_set (_pin_number, logic_value);
		return *this;
	}


template<class M>
	inline void
	BasicPin<M>::set_high() const
	{
		_port.pin_set_high (_pin_number);
	}


template<class M>
	inline void
	BasicPin<M>::set_low() const
	{
		_port.pin_set_low (_pin_number);
	}


template<class M>
	inline void
	BasicPin<M>::toggle() const
	{
		_port.pin_toggle (_pin_number);
	}

} // namespace xmega_au
} // namespace avr
} // namespace mulabs

#endif

