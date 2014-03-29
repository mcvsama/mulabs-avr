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

#ifndef MULABS_AVR__I2C_USI_SLAVE_H__INCLUDED
#define MULABS_AVR__I2C_USI_SLAVE_H__INCLUDED

// STD
#include <stdint.h>

// Local:
#include "i2c_slave.h"
#include "mcu.h"
#include "usi.h"


namespace mulabs {
namespace avr {

class I2CUSISlave: public I2CSlave
{
	enum class OnOverflow: uint8_t
	{
		Idle,
		CheckAddress,
		ReceiveByte,
		AckReceivedByte,
		SendByte,
		CheckMasterAck,
	};

  public:
	/**
	 * Configures USI for I2C.
	 *
	 * Assumes that USI_START and USI_OVF interrupts are already
	 * configured to call start_condition_detected() and data_transfer_completed(),
	 * respectively.
	 */
	void
	configure();

	/**
	 * Reset to waiting for start-condition state.
	 */
	void
	reset();

	/**
	 * Must be called when start condition is detected by USI (USI_START),
	 * usually from an interrupt handler.
	 */
	void
	start_condition();

	/**
	 * Must be called when data byte has been transferred (USI_OVF).
	 */
	void
	counter_overflow();

  private:
	/**
	 * Set high (open-drain) state on SDA.
	 */
	void
	sda_up();

	/**
	 * Set low state on SDA.
	 */
	void
	sda_down();

  private:
	OnOverflow	_on_overflow	= OnOverflow::Idle;
	bool		_skip_ack_check	= false;
};


inline void
I2CUSISlave::configure()
{
	// Configure USI for our needs.
	USI::select_mode (USI::Mode::TwoWireSCLHoldOnOverflow);
	USI::scl.configure_as_input();
	USI::scl.set_high();
	reset();
}


inline void
I2CUSISlave::reset()
{
	USI::set_counter_overflow_interrupt_enabled (false);
	// Make sure not to hold down SDA:
	sda_up();
	_on_overflow = OnOverflow::Idle;
}


inline void
I2CUSISlave::start_condition()
{
	// Ensure SCL went low:
	while (USI::scl() && !USI::sda())
	{ }

	if (USI::sda())
	{
		// SDA still high. Nothing interesing here, start condition didn't occur.
		reset();
	}
	else
	{
		// Setup SDA for receiving:
		USI::sda.configure_as_input();
		_on_overflow = OnOverflow::CheckAddress;
		USI::set_counter_overflow_interrupt_enabled (true);
		// Count 16 edges:
		USI::set_counter_value (0);
	}

	USI::start_condition_handled();
}


inline void
I2CUSISlave::counter_overflow()
{
	if (USI::scl.read())
	{
		reset();
		return;
	}

	switch (_on_overflow)
	{
		case OnOverflow::CheckAddress:
		{
			uint8_t address = USI::data();
			Direction direction = static_cast<Direction> (address & 0x01);
			address >>= 1U;

			if (addressed (address, direction))
			{
				// Setup for sending ACK:
				sda_down();
				// 2 edges (16 - 2 = 14) for ACK:
				USI::set_counter_value (14);

				if (direction == MasterWrites)
					_on_overflow = OnOverflow::ReceiveByte;
				else
				{
					// This is to skip master ACK check:
					_skip_ack_check = true;
					_on_overflow = OnOverflow::SendByte;
				}
			}
			else
				reset();
			break;
		}

		case OnOverflow::ReceiveByte:
			// Setup for byte receiving:
			sda_up();
			// 16 edges, 8 bits (16 - 16 = 0):
			USI::set_counter_value (0);

			_on_overflow = OnOverflow::AckReceivedByte;
			break;

		case OnOverflow::AckReceivedByte:
			// Done reading a byte.
			if (got_byte (USI::data()))
			{
				// Setup for sending ACK:
				sda_down();
				// 2 edges (16 - 2 = 14) for ACK:
				USI::set_counter_value (14);

				_on_overflow = OnOverflow::ReceiveByte;
			}
			else
				reset();
			break;

		case OnOverflow::SendByte:
			// If master has ACKed (or it is the first byte)
			// (LSB will be false):
			if (!get_bit (USI::data(), 0) || _skip_ack_check)
			{
				// Continue.
				USI::set_data (request_byte());
				// 16 edges, 8 bits (16 - 16 = 0):
				USI::set_counter_value (0);
				USI::sda.configure_as_output();

				_on_overflow = OnOverflow::CheckMasterAck;
				_skip_ack_check = false;
			}
			else
				reset();
			break;

		case OnOverflow::CheckMasterAck:
			// Setup for receiving ACK:
			sda_up();
			// 2 edges (16 - 2 = 14):
			USI::set_counter_value (14);

			_on_overflow = OnOverflow::SendByte;
			break;
	}

	USI::counter_overflow_handled();
}


inline void
I2CUSISlave::sda_up()
{
	USI::sda.configure_as_input();
	USI::set_data (0xff);
}


inline void
I2CUSISlave::sda_down()
{
	USI::sda.configure_as_output();
	USI::set_data (0x00);
}

} // namespace avr
} // namespace mulabs

#endif

