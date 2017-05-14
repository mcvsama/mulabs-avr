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


namespace mulabs {
namespace avr {
namespace xmega_au {

template<class pMCU>
	class BasicPort;


template<class pMCU>
	class BasicPin
	{
		typedef pMCU			MCU;
		typedef BasicPort<MCU>	Port; // TODO const Port, const Pin, etc

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
		constexpr
		BasicPin (Port port, uint8_t pin_number);

		/**
		 * Set pin level to logic_value. True means high, false mans low.
		 */
		BasicPin const&
		operator= (bool logic_value) const;

		/**
		 * Return the pin number in the port.
		 */
		constexpr uint8_t
		pin_number() const;

		/**
		 * Return pin number for the PinSet object
		 * (taking into account port number as well).
		 */
		constexpr size_t
		absolute_pin_number() const;

		/**
		 * Return port.
		 */
		constexpr Port
		port() const;

		/**
		 * Read pin value.
		 */
		bool
		get() const;

		/**
		 * Wait in loop until pin is in AwaitedState.
		 */
		template<bool AwaitedState>
			void
			wait_for() const;

		/**
		 * Configure pin as input.
		 */
		void
		configure_as_input() const;

		/**
		 * Configure pin as output.
		 */
		void
		configure_as_output() const;

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
		 * Set pin level to high.
		 */
		void
		set_high() const;

		/**
		 * Set pin level to low.
		 */
		void
		set_low() const;

		/**
		 * Toggle pin level.
		 */
		void
		toggle() const;

		/**
		 * Toggle two times.
		 */
		void
		signal() const;

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

	  private:
		Port	_port;
		uint8_t	_pin_number;
	};

} // namespace xmega_au
} // namespace avr
} // namespace mulabs

#include "basic_pin_i.h"

#endif

