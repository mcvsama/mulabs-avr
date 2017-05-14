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
	inline BasicPin<M> const&
	BasicPin<M>::operator= (bool logic_value) const
	{
		_port.pin_set (_pin_number, logic_value);
		return *this;
	}


template<class M>
	constexpr uint8_t
	BasicPin<M>::pin_number() const
	{
		return _pin_number;
	}


template<class M>
	constexpr size_t
	BasicPin<M>::absolute_pin_number() const
	{
		return _port.port_number() * 8 * sizeof (typename MCU::PortIntegerType) + pin_number();
	}


template<class M>
	inline bool
	BasicPin<M>::get() const
	{
		return _port.pin_get (_pin_number);
	}


template<class M>
	template<bool State>
		inline void
		BasicPin<M>::wait_for() const
		{
			while (get() != State)
				continue;
		}


template<class M>
	constexpr typename BasicPin<M>::Port
	BasicPin<M>::port() const
	{
		return _port;
	}


template<class M>
	inline void
	BasicPin<M>::configure_as_input() const
	{
		_port.pin_configure_as_input (_pin_number);
	}


template<class M>
	inline void
	BasicPin<M>::configure_as_output() const
	{
		_port.pin_configure_as_output (_pin_number);
	}


template<class M>
	inline bool
	BasicPin<M>::slew_rate_limit() const
	{
		return get_bit (pinctrl_register().read(), 7);
	}


template<class M>
	inline void
	BasicPin<M>::set_slew_rate_limit (bool enabled) const
	{
		set_bit_value (pinctrl_register().ref(), 7, enabled);
	}


template<class M>
	inline bool
	BasicPin<M>::inverted_io() const
	{
		return get_bit (pinctrl_register().read(), 6);
	}


template<class M>
	inline void
	BasicPin<M>::set_inverted_io (bool enabled) const
	{
		set_bit_value (pinctrl_register().ref(), 6, enabled);
	}


template<class M>
	inline typename BasicPin<M>::Configuration
	BasicPin<M>::configuration() const
	{
		return static_cast<Configuration> (pinctrl_register().read() & 0b00'111'000);
	}


template<class M>
	inline void
	BasicPin<M>::set_configuration (Configuration new_configuration) const
	{
		pinctrl_register() = (pinctrl_register().read() & 0b11'000'111) | new_configuration;
	}


template<class M>
	inline typename BasicPin<M>::SenseConfiguration
	BasicPin<M>::sense_configuration() const
	{
		return static_cast<Configuration> (pinctrl_register().read() & 0b00'000'111);
	}


template<class M>
	inline void
	BasicPin<M>::set_sense_configuration (SenseConfiguration new_sense_configuration) const
	{
		pinctrl_register() = (pinctrl_register().read() & 0b11'111'000) | new_sense_configuration;
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


template<class M>
	inline void
	BasicPin<M>::signal() const
	{
		toggle();
		toggle();
	}


template<class M>
	constexpr typename BasicPin<M>::MCU::Register8
	BasicPin<M>::pinctrl_register() const
	{
		return _port.pinctrl_register (_pin_number);
	}

} // namespace xmega_au
} // namespace avr
} // namespace mulabs

#endif

