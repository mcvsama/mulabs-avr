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

#ifndef MULABS_AVR__UTILITY__CRAP_DECODER_H__INCLUDED
#define MULABS_AVR__UTILITY__CRAP_DECODER_H__INCLUDED

#include "gray_decoder.h"


namespace mulabs {
namespace avr {

/**
 * This is a decoder for crappy rotary encoders, designed most probably by some morons,
 * that change output by two Gray steps on every rotation step, instead of one.
 *
 * Crap-encoder output table:
 *   A: 1 1 0 0 1 1 0
 *   B: 1 0 0 1 1 0 0
 *        ↑   ↑   ↑ - Stable position
 */
class CrapDecoder
{
  public:
	CrapDecoder (bool init_a = false, bool init_b = false):
		_gray_decoder (init_a, init_b)
	{ }

	/**
	 * Reset state to given values.
	 */
	void
	reset (bool init_a, bool init_b)
	{
		_gray_decoder.reset (init_a, init_b);
	}

	/**
	 * Return -1, 0 or +1, depending on change
	 * detected on input data.
	 */
	int8_t
	update (bool a, bool b)
	{
		int8_t result = 0;
		_sum += _gray_decoder.update (a, b);

		if (_sum == 2 || _sum == -2)
		{
			result = _sum / 2;
			_sum = 0;
		}

		return result;
	}

  private:
	GrayDecoder	_gray_decoder;
	int8_t		_sum = 0;
};

} // namespace avr
} // namespace mulabs

#endif

