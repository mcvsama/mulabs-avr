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

#ifndef MULABS_AVR__STD__INITIALIZER_LIST_H__INCLUDED
#define MULABS_AVR__STD__INITIALIZER_LIST_H__INCLUDED

namespace std {

/**
 * This works with GCC.
 */
template<class Type>
	class initializer_list
	{
	  public:
		using value_type		= Type;
		using reference			= Type const&;
		using const_reference	= Type const&;
		using size_type			= size_t;
		using iterator			= Type const*;
		using const_iterator	= Type const*;

	  private:
		// The compiler can call a private constructor, but user shouldn't.
		constexpr
		initializer_list (const_iterator begin, size_type size):
			_begin (begin),
			_size (size)
		{ }

	  public:
		constexpr
		initializer_list() noexcept:
			_begin (nullptr),
			_size (0)
		{ }

		constexpr size_type
		size() const noexcept
		{
			return _size;
		}

		constexpr const_iterator
		begin() const noexcept
		{
			return _begin;
		}

		// One past the last element.
		constexpr const_iterator
		end() const noexcept
		{
			return begin() + size();
		}

      private:
		iterator		_begin;
		size_type		_size;
    };


template<class Type>
	constexpr Type const*
	begin (initializer_list<Type> list) noexcept
	{
		return list.begin();
	}


template<class Type>
	constexpr Type const*
	end (initializer_list<Type> list) noexcept
	{
		return list.end();
	}

} // namespace std


namespace mulabs {
namespace avr {
namespace std {

template<class T>
	using initializer_list = ::std::initializer_list<T>;

} // namespace std
} // namespace avr
} // namespace mulabs

#endif

