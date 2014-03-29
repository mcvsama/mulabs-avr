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

class InterruptsLock;


class InterruptsControl
{
  public:
	class NoControlFlag { };

  public:
	// Ctor
	InterruptsControl()
	{ }

	// Ctor
	InterruptsControl (NoControlFlag)
	{ }

	/**
	 * Execute code with interrupts disabled.
	 */
	template<class Function>
		static void
		synchronize (Function function)
		{
			InterruptsLock lock;
			function();
		}
};


class InterruptsLock: public InterruptsControl
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

  private:
	static uint8_t volatile _level;
};


class NoInterruptsLock: public InterruptsControl
{
  public:
	class Flag { };

  public:
	// Ctor
	NoInterruptsLock()
	{ }

	// Dtor
	~NoInterruptsLock()
	{ }
};


/**
 * Common object used to indicate that no interrupts are needed.
 */
static constexpr InterruptsControl::NoControlFlag no_interrupts_lock;

} // namespace avr
} // namespace mulabs

#endif

