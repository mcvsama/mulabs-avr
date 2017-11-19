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

#ifndef MULABS_AVR__UTILITY__SPAN_H__INCLUDED
#define MULABS_AVR__UTILITY__SPAN_H__INCLUDED

// Standard:
#include <stddef.h>

// Local:
#include <mulabs_avr/utility/bits.h>


namespace mulabs {
namespace avr {

// TODO have two versions, for const data and non-const data.
template<class pValue>
	class Span
	{
	  public:
		typedef pValue				Value;

		// Standard compatibility:
		typedef pValue				value_type;
		typedef size_t				size_type;
		typedef ptrdiff_t			difference_type;
		typedef value_type&			reference;
		typedef value_type const&	const_reference;
		typedef value_type*			pointer;
		typedef value_type const*	const_pointer;

		// Thrown by as() method.
		class BufferTooSmall
		{ };

	  public:
		// Ctor
		constexpr
		Span() = default;

		// Ctor
		constexpr
		Span (Span const&) = default;

		// Ctor
		constexpr
		Span (Value* data, size_t length);

		// Ctor
		template<size_t N>
			constexpr
			Span (Value (&data)[N]);

		// Ctor
		template<size_t N>
			constexpr
			Span (Array<Value, N>&);

		constexpr reference
		operator[] (size_type pos);

		constexpr const_reference
		operator[] (size_type pos) const;

		constexpr reference
		front();

		constexpr const_reference
		front() const;

		constexpr reference
		back();

		constexpr const_reference
		back() const;

		constexpr pointer
		data();

		constexpr const_pointer
		data() const;

		constexpr bool
		empty() const;

		constexpr size_type
		size() const;

		constexpr size_type
		max_size() const;

		constexpr void
		remove_prefix (size_type n);

		constexpr void
		remove_suffix (size_type n);

		/**
		 * Fill array with given value.
		 */
		constexpr void
		fill (const_reference value);

		/**
		 * Set given bit in the array to 1.
		 */
		constexpr void
		set_bit_value (size_t bit_number, bool value);

		/**
		 * Return buffer casted to given type.
		 */
		template<class Target>
			constexpr Target&
			as();

		/**
		 * Return buffer casted to given type.
		 */
		template<class Target>
			constexpr Target const&
			as() const;

	  private:
		pointer	_data	= nullptr;
		size_t	_size	= 0;
	};


template<class V>
	constexpr
	Span<V>::Span (Value* data, size_t size):
		_data (data),
		_size (size)
	{ }


template<class V>
	template<size_t N>
		constexpr
		Span<V>::Span (Value (&data)[N]):
			_data (data),
			_size (N)
		{ }


template<class V>
	template<size_t N>
		constexpr
		Span<V>::Span (Array<Value, N>& array):
			_data (array.data()),
			_size (array.size())
		{ }


template<class V>
	constexpr typename Span<V>::reference
	Span<V>::operator[] (size_type pos)
	{
		return _data[pos];
	}


template<class V>
	constexpr typename Span<V>::const_reference
	Span<V>::operator[] (size_type pos) const
	{
		return _data[pos];
	}


template<class V>
	constexpr typename Span<V>::reference
	Span<V>::front()
	{
		return _data[0];
	}


template<class V>
	constexpr typename Span<V>::const_reference
	Span<V>::front() const
	{
		return _data[0];
	}


template<class V>
	constexpr typename Span<V>::reference
	Span<V>::back()
	{
		return _data[size() - 1];
	}


template<class V>
	constexpr typename Span<V>::const_reference
	Span<V>::back() const
	{
		return _data[size() - 1];
	}


template<class V>
	constexpr typename Span<V>::pointer
	Span<V>::data()
	{
		return _data;
	}


template<class V>
	constexpr typename Span<V>::const_pointer
	Span<V>::data() const
	{
		return _data;
	}


template<class V>
	constexpr bool
	Span<V>::empty() const
	{
		return size() == 0;
	}


template<class V>
	constexpr typename Span<V>::size_type
	Span<V>::size() const
	{
		return _size;
	}


template<class V>
	constexpr typename Span<V>::size_type
	Span<V>::max_size() const
	{
		return _size;
	}


template<class V>
	constexpr void
	Span<V>::remove_prefix (size_type n)
	{
		_data += n;
	}


template<class V>
	constexpr void
	Span<V>::remove_suffix (size_type n)
	{
		_size -= n;
	}


template<class V>
	constexpr void
	Span<V>::fill (const_reference value)
	{
		for (size_type i = 0; i < size(); ++i)
			_data[i] = value;
	}


template<class V>
	constexpr void
	Span<V>::set_bit_value (size_t bit_number, bool value)
	{
		constexpr size_t pos = bit_number / sizeof (value_type);
		constexpr value_type mask = static_cast<value_type> (1) << (bit_number % sizeof (value_type));

		if (value)
			_data[pos] |= mask;
		else
			_data[pos] &= ~mask;
	}


template<class V>
	template<class Target>
		constexpr Target&
		Span<V>::as()
		{
			if (sizeof (Target) > size() * sizeof (Value))
				throw BufferTooSmall();

			return reinterpret_cast<Target&> (*data());
		}


template<class V>
	template<class Target>
		constexpr Target const&
		Span<V>::as() const
		{
			return const_cast<Span<V>*> (this)->as<Target>();
		}

} // namespace avr
} // namespace mulabs

#endif

