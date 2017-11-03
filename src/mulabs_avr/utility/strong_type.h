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

#ifndef MULABS_AVR__UTILITY__STRONG_TYPE_H__INCLUDED
#define MULABS_AVR__UTILITY__STRONG_TYPE_H__INCLUDED

// Mulabs:
#include <mulabs_avr/std/utility.h>


namespace mulabs {
namespace avr {

/**
 * Strong type to distinguish meaning of a type, when the representation
 * uses the same basic type. Eg. amplitude and absolute frequency can both
 * be represented with a float, but it's better if they have different types
 * that are not implicitly convertible to each other.
 *
 * Usage:
 *   using Amplitude = StrongType<float, struct AmplitudeType>;
 *   using AbsoluteFrequency = StrongType<float, struct AbsoluteFrequencyType>;
 *
 * The name of the second parameter struct is arbitrary, it only needs to be unique,
 * however, stick to the above convention unless not possible.
 */
template<class pValue, class DistinguisherType>
	class StrongType
	{
	  public:
		using Value = pValue;

	  public:
		// Ctor
		constexpr
		StrongType()
		{ }

		// Ctor
		explicit constexpr
		StrongType (Value const& value):
			_value (value)
		{ }

		// Ctor
		explicit constexpr
		StrongType (Value&& value):
			_value (std::move (value))
		{ }

		constexpr Value
		operator*() const
		{
			return _value;
		}

	  private:
		Value _value {};
	};

} // namespace avr
} // namespace mulabs

#endif


