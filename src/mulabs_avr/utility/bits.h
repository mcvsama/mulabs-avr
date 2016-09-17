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

#ifndef MULABS_AVR__UTILITY__BITS_H__INCLUDED
#define MULABS_AVR__UTILITY__BITS_H__INCLUDED

// Local:
#include "range.h"


namespace mulabs {
namespace avr {

template<class T>
	static constexpr T
	bitnum (uint8_t bit)
	{
		return 1U << bit;
	}


template<class T>
	static constexpr T
	neg_bitnum (uint8_t bit)
	{
		return ~(1U << bit);
	}


template<class R>
	static constexpr void
	set_bit (R volatile& reg, uint8_t bit)
	{
		reg |= bitnum<R> (bit);
	}


template<class R>
	static constexpr void
	clear_bit (R volatile& reg, uint8_t bit)
	{
		reg &= neg_bitnum<R> (bit);
	}


template<class R>
	static constexpr void
	set_bit_value (R volatile& reg, uint8_t bit, bool value)
	{
		value ? set_bit (reg, bit) : clear_bit (reg, bit);
	}


template<class R>
	static constexpr bool
	get_bit (R const& reg, uint8_t bit)
	{
		return reg & bitnum<R> (bit);
	}


constexpr float
renormalize (float value, float a1, float b1, float a2, float b2) noexcept
{
	return b1 == a1
		? a2
		: (b2 - a2) / (b1 - a1) * value + (-(b2 - a2) / (b1 - a1) * a1 + a2);
}


constexpr float
renormalize (float value, Range<float> range1, Range<float> range2) noexcept
{
	return renormalize (value, range1.min(), range1.max(), range2.min(), range2.max());
}

} // namespace avr
} // namespace mulabs

#endif

