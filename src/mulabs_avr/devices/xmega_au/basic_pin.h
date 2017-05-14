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
	class BasicPort;


template<class pMCU>
	class BasicPin: public CommonBasicPin<pMCU>
	{
		using MCU	= pMCU;
		using Port	= typename MCU::Port;

		enum class Configuration: uint8_t
		{
			TotemPole		= 0b00'000'000,
			BusKeeper		= 0b00'001'000,
			PullDown		= 0b00'010'000,
			PullUp			= 0b00'011'000,
			WiredOr			= 0b00'100'000,
			WiredAnd		= 0b00'101'000,
			WiredOrPull		= 0b00'110'000,
			WiredAndPull	= 0b00'111'000,
		};

		enum class SenseConfiguration: uint8_t
		{
			BothEdges		= 0b00000'000,
			RisingEdge		= 0b00000'001,
			FallingEdge		= 0b00000'010,
			LowLevel		= 0b00000'011,
			InputDisable	= 0b00000'111,
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
		set_configuration (Configuration) const;

		/**
		 * Return current input/sense configuration.
		 */
		SenseConfiguration
		sense_configuration() const;

		/**
		 * Set input/sense configuration.
		 */
		void
		set_sense_configuration (SenseConfiguration) const;

		/**
		 * Enable this pin to be source for interrupt 0.
		 */
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
		return get_bit (pinctrl_register().read(), 7);
	}


template<class M>
	inline void
	BasicPin<M>::set_slew_rate_limit (bool enabled) const
	{
		set_bit_value (pinctrl_register().ref(), 7, enabled);
	}


template<class M>
	inline bool
	BasicPin<M>::inverted_io() const
	{
		return get_bit (pinctrl_register().read(), 6);
	}


template<class M>
	inline void
	BasicPin<M>::set_inverted_io (bool enabled) const
	{
		set_bit_value (pinctrl_register().ref(), 6, enabled);
	}


template<class M>
	inline typename BasicPin<M>::Configuration
	BasicPin<M>::configuration() const
	{
		return static_cast<Configuration> (pinctrl_register().read() & 0b00'111'000);
	}


template<class M>
	inline void
	BasicPin<M>::set_configuration (Configuration new_configuration) const
	{
		pinctrl_register() = (pinctrl_register().read() & 0b11'000'111) | new_configuration;
	}


template<class M>
	inline typename BasicPin<M>::SenseConfiguration
	BasicPin<M>::sense_configuration() const
	{
		return static_cast<Configuration> (pinctrl_register().read() & 0b00'000'111);
	}


template<class M>
	inline void
	BasicPin<M>::set_sense_configuration (SenseConfiguration new_sense_configuration) const
	{
		pinctrl_register() = (pinctrl_register().read() & 0b11'111'000) | new_sense_configuration;
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

