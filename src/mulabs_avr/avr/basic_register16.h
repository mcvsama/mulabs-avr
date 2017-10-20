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
	BasicRegister16 (size_t base_address);

	// Ctor
	constexpr
	BasicRegister16 (BasicRegister8 low, BasicRegister8 high);

	uint16_t
	read() const;

	void
	write (uint16_t value) const;

	BasicRegister16 const&
	operator= (uint16_t value) const;

	/**
	 * Reference the memory-mapped register.
	 */
	uint16_t volatile&
	ref() const;

  private:
	size_t	_address;
};


constexpr
BasicRegister16::BasicRegister16 (size_t base_address):
	_address (base_address)
{ }


inline uint16_t
BasicRegister16::read() const
{
	return ref();
}


inline void
BasicRegister16::write (uint16_t value) const
{
	ref() = value;
}


inline BasicRegister16 const&
BasicRegister16::operator= (uint16_t value) const
{
	write (value);
	return *this;
}


inline uint16_t volatile&
BasicRegister16::ref() const
{
	return *reinterpret_cast<uint16_t volatile*> (_address);
}

} // namespace avr
} // namespace mulabs

#endif

