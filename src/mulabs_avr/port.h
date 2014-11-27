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

// Local:
#include "pin_set.h"


namespace mulabs {
namespace avr {

template<class pRegister>
	class PortTemplate
	{
	  public:
		typedef pRegister Register;

	  public:
		constexpr
		PortTemplate (Register dir, Register in, Register out, Register pue):
			_dir (dir),
			_in (in),
			_out (out),
			_pue (pue)
		{ }

		constexpr bool
		operator== (PortTemplate const& other) const
		{
			return &_dir == &other._dir
				&& &_in == &other._in
				&& &_out == &other._out
				&& &_pue == &other._pue;
		}

		constexpr Register
		dir() noexcept
		{
			return _dir;
		}

		constexpr Register
		in() noexcept
		{
			return _in;
		}

		constexpr Register
		out() noexcept
		{
			return _out;
		}

		constexpr Register
		pue() noexcept
		{
			return _pue;
		}

		/**
		 * Recursive function that collects Pin bits.
		 */
		template<class Pin, class ...Pins>
			constexpr PinSet
			make_pin_set (Pin pin, Pins ...pins) const
			{
				return PinSet (pin.bit() | make_pin_set (pins...).bits());
			}

		/**
		 * Recursive stop-condition for make_pin_set().
		 */
		template<class Pin>
			constexpr PinSet
			make_pin_set (Pin pin) const
			{
				return PinSet (pin.bit());
			}

		/**
		 * Pull up a set of pins at once.
		 * Other pins won't be changed.
		 * TODO make sure pins have the same _pue as this->_pue.
		 */
		void
		pull_up (PinSet pins) const
		{
			_pue |= pins.bits();
		}

		/**
		 * Same as pull_up (PinSet).
		 */
		template<class ...Pins>
			void
			pull_up (Pins ...pins) const
			{
				pull_up (make_pin_set (pins...));
			}

		/**
		 * Pull up this set of pins, and only this set.
		 * Other pins will be set to tri-state.
		 */
		void
		pull_up_exclusive (PinSet pins) const
		{
			_pue = pins.bits();
		}

		/**
		 * Same as pull_up_exclusive (PinSet).
		 */
		template<class ...Pins>
			void
			pull_up_exclusive (Pins ...pins) const
			{
				pull_up_exclusive (make_pin_set (pins...));
			}

		/**
		 * Configure as input a set of pins.
		 * Configuration of other pins won't be changed.
		 */
		void
		configure_as_input (PinSet pins) const
		{
			_dir &= ~pins.bits();
		}

		/**
		 * Same as configure_as_input (PinSet);
		 */
		template<class ...Pins>
			void
			configure_as_input (Pins ...pins) const
			{
				configure_as_input (make_pin_set (pins...));
			}

		/**
		 * Configure as input this set of pins, and only this set.
		 * Other pins will be configured as outputs.
		 */
		void
		configure_as_input_exclusive (PinSet pins) const
		{
			_dir = ~pins.bits();
		}

		/**
		 * Same as configure_as_input_exclusive (PinSet).
		 */
		template<class ...Pins>
			void
			configure_as_input_exclusive (Pins ...pins) const
			{
				configure_as_input_exclusive (make_pin_set (pins...));
			}

		/**
		 * Configure as output a set of pins.
		 * Configuration of other pins won't be changed.
		 */
		void
		configure_as_output (PinSet pins) const
		{
			_dir |= pins.bits();
		}

		/**
		 * Same as configure_as_output (PinSet).
		 */
		template<class ...Pins>
			void
			configure_as_output (Pins ...pins) const
			{
				configure_as_output (make_pin_set (pins...));
			}

		/**
		 * Configure as output this set of pins, and only this set.
		 * Other pins will be configured as inputs.
		 */
		void
		configure_as_output_exclusive (PinSet pins) const
		{
			_dir = pins.bits();
		}

		/**
		 * Same as configure_as_output_exclusive (PinSet).
		 */
		template<class ...Pins>
			void
			configure_as_output_exclusive (Pins ...pins) const
			{
				configure_as_output_exclusive (make_pin_set (pins...));
			}

		/**
		 * Set high level on given pins.
		 * Other pins won't be changed.
		 */
		void
		set_high (PinSet pins) const
		{
			_out |= pins.bits();
		}

		/**
		 * Same as set_high (PinSet).
		 */
		template<class ...Pins>
			void
			set_high (Pins ...pins) const
			{
				set_high (make_pin_set (pins...));
			}

		/**
		 * Set high level on given pins, and only on those pins.
		 * Other pins will be set to low.
		 */
		void
		set_high_exclusive (PinSet pins) const
		{
			_out = pins.bits();
		}

		/**
		 * Same as set_high_exclusive (PinSet).
		 */
		template<class ...Pins>
			void
			set_high_exclusive (Pins ...pins) const
			{
				set_high_exclusive (make_pin_set (pins...));
			}

		/**
		 * Set low level on given pins.
		 * Other pins won't be changed.
		 */
		void
		set_low (PinSet pins) const
		{
			_out &= ~pins.bits();
		}

		/**
		 * Same as set_low (PinSet).
		 */
		template<class ...Pins>
			void
			set_low (Pins ...pins) const
			{
				set_low (make_pin_set (pins...));
			}

		/**
		 * Set low level on given pins, and only on those pins.
		 * Other pins will be set to high.
		 */
		void
		set_low_exclusive (PinSet pins) const
		{
			_out = ~pins.bits();
		}

		/**
		 * Same as set_low_exclusive (PinSet).
		 */
		template<class ...Pins>
			void
			set_low_exclusive (Pins ...pins) const
			{
				set_low_exclusive (make_pin_set (pins...));
			}

	  private:
		Register const	_dir;
		Register const	_in;
		Register const	_out;
		Register const	_pue;
	};

} // namespace avr
} // namespace mulabs

#endif

