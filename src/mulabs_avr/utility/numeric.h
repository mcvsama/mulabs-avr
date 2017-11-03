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

#ifndef MULABS_AVR__UTILITY__NUMERIC_H__INCLUDED
#define MULABS_AVR__UTILITY__NUMERIC_H__INCLUDED

// Mulabs:
#include <mulabs_avr/utility/range.h>


namespace mulabs {
namespace avr {

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

