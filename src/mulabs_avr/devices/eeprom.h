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

#ifndef MULABS_AVR__DEVICES__EEPROM_H__INCLUDED
#define MULABS_AVR__DEVICES__EEPROM_H__INCLUDED

// Mulabs AVR:
#include <mulabs_avr/avr/interrupts_lock.h>


namespace mulabs {
namespace avr {

class EEPROM256
{
  private:
	enum class Mode: uint8_t
	{
		Rewrite		= 0b00,
		EraseOnly	= 0b01,
		WriteOnly	= 0b10,
	};

	class ByteProxy
	{
	  public:
		constexpr explicit
		ByteProxy (EEPROM256 const& eeprom, uint8_t const address):
			_eeprom (eeprom),
			_address (address)
		{ }

		/**
		 * Rewrite byte in EEPROM.
		 */
		void
		operator= (uint8_t const byte) const
		{
			_eeprom.rewrite (_address, byte);
		}

		/**
		 * Read byte from EEPROM.
		 */
		operator
		uint8_t() const
		{
			return _eeprom.read (_address);
		}

	  private:
		EEPROM256 const&	_eeprom;
		uint8_t				_address;
	};

  public:
	/**
	 * Return proxy object for writing/reading operation.
	 */
	ByteProxy
	operator[] (uint8_t address) const
	{
		return ByteProxy (*this, address);
	}

	/**
	 * Write byte to EEPROM, without erasing it first.
	 */
	void
	write (uint8_t address, uint8_t value) const
	{
		InterruptsLock lock;

		wait();

		// TODO
		throw "unimplemented";
	}

	/**
	 * Rewrite byte in EEPROM (erase + write).
	 */
	void
	rewrite (uint8_t address, uint8_t value) const
	{
		InterruptsLock lock;

		wait();
		EECR = static_cast<uint8_t> (Mode::Rewrite);
		EEAR = address;
		EEDR = value;

		set_bit (EECR, EEMPE);
		set_bit (EECR, EEPE);
	}

	/**
	 * Read byte from EEPROM.
	 */
	uint8_t
	read (uint8_t address) const
	{
		InterruptsLock lock;

		wait();
		EEAR = address;
		set_bit (EECR, EERE);

		return EEDR;
	}

	/**
	 * Wait until EEPROM operation is completed.
	 */
	void
	wait() const
	{
		while (get_bit (EECR, EEPE))
			continue;
	}
};

} // namespace avr
} // namespace mulabs

#endif

