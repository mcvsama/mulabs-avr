/* vim:ts=4
 *
 * Copyleft 2012…2017  Michał Gawron
 * Marduk Unix Labs, http://mulabs.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Visit http://www.gnu.org/licenses/gpl-3.0.html for more information on licensing.
 */

#ifndef MULABS_AVR__STD__ALGORITHM_H__INCLUDED
#define MULABS_AVR__STD__ALGORITHM_H__INCLUDED

namespace mulabs {
namespace avr {
namespace std {

template<class Iterator, class Predicate>
	constexpr bool
	all_of (Iterator begin, Iterator end, Predicate pred)
	{
		while (begin != end)
		{
			if (!pred (*begin))
				return false;

			++begin;
		}

		return true;
	}


template<class Iterator, class Predicate>
	constexpr bool
	any_of (Iterator begin, Iterator end, Predicate pred)
	{
		while (begin != end)
		{
			if (pred (*begin))
				return true;

			++begin;
		}

		return false;
	}


template<class Iterator, class Predicate>
	constexpr bool
	none_of (Iterator begin, Iterator end, Predicate pred)
	{
		while (begin != end)
		{
			if (pred (*begin))
				return false;

			++begin;
		}

		return true;
	}

} // namespace std
} // namespace avr
} // namespace mulabs

#endif

