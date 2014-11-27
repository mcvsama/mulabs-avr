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

#ifndef MULABS_AVR__I2C_USI_MASTER_H__INCLUDED
#define MULABS_AVR__I2C_USI_MASTER_H__INCLUDED

// STD
#include <stdint.h>

// Local:
#include "i2c_slave.h"
#include "mcu.h"
#include "usi.h"


namespace mulabs {
namespace avr {

/**
 * An object of this class can handle I2C communication as I2C slave.
 * One method should be called:
 *  - counter_overflow() on USI_OVF interrupt.
 * It will call I2CMaster virtual methods to handle addressing and data.
 */
class I2CUSIMaster: public I2CMaster
{
	/**
	 * Configures USI for I2C.
	 *
	 * Assumes that interrupts are already configured.
	 */
	void
	configure();
};


inline void
I2CUSIMaster::configure()
{
	// Configure USI for our needs.
	USI::select_mode (USI::Mode::TwoWire);
	USI::scl.configure_as_input();
	USI::scl.set_high();
	reset();
}

} // namespace avr
} // namespace mulabs

#endif

