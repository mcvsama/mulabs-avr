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

#ifndef MULABS_AVR__AVR__AVR_FIXES_H__INCLUDED
#define MULABS_AVR__AVR__AVR_FIXES_H__INCLUDED

// Fixing AVR broken define-ased C "API":
#define __AVR_ADC ADC
#undef ADC

#define __AVR_MCU MCU
#undef MCU

#define __AVR_USB USB
#undef USB


// C++ doesn't allow reinterpret_cast<> in constexpr functions, even if the result is well-defined,
// but the reinterpret_cast<> used directly in a constexpr expression is allowed, so the only remaining
// option is to use preprocessor for getting addresses of registers.
#define MULABS_REG(avr_register) reinterpret_cast<size_t> (&avr_register)


// Missing declarations:
#ifdef _SFR_MEM8
# ifndef PORTCFG_EBIOUT
#  define PORTCFG_EBIOUT	_SFR_MEM8(0x00B5)
# endif
# ifndef PORTCFG_EVCTRL
#  define PORTCFG_EVCTRL	_SFR_MEM8(0x00B6)
# endif
#endif

#endif

