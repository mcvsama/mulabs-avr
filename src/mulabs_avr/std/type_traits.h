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

#ifndef MULABS_AVR__STD__TYPE_TRAITS_H__INCLUDED
#define MULABS_AVR__STD__TYPE_TRAITS_H__INCLUDED

namespace mulabs {
namespace avr {
namespace std {

template<class T>
	struct remove_reference
	{
		typedef T type;
	};


template<class T>
	struct remove_reference<T&>
	{
		typedef T type;
	};


template<class T>
	struct remove_reference<T&&>
	{
		typedef T type;
	};


template<class T>
	using remove_reference_t = typename remove_reference<T>::type;

} // namespace std
} // namespace avr
} // namespace mulabs

#endif

