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

#ifndef MULABS_AVR__DEVICES__XMEGA_AU__BASIC_PORT_H__INCLUDED
#define MULABS_AVR__DEVICES__XMEGA_AU__BASIC_PORT_H__INCLUDED

#include <mulabs_avr/devices/xmega_au/interrupt_system.h>
#include <mulabs_avr/utility/bits.h>


namespace mulabs {
namespace avr {
namespace xmega_au {

template<class pMCU>
	class BasicPort
	{
	  public:
		using MCU		= pMCU;
		using Register8	= typename MCU::Register8;
		using Pin		= typename MCU::Pin;
		using PinBits	= typename MCU::PortIntegerType;

	  public:
		// Ctor:
		constexpr
		BasicPort (uint8_t port_number,
				   Register8 dir, Register8 dirset, Register8 dirclr, Register8 dirtgl,
				   Register8 out, Register8 outset, Register8 outclr, Register8 outtgl,
				   Register8 in,
				   Register8 intctrl, Register8 int0mask, Register8 int1mask, Register8 intflags,
				   Register8 pin0ctrl, Register8 pin1ctrl, Register8 pin2ctrl, Register8 pin3ctrl,
				   Register8 pin4ctrl, Register8 pin5ctrl, Register8 pin6ctrl, Register8 pin7ctrl);

		constexpr bool
		operator== (BasicPort const& other) const;

		constexpr bool
		operator!= (BasicPort const& other) const;

		/**
		 * Return Pin object by bit number.
		 */
		constexpr Pin
		pin (uint8_t pin_number) const;

		/**
		 * Return this port's number (A is 0, B is 1, etc).
		 */
		constexpr uint8_t
		port_number() const;

		/**
		 * Read byte from the port.
		 */
		PinBits
		get() const;

		/**
		 * Alias for get().
		 */
		operator PinBits() const;

		/**
		 * Write byte to all pins on port at once.
		 */
		void
		set (PinBits pin_bits) const;

		/**
		 * Alias for set (uint8_t).
		 */
		BasicPort const&
		operator= (PinBits pin_bits) const;

		/**
		 * Set selected pins to high level.
		 */
		void
		set_high (PinBits pin_bits) const;

		/**
		 * Set selected pins to low level.
		 */
		void
		set_low (PinBits pin_bits) const;

		/**
		 * Toggle selected pins (switch levels).
		 */
		void
		toggle (PinBits pin_bits) const;

		/**
		 * Configure pins denoted by set bits as inputs.
		 */
		void
		configure_as_inputs (PinBits pin_bits) const;

		/**
		 * Configure pins denoted by set bits as outputs.
		 */
		void
		configure_as_outputs (PinBits pin_bits) const;

		/**
		 * Configure individual pin as input.
		 */
		void
		pin_configure_as_input (uint8_t pin_number) const;

		/**
		 * Configure individual pin as output.
		 */
		void
		pin_configure_as_output (uint8_t pin_number) const;

		/**
		 * Get individual pin level.
		 */
		bool
		pin_get (uint8_t pin_number) const;

		/**
		 * Set individual pin to given logic value.
		 */
		void
		pin_set (uint8_t pin_number, bool logic_value) const;

		/**
		 * Set individual pin to high level.
		 */
		void
		pin_set_high (uint8_t pin_number) const;

		/**
		 * Set individual pin to low level.
		 */
		void
		pin_set_low (uint8_t pin_number) const;

		/**
		 * Toggle individual pin level.
		 */
		void
		pin_toggle (uint8_t pin_number) const;

		/**
		 * Configure interrupt 0/1.
		 */
		template<uint8_t Interrupt>
			void
			set_interrupt (InterruptSystem::Level) const;

		/**
		 * Select source pins for interrupt 0/1.
		 * The sense configuration is set with Pin::set_sense_configuration().
		 */
		template<uint8_t Interrupt, class ...Pins>
			void
			select_pins_for_interrupt (Pins const ...pins) const;

		/**
		 * Enable given source pins for interrupts 0/1.
		 */
		template<uint8_t Interrupt, class ...Pins>
			void
			enable_pins_for_interrupt (Pins const ...pins) const;

		/**
		 * Disable given source pins for interrupts 0/1.
		 */
		template<uint8_t Interrupt, class ...Pins>
			void
			disable_pins_for_interrupt (Pins const ...pins) const;

		// TODO pins remapping REMAP

		constexpr typename MCU::Register8
		pinctrl_register (uint8_t pin_number) const;

	  private:
		/**
		 * Recursive function that collects Pins belonging to this port only.
		 */
		template<class Pin, class ...Pins>
			// TODO zamiast MCU::PortIntegerType zrób PortPinSet czy PinSet8
			// TODO PinSet i PortPinSet powinny zwracać obiekty Pin na żądanie
			constexpr typename MCU::PortIntegerType
			make_pin_set (Pin const pin, Pins const ...pins) const
			{
				// TODO: if constexpr (pin.port() != *this)
				// TODO:	static_assert (false, "wrong Pin passed to the port");

				return bitnum<typename MCU::PortIntegerType> (pin.pin_number()) | make_pin_set (pins...);
			}

