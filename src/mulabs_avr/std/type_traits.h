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

/*
 * std::integral_constant
 * std::true_type
 * std::false_type
 */


template<class T, T pValue>
	struct integral_constant
	{
		using value_type	= T;
		using type			= integral_constant<T, pValue>;

		static constexpr T value = pValue;

		constexpr
		operator value_type() const
		{
			return value;
		}

		constexpr value_type
		operator()() const
		{
			return value;
		}
	};


template<class T, T pValue>
	constexpr T integral_constant<T, pValue>::value;


using true_type		= integral_constant<bool, true>;
using false_type	= integral_constant<bool, false>;


/*
 * std::remove_reference
 * std::remove_reference_t
 */


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


/*
 * std::is_lvalue_reference
 * std::is_lvalue_reference_v
 */


template<class>
	struct is_lvalue_reference: public false_type
	{ };


template<class T>
	struct is_lvalue_reference<T&>: public true_type
	{ };


template<class T>
	constexpr bool is_lvalue_reference_v = is_lvalue_reference<T>::value;


/*
 * std::is_const
 * std::is_const_v
 */


template<class>
	struct is_const: public false_type
	{ };


template<class T>
	struct is_const<T const>: public true_type
	{ };


template<class T>
	constexpr bool is_const_v = is_const<T>::value;


/*
 * std::is_literal_type
 */


template<class T>
	struct is_literal_type: public integral_constant<bool, __is_literal_type (T)>
	{ };


template<class T>
	constexpr bool is_literal_type_v = is_literal_type<T>::value;


/*
 * std::is_same
 * std::is_same_v
 */


template<class, class>
	struct is_same: public false_type
	{ };


template<class T>
	struct is_same<T, T>: public true_type
	{ };


template<class T1, class T2>
	constexpr bool is_same_v = is_same<T1, T2>::value;

} // namespace std
} // namespace avr
} // namespace mulabs

#endif

