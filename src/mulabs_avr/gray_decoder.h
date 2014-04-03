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

#ifndef MULABS_AVR__GRAY_DECODER_H__INCLUDED
#define MULABS_AVR__GRAY_DECODER_H__INCLUDED

namespace mulabs {
namespace avr {

class GrayDecoder
{
  public:
	GrayDecoder (bool init_a = false, bool init_b = false):
		_prev_a (init_a),
		_prev_b (init_b)
	{ }

	/**
	 * Reset state to given values.
	 */
	void
	reset (bool init_a, bool init_b)
	{
		_prev_a = init_a;
		_prev_b = init_b;
	}

	/**
	 * Return -1, 0 or +1, depending on change
	 * detected on input data.
	 */
	int8_t
	update (bool a, bool b)
	{
		uint8_t da = _prev_a - a;
		uint8_t db = _prev_b - b;

		if (da == 0 && db == 0)
			return 0;

		uint8_t result = (da == 1 && b == 0) || (a == 1 && db == 1) || (da == -1 && b == 1) || (a == 0 && db == -1)
			? +1
			: -1;

		_prev_a = a;
		_prev_b = b;

		return result;
	}

  private:
	bool	_prev_a;
	bool	_prev_b;
};

} // namespace avr
} // namespace mulabs

#endif

