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

#ifndef MULABS_AVR__UTILITY__ATOMIC_H__INCLUDED
#define MULABS_AVR__UTILITY__ATOMIC_H__INCLUDED

namespace mulabs {
namespace avr {

/**
 * This class assumes that 8-bit boolean read/write is atomic.
 */
template<class pValue>
	class Atomic
	{
		static_assert (sizeof (pValue) == 1, "value must fit in 8 bits");

	  public:
		using Value = pValue;

	  public:
		// Ctor
		Atomic() = default;

		// Ctor
		Atomic (Value initial_value);

		// Copy ctor
		Atomic (Atomic const& other);

		// Assignment operator
		Atomic const&
		operator= (Atomic const& other);

		/**
		 * Load value atomically.
		 */
		Value
		load() const;

		/**
		 * Atore value atomically.
		 */
		void
		store (Value);

	  private:
		Value volatile _value {};
	};


template<class V>
	inline
	Atomic<V>::Atomic (Value initial_value):
		_value (initial_value)
	{ }


template<class V>
	inline
	Atomic<V>::Atomic (Atomic const& other):
		_value (other._value)
	{ }


template<class V>
	inline Atomic<V> const&
	Atomic<V>::operator= (Atomic const& other)
	{
		_value = other._value;
		return *this;
	}


template<class V>
	inline typename Atomic<V>::Value
	Atomic<V>::load() const
	{
		return _value;
	}


template<class V>
	inline void
	Atomic<V>::store (Value value)
	{
		_value = value;
	}

} // namespace avr
} // namespace mulabs

#endif

