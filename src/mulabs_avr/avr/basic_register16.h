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

#ifndef MULABS_AVR__AVR__BASIC_REGISTER16_H__INCLUDED
#define MULABS_AVR__AVR__BASIC_REGISTER16_H__INCLUDED

// Standard:
#include <stdlib.h>

// Mulabs:
#include <mulabs_avr/avr/interrupts_lock.h>


namespace mulabs {
namespace avr {

class BasicRegister16
{
  public:
	// Ctor
	constexpr
	BasicRegister16 (BasicRegister8 low, BasicRegister8 high);

	uint16_t
	read() const;

	operator uint16_t() const;

	void
	write (uint16_t value) const;

	BasicRegister16 const&
	operator= (uint16_t value) const;

  private:
	BasicRegister8	_low;
	BasicRegister8	_high;
};


constexpr
BasicRegister16::BasicRegister16 (BasicRegister8 low, BasicRegister8 high):
	_low (low),
	_high (high)
{
}


inline uint16_t
BasicRegister16::read() const
{
	InterruptsLock lock;

	uint8_t l = _low;
	uint8_t h = _high;
	return static_cast<uint16_t> (h) << 8 | l;
}


inline void
BasicRegister16::write (uint16_t value) const
{
	InterruptsLock lock;

	uint8_t l = value & 0xff;
	uint8_t h = value >> 8;
	_high = h;
	_low = l;
}

} // namespace avr
} // namespace mulabs

#endif

