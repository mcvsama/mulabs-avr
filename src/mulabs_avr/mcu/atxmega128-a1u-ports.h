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

#ifndef MULABS_AVR__MCU__ATXMEGA128_A1U_PORTS_H__INCLUDED
#define MULABS_AVR__MCU__ATXMEGA128_A1U_PORTS_H__INCLUDED

namespace mulabs {
namespace avr {

#define MULABS_AVR_DECLARE_PORT(constant_prefix, base_address) \
	static constexpr size_t constant_prefix				= base_address; \
	static constexpr size_t constant_prefix##_DIR		= constant_prefix + 0x00; \
	static constexpr size_t constant_prefix##_DIRSET	= constant_prefix + 0x01; \
	static constexpr size_t constant_prefix##_DIRCLR	= constant_prefix + 0x02; \
	static constexpr size_t constant_prefix##_DIRTGL	= constant_prefix + 0x03; \
	static constexpr size_t constant_prefix##_OUT		= constant_prefix + 0x04; \
	static constexpr size_t constant_prefix##_OUTSET	= constant_prefix + 0x05; \
	static constexpr size_t constant_prefix##_OUTCLR	= constant_prefix + 0x06; \
	static constexpr size_t constant_prefix##_OUTTGL	= constant_prefix + 0x07; \
	static constexpr size_t constant_prefix##_IN		= constant_prefix + 0x08; \
	static constexpr size_t constant_prefix##_INTCTRL	= constant_prefix + 0x09; \
	static constexpr size_t constant_prefix##_INT0MASK	= constant_prefix + 0x0A; \
	static constexpr size_t constant_prefix##_INT1MASK	= constant_prefix + 0x0B; \
	static constexpr size_t constant_prefix##_INTFLAGS	= constant_prefix + 0x0C; \
	static constexpr size_t constant_prefix##_PIN0CTRL	= constant_prefix + 0x10; \
	static constexpr size_t constant_prefix##_PIN1CTRL	= constant_prefix + 0x11; \
	static constexpr size_t constant_prefix##_PIN2CTRL	= constant_prefix + 0x12; \
	static constexpr size_t constant_prefix##_PIN3CTRL	= constant_prefix + 0x13; \
	static constexpr size_t constant_prefix##_PIN4CTRL	= constant_prefix + 0x14; \
	static constexpr size_t constant_prefix##_PIN5CTRL	= constant_prefix + 0x15; \
	static constexpr size_t constant_prefix##_PIN6CTRL	= constant_prefix + 0x16; \
	static constexpr size_t constant_prefix##_PIN7CTRL	= constant_prefix + 0x17;

MULABS_AVR_DECLARE_PORT(kPORTA, 0x600);
MULABS_AVR_DECLARE_PORT(kPORTB, 0x620);
MULABS_AVR_DECLARE_PORT(kPORTC, 0x640);
MULABS_AVR_DECLARE_PORT(kPORTD, 0x660);
MULABS_AVR_DECLARE_PORT(kPORTE, 0x680);
MULABS_AVR_DECLARE_PORT(kPORTF, 0x6A0);
MULABS_AVR_DECLARE_PORT(kPORTH, 0x6E0);
MULABS_AVR_DECLARE_PORT(kPORTJ, 0x700);
MULABS_AVR_DECLARE_PORT(kPORTK, 0x720);
MULABS_AVR_DECLARE_PORT(kPORTQ, 0x7C0);
MULABS_AVR_DECLARE_PORT(kPORTR, 0x7E0);

#undef MULABS_AVR_DECLARE_PORT

#define MULABS_AVR_DECLARE_TC0(constant_prefix, base_address) \
	static constexpr size_t constant_prefix				= base_address; \
	static constexpr size_t constant_prefix##_CTRLA		= constant_prefix + 0x00; \
	static constexpr size_t constant_prefix##_CTRLB		= constant_prefix + 0x01; \
	static constexpr size_t constant_prefix##_CTRLC		= constant_prefix + 0x02; \
	static constexpr size_t constant_prefix##_CTRLD		= constant_prefix + 0x03; \
	static constexpr size_t constant_prefix##_CTRLE		= constant_prefix + 0x04; \
	static constexpr size_t constant_prefix##_INTCTRLA	= constant_prefix + 0x06; \
	static constexpr size_t constant_prefix##_INTCTRLB	= constant_prefix + 0x07; \
	static constexpr size_t constant_prefix##_CTRLFCLR	= constant_prefix + 0x08; \
	static constexpr size_t constant_prefix##_CTRLFSET	= constant_prefix + 0x08; \
	static constexpr size_t constant_prefix##_CTRLGCLR	= constant_prefix + 0x0A; \
	static constexpr size_t constant_prefix##_CTRLGSET	= constant_prefix + 0x0B; \
	static constexpr size_t constant_prefix##_INTFLAGS	= constant_prefix + 0x0C; \
	static constexpr size_t constant_prefix##_TEMP		= constant_prefix + 0x0F; \
	static constexpr size_t constant_prefix##_CNT		= constant_prefix + 0x20; \
	static constexpr size_t constant_prefix##_CNTL		= constant_prefix + 0x20; \
	static constexpr size_t constant_prefix##_CNTH		= constant_prefix + 0x21; \
	static constexpr size_t constant_prefix##_PER		= constant_prefix + 0x26; \
	static constexpr size_t constant_prefix##_PERL		= constant_prefix + 0x26; \
	static constexpr size_t constant_prefix##_PERH		= constant_prefix + 0x27; \
	static constexpr size_t constant_prefix##_CCA		= constant_prefix + 0x28; \
	static constexpr size_t constant_prefix##_CCAL		= constant_prefix + 0x28; \
	static constexpr size_t constant_prefix##_CCAH		= constant_prefix + 0x29; \
	static constexpr size_t constant_prefix##_CCB		= constant_prefix + 0x2A; \
	static constexpr size_t constant_prefix##_CCBL		= constant_prefix + 0x2A; \
	static constexpr size_t constant_prefix##_CCBH		= constant_prefix + 0x2B; \
	static constexpr size_t constant_prefix##_CCC		= constant_prefix + 0x2C; \
	static constexpr size_t constant_prefix##_CCCL		= constant_prefix + 0x2C; \
	static constexpr size_t constant_prefix##_CCCH		= constant_prefix + 0x2D; \
	static constexpr size_t constant_prefix##_CCD		= constant_prefix + 0x2E; \
	static constexpr size_t constant_prefix##_CCDL		= constant_prefix + 0x2E; \
	static constexpr size_t constant_prefix##_CCDH		= constant_prefix + 0x2F; \
	static constexpr size_t constant_prefix##_PERBUF	= constant_prefix + 0x36; \
	static constexpr size_t constant_prefix##_PERBUFL	= constant_prefix + 0x36; \
	static constexpr size_t constant_prefix##_PERBUFH	= constant_prefix + 0x37; \
	static constexpr size_t constant_prefix##_CCABUF	= constant_prefix + 0x38; \
	static constexpr size_t constant_prefix##_CCABUFL	= constant_prefix + 0x38; \
	static constexpr size_t constant_prefix##_CCABUFH	= constant_prefix + 0x39; \
	static constexpr size_t constant_prefix##_CCBBUF	= constant_prefix + 0x3A; \
	static constexpr size_t constant_prefix##_CCBBUFL	= constant_prefix + 0x3A; \
	static constexpr size_t constant_prefix##_CCBBUFH	= constant_prefix + 0x3B; \
	static constexpr size_t constant_prefix##_CCCBUF	= constant_prefix + 0x3C; \
	static constexpr size_t constant_prefix##_CCCBUFL	= constant_prefix + 0x3C; \
	static constexpr size_t constant_prefix##_CCCBUFH	= constant_prefix + 0x3D; \
	static constexpr size_t constant_prefix##_CCDBUF	= constant_prefix + 0x3E; \
	static constexpr size_t constant_prefix##_CCDBUFL	= constant_prefix + 0x3E; \
	static constexpr size_t constant_prefix##_CCDBUFH	= constant_prefix + 0x3F;

MULABS_AVR_DECLARE_TC0(kTCC0, 0x0800)
MULABS_AVR_DECLARE_TC0(kTCD0, 0x0900)
MULABS_AVR_DECLARE_TC0(kTCE0, 0x0A00)
MULABS_AVR_DECLARE_TC0(kTCF0, 0x0B00)

#undef MULABS_AVR_DECLARE_TC0

#define MULABS_AVR_DECLARE_TC1(constant_prefix, base_address) \
	static constexpr size_t constant_prefix				= base_address; \
	static constexpr size_t constant_prefix##_CTRLA		= constant_prefix + 0x00; \
	static constexpr size_t constant_prefix##_CTRLB		= constant_prefix + 0x01; \
	static constexpr size_t constant_prefix##_CTRLC		= constant_prefix + 0x02; \
	static constexpr size_t constant_prefix##_CTRLD		= constant_prefix + 0x03; \
	static constexpr size_t constant_prefix##_CTRLE		= constant_prefix + 0x04; \
	static constexpr size_t constant_prefix##_INTCTRLA	= constant_prefix + 0x06; \
	static constexpr size_t constant_prefix##_INTCTRLB	= constant_prefix + 0x07; \
	static constexpr size_t constant_prefix##_CTRLFCLR	= constant_prefix + 0x08; \
	static constexpr size_t constant_prefix##_CTRLFSET	= constant_prefix + 0x08; \
	static constexpr size_t constant_prefix##_CTRLGCLR	= constant_prefix + 0x0A; \
	static constexpr size_t constant_prefix##_CTRLGSET	= constant_prefix + 0x0B; \
	static constexpr size_t constant_prefix##_INTFLAGS	= constant_prefix + 0x0C; \
	static constexpr size_t constant_prefix##_TEMP		= constant_prefix + 0x0F; \
	static constexpr size_t constant_prefix##_CNT		= constant_prefix + 0x20; \
	static constexpr size_t constant_prefix##_CNTL		= constant_prefix + 0x20; \
	static constexpr size_t constant_prefix##_CNTH		= constant_prefix + 0x21; \
	static constexpr size_t constant_prefix##_PER		= constant_prefix + 0x26; \
	static constexpr size_t constant_prefix##_PERL		= constant_prefix + 0x26; \
	static constexpr size_t constant_prefix##_PERH		= constant_prefix + 0x27; \
	static constexpr size_t constant_prefix##_CCA		= constant_prefix + 0x28; \
	static constexpr size_t constant_prefix##_CCAL		= constant_prefix + 0x28; \
	static constexpr size_t constant_prefix##_CCAH		= constant_prefix + 0x29; \
	static constexpr size_t constant_prefix##_CCB		= constant_prefix + 0x2A; \
	static constexpr size_t constant_prefix##_CCBL		= constant_prefix + 0x2A; \
	static constexpr size_t constant_prefix##_CCBH		= constant_prefix + 0x2B; \
	static constexpr size_t constant_prefix##_PERBUF	= constant_prefix + 0x36; \
	static constexpr size_t constant_prefix##_PERBUFL	= constant_prefix + 0x36; \
	static constexpr size_t constant_prefix##_PERBUFH	= constant_prefix + 0x37; \
	static constexpr size_t constant_prefix##_CCABUF	= constant_prefix + 0x38; \
	static constexpr size_t constant_prefix##_CCABUFL	= constant_prefix + 0x38; \
	static constexpr size_t constant_prefix##_CCABUFH	= constant_prefix + 0x39; \
	static constexpr size_t constant_prefix##_CCBBUF	= constant_prefix + 0x3A; \
	static constexpr size_t constant_prefix##_CCBBUFL	= constant_prefix + 0x3A; \
	static constexpr size_t constant_prefix##_CCBBUFH	= constant_prefix + 0x3B; \
	/* Those are not real registers */ \
	static constexpr size_t constant_prefix##_CCC		= constant_prefix + 0x2C; \
	static constexpr size_t constant_prefix##_CCCL		= constant_prefix + 0x2C; \
	static constexpr size_t constant_prefix##_CCCH		= constant_prefix + 0x2D; \
	static constexpr size_t constant_prefix##_CCD		= constant_prefix + 0x2E; \
	static constexpr size_t constant_prefix##_CCDL		= constant_prefix + 0x2E; \
	static constexpr size_t constant_prefix##_CCDH		= constant_prefix + 0x2F; \
	static constexpr size_t constant_prefix##_CCCBUF	= constant_prefix + 0x3C; \
	static constexpr size_t constant_prefix##_CCCBUFL	= constant_prefix + 0x3C; \
	static constexpr size_t constant_prefix##_CCCBUFH	= constant_prefix + 0x3D; \
	static constexpr size_t constant_prefix##_CCDBUF	= constant_prefix + 0x3E; \
	static constexpr size_t constant_prefix##_CCDBUFL	= constant_prefix + 0x3E; \
	static constexpr size_t constant_prefix##_CCDBUFH	= constant_prefix + 0x3F;

MULABS_AVR_DECLARE_TC1(kTCC1, 0x0840)
MULABS_AVR_DECLARE_TC1(kTCD1, 0x0940)
MULABS_AVR_DECLARE_TC1(kTCE1, 0x0A40)
MULABS_AVR_DECLARE_TC1(kTCF1, 0x0B40)

#undef MULABS_AVR_DECLARE_TC1

#define MULABS_AVR_DECLARE_USART(constant_prefix, base_address) \
	static constexpr size_t constant_prefix				= base_address; \
	static constexpr size_t constant_prefix##_DATA		= constant_prefix + 0x00; \
	static constexpr size_t constant_prefix##_STATUS	= constant_prefix + 0x01; \
	static constexpr size_t constant_prefix##_CTRLA		= constant_prefix + 0x03; \
	static constexpr size_t constant_prefix##_CTRLB		= constant_prefix + 0x04; \
	static constexpr size_t constant_prefix##_CTRLC		= constant_prefix + 0x05; \
	static constexpr size_t constant_prefix##_BAUDCTRLA	= constant_prefix + 0x06; \
	static constexpr size_t constant_prefix##_BAUDCTRLB	= constant_prefix + 0x07;

MULABS_AVR_DECLARE_USART(kUSARTC0, 0x08A0)
MULABS_AVR_DECLARE_USART(kUSARTC1, 0x08B0)
MULABS_AVR_DECLARE_USART(kUSARTD0, 0x09A0)
MULABS_AVR_DECLARE_USART(kUSARTD1, 0x09B0)
MULABS_AVR_DECLARE_USART(kUSARTE0, 0x0AA0)
MULABS_AVR_DECLARE_USART(kUSARTE1, 0x0AB0)
MULABS_AVR_DECLARE_USART(kUSARTF0, 0x0BA0)
MULABS_AVR_DECLARE_USART(kUSARTF1, 0x0BB0)

#undef MULABS_AVR_DECLARE_USART

} // namespace avr
} // namespace mulabs

#endif

