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

#ifndef MULABS_AVR__UTILITY__ARRAY_VIEW_H__INCLUDED
#define MULABS_AVR__UTILITY__ARRAY_VIEW_H__INCLUDED

// Standard:
#include <stddef.h>

// Local:
#include <mulabs_avr/utility/bits.h>


namespace mulabs {
namespace avr {

template<class pValue>
	class ArrayView
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
		ArrayView() = default;

		// Ctor
		constexpr
		ArrayView (ArrayView const&) = default;

		// Ctor
		constexpr
		ArrayView (Value* data, size_t length);

		// Ctor
		template<size_t N>
			constexpr
			ArrayView (Value (&data)[N]);

		// Ctor
		template<size_t N>
			constexpr
			ArrayView (Array<Value, N>&);

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
			as (size_t offset_bytes = 0);

		/**
		 * Return buffer casted to given type.
		 */
		template<class Target>
			constexpr Target const&
			as (size_t offset_bytes = 0) const;

	  private:
		pointer	_data	= nullptr;
		size_t	_size	= 0;
	};


template<class V>
	constexpr
	ArrayView<V>::ArrayView (Value* data, size_t size):
		_data (data),
		_size (size)
	{ }


template<class V>
	template<size_t N>
		constexpr
		ArrayView<V>::ArrayView (Value (&data)[N]):
			_data (data),
			_size (N)
		{ }


template<class V>
	template<size_t N>
		constexpr
		ArrayView<V>::ArrayView (Array<Value, N>& array):
			_data (array.data()),
			_size (array.size())
		{ }


template<class V>
	constexpr typename ArrayView<V>::reference
	ArrayView<V>::operator[] (size_type pos)
	{
		return _data[pos];
	}


template<class V>
	constexpr typename ArrayView<V>::const_reference
	ArrayView<V>::operator[] (size_type pos) const
	{
		return _data[pos];
	}


template<class V>
	constexpr typename ArrayView<V>::reference
	ArrayView<V>::front()
	{
		return _data[0];
	}


template<class V>
	constexpr typename ArrayView<V>::const_reference
	ArrayView<V>::front() const
	{
		return _data[0];
	}


template<class V>
	constexpr typename ArrayView<V>::reference
	ArrayView<V>::back()
	{
		return _data[size() - 1];
	}


template<class V>
	constexpr typename ArrayView<V>::const_reference
	ArrayView<V>::back() const
	{
		return _data[size() - 1];
	}


template<class V>
	constexpr typename ArrayView<V>::pointer
	ArrayView<V>::data()
	{
		return _data;
	}


template<class V>
	constexpr typename ArrayView<V>::const_pointer
	ArrayView<V>::data() const
	{
		return _data;
	}


template<class V>
	constexpr bool
	ArrayView<V>::empty() const
	{
		return size() == 0;
	}


template<class V>
	constexpr typename ArrayView<V>::size_type
	ArrayView<V>::size() const
	{
		return _size;
	}


template<class V>
	constexpr typename ArrayView<V>::size_type
	ArrayView<V>::max_size() const
	{
		return _size;
	}


template<class V>
	constexpr void
	ArrayView<V>::remove_prefix (size_type n)
	{
		_data += n;
	}


template<class V>
	constexpr void
	ArrayView<V>::remove_suffix (size_type n)
	{
		_size -= n;
	}


template<class V>
	constexpr void
	ArrayView<V>::fill (const_reference value)
	{
		for (size_type i = 0; i < size(); ++i)
			_data[i] = value;
	}


template<class V>
	constexpr void
	ArrayView<V>::set_bit_value (size_t bit_number, bool value)
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
		ArrayView<V>::as (size_t offset_bytes)
		{
			// TODO take offset_bytes into account
			if (sizeof (Target) > size() * sizeof (Value))
				throw BufferTooSmall();

			uint8_t* raw_data = reinterpret_cast<uint8_t*> (data());
			return reinterpret_cast<Target&> (*(raw_data + offset_bytes));
		}


template<class V>
	template<class Target>
		constexpr Target const&
		ArrayView<V>::as (size_t offset_bytes) const
		{
			return const_cast<ArrayView<V>*> (this)->as<Target> (offset_bytes);
		}

} // namespace avr
} // namespace mulabs

#endif

