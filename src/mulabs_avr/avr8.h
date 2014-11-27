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

#ifndef MULABS_AVR__AVR8_H__INCLUDED
#define MULABS_AVR__AVR8_H__INCLUDED

// Local:
#include "interrupts.h"
#include "port.h"
#include "pin.h"


namespace mulabs {
namespace avr {

class AVR8
{
  public:
	typedef uint8_t volatile& Register;

	typedef PortTemplate<Register>	Port;
	typedef PinTemplate<Register>	Pin;
};

} // namespace avr
} // namespace mulabs

#endif

