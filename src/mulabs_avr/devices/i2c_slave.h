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

#ifndef MULABS_AVR__DEVICES__I2C_SLAVE_H__INCLUDED
#define MULABS_AVR__DEVICES__I2C_SLAVE_H__INCLUDED

// STD:
#include <stdint.h>


namespace mulabs {
namespace avr {

/**
 * Public interface to I2C slaves.
 */
class I2CSlave
{
  protected:
	/**
	 * The direction of data transfer, indicated by a direction bit
	 * just after the 7-bit address.
	 */
	enum Direction
	{
		MasterWrites	= 0,
		MasterReads		= 1,
	};

  public:
	/**
	 * Called when device is addressed.
	 *
	 * \param	address
	 * 			7-bit device address.
	 * \param	Direction
	 * 			The direction of data transfer.
	 * \return	true if byte stream should be handled, false otherwise.
	 */
	virtual bool
	addressed (uint8_t address, Direction) = 0;

	/**
	 * Called when next data byte is received.
	 *
	 * \return	true if acknowledge should be sent, false otherwise.
	 */
	virtual bool
	got_byte (uint8_t byte) = 0;

	/**
	 * Called when next byte should be sent to the master.
	 *
	 * \return	true if there is byte to send, false otherwise.
	 */
	virtual uint8_t
	request_byte() = 0;

	/**
	 * Set SDA up (open drain).
	 */
	void
	sda_up();

	/**
	 * Set SDA to low.
	 */
	void
	sda_down();

	/**
	 * Set SCL up (open drain).
	 */
	void
	scl_up();

	/**
	 * Set SCL to low.
	 */
	void
	scl_down();
};

} // namespace avr
} // namespace mulabs

#endif

