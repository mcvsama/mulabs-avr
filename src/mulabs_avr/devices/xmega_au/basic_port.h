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

#ifndef MULABS_AVR__DEVICES__XMEGA_AU__BASIC_PORT_H__INCLUDED
#define MULABS_AVR__DEVICES__XMEGA_AU__BASIC_PORT_H__INCLUDED

#include <mulabs_avr/utility/bits.h>


namespace mulabs {
namespace avr {
namespace xmega_au {

template<class pMCU>
	class BasicPin;


template<class pMCU>
	class BasicPort
	{
	  public:
		typedef pMCU					MCU;
		typedef typename MCU::Register8	Register8;
		typedef BasicPin<MCU>			Pin;

	  public:
		// Ctor:
		constexpr
		BasicPort (uint8_t index,
				   Register8 dir, Register8 dirset, Register8 dirclr, Register8 dirtgl,
				   Register8 out, Register8 outset, Register8 outclr, Register8 outtgl,
				   Register8 in,
				   Register8 intctrl, Register8 int0mask, Register8 int1mask, Register8 intflags,
				   Register8 pin0ctrl, Register8 pin1ctrl, Register8 pin2ctrl, Register8 pin3ctrl,
				   Register8 pin4ctrl, Register8 pin5ctrl, Register8 pin6ctrl, Register8 pin7ctrl);

		constexpr bool
		operator== (BasicPort const& other) const;

		constexpr bool
		operator!= (BasicPort const& other) const;

		constexpr Pin
		pin (uint8_t pin_number) const;

		// TODO set dir

		/**
		 * Write byte to all pins on port at once.
		 */
		void
		set (uint8_t byte) const;

		/**
		 * Alias for set (uint8_t).
		 */
		BasicPort const&
		operator= (uint8_t byte) const;

		/**
		 * Set selected pins to high level.
		 */
		void
		set_high (uint8_t byte) const;

		/**
		 * Set selected pins to low level.
		 */
		void
		set_low (uint8_t byte) const;

		/**
		 * Toggle selected pins (switch levels).
		 */
		void
		toggle (uint8_t byte) const;

		/**
		 * Read byte from the port.
		 */
		uint8_t
		get() const;

		/**
		 * Alias for get().
		 */
		operator uint8_t() const;

		/**
		 * Configure individual pin as output.
		 */
		void
		pin_configure_as_output (uint8_t pin_number) const;

		/**
		 * Configure individual pin as input.
		 */
		void
		pin_configure_as_input (uint8_t pin_number) const;

		/**
		 * Set individual pin to given logic value.
		 */
		void
		pin_set (uint8_t pin_number, bool logic_value) const;

		/**
		 * Set individual pin to high level.
		 */
		void
		pin_set_high (uint8_t pin_number) const;

		/**
		 * Set individual pin to low level.
		 */
		void
		pin_set_low (uint8_t pin_number) const;

		/**
		 * Toggle individual pin level.
		 */
		void
		pin_toggle (uint8_t pin_number) const;

		/**
		 * Get individual pin level.
		 */
		bool
		pin_get (uint8_t pin_number) const;

