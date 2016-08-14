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

#ifndef MULABS_AVR__DEVICES__XMEGA_AU__BASIC_IO_H__INCLUDED
#define MULABS_AVR__DEVICES__XMEGA_AU__BASIC_IO_H__INCLUDED

// Mulabs AVR:
#include <mulabs_avr/devices/xmega_au/basic_pin_set.h>


namespace mulabs {
namespace avr {
namespace xmega_au {

template<class pMCU>
	class BasicIO
	{
	  public:
		typedef pMCU MCU;

	  public:
		enum class InterruptTrigger: uint8_t
		{
			LowLevel		= 0b00,
			AnyChange		= 0b01,
			FallingEdge		= 0b10,
			RisingEdge		= 0b11,
		};

	  public:
#if 0
		/**
		 * Recursive function that collects Pins.
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
#endif
	};

} // namespace xmega_au
} // namespace avr
} // namespace mulabs

#endif

