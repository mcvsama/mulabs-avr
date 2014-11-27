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

#ifndef MULABS_AVR__PIN_SET_H__INCLUDED
#define MULABS_AVR__PIN_SET_H__INCLUDED

// AVR:
#include <avr/io.h>


namespace mulabs {
namespace avr {

/**
 * Represents set of pins. Used by some functions that can work
 * on a whole set at once, provided that all pins refer to the
 * same port.
 */
class PinSet
{
  public:
	constexpr explicit
	PinSet (uint8_t bits):
		_bits (bits)
	{ }

	constexpr uint8_t
	bits() const
	{
		return _bits;
	}

  private:
	uint8_t const _bits;
	// TODO Register const	reg;
};

} // namespace avr
} // namespace mulabs

#endif