	  private:
		uint8_t		_index;
		Register8	_dir, _dirset, _dirclr, _dirtgl;
		Register8	_out, _outset, _outclr, _outtgl;
		Register8	_in;
		Register8	_intctrl, _int0mask, _int1mask, _intflags;
		Register8	_pin0ctrl, _pin1ctrl, _pin2ctrl, _pin3ctrl;
		Register8	_pin4ctrl, _pin5ctrl, _pin6ctrl, _pin7ctrl;
	};


template<class M>
	constexpr
	BasicPort<M>::BasicPort (uint8_t index,
							 Register8 dir, Register8 dirset, Register8 dirclr, Register8 dirtgl,
							 Register8 out, Register8 outset, Register8 outclr, Register8 outtgl,
							 Register8 in,
							 Register8 intctrl, Register8 int0mask, Register8 int1mask, Register8 intflags,
							 Register8 pin0ctrl, Register8 pin1ctrl, Register8 pin2ctrl, Register8 pin3ctrl,
							 Register8 pin4ctrl, Register8 pin5ctrl, Register8 pin6ctrl, Register8 pin7ctrl):
		_index (index),
		_dir (dir), _dirset (dirset), _dirclr (dirclr), _dirtgl (dirtgl),
		_out (out), _outset (outset), _outclr (outclr), _outtgl (outtgl),
		_in (in),
		_intctrl (intctrl), _int0mask (int0mask), _int1mask (int1mask), _intflags (intflags),
		_pin0ctrl (pin0ctrl), _pin1ctrl (pin1ctrl), _pin2ctrl (pin2ctrl), _pin3ctrl (pin3ctrl),
		_pin4ctrl (pin4ctrl), _pin5ctrl (pin5ctrl), _pin6ctrl (pin6ctrl), _pin7ctrl (pin7ctrl)
	{
	}


template<class M>
	constexpr bool
	BasicPort<M>::operator== (BasicPort const& other) const
	{
		return _index == other._index
			&& &_dir == &other._dir && _dirset == &other._dirset && &_dirclr == &other._dirclr && &_dirtgl == &other._dirtgl
			&& &_out == &other._out && _outset == &other._outset && &_outclr == &other._outclr && &_outtgl == &other._outtgl
			&& &_in == &other._in
			&& &_intctrl == &other._intctrl && &_int0mask == &other._int0mask && &_int1mask == &other._int1mask && &_intflags == &other._intflags
			&& &_pin0ctrl == &other._pin0ctrl && &_pin1ctrl == &other._pin1ctrl && &_pin2ctrl == &other._pin2ctrl && &_pin3ctrl == &other._pin3ctrl
			&& &_pin4ctrl == &other._pin4ctrl && &_pin5ctrl == &other._pin5ctrl && &_pin6ctrl == &other._pin6ctrl && &_pin7ctrl == &other._pin7ctrl;
	}


template<class M>
	constexpr bool
	BasicPort<M>::operator!= (BasicPort const& other) const
	{
		return !(*this == other);
	}


template<class M>
	constexpr typename BasicPort<M>::Pin
	BasicPort<M>::pin (uint8_t pin_number) const
	{
		return Pin (*this, pin_number);
	}


template<class M>
	inline void
	BasicPort<M>::set (uint8_t byte) const
	{
		_out = byte;
	}


template<class M>
	inline BasicPort<M> const&
	BasicPort<M>::operator= (uint8_t byte) const
	{
		set (byte);
		return *this;
	}


template<class M>
	inline void
	BasicPort<M>::set_high (uint8_t byte) const
	{
		_outset = byte;
	}


template<class M>
	inline void
	BasicPort<M>::set_low (uint8_t byte) const
	{
		_outclr = byte;
	}


template<class M>
	inline void
	BasicPort<M>::toggle (uint8_t byte) const
	{
		_outtgl = byte;
	}


template<class M>
	inline uint8_t
	BasicPort<M>::get() const
	{
		return _in;
	}


template<class M>
	inline
	BasicPort<M>::operator uint8_t() const
	{
		return get();
	}


template<class M>
	inline void
	BasicPort<M>::pin_configure_as_output (uint8_t pin_number) const
	{
		_dirset = bitnum<uint8_t> (pin_number);
	}


template<class M>
	inline void
	BasicPort<M>::pin_configure_as_input (uint8_t pin_number) const
	{
		_dirclr = bitnum<uint8_t> (pin_number);
	}


template<class M>
	inline void
	BasicPort<M>::pin_set (uint8_t pin_number, bool logic_value) const
	{
		if (logic_value)
			pin_set_high (pin_number);
		else
			pin_set_low (pin_number);
	}


template<class M>
	inline void
	BasicPort<M>::pin_set_high (uint8_t pin_number) const
	{
		_outset = bitnum<uint8_t> (pin_number);
	}


template<class M>
	inline void
	BasicPort<M>::pin_set_low (uint8_t pin_number) const
	{
		_outclr = bitnum<uint8_t> (pin_number);
	}


template<class M>
	inline void
	BasicPort<M>::pin_toggle (uint8_t pin_number) const
	{
		_outtgl = bitnum<uint8_t> (pin_number);
	}


template<class M>
	inline bool
	BasicPort<M>::pin_get (uint8_t pin_number) const
	{
		return _in.read() & bitnum<uint8_t> (pin_number);
	}

} // namespace xmega_au
} // namespace avr
} // namespace mulabs

#endif

