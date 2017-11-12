/* vim:ts=4
 *
 * Copyleft 2012…2014	Michał Gawron
 * Marduk Unix Labs, http://mulabs.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option any later version.
 *
 * Visit http://www.gnu.org/licenses/gpl-3.0.html for more information on licensing.
 */

#ifndef MULABS_AVR__MCU__ATMEGA32_U4_REGISTERS_H__INCLUDED
#define MULABS_AVR__MCU__ATMEGA32_U4_REGISTERS_H__INCLUDED

namespace mulabs {
namespace avr {

// For IN/OUT instructions, kPortsMemBase is not needed, but for memory-mapped IO, one needs to add
// 0x20 to all port addresses that would be normally used with IN/OUT instructions.
static constexpr size_t kPortsMemBase	= 0x20;

static constexpr size_t kPORTB_PIN		= 0x03 + kPortsMemBase;
static constexpr size_t kPORTB_DDR      = 0x04 + kPortsMemBase;
static constexpr size_t kPORTB_PORT     = 0x05 + kPortsMemBase;

static constexpr size_t kPORTC_PIN		= 0x06 + kPortsMemBase;
static constexpr size_t kPORTC_DDR      = 0x07 + kPortsMemBase;
static constexpr size_t kPORTC_PORT     = 0x08 + kPortsMemBase;

static constexpr size_t kPORTD_PIN		= 0x09 + kPortsMemBase;
static constexpr size_t kPORTD_DDR      = 0x0a + kPortsMemBase;
static constexpr size_t kPORTD_PORT     = 0x0b + kPortsMemBase;

static constexpr size_t kPORTE_PIN		= 0x0c + kPortsMemBase;
static constexpr size_t kPORTE_DDR      = 0x0d + kPortsMemBase;
static constexpr size_t kPORTE_PORT     = 0x0e + kPortsMemBase;

static constexpr size_t kPORTF_PIN		= 0x0f + kPortsMemBase;
static constexpr size_t kPORTF_DDR      = 0x10 + kPortsMemBase;
static constexpr size_t kPORTF_PORT     = 0x11 + kPortsMemBase;


} // namespace avr
} // namespace mulabs

#endif

