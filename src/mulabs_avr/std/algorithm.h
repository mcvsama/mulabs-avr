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


template<class T>
	constexpr T const&
	min (const T& a, const T& b)
	{
		if (b < a)
			return b;
		return a;
	}


template<class T, class Compare>
	constexpr T const&
	min (const T& a, const T& b, Compare comp)
	{
		if (comp (b, a))
			return b;
		return a;
	}


template<class T>
	constexpr T const&
	max (const T& a, const T& b)
	{
		if (a < b)
			return b;
		return a;
	}


template<class T, class Compare>
	constexpr T const&
	max (const T& a, const T& b, Compare comp)
	{
		if (comp (a, b))
			return b;
		return a;
	}


#if 0
Disabled until min_element/max_element is implemented
template<class T>
	constexpr T
	min (initializer_list<T> list)
	{
		return *min_element (list.begin(), list.end());
	}


template<class T, class Compare>
	constexpr T
	min (initializer_list<T> list, Compare comp)
	{
		return *min_element (list.begin(), list.end(), comp);
	}


template<class T>
	constexpr T
	max (initializer_list<T> list)
	{
		return *max_element (list.begin(), list.end());
	}


template<class T, class Compare>
	constexpr T
	max (initializer_list<T> list, Compare comp)
	{
		return *max_element (list.begin(), list.end(), comp);
	}
#endif

} // namespace std
} // namespace avr
} // namespace mulabs

#endif

