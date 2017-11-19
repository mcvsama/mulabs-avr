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

#ifndef MULABS_AVR__UTILITY__ARRAY_H__INCLUDED
#define MULABS_AVR__UTILITY__ARRAY_H__INCLUDED

// Standard:
#include <stddef.h>

// Local:
#include <mulabs_avr/utility/bits.h>


namespace mulabs {
namespace avr {

template<class pValue, size_t pSize>
	class Array
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

	  public:
		/*
		 * These methods implement std::array() functionality.
		 */

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

		static constexpr size_type
		size();

		static constexpr size_type
		max_size();

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
			as (size_t offset_bytes = 0u);

		/**
		 * Return buffer casted to given type.
		 */
		template<class Target>
			constexpr Target const&
			as (size_t offset_bytes = 0u) const;

	  private:
		Value _data[pSize];
	};


template<class V, size_t S>
	constexpr typename Array<V, S>::reference
	Array<V, S>::operator[] (size_type pos)
	{
		return _data[pos];
	}


template<class V, size_t S>
	constexpr typename Array<V, S>::const_reference
	Array<V, S>::operator[] (size_type pos) const
	{
		return _data[pos];
	}


template<class V, size_t S>
	constexpr typename Array<V, S>::reference
	Array<V, S>::front()
	{
		return _data[0];
	}


template<class V, size_t S>
	constexpr typename Array<V, S>::const_reference
	Array<V, S>::front() const
	{
		return _data[0];
	}


template<class V, size_t S>
	constexpr typename Array<V, S>::reference
	Array<V, S>::back()
	{
		return _data[size() - 1];
	}


template<class V, size_t S>
	constexpr typename Array<V, S>::const_reference
	Array<V, S>::back() const
	{
		return _data[size() - 1];
	}


template<class V, size_t S>
	constexpr typename Array<V, S>::pointer
	Array<V, S>::data()
	{
		return _data;
	}


template<class V, size_t S>
	constexpr typename Array<V, S>::const_pointer
	Array<V, S>::data() const
	{
		return _data;
	}


template<class V, size_t S>
	constexpr bool
	Array<V, S>::empty() const
	{
		return size() == 0;
	}


template<class V, size_t S>
	constexpr typename Array<V, S>::size_type
	Array<V, S>::size()
	{
		return S;
	}


template<class V, size_t S>
	constexpr typename Array<V, S>::size_type
	Array<V, S>::max_size()
	{
		return S;
	}


template<class V, size_t S>
	constexpr void
	Array<V, S>::fill (const_reference value)
	{
		for (size_type i = 0; i < size(); ++i)
			_data[i] = value;
	}


template<class V, size_t S>
	constexpr void
	Array<V, S>::set_bit_value (size_t bit_number, bool value)
	{
		constexpr size_t pos = bit_number / sizeof (value_type);
		constexpr value_type mask = static_cast<value_type> (1) << (bit_number % sizeof (value_type));

		if (value)
			_data[pos] |= mask;
		else
			_data[pos] &= ~mask;
	}


template<class V, size_t S>
	template<class Target>
		constexpr Target&
		Array<V, S>::as (size_t offset_bytes)
		{
			static_assert (sizeof (Target) <= S * sizeof (Value), "array buffer too small for target type");

			uint8_t* raw_data = reinterpret_cast<uint8_t*> (data());
			return reinterpret_cast<Target&> (*(raw_data + offset_bytes));
		}


template<class V, size_t S>
	template<class Target>
		constexpr Target const&
		Array<V, S>::as (size_t offset_bytes) const
		{
			return const_cast<Array<V, S>*> (this)->as<Target> (offset_bytes);
		}

} // namespace avr
} // namespace mulabs

#endif

