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

#ifndef MULABS_AVR__AVR__BASIC_REGISTER8_H__INCLUDED
#define MULABS_AVR__AVR__BASIC_REGISTER8_H__INCLUDED

// Standard:
#include <stdlib.h>


namespace mulabs {
namespace avr {

class BasicRegister8
{
  public:
	// Ctor
	constexpr
	BasicRegister8 (size_t address);

	// Ctor
	constexpr
	BasicRegister8 (uint8_t volatile& register_reference);

	/**
	 * Read value from the register.
	 */
	uint8_t
	read() const;

	/**
	 * Alias for read().
	 */
	operator uint8_t() const;

	/**
	 * Write value to the register.
	 */
	void
	write (uint8_t value) const;

	/**
	 * Alias for write (uint8_t).
	 */
	BasicRegister8 const&
	operator= (uint8_t value) const;

	/**
	 * Reference the memory-mapped register.
	 */
	uint8_t volatile&
	ref() const;

  private:
	size_t	_address;
};


constexpr
BasicRegister8::BasicRegister8 (size_t address):
	_address (address)
{ }


constexpr
BasicRegister8::BasicRegister8 (uint8_t volatile& register_reference):
	_address (reinterpret_cast<size_t> (&register_reference))
{
}


inline uint8_t
BasicRegister8::read() const
{
	return ref();
}


inline
BasicRegister8::operator uint8_t() const
{
	return read();
}


inline void
BasicRegister8::write (uint8_t value) const
{
	ref() = value;
}


inline BasicRegister8 const&
BasicRegister8::operator= (uint8_t value) const
{
	write (value);
	return *this;
}


inline uint8_t volatile&
BasicRegister8::ref() const
{
	return *reinterpret_cast<uint8_t volatile*> (_address);
}

} // namespace avr
} // namespace mulabs

#endif

