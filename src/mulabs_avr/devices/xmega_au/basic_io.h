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
#include <mulabs_avr/devices/xmega_au/basic_pin.h>
#include <mulabs_avr/devices/xmega_au/basic_pin_set.h>
#include <mulabs_avr/devices/xmega_au/basic_port.h>
#include <mulabs_avr/utility/bits.h>


namespace mulabs {
namespace avr {
namespace xmega_au {

template<class pMCU>
	class BasicIO
	{
	  public:
		typedef pMCU				MCU;
		typedef BasicPin<MCU>		Pin;
		typedef BasicPort<MCU>		Port;
		typedef BasicPinSet<MCU>	PinSet;

	  public:
		/**
		 * Configure given pins as inputs.
		 */
		static void
		configure_as_inputs (PinSet pin_set);

		/**
		 * Configure given pins as inputs.
		 */
		template<class ...Pins>
			static constexpr void
			configure_as_inputs (Pins ...pins);

		/**
		 * Return port object by number.
		 */
		static constexpr Port
		port_at (uint8_t port_number);

		/**
		 * Configure given pins as outputs.
		 */
		static constexpr void
		configure_as_outputs (PinSet pin_set);

		/**
		 * Configure given pins as outputs.
		 */
		template<class ...Pins>
			static constexpr void
			configure_as_outputs (Pins ...pins);

		/**
		 * Recursive function that collects Pins.
		 */
		template<class Pin, class ...Pins>
			static constexpr PinSet
			make_pin_set (Pin const pin, Pins const ...pins)
			{
				auto new_pins = make_pin_set (pins...).pins();
				new_pins.set_bit (pin.absolute_pin_number());
				return PinSet (new_pins);
			}

		/**
		 * Recursive stop-condition for make_pin_set().
		 */
		template<class Pin>
			static constexpr PinSet
			make_pin_set (Pin const pin)
			{
				typename MCU::Pins pins { };
				pins.set_bit (pin.absolute_pin_number());
				return PinSet (pins);
			}

		// TODO virtual port mapping VPCTRLA VPCTRLB
		// TODO CLKEVOUT
		// TODO EBIOUT
		// TODO EVCTRL
	};


template<class M>
	constexpr typename BasicIO<M>::Port
	BasicIO<M>::port_at (uint8_t port_number)
	{
		return MCU::ports_index[port_number];
	}


template<class M>
	void
	BasicIO<M>::configure_as_inputs (PinSet pset)
	{
		auto const sorted_pins = pset.sorted_pins();

		for (size_t i = 0; i < sorted_pins.first_zero_element; ++i)
		{
			auto pp = sorted_pins.ports_and_pin_bits[i];
			port_at (pp.port_number).configure_as_inputs (pp.pin_bits);
		}
	}


template<class M>
	template<class ...Pins>
		constexpr void
		BasicIO<M>::configure_as_inputs (Pins ...pins)
		{
			configure_as_inputs (make_pin_set (pins...));
		}


template<class M>
	constexpr void
	BasicIO<M>::configure_as_outputs (PinSet pset)
	{
		auto const sorted_pins = pset.sorted_pins();

		for (size_t i = 0; i < sorted_pins.first_zero_element; ++i)
		{
			auto pp = sorted_pins.ports_and_pin_bits[i];
			port_at (pp.port_number).configure_as_outputs (pp.pin_bits);
		}
	}


template<class M>
	template<class ...Pins>
		constexpr void
		BasicIO<M>::configure_as_outputs (Pins ...pins)
		{
			configure_as_outputs (make_pin_set (pins...));
		}

} // namespace xmega_au
} // namespace avr
} // namespace mulabs

#endif

