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

#ifndef MULABS_AVR__DEVICES__MEGA32U4__BASIC_JTAG_H__INCLUDED
#define MULABS_AVR__DEVICES__MEGA32U4__BASIC_JTAG_H__INCLUDED


namespace mulabs {
namespace avr {
namespace mega32u4 {

template<class pMCU>
	class BasicJTAG
	{
	  public:
		using MCU = pMCU;

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
		uint8_t v = MCUCR;
		set_bit (v, JTD);
		// Double write is needed as a protection measure:
		MCUCR = v;
		MCUCR = v;
	}


template<class M>
	inline void
	BasicJTAG<M>::disable()
	{
		uint8_t v = MCUCR;
		set_bit (v, JTD);
		// Double write is needed as a protection measure:
		MCUCR = v;
		MCUCR = v;
	}

} // namespace mega32u4
} // namespace avr
} // namespace mulabs

#endif

