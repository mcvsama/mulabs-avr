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
    constexpr typename std::remove_reference<T>::type&&
    move (T&& t) noexcept
    {
		return static_cast<typename std::remove_reference<T>::type&&> (t);
	}

} // namespace std
} // namespace avr
} // namespace mulabs

#endif

