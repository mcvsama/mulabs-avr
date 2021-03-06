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

#ifndef MULABS_AVR__UTILITY__BITS_H__INCLUDED
#define MULABS_AVR__UTILITY__BITS_H__INCLUDED

namespace mulabs {
namespace avr {

template<uint8_t Bit>
	constexpr uint8_t bit8 = static_cast<uint8_t> (1u) << Bit;


template<uint8_t Bit>
	constexpr uint16_t bit16 = static_cast<uint16_t> (1u) << Bit;


template<uint8_t Bit>
	constexpr uint32_t bit32 = static_cast<uint32_t> (1u) << Bit;


template<uint8_t Bit>
	constexpr uint32_t bit = static_cast<uint32_t> (1u) << Bit;


template<uint8_t Bit, class R>
	static constexpr void
	set_bit (R volatile& reg)
	{
		reg |= (static_cast<R> (1) << Bit);
	}


template<uint8_t Bit, class R>
	static constexpr void
	clear_bit (R volatile& reg)
	{
		reg &= ~(static_cast<R> (1) << Bit);
	}


template<uint8_t Bit, class R>
	static constexpr void
	set_bit_value (R volatile& reg, bool value)
	{
		if (value)
			set_bit<Bit> (reg);
		else
			clear_bit<Bit> (reg);
	}


template<uint8_t Bit, class R>
	static constexpr bool
	get_bit (R const volatile& reg)
	{
		return reg & (static_cast<R> (1) << Bit);
	}


template<uint8_t MostSignificantBit, uint8_t LeastSignificantBit, class R>
	static constexpr R
	mask_of_ones()
	{
		constexpr uint8_t BitWidth = sizeof (R) * 8;

		static_assert (LeastSignificantBit <= MostSignificantBit);
		static_assert (MostSignificantBit < BitWidth);

		R const ffff = static_cast<R> (-1);
		R const upper_mask = MostSignificantBit + 1 < BitWidth
			? static_cast<R> (ffff << (MostSignificantBit + 1))
			: 0;
		R const lower_mask = LeastSignificantBit > 0
			? static_cast<R> (ffff >> (BitWidth - LeastSignificantBit))
			: 0;

		return static_cast<R> (~(upper_mask | lower_mask));
	}


template<uint8_t MostSignificantBit, uint8_t LeastSignificantBit, class R>
	static constexpr void
	set_bits_value (R volatile& reg, R value)
	{
		reg = (reg & ~mask_of_ones<MostSignificantBit, LeastSignificantBit, R>()) | (value << LeastSignificantBit);
	}


template<uint8_t MostSignificantBit, uint8_t LeastSignificantBit, class R>
	static constexpr R
	get_bits_value (R volatile& reg)
	{
		return (reg & mask_of_ones<MostSignificantBit, LeastSignificantBit, R>()) >> LeastSignificantBit;
	}


/**
 * Swap MSB-LSB bits in a byte.
 */
constexpr uint8_t
reverse_bits (uint8_t byte)
{
    byte = ((byte >> 1) & 0x55) | ((byte << 1) & 0xaa);
    byte = ((byte >> 2) & 0x33) | ((byte << 2) & 0xcc);
    byte = ((byte >> 4) & 0x0f) | ((byte << 4) & 0xf0);
    return byte;
}

} // namespace avr
} // namespace mulabs

#endif

