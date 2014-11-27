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

#ifndef MULABS_AVR__INTERRUPTS_H__INCLUDED
#define MULABS_AVR__INTERRUPTS_H__INCLUDED

// AVR:
#include <avr/io.h>
#include <avr/interrupt.h>


namespace mulabs {
namespace avr {

class InterruptsLock
{
  public:
	// Ctor
	InterruptsLock()
	{
		cli();
	}

	// Dtor
	~InterruptsLock()
	{
		sei();
	}
};


class RecursiveInterruptsLock
{
  public:
	// Ctor
	RecursiveInterruptsLock():
		_saved_sreg (SREG)
	{
		cli();
	}

	// Dtor
	~RecursiveInterruptsLock()
	{
		SREG = _saved_sreg;
	}

  private:
	uint8_t _saved_sreg;
};

} // namespace avr
} // namespace mulabs

#endif

