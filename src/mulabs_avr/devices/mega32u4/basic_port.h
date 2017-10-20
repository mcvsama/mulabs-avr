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

#ifndef MULABS_AVR__DEVICES__MEGA32U4__BASIC_PORT_H__INCLUDED
#define MULABS_AVR__DEVICES__MEGA32U4__BASIC_PORT_H__INCLUDED

#include <mulabs_avr/utility/bits.h>


namespace mulabs {
namespace avr {
namespace mega32u4 {

template<class pMCU>
	class BasicPort
	{
	  public:
		using MCU		= pMCU;
		using Register8	= typename MCU::Register8;
		using Pin		= typename MCU::Pin;
		using PinBits	= typename MCU::PortIntegerType;

	  public:
		// Ctor:
		constexpr
		BasicPort (uint8_t port_number,
				   Register8 pin_register, Register8 ddr_register, Register8 port_register);

		constexpr bool
		operator== (BasicPort const& other) const;

		constexpr bool
		operator!= (BasicPort const& other) const;

		/**
		 * Return Pin object by bit number.
		 */
		constexpr Pin
		pin (uint8_t pin_number) const;

		/**
		 * Return this port's number (A is 0, B is 1, etc).
		 */
		constexpr uint8_t
		port_number() const;

		/**
		 * Read byte from the port.
		 */
		PinBits
		get() const;

		/**
		 * Alias for get().
		 */
		operator PinBits() const;

		/**
		 * Write byte to all pins on port at once.
		 */
		void
		set (PinBits pin_bits) const;

		/**
		 * Alias for set (uint8_t).
		 */
		BasicPort const&
		operator= (PinBits pin_bits) const;

		/**
		 * Set selected pins to high level.
		 */
		void
		set_high (PinBits pin_bits) const;

		/**
		 * Set selected pins to low level.
		 */
		void
		set_low (PinBits pin_bits) const;

		/**
		 * Toggle selected pins (switch levels).
		 */
		void
		toggle (PinBits pin_bits) const;

		/**
		 * Enable pull-ups on selected pins.
		 * Makes sense only if given pins are configured as inputs.
		 */
		void
		enable_pull_up (PinBits pin_bits) const;

		/**
		 * Disable pull-ups on selected pins.
		 * Makes sense only if given pins are configured as inputs.
		 */
		void
		disable_pull_up (PinBits pin_bits) const;

		/**
		 * Configure pins denoted by set bits as inputs.
		 */
		void
		configure_as_inputs (PinBits pin_bits) const;

		/**
		 * Configure pins denoted by set bits as outputs.
		 */
		void
		configure_as_outputs (PinBits pin_bits) const;

		/**
		 * Configure individual pin as input.
		 */
		void
		pin_configure_as_input (uint8_t pin_number) const;

		/**
		 * Configure individual pin as output.
		 */
		void
		pin_configure_as_output (uint8_t pin_number) const;

		/**
		 * Get individual pin level.
		 */
		bool
		pin_get (uint8_t pin_number) const;

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
		 * Enable internal pull-up on individual pin.
		 * Makes sense only if the pin is configured as input.
		 */
		void
		pin_enable_pull_up (uint8_t pin_number) const;

		/**
		 * Disable internal pull-up on individual pin.
		 * Makes sense only if the pin is configured as input.
		 */
		void
		pin_disable_pull_up (uint8_t pin_number) const;

	  private:
		uint8_t		_port_number;
		Register8	_pin;	// PINx register
		Register8	_ddr;	// DDRx register
		Register8	_port;	// PORTx register
	};


template<class M>
	constexpr
	BasicPort<M>::BasicPort (uint8_t port_number,
							 Register8 pin_register, Register8 ddr_register, Register8 port_register):
		_port_number (port_number),
		_pin (pin_register),
		_ddr (ddr_register),
		_port (port_register)
	{ }


template<class M>
	constexpr bool
	BasicPort<M>::operator== (BasicPort const& other) const
	{
		return _port_number == other._port_number
			&& &_pin == &other._pin
			&& &_ddr == &other._ddr
			&& &_port == &other._port;
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
	constexpr uint8_t
	BasicPort<M>::port_number() const
	{
		return _port_number;
	}


template<class M>
	inline typename BasicPort<M>::PinBits
	BasicPort<M>::get() const
	{
		return _pin;
	}


template<class M>
	inline
	BasicPort<M>::operator PinBits() const
	{
		return get();
	}


template<class M>
	inline void
	BasicPort<M>::set (PinBits pin_bits) const
	{
		_port = pin_bits;
	}


template<class M>
	inline BasicPort<M> const&
	BasicPort<M>::operator= (PinBits pin_bits) const
	{
		set (pin_bits);
		return *this;
	}


template<class M>
	inline void
	BasicPort<M>::set_high (PinBits pin_bits) const
	{
		_port.ref() |= pin_bits;
	}


template<class M>
	inline void
	BasicPort<M>::set_low (PinBits pin_bits) const
	{
		_port.ref() &= ~pin_bits;
	}


template<class M>
	inline void
	BasicPort<M>::toggle (PinBits pin_bits) const
	{
		_pin = pin_bits;
	}


template<class M>
	inline void
	BasicPort<M>::enable_pull_up (PinBits pin_bits) const
	{
		_port |= pin_bits;
	}


template<class M>
	inline void
	BasicPort<M>::disable_pull_up (PinBits pin_bits) const
	{
		_port &= ~pin_bits;
	}


template<class M>
	inline void
	BasicPort<M>::configure_as_inputs (PinBits pin_bits) const
	{
		_ddr = _ddr & ~pin_bits;
	}


template<class M>
	inline void
	BasicPort<M>::configure_as_outputs (PinBits pin_bits) const
	{
		_ddr = _ddr | pin_bits;
	}


template<class M>
	inline void
	BasicPort<M>::pin_configure_as_input (uint8_t pin_number) const
	{
		configure_as_inputs (bitnum<uint8_t> (pin_number));
	}


template<class M>
	inline void
	BasicPort<M>::pin_configure_as_output (uint8_t pin_number) const
	{
		configure_as_outputs (bitnum<uint8_t> (pin_number));
	}


template<class M>
	inline bool
	BasicPort<M>::pin_get (uint8_t pin_number) const
	{
		return get() & bitnum<uint8_t> (pin_number);
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
		set_high (bitnum<uint8_t> (pin_number));
	}


template<class M>
	inline void
	BasicPort<M>::pin_set_low (uint8_t pin_number) const
	{
		set_low (bitnum<uint8_t> (pin_number));
	}


template<class M>
	inline void
	BasicPort<M>::pin_toggle (uint8_t pin_number) const
	{
		toggle (bitnum<uint8_t> (pin_number));
	}


template<class M>
	inline void
	BasicPort<M>::pin_enable_pull_up (uint8_t pin_number) const
	{
		enable_pull_up (bitnum<uint8_t> (pin_number));
	}


template<class M>
	inline void
	BasicPort<M>::pin_disable_pull_up (uint8_t pin_number) const
	{
		disable_pull_up (bitnum<uint8_t> (pin_number));
	}

} // namespace mega32u4
} // namespace avr
} // namespace mulabs

#endif

