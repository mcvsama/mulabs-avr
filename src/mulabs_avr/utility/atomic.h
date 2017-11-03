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

// Mulabs:
#include <mulabs_avr/utility/bits.h>


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


/*
 * Global functions
 *
 * Warning: These atomic instructions do not work on memory-mapped registers. Only regular SRAM memory can be modified
 * with these. Also memory modified by the instructions must be within first 64 kB of SRAM.
 */


template<uint8_t BitMask>
	inline void
	atomic_sram_set_bitmask_value (uint8_t volatile& where, bool value)
	{
		if (value)
		{
			asm volatile (
				"ldi	r16, %[mask]	\n\t"
				".dc.w	0x9305			\n\t" // LAS instruction
				: "+m" (where)
				: "z" (&where), [mask] "i" (BitMask)
				: "r16"
			);
		}
		else
		{
			asm volatile (
				"ldi	r16, %[mask]	\n\t"
				".dc.w	0x9306			\n\t" // LAC instruction
				: "+m" (where)
				: "z" (&where), [mask] "i" (BitMask)
				: "r16"
			);
		}
	}


template<uint8_t Bit>
	inline void
	atomic_sram_set_bit_value (uint8_t volatile& where, bool value)
	{
		atomic_sram_set_bitmask_value<bit<Bit>> (where, value);
	}


template<uint8_t BitMask>
	inline void
	atomic_sram_set_bitmask (uint8_t volatile& where)
	{
		atomic_sram_set_bitmask_value<BitMask> (where, true);
	}


template<uint8_t Bit>
	inline void
	atomic_sram_set_bit (uint8_t volatile& where)
	{
		atomic_sram_set_bit_value<Bit> (where, true);
	}


template<uint8_t BitMask>
	inline void
	atomic_sram_clear_bitmask (uint8_t volatile& where)
	{
		atomic_sram_set_bitmask_value<BitMask> (where, false);
	}


template<uint8_t Bit>
	inline void
	atomic_sram_clear_bit (uint8_t volatile& where)
	{
		atomic_sram_set_bit_value<Bit> (where, false);
	}


template<uint8_t BitMask>
	inline void
	atomic_sram_toggle_bitmask (uint8_t volatile& where)
	{
		asm volatile (
			"ldi	r16, %[mask]	\n\t"
			".dc.w	0x9307			\n\t" // LAT instruction
			: "+m" (where)
			: "z" (&where), [mask] "i" (BitMask)
			: "r16"
		);
	}


template<uint8_t Bit>
	inline void
	atomic_sram_toggle_bit (uint8_t volatile& where)
	{
		atomic_sram_toggle_bitmask<bit<Bit>> (where);
	}

} // namespace avr
} // namespace mulabs

#endif

