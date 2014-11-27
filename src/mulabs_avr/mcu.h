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

#ifndef MULABS_AVR__MCU_H__INCLUDED
#define MULABS_AVR__MCU_H__INCLUDED

#if defined(__AVR_ATtiny10__)
#include "mcu_t10.h"
#endif

#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
#include "mcu_tx5.h"
#endif

#if defined(__AVR_ATtiny261__) || defined(__AVR_ATtiny461__) || defined(__AVR_ATtiny861__)
#include "mcu_tx61.h"
#endif

#if defined(__AVR_ATmega32__)
#include "mcu_m32.h"
#endif

#endif

