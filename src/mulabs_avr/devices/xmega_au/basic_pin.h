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

#ifndef MULABS_AVR__DEVICES__XMEGA_AU__BASIC_PIN_H__INCLUDED
#define MULABS_AVR__DEVICES__XMEGA_AU__BASIC_PIN_H__INCLUDED

#include <mulabs_avr/devices/common/common_basic_pin.h>


namespace mulabs {
namespace avr {
namespace xmega_au {

template<class pMCU>
	class BasicPin: public CommonBasicPin<pMCU>
	{
		using MCU	= pMCU;
		using Port	= typename MCU::Port;

	  public:
		enum class Configuration: uint8_t
		{
			TotemPole		= 0b000 << 3,
			BusKeeper		= 0b001 << 3,
			PullDown		= 0b010 << 3,
			PullUp			= 0b011 << 3,
			WiredOr			= 0b100 << 3,
			WiredAnd		= 0b101 << 3,
			WiredOrPull		= 0b110 << 3,
			WiredAndPull	= 0b111 << 3,
		};

		enum class SenseConfiguration: uint8_t
		{
			BothEdges		= 0b000,
			RisingEdge		= 0b001,
			FallingEdge		= 0b010,
			LowLevel		= 0b011,
			DigitalDisable	= 0b111, // Only ports A…F support disabling.
		};

	  public:
		using CommonBasicPin<MCU>::CommonBasicPin;
		using CommonBasicPin<MCU>::operator=;

		/**
		 * Return true if slew-rate limit is enabled.
		 */
		bool
		slew_rate_limit() const;

		/**
		 * Enable/disable slew-rate limit.
		 */
		void
		set_slew_rate_limit (bool enabled) const;

		/**
		 * Return true if inverted I/O is enabled.
		 */
		bool
		inverted_io() const;

		/**
		 * Enable/disable inverted I/O (high = low, low = high).
		 */
		void
		set_inverted_io (bool enabled) const;

		/**
		 * Return pin configuration.
		 */
		Configuration
		configuration() const;

		/**
		 * Set pin configuration.
		 */
		void
		set (Configuration) const;

		/**
		 * Return current input/sense configuration.
		 */
		SenseConfiguration
		sense_configuration() const;

		/**
		 * Set input/sense configuration.
		 */
		void
		set (SenseConfiguration) const;

		/**
		 * Enable/disable this pin to be source for interrupt 0/1.
		 */
		template<uint8_t Interrupt>
			void
			set_selected_for_interrupt (bool selected) const;

		// TODO int0 and int1 source pin selection mask (INT0MASK, INT1MASK)

	  private:
		/**
		 * Return related PORTx_PINnCTRL register from the port object.
		 */
		constexpr typename MCU::Register8
		pinctrl_register() const;
	};


template<class M>
	inline bool
	BasicPin<M>::slew_rate_limit() const
	{
		return get_bit<7> (pinctrl_register().read());
	}


template<class M>
	inline void
	BasicPin<M>::set_slew_rate_limit (bool enabled) const
	{
		set_bit_value<7> (pinctrl_register().ref(), enabled);
	}


template<class M>
	inline bool
	BasicPin<M>::inverted_io() const
	{
		return get_bit<6> (pinctrl_register().read());
	}


template<class M>
	inline void
	BasicPin<M>::set_inverted_io (bool enabled) const
	{
		set_bit_value<6> (pinctrl_register().ref(), enabled);
	}


template<class M>
	inline typename BasicPin<M>::Configuration
	BasicPin<M>::configuration() const
	{
		return static_cast<Configuration> (pinctrl_register().read() & 0b00'111'000);
	}


template<class M>
	inline void
	BasicPin<M>::set (Configuration new_configuration) const
	{
		pinctrl_register().write ((pinctrl_register().read() & 0b11'000'111) | static_cast<uint8_t> (new_configuration));
	}


template<class M>
	inline typename BasicPin<M>::SenseConfiguration
	BasicPin<M>::sense_configuration() const
	{
		return static_cast<Configuration> (pinctrl_register().read() & 0b00'000'111);
	}


template<class M>
	inline void
	BasicPin<M>::set (SenseConfiguration new_sense_configuration) const
	{
		pinctrl_register().write ((pinctrl_register().read() & 0b11'111'000) | static_cast<uint8_t> (new_sense_configuration));
	}


template<class M>
	template<uint8_t Interrupt>
		inline void
		BasicPin<M>::set_selected_for_interrupt (bool selected) const
		{
			static_assert (Interrupt == 0 || Interrupt == 1, "interrupt parameter must be 0 or 1");

			if (selected)
				this->port().template enable_pins_for_interrupt<Interrupt> (*this);
			else
				this->port().template disable_pins_for_interrupt<Interrupt> (*this);
		}


template<class M>
	constexpr typename BasicPin<M>::MCU::Register8
	BasicPin<M>::pinctrl_register() const
	{
		return CommonBasicPin<M>::_port.pinctrl_register (CommonBasicPin<M>::pin_number());
	}

} // namespace xmega_au
} // namespace avr
} // namespace mulabs

#endif

