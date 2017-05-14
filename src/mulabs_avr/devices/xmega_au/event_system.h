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

#ifndef MULABS_AVR__DEVICES__XMEGA_AU__EVENT_SYSTEM_H__INCLUDED
#define MULABS_AVR__DEVICES__XMEGA_AU__EVENT_SYSTEM_H__INCLUDED


namespace mulabs {
namespace avr {
namespace xmega_au {

class EventSystem
{
  public:
	enum class EventSource: uint8_t
	{
		None				= 0b0000'0000,
		RTCOverflow			= 0b0000'1000,
		RTCCompareMatch		= 0b0000'1001,
		ACAChannel0			= 0b0001'0000,
		ACAChannel1			= 0b0001'0001,
		ACAWindow			= 0b0001'0010,
		ACBChannel0			= 0b0001'0011,
		ACBChannel1			= 0b0001'0100,
		ACBWindow			= 0b0001'0101,
		ADCAChannel0		= 0b0010'0000,
		ADCAChannel1		= 0b0010'0001,
		ADCAChannel2		= 0b0010'0010,
		ADCAChannel3		= 0b0010'0011,
		ADCBChannel0		= 0b0010'0100,
		ADCBChannel1		= 0b0010'0101,
		ADCBChannel2		= 0b0010'0110,
		ADCBChannel3		= 0b0010'0111,
		PortAPin0			= 0b0101'0000,
		PortAPin1			= 0b0101'0001,
		PortAPin2			= 0b0101'0010,
		PortAPin3			= 0b0101'0011,
		PortAPin4			= 0b0101'0100,
		PortAPin5			= 0b0101'0101,
		PortAPin6			= 0b0101'0110,
		PortAPin7			= 0b0101'0111,
		PortBPin0			= 0b0101'1000,
		PortBPin1			= 0b0101'1001,
		PortBPin2			= 0b0101'1010,
		PortBPin3			= 0b0101'1011,
		PortBPin4			= 0b0101'1100,
		PortBPin5			= 0b0101'1101,
		PortBPin6			= 0b0101'1110,
		PortBPin7			= 0b0101'1111,
		PortCPin0			= 0b0110'0000,
		PortCPin1			= 0b0110'0001,
		PortCPin2			= 0b0110'0010,
		PortCPin3			= 0b0110'0011,
		PortCPin4			= 0b0110'0100,
		PortCPin5			= 0b0110'0101,
		PortCPin6			= 0b0110'0110,
		PortCPin7			= 0b0110'0111,
		PortDPin0			= 0b0110'1000,
		PortDPin1			= 0b0110'1001,
		PortDPin2			= 0b0110'1010,
		PortDPin3			= 0b0110'1011,
		PortDPin4			= 0b0110'1100,
		PortDPin5			= 0b0110'1101,
		PortDPin6			= 0b0110'1110,
		PortDPin7			= 0b0110'1111,
		PortEPin0			= 0b0111'0000,
		PortEPin1			= 0b0111'0001,
		PortEPin2			= 0b0111'0010,
		PortEPin3			= 0b0111'0011,
		PortEPin4			= 0b0111'0100,
		PortEPin5			= 0b0111'0101,
		PortEPin6			= 0b0111'0110,
		PortEPin7			= 0b0111'0111,
		PortFPin0			= 0b0111'1000,
		PortFPin1			= 0b0111'1001,
		PortFPin2			= 0b0111'1010,
		PortFPin3			= 0b0111'1011,
		PortFPin4			= 0b0111'1100,
		PortFPin5			= 0b0111'1101,
		PortFPin6			= 0b0111'1110,
		PortFPin7			= 0b0111'1111,
		Prescaler1			= 0b1000'0000,
		Prescaler2			= 0b1000'0001,
		Prescaler4			= 0b1000'0010,
		Prescaler8			= 0b1000'0011,
		Prescaler16			= 0b1000'0100,
		Prescaler32			= 0b1000'0101,
		Prescaler64			= 0b1000'0110,
		Prescaler128		= 0b1000'0111,
		Prescaler256		= 0b1000'1000,
		Prescaler512		= 0b1000'1001,
		Prescaler1024		= 0b1000'1010,
		Prescaler2048		= 0b1000'1011,
		Prescaler4096		= 0b1000'1100,
		Prescaler8192		= 0b1000'1101,
		Prescaler16384		= 0b1000'1110,
		Prescaler32768		= 0b1000'1111,
	};

	enum class TimerCounter: uint8_t
	{
		C0					= 0b1100'0000,
		C1					= 0b1100'1000,
		D0					= 0b1101'0000,
		D1					= 0b1101'1000,
		E0					= 0b1110'0000,
		E1					= 0b1110'1000,
		F0					= 0b1111'0000,
		F1					= 0b1111'1000,
	};

	enum class TimerEventType: uint8_t
	{
		OverOrUnderflow		= 0b000,
		Error				= 0b001,
		CaptureOrCompareA	= 0b100,
		CaptureOrCompareB	= 0b101,
		CaptureOrCompareC	= 0b110,
		CaptureOrCompareD	= 0b111,
	};

