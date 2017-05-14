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

#ifndef MULABS_AVR__DEVICES__XMEGA_AU__INTERRUPT_SYSTEM_H__INCLUDED
#define MULABS_AVR__DEVICES__XMEGA_AU__INTERRUPT_SYSTEM_H__INCLUDED


namespace mulabs {
namespace avr {
namespace xmega_au {

class InterruptSystem
{
  public:
	enum class Config: uint8_t
	{
		Level0	= 0b00,	// Interrupts disabled
		Level1	= 0b01,	// Low level interrupt
		Level2	= 0b10,	// Medium-level interrupt
		Level3	= 0b11,	// High-level interrupt
	};
};

} // namespace xmega_au
} // namespace avr
} // namespace mulabs

#endif

