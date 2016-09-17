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

#ifndef MULABS_AVR__UTILITY__TYPE_TRAITS_H__INCLUDED
#define MULABS_AVR__UTILITY__TYPE_TRAITS_H__INCLUDED


namespace mulabs {
namespace avr {

/**
 * std::integral_constant
 */
template<class pType, pType pValue>
	class integral_constant
	{
	  public:
		typedef pType								value_type;
		typedef integral_constant<pType, pValue>	type;

		static constexpr value_type					value = pValue;

		constexpr value_type
		operator()() const
		{
			return value;
		}
	};


template<class T, T V>
	constexpr typename integral_constant<T, V>::value_type integral_constant<T, V>::value;


/**
 * std::bool_constant and related
 */
template<bool pValue>
	using bool_constant = integral_constant<bool, pValue>;


typedef integral_constant<bool, true>     true_type;

typedef integral_constant<bool, false>    false_type;


/*
 * Meta-helpers.
 */


template<class T>
	struct is_literal_type: public integral_constant<bool, __is_literal_type (T)>
	{ };

} // namespace avr
} // namespace mulabs

#endif

