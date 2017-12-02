/* vim:ts=4
 *
 * Copyleft 2012…2017  Michał Gawron
 * Marduk Unix Labs, http://mulabs.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Visit http://www.gnu.org/licenses/gpl-3.0.html for more information on licensing.
 */

#ifndef MULABS_AVR__SUPPORT__PROTOCOLS__USB_STATE_H__INCLUDED
#define MULABS_AVR__SUPPORT__PROTOCOLS__USB_STATE_H__INCLUDED

namespace mulabs {
namespace avr {
namespace usb {

template<class cUSBSIE, cUSBSIE const& vUSBSIE, usb::Device const& vDevice, size_t vMaxPacketSize>
	class State
	{
	  public:
		using USBSIE			= cUSBSIE;
		using EndpointsTable	= typename USBSIE::template EndpointsTable<vDevice.maximum_endpoint_address() + 1>;
		using OutputEndpoint	= typename USBSIE::OutputEndpoint;
		using InputEndpoint		= typename USBSIE::InputEndpoint;

	  private:
		static constexpr size_t				kMaxPacketSize	= vMaxPacketSize;

	  public:
		State (typename USBSIE::Speed);

		void
		setup();

		void
		reset();

		template<class Debug>
			void
			handle_interrupt (Debug&& debug);

	  private:
		constexpr size_t
		required_buffers_size() const
		{
			return 400;
			// TODO traverse vDevice and figure out max required buffers size
		}

	  private:
		static constexpr USBSIE const&		_usb_sie					{ vUSBSIE };
		static constexpr usb::Device const&	_device						{ vDevice };

	  private:
		usb::DeviceStrings<vDevice>			_usb_device_strings			{ };
		usb::ConfigurationValue				_current_configuration		{ 0 };
		typename USBSIE::Speed				_usb_speed;
		EndpointsTable						_endpoints					{ _device };
		// TODO Array<uint8_t, required_buffers_size()>	_long_buffer;
		OutputEndpoint						_usb_control_out			{ _endpoints.nth_output (0) };
		InputEndpoint						_usb_control_in				{ _endpoints.nth_input (0) };
		Array<uint8_t, kMaxPacketSize>		_usb_control_buffer_in;
		Array<uint8_t, kMaxPacketSize>		_usb_control_buffer_out;
		usb::SetupConductor<USBSIE, _usb_sie, usb::DeviceStrings<vDevice>, decltype (_usb_control_buffer_in), decltype (_usb_control_buffer_out)>
											_usb_setup_conductor		{ vDevice, _usb_device_strings, _usb_control_in, _usb_control_buffer_in, _usb_control_out, _usb_control_buffer_out };
	};


template<class cU, cU const& vU, usb::Device const& vD, size_t vM>
	inline
	State<cU, vU, vD, vM>::State (typename USBSIE::Speed speed):
		_usb_speed (speed)
	{ }


template<class cU, cU const& vU, usb::Device const& vD, size_t vM>
	inline void
	State<cU, vU, vD, vM>::setup()
	{
		_usb_control_buffer_out.fill (0);
		_usb_control_buffer_in.fill (0);

		_usb_control_out.initialize();
		_usb_control_out.set (USBSIE::Endpoint::Type::Control);
		_usb_control_out.set_buffer (_usb_control_buffer_out.data(), USBSIE::Endpoint::ControlBulkBufferSize::_64);
		_usb_control_out.set_ready();

		_usb_control_in.initialize();
		_usb_control_in.set (USBSIE::Endpoint::Type::Control);
		_usb_control_in.set_buffer (_usb_control_buffer_in.data(), USBSIE::Endpoint::ControlBulkBufferSize::_64);
		_usb_control_in.set_nack_all (USBSIE::Endpoint::Buffer::_0, true);
		_usb_control_in.set_azlp_enabled (false);

		_usb_sie.calibrate();
		_usb_sie.set (_usb_speed);
		_usb_sie.set_endpoints_table (_endpoints);
		// _usb_sie.set_store_framenum_enabled (true); // FIXME doesn't work?
		_usb_sie.enable (USBSIE::Interrupt::BusEvent, // XXX
						 USBSIE::Interrupt::Stall,
						 USBSIE::Interrupt::TransactionComplete,
						 USBSIE::Interrupt::SetupTransactionComplete);
		_usb_sie.set_enabled (true);
		_usb_sie.set_attached (true);

		reset();
	}


template<class cU, cU const& vU, usb::Device const& vD, size_t vM>
	inline void
	State<cU, vU, vD, vM>::reset()
	{
		_usb_sie.set_address (0);
		_usb_setup_conductor.reset();
	}


template<class cU, cU const& vU, usb::Device const& vD, size_t vM>
	template<class Debug>
		inline void
		State<cU, vU, vD, vM>::handle_interrupt (Debug&& debug)
		{
			_usb_setup_conductor.handle_interrupt (debug);
		}

} // namespace usb
} // namespace avr
} // namespace mulabs

#endif

