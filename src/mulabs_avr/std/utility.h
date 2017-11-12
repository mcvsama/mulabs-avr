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

#ifndef MULABS_AVR__STD__UTILITY_H__INCLUDED
#define MULABS_AVR__STD__UTILITY_H__INCLUDED

// Local:
#include "type_traits.h"


namespace mulabs {
namespace avr {
namespace std {

template<class T>
	constexpr T&&
	forward (remove_reference_t<T>& value) noexcept
	{
		return static_cast<T&&> (value);
	}


template<class T>
	constexpr T&&
	forward (remove_reference_t<T>&& value) noexcept
	{
		static_assert (!is_lvalue_reference_v<T>, "template argument substituting T is an lvalue reference type");

		return static_cast<T&&> (value);
	}


template<class T>
    constexpr remove_reference_t<T>&&
    move (T&& t) noexcept
    {
		return static_cast<remove_reference_t<T>&&> (t);
	}


template<class T, class U = T>
	inline T
	exchange (T& obj, U&& new_val)
	{
		T old_val = move (obj);
		obj = forward<U> (new_val);
		return old_val;
	}

} // namespace std
} // namespace avr
} // namespace mulabs

#endif

