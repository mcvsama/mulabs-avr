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

#ifndef MULABS_AVR__PORT_H__INCLUDED
#define MULABS_AVR__PORT_H__INCLUDED

// AVR:
#include <avr/io.h>


namespace mulabs {
namespace avr {

class Port
{
  public:
	typedef uint8_t volatile& Addr;

  public:
	constexpr
	Port (Addr dir, Addr in, Addr out, Addr pue):
		_dir (dir),
		_in (in),
		_out (out),
		_pue (pue)
	{ }

	constexpr Addr
	dir() noexcept
	{
		return _dir;
	}

	constexpr Addr
	in() noexcept
	{
		return _in;
	}

	constexpr Addr
	out() noexcept
	{
		return _out;
	}

	constexpr Addr
	pue() noexcept
	{
		return _pue;
	}

  private:
	Addr const	_dir;
	Addr const	_in;
	Addr const	_out;
	Addr const	_pue;
};

#if 0
		/**
		 * Configure pins direction, 1s denote outputs, 0s - inputs.
		 *
		 * See notes for InputPin::read().
		 */
		constexpr void
		configure_directions (Type outputs_mask) noexcept;

		/**
		 * Configure pins as outputs. Rest remains unchanged.
		 */
		constexpr void
		configure_pins_as_outputs (Type outputs_mask) noexcept;

		/*
		 * Configure pins as inputs. Rest remains unchanged.
		 *
		 * See notes for InputPin::read().
		 */
		constexpr void
		configure_pins_as_inputs (Type inputs_mask) noexcept;

		/**
		 * Get pin mask for configure_* methods.
		 */
		static constexpr Type
		pin (uint8_t pin_num) noexcept;

		/**
		 * Get InputPin object for this port.
		 */
		constexpr InputPinType const
		get_input_pin (uint8_t pin) noexcept;

		/**
		 * Get OutputPin object for this port.
		 */
		constexpr OutputPinType const
		get_output_pin (uint8_t pin) noexcept;


template<class T>
	constexpr
	Port<T>::Port (Type volatile& p_out, Type volatile& p_in, Type volatile& direction, Type volatile& pull_up_enable) noexcept:
		_p_out (p_out),
		_p_in (p_in),
		_direction (direction),
		_pue (pull_up_enable)
	{ }


template<class T>
	inline constexpr void
	Port<T>::configure_directions (Type outputs_mask) noexcept
	{
		_direction = outputs_mask;
	}


template<class T>
	inline constexpr void
	Port<T>::configure_pins_as_outputs (Type outputs_mask) noexcept
	{
		_direction |= outputs_mask;
	}


template<class T>
	inline constexpr void
	Port<T>::configure_pins_as_inputs (Type inputs_mask) noexcept
	{
		_direction &= ~inputs_mask;
	}


template<class T>
	inline constexpr typename Port<T>::Type
	Port<T>::pin (uint8_t pin_num) noexcept
	{
		return bitnum<Type> (pin_num);
	}


template<class T>
	inline constexpr typename Port<T>::InputPinType const
	Port<T>::get_input_pin (uint8_t pin) noexcept
	{
		return InputPinType (*this, pin);
	}


template<class T>
	inline constexpr typename Port<T>::OutputPinType const
	Port<T>::get_output_pin (uint8_t pin) noexcept
	{
		return OutputPinType (*this, pin);
	}
#endif

} // namespace avr
} // namespace mulabs

#endif

