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

// Mulabs AVR:
#include <mulabs_avr/utility/array.h>


namespace mulabs {
namespace avr {
namespace xmega_au {

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

		struct PortAndPinBits
		{
			uint8_t	port_number;
			uint8_t	pin_bits;
		};

		struct SortedPins
		{
			Array<PortAndPinBits, Pins::size()>	ports_and_pin_bits;
			size_t								first_zero_element;
		};

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

		/**
		 * Return array of pins so that ports that have any pins set to 1
		 * will be at the beginning, and the rest will be at the end of resulting
		 * SortedPins array.
		 */
		constexpr SortedPins
		sorted_pins() const;

	  private:
		/**
		 * Find next non-zero value in @array, starting at @start_at.
		 */
		template<size_t N>
			static constexpr size_t
			next_non_zero (Array<PortAndPinBits, N> array, size_t start_at)
			{
				if (array[start_at].pin_bits)
					return start_at;
				else if (start_at + 1 < array.size())
					return next_non_zero (array, start_at + 1);
				else
					return N;
			}

		/**
		 * Recursive helper for sorted_pins().
		 */
		template<size_t N>
			static constexpr SortedPins
			sort_helper (Array<PortAndPinBits, N> array, size_t start_at)
			{
				if (array[start_at].pin_bits)
					return sort_helper<N> (array, start_at + 1);
				else
				{
					auto nnz = next_non_zero (array, start_at);

					if (nnz == N)
						return { array, start_at };
					else
					{
						array[start_at] = array[nnz];
						array[nnz] = { 0, 0 };
						return sort_helper<N> (array, start_at + 1);
					}
				}
			}

		/**
		 * Actual implementation of sorted_pins().
		 */
		template<size_t N>
			static constexpr SortedPins
			sorted_pins (Array<PortAndPinBits, N> array)
			{
				return sort_helper<N> (array, 0);
			}

		/**
		 * Recursive helper for function make_port_and_pin_bits().
		 */
		template<size_t N>
			static constexpr Array<PortAndPinBits, N>
			make_port_and_pin_bits_helper (Pins pins, Array<PortAndPinBits, N> partial_result, size_t index)
			{
				if (index < N)
				{
					partial_result[index].port_number = index;
					partial_result[index].pin_bits = pins[index];
				}

				return partial_result;
			}

		/**
		 * Create array of PortAndPinBits from simple array of ints.
		 *
		 * \param	index
		 * 			Start index.
		 */
		static constexpr Array<PortAndPinBits, Pins::size()>
		make_port_and_pin_bits (Pins pins)
		{
			return make_port_and_pin_bits_helper (pins, Array<PortAndPinBits, Pins::size()>(), 0);
		}

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


template<class M>
	constexpr typename BasicPinSet<M>::SortedPins
	BasicPinSet<M>::sorted_pins() const
	{
		return sorted_pins (make_port_and_pin_bits (pins()));
	}

} // namespace xmega_au
} // namespace avr
} // namespace mulabs

#endif

