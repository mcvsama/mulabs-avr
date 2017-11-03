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

#ifndef MULABS_AVR__DEVICES__COMMON__COMMON_BASIC_PINSET_H__INCLUDED
#define MULABS_AVR__DEVICES__COMMON__COMMON_BASIC_PINSET_H__INCLUDED

// Mulabs AVR:
#include <mulabs_avr/utility/array.h>


namespace mulabs {
namespace avr {

/**
 * Stores list of all pins as bits set to 1 in an array
 * of all ports. Each port is 8-bit integer, each bit
 * represents one pin.
 */
template<class pMCU>
	class CommonBasicPinSet
	{
	  public:
		using MCU		= pMCU;
		using AllPins	= Array<typename MCU::PortIntegerType, MCU::kNumPorts>;

		struct PortAndPinBits
		{
			uint8_t	port_number;
			uint8_t	pin_bits;
		};

		struct SortedPins
		{
			Array<PortAndPinBits, AllPins::size()>	ports_and_pin_bits;
			size_t									first_zero_element;
		};

	  public:
		// Ctor
		explicit constexpr
		CommonBasicPinSet (AllPins pins);

		/**
		 * Return array of ports/pins.
		 * If a bit is set to 1, then pin is considered existing
		 * in the list.
		 */
		constexpr AllPins
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
			make_port_and_pin_bits_helper (AllPins pins, Array<PortAndPinBits, N> partial_result, size_t index)
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
		 *			Start index.
		 */
		static constexpr Array<PortAndPinBits, AllPins::size()>
		make_port_and_pin_bits (AllPins pins)
		{
			return make_port_and_pin_bits_helper (pins, Array<PortAndPinBits, AllPins::size()>(), 0);
		}

	  private:
		AllPins const	_all_pins;
	};


template<class M>
	constexpr
	CommonBasicPinSet<M>::CommonBasicPinSet (AllPins pins):
		_all_pins (pins)
	{
	}


template<class M>
	constexpr typename CommonBasicPinSet<M>::AllPins
	CommonBasicPinSet<M>::pins() const
	{
		return _all_pins;
	}


template<class M>
	constexpr typename CommonBasicPinSet<M>::SortedPins
	CommonBasicPinSet<M>::sorted_pins() const
	{
		return sorted_pins (make_port_and_pin_bits (pins()));
	}

} // namespace avr
} // namespace mulabs

#endif

