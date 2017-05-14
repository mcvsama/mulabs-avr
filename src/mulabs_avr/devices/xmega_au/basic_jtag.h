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

#ifndef MULABS_AVR__DEVICES__XMEGA_AU__BASIC_JTAG_H__INCLUDED
#define MULABS_AVR__DEVICES__XMEGA_AU__BASIC_JTAG_H__INCLUDED

// Mulabs AVR:
#include <mulabs_avr/devices/xmega_au/basic_pin_set.h>


namespace mulabs {
namespace avr {
namespace xmega_au {

template<class pMCU>
	class BasicJTAG
	{
	  public:
		typedef pMCU MCU;

	  public:
		/**
		 * Enable JTAG.
		 */
		static void
		enable();

		/**
		 * Disable JTAG and free I/O pins.
		 */
		static void
		disable();
	};


template<class M>
	inline void
	BasicJTAG<M>::enable()
	{
		MCU::disable_configuration_change_protection_for_register();
		MCU_MCUCR = 0;
	}


template<class M>
	inline void
	BasicJTAG<M>::disable()
	{
		MCU::disable_configuration_change_protection_for_register();
		MCU_MCUCR = 1;
	}

} // namespace xmega_au
} // namespace avr
} // namespace mulabs

#endif

