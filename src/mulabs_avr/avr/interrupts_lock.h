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

#ifndef MULABS_AVR__AVR__INTERRUPTS_LOCK_H__INCLUDED
#define MULABS_AVR__AVR__INTERRUPTS_LOCK_H__INCLUDED

// AVR:
#include <avr/io.h>
#include <avr/interrupt.h>

// Local:
#include <mulabs_avr/utility/bits.h>


namespace mulabs {
namespace avr {

class InterruptsLock
{
  public:
	// Ctor
	InterruptsLock();

	// Dtor
	~InterruptsLock();

  private:
	int _saved_interrupt_flag;
};


InterruptsLock::InterruptsLock()
{
	_saved_interrupt_flag = get_bit (SREG, 7);
	cli();
}


InterruptsLock::~InterruptsLock()
{
	if (_saved_interrupt_flag)
		set_bit (SREG, 7);
	else
		clear_bit (SREG, 7);
}

} // namespace avr
} // namespace mulabs

#endif

