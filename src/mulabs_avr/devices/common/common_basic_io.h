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

#ifndef MULABS_AVR__DEVICES__COMMON__COMMON_BASIC_IO_H__INCLUDED
#define MULABS_AVR__DEVICES__COMMON__COMMON_BASIC_IO_H__INCLUDED

// Mulabs AVR:
#include <mulabs_avr/utility/bits.h>


namespace mulabs {
namespace avr {

template<class pMCU>
	class CommonBasicIO
	{
	  public:
		using MCU		= pMCU;
		using Pin		= typename MCU::Pin;
		using Port		= typename MCU::Port;
		using PinSet	= typename MCU::PinSet;

	  public:
		/**
		 * Return port object by number.
		 */
		static constexpr Port
		port_at (uint8_t port_number);

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
		template<class Pin, class ...pPins>
			static constexpr PinSet
			make_pin_set (Pin const pin, pPins const ...pins)
			{
				if constexpr (sizeof... (pins) > 0)
				{
					auto new_pins = make_pin_set (pins...).pins();
					new_pins.set_bit (pin.absolute_pin_number());
					return PinSet (new_pins);
				}
				else
				{
					typename MCU::Pins result_pins { };
					result_pins.set_bit (pin.absolute_pin_number());
					return PinSet (result_pins);
				}
			}
	};


template<class M>
	constexpr typename CommonBasicIO<M>::Port
	CommonBasicIO<M>::port_at (uint8_t port_number)
	{
		return MCU::ports_index[port_number];
	}


template<class M>
	void
	CommonBasicIO<M>::configure_as_inputs (PinSet pset)
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
		CommonBasicIO<M>::configure_as_inputs (Pins ...pins)
		{
			configure_as_inputs (make_pin_set (pins...));
		}


template<class M>
	constexpr void
	CommonBasicIO<M>::configure_as_outputs (PinSet pset)
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
		CommonBasicIO<M>::configure_as_outputs (Pins ...pins)
		{
			configure_as_outputs (make_pin_set (pins...));
		}

} // namespace avr
} // namespace mulabs

#endif