	// TODO Bus object that represents Bus.
	// TODO Data object that represents data for an event.

  public:
	/**
	 * Configure given EventSource as event source for given Bus.
	 * Bus is 0…7.
	 */
	template<uint8_t Bus>
		static void
		set_event_source_for_bus (EventSource);
	// TODO above function must also be overloaded with Pin argument.

	// TODO CHnCTRL (for quadrature-decoding)

	/**
	 * Generate event on given Bus.
	 */
	template<uint8_t Bus>
		static void
		strobe();

	/**
	 * Generate event on all event buses for which corresponding
	 * bits are set to 1.
	 */
	static void
	strobe (uint8_t buses);

	/**
	 * Same as strobe, but with additional data for generated event.
	 */
	static void
	strobe (uint8_t buses, uint8_t data);

	/**
	 * \param	channel
	 *			0, 1, 2 or 3.
	 */
	static constexpr EventSource
	event_source_for_adca_channel (uint8_t channel);

	/**
	 * \param	channel
	 *			0, 1, 2 or 3.
	 */
	static constexpr EventSource
	event_source_for_adcb_channel (uint8_t channel);

	/**
	 * \param	pin number
	 *			0…7
	 */
	static constexpr EventSource
	event_source_for_port_a_pin (uint8_t pin_number);

	/**
	 * \param	pin number
	 *			0…7
	 */
	static constexpr EventSource
	event_source_for_port_b_pin (uint8_t pin_number);

	/**
	 * \param	pin number
	 *			0…7
	 */
	static constexpr EventSource
	event_source_for_port_c_pin (uint8_t pin_number);

	/**
	 * \param	pin number
	 *			0…7
	 */
	static constexpr EventSource
	event_source_for_port_d_pin (uint8_t pin_number);

	/**
	 *\param	pin number
	 *			0…7
	 */
	static constexpr EventSource
	event_source_for_port_e_pin (uint8_t pin_number);

	/**
	 * \param	pin number
	 *			0…7
	 */
	static constexpr EventSource
	event_source_for_port_f_pin (uint8_t pin_number);

	/**
	 * \param	prescaler 2^power
	 *			0…15, gives CLK_PER divided by 2^n
	 */
	static constexpr EventSource
	event_source_for_prescaler (uint8_t power);

	static constexpr EventSource
	event_source_for_timer (TimerCounter timer, TimerEventType timer_event_type);
};


template<uint8_t Bus>
	inline void
	EventSystem::set_event_source_for_bus (EventSource source)
	{
		static_assert (Bus >= 0 && Bus <= 7, "Bus must be between 0 and 7");

		// CHnMUX registers are laid out continuously in memory:
		*(&EVSYS.CH0MUX + Bus) = static_cast<uint8_t> (source);
	}


constexpr EventSystem::EventSource
EventSystem::event_source_for_adca_channel (uint8_t channel)
{
	return static_cast<EventSource> (static_cast<uint8_t> (EventSource::ADCAChannel0) | channel);
}


constexpr EventSystem::EventSource
EventSystem::event_source_for_adcb_channel (uint8_t channel)
{
	return static_cast<EventSource> (static_cast<uint8_t> (EventSource::ADCBChannel0) | channel);
}


constexpr EventSystem::EventSource
EventSystem::event_source_for_port_a_pin (uint8_t pin_number)
{
	return static_cast<EventSource> (static_cast<uint8_t> (EventSource::PortAPin0) | pin_number);
}


constexpr EventSystem::EventSource
EventSystem::event_source_for_port_b_pin (uint8_t pin_number)
{
	return static_cast<EventSource> (static_cast<uint8_t> (EventSource::PortBPin0) | pin_number);
}


constexpr EventSystem::EventSource
EventSystem::event_source_for_port_c_pin (uint8_t pin_number)
{
	return static_cast<EventSource> (static_cast<uint8_t> (EventSource::PortCPin0) | pin_number);
}


constexpr EventSystem::EventSource
EventSystem::event_source_for_port_d_pin (uint8_t pin_number)
{
	return static_cast<EventSource> (static_cast<uint8_t> (EventSource::PortDPin0) | pin_number);
}


constexpr EventSystem::EventSource
EventSystem::event_source_for_port_e_pin (uint8_t pin_number)
{
	return static_cast<EventSource> (static_cast<uint8_t> (EventSource::PortEPin0) | pin_number);
}


constexpr EventSystem::EventSource
EventSystem::event_source_for_port_f_pin (uint8_t pin_number)
{
	return static_cast<EventSource> (static_cast<uint8_t> (EventSource::PortFPin0) | pin_number);
}


constexpr EventSystem::EventSource
EventSystem::event_source_for_prescaler (uint8_t power)
{
	return static_cast<EventSource> (static_cast<uint8_t> (EventSource::Prescaler1) | power);
}


constexpr EventSystem::EventSource
EventSystem::event_source_for_timer (TimerCounter timer, TimerEventType timer_event_type)
{
	return static_cast<EventSource> (static_cast<uint8_t> (timer) | static_cast<uint8_t> (timer_event_type));
}

} // namespace xmega_au
} // namespace avr
} // namespace mulabs

#endif

