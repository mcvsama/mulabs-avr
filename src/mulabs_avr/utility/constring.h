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

#ifndef MULABS_AVR__UTILITY__CONSTRING_H__INCLUDED
#define MULABS_AVR__UTILITY__CONSTRING_H__INCLUDED

namespace mulabs {
namespace avr {

template<class pChar>
	class Constring
	{
	  public:
		using Char = pChar;

		/**
		 * Thrown by operator[].
		 */
		class OutOfRange
		{ };

	  public:
		// Ctor
		constexpr
		Constring();

		// Ctor
		template<size_t N>
			constexpr
			Constring (Constring const& other);

		// Ctor
		template<size_t N>
			constexpr
			Constring (Char const (&data)[N]);

		template<size_t N>
			constexpr Constring const&
			operator= (Char const (&data)[N]);

		constexpr Char const*
		data() const;

		constexpr size_t
		size() const;

		constexpr Char
		operator[] (size_t n) const;

	  private:
		Char const*	_data;
		size_t		_size;
	};


template<class C>
	constexpr
	Constring<C>::Constring():
		_data (nullptr),
		_size (0)
	{ }


template<class C>
	template<size_t N>
		constexpr
		Constring<C>::Constring (Constring const& other):
			_data (other._data),
			_size (other._size)
		{ }


template<class C>
	template<size_t N>
		constexpr
		Constring<C>::Constring (Char const (&data)[N]):
			_data (data),
			_size (N - 1) // -1 is not to count the "\0" from the string literal
		{ }


template<class C>
	template<size_t N>
		constexpr Constring<C> const&
		Constring<C>::operator= (Char const (&data)[N])
		{
			_data = data;
			_size = N - 1;
			return *this;
		}


template<class C>
	constexpr typename Constring<C>::Char const*
	Constring<C>::data() const
	{
		return _data;
	}


template<class C>
	constexpr size_t
	Constring<C>::size() const
	{
		return _size;
	}


template<class C>
	constexpr typename Constring<C>::Char
	Constring<C>::operator[] (size_t n) const
	{
		//if (n >= _size)
		//	throw OutOfRange();

		return _data[n];
	}


/*
 * Global functions
 */


template<class C>
	constexpr bool
	operator== (Constring<C> const& a, Constring<C> const& b)
	{
		if (a.size() != b.size())
			return false;

		for (size_t i = 0; i < a.size(); ++i)
			if (a[i] != b[i])
				return false;

		return true;
	}

} // namespace avr
} // namespace mulabs

#endif

