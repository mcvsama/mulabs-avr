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

#ifndef MULABS_AVR__DEVICES__COMMON__COMMON_BASIC_PIN_H__INCLUDED
#define MULABS_AVR__DEVICES__COMMON__COMMON_BASIC_PIN_H__INCLUDED


namespace mulabs {
namespace avr {

template<class pMCU>
	class BasicPort;


template<class pMCU>
	class CommonBasicPin
	{
		using MCU	= pMCU;
		using Port	= typename MCU::Port;

	  public:
		constexpr
		CommonBasicPin (Port port, uint8_t pin_number);

		/**
		 * Set pin level to logic_value. True means high, false mans low.
		 */
		CommonBasicPin const&
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
		 * Wait in loop until pin is in awaited_state.
		 */
		void
		wait_for (bool awaited_state) const;

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
		 * Send signal N times.
		 */
		template<class Integer>
			void
			signal (Integer times) const;

	  protected:
		Port	_port;
		uint8_t	_pin_number;
	};


template<class M>
	constexpr
	CommonBasicPin<M>::CommonBasicPin (Port port, uint8_t pin_number):
		_port (port),
		_pin_number (pin_number)
	{
	}


template<class M>
	inline CommonBasicPin<M> const&
	CommonBasicPin<M>::operator= (bool logic_value) const
	{
		_port.pin_set (_pin_number, logic_value);
		return *this;
	}


template<class M>
	constexpr uint8_t
	CommonBasicPin<M>::pin_number() const
	{
		return _pin_number;
	}


template<class M>
	constexpr size_t
	CommonBasicPin<M>::absolute_pin_number() const
	{
		return _port.port_number() * 8 * sizeof (typename MCU::PortIntegerType) + pin_number();
	}


template<class M>
	constexpr typename CommonBasicPin<M>::Port
	CommonBasicPin<M>::port() const
	{
		return _port;
	}


template<class M>
	inline bool
	CommonBasicPin<M>::get() const
	{
		return _port.pin_get (_pin_number);
	}


template<class M>
	template<bool State>
		inline void
		CommonBasicPin<M>::wait_for() const
		{
			while (get() != State)
				continue;
		}


template<class M>
	inline void
	CommonBasicPin<M>::wait_for (bool awaited_state) const
	{
		while (get() != awaited_state)
			continue;
	}


template<class M>
	inline void
	CommonBasicPin<M>::configure_as_input() const
	{
		_port.pin_configure_as_input (_pin_number);
	}


template<class M>
	inline void
	CommonBasicPin<M>::configure_as_output() const
	{
		_port.pin_configure_as_output (_pin_number);
	}


template<class M>
	inline void
	CommonBasicPin<M>::set_high() const
	{
		_port.pin_set_high (_pin_number);
	}


template<class M>
	inline void
	CommonBasicPin<M>::set_low() const
	{
		_port.pin_set_low (_pin_number);
	}


template<class M>
	inline void
	CommonBasicPin<M>::toggle() const
	{
		_port.pin_toggle (_pin_number);
	}


template<class M>
	inline void
	CommonBasicPin<M>::signal() const
	{
		toggle();
		toggle();
	}


template<class M>
	template<class Integer>
		inline void
		CommonBasicPin<M>::signal (Integer times) const
		{
			for (Integer i = 0; i < times; ++i)
				signal();
		}

} // namespace avr
} // namespace mulabs

#endif