		/**
		 * Recursive stop-condition for make_pin_set().
		 */
		template<class Pin>
			constexpr typename MCU::PortIntegerType
			make_pin_set (Pin const pin) const
			{
				// TODO: if constexpr (pin.port() != *this)
				// TODO:	static_assert (false, "wrong Pin passed to the port");

				return bitnum<typename MCU::PortIntegerType> (pin.pin_number());
			}

	  private:
		uint8_t		_port_number;
		Register8	_dir, _dirset, _dirclr, _dirtgl;
		Register8	_out, _outset, _outclr, _outtgl;
		Register8	_in;
		Register8	_intctrl, _int0mask, _int1mask, _intflags;
		Register8	_pin0ctrl, _pin1ctrl, _pin2ctrl, _pin3ctrl;
		Register8	_pin4ctrl, _pin5ctrl, _pin6ctrl, _pin7ctrl;
	};


template<class M>
	constexpr
	BasicPort<M>::BasicPort (uint8_t port_number,
							 Register8 dir, Register8 dirset, Register8 dirclr, Register8 dirtgl,
							 Register8 out, Register8 outset, Register8 outclr, Register8 outtgl,
							 Register8 in,
							 Register8 intctrl, Register8 int0mask, Register8 int1mask, Register8 intflags,
							 Register8 pin0ctrl, Register8 pin1ctrl, Register8 pin2ctrl, Register8 pin3ctrl,
							 Register8 pin4ctrl, Register8 pin5ctrl, Register8 pin6ctrl, Register8 pin7ctrl):
		_port_number (port_number),
		_dir (dir), _dirset (dirset), _dirclr (dirclr), _dirtgl (dirtgl),
		_out (out), _outset (outset), _outclr (outclr), _outtgl (outtgl),
		_in (in),
		_intctrl (intctrl), _int0mask (int0mask), _int1mask (int1mask), _intflags (intflags),
		_pin0ctrl (pin0ctrl), _pin1ctrl (pin1ctrl), _pin2ctrl (pin2ctrl), _pin3ctrl (pin3ctrl),
		_pin4ctrl (pin4ctrl), _pin5ctrl (pin5ctrl), _pin6ctrl (pin6ctrl), _pin7ctrl (pin7ctrl)
	{ }


template<class M>
	constexpr bool
	BasicPort<M>::operator== (BasicPort const& other) const
	{
#define BASIC_PORT_EQ(n) (&_##n == &other._##n)
		return _port_number == other._port_number
			&& BASIC_PORT_EQ (dir) && BASIC_PORT_EQ (dirset) && BASIC_PORT_EQ (dirclr) && BASIC_PORT_EQ (dirtgl)
			&& BASIC_PORT_EQ (out) && BASIC_PORT_EQ (outset) && BASIC_PORT_EQ (outclr) && BASIC_PORT_EQ (outtgl)
			&& BASIC_PORT_EQ (in)
			&& BASIC_PORT_EQ (intctrl) && BASIC_PORT_EQ (int0mask) && BASIC_PORT_EQ (int1mask) && BASIC_PORT_EQ (intflags)
			&& BASIC_PORT_EQ (pin0ctrl) && BASIC_PORT_EQ (pin1ctrl) && BASIC_PORT_EQ (pin2ctrl) && BASIC_PORT_EQ (pin3ctrl)
			&& BASIC_PORT_EQ (pin4ctrl) && BASIC_PORT_EQ (pin5ctrl) && BASIC_PORT_EQ (pin6ctrl) && BASIC_PORT_EQ (pin7ctrl);
#undef BASIC_PORT_EQ
	}


template<class M>
	constexpr bool
	BasicPort<M>::operator!= (BasicPort const& other) const
	{
		return !(*this == other);
	}


template<class M>
	constexpr typename BasicPort<M>::Pin
	BasicPort<M>::pin (uint8_t pin_number) const
	{
		return Pin (*this, pin_number);
	}


template<class M>
	constexpr uint8_t
	BasicPort<M>::port_number() const
	{
		return _port_number;
	}


template<class M>
	inline typename BasicPort<M>::PinBits
	BasicPort<M>::get() const
	{
		return _in;
	}


template<class M>
	inline
	BasicPort<M>::operator PinBits() const
	{
		return get();
	}


template<class M>
	inline void
	BasicPort<M>::set (PinBits pin_bits) const
	{
		_out = pin_bits;
	}


template<class M>
	inline BasicPort<M> const&
	BasicPort<M>::operator= (PinBits pin_bits) const
	{
		set (pin_bits);
		return *this;
	}


template<class M>
	inline void
	BasicPort<M>::set_high (PinBits pin_bits) const
	{
		_outset = pin_bits;
	}


template<class M>
	inline void
	BasicPort<M>::set_low (PinBits pin_bits) const
	{
		_outclr = pin_bits;
	}


template<class M>
	inline void
	BasicPort<M>::toggle (PinBits pin_bits) const
	{
		_outtgl = pin_bits;
	}


template<class M>
	inline void
	BasicPort<M>::configure_as_inputs (PinBits pin_bits) const
	{
		_dirclr = pin_bits;
	}


template<class M>
	inline void
	BasicPort<M>::configure_as_outputs (PinBits pin_bits) const
	{
		_dirset = pin_bits;
	}


template<class M>
	inline void
	BasicPort<M>::pin_configure_as_input (uint8_t pin_number) const
	{
		_dirclr = bitnum<uint8_t> (pin_number);
	}


template<class M>
	inline void
	BasicPort<M>::pin_configure_as_output (uint8_t pin_number) const
	{
		_dirset = bitnum<uint8_t> (pin_number);
	}


template<class M>
	inline bool
	BasicPort<M>::pin_get (uint8_t pin_number) const
	{
		return _in.read() & bitnum<uint8_t> (pin_number);
	}


template<class M>
	inline void
	BasicPort<M>::pin_set (uint8_t pin_number, bool logic_value) const
	{
		if (logic_value)
			pin_set_high (pin_number);
		else
			pin_set_low (pin_number);
	}


template<class M>
	inline void
	BasicPort<M>::pin_set_high (uint8_t pin_number) const
	{
		_outset = bitnum<uint8_t> (pin_number);
	}


template<class M>
	inline void
	BasicPort<M>::pin_set_low (uint8_t pin_number) const
	{
		_outclr = bitnum<uint8_t> (pin_number);
	}


template<class M>
	inline void
	BasicPort<M>::pin_toggle (uint8_t pin_number) const
	{
		_outtgl = bitnum<uint8_t> (pin_number);
	}


template<class M>
	template<uint8_t Interrupt>
		inline void
		BasicPort<M>::set_interrupt (InterruptSystem::Level level) const
		{
			static_assert (Interrupt == 0 || Interrupt == 1, "interrupt parameter must be 0 or 1");

			if constexpr (Interrupt == 0)
				_intctrl = (_intctrl & ~0b0011) | static_cast<uint8_t> (level);
			else if constexpr (Interrupt == 1)
				_intctrl = (_intctrl & ~0b1100) | (static_cast<uint8_t> (level) << 2);
		}


template<class M>
	template<uint8_t Interrupt, class ...Pins>
		inline void
		BasicPort<M>::select_pins_for_interrupt (Pins const ...pins) const
		{
			static_assert (Interrupt == 0 || Interrupt == 1, "interrupt parameter must be 0 or 1");

			if constexpr (Interrupt == 0)
				_int0mask = make_pin_set (pins...);
			else if constexpr (Interrupt == 1)
				_int1mask = make_pin_set (pins...);
		}


template<class M>
	template<uint8_t Interrupt, class ...Pins>
		inline void
		BasicPort<M>::enable_pins_for_interrupt (Pins const ...pins) const
		{
			static_assert (Interrupt == 0 || Interrupt == 1, "interrupt parameter must be 0 or 1");

			if constexpr (Interrupt == 0)
				_int0mask = _int0mask.read() | make_pin_set (pins...);
			else if constexpr (Interrupt == 1)
				_int1mask = _int1mask.read() | make_pin_set (pins...);
		}


template<class M>
	template<uint8_t Interrupt, class ...Pins>
		inline void
		BasicPort<M>::disable_pins_for_interrupt (Pins const ...pins) const
		{
			static_assert (Interrupt == 0 || Interrupt == 1, "interrupt parameter must be 0 or 1");

			if constexpr (Interrupt == 0)
				_int0mask = _int0mask & ~make_pin_set (pins...);
			else if constexpr (Interrupt == 1)
				_int1mask = _int1mask & ~make_pin_set (pins...);
		}


template<class M>
	constexpr typename BasicPort<M>::MCU::Register8
	BasicPort<M>::pinctrl_register (uint8_t pin_number) const
	{
		switch (pin_number)
		{
			case 0:
				return _pin0ctrl;

			case 1:
				return _pin1ctrl;

			case 2:
				return _pin2ctrl;

			case 3:
				return _pin3ctrl;

			case 4:
				return _pin4ctrl;

			case 5:
				return _pin5ctrl;

			case 6:
				return _pin6ctrl;

			case 7:
			default:
				return _pin7ctrl;
		}
	}

} // namespace xmega_au
} // namespace avr
} // namespace mulabs

#endif

