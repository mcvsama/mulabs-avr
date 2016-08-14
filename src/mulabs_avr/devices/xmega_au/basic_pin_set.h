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

#ifndef MULABS_AVR__DEVICES__XMEGA_AU__BASIC_PINSET_H__INCLUDED
#define MULABS_AVR__DEVICES__XMEGA_AU__BASIC_PINSET_H__INCLUDED


namespace mulabs {
namespace avr {

/**
 * Stores list of pins as bits set to 1 in an array
 * of all ports. Each ports is 8-bit integer, each bit
 * represents one pin.
 */
template<class pMCU>
	class BasicPinSet
	{
	  public:
		typedef pMCU				MCU;
		typedef typename MCU::Pins	Pins;

	  public:
		// Ctor
		constexpr explicit
		BasicPinSet (Pins pins);

		/**
		 * Return array of ports/pins.
		 * If a bit is set to 1, then pin is considered existing
		 * in the list.
		 */
		constexpr Pins
		pins() const;

	  private:
		Pins _pins;
	};


template<class M>
	constexpr
	BasicPinSet<M>::BasicPinSet (Pins pins):
		_pins (pins)
	{
	}


template<class M>
	constexpr typename BasicPinSet<M>::Pins
	BasicPinSet<M>::pins() const
	{
		return _pins;
	}

} // namespace avr
} // namespace mulabs

#endif

