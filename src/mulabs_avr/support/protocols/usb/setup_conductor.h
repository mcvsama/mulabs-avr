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

#ifndef MULABS_AVR__SUPPORT__PROTOCOLS__SETUP_CONDUCTOR_H__INCLUDED
#define MULABS_AVR__SUPPORT__PROTOCOLS__SETUP_CONDUCTOR_H__INCLUDED

// Mulabs:
#include <mulabs_avr/support/protocols/usb/buffered_transfer.h>
#include <mulabs_avr/support/protocols/usb/control_transfer.h>
#include <mulabs_avr/support/protocols/usb/device_definition.h>
#include <mulabs_avr/support/protocols/usb/descriptors.h>
#include <mulabs_avr/utility/span.h>


namespace mulabs {
namespace avr {
namespace usb {

/**
 * This class handles the setup packets on the USB bus and replies according to the provided USB device definition.
 */
template<class pUSBSIE, pUSBSIE const& vUSBSIE, class pDeviceStrings, class pInputBuffer, class pOutputBuffer>
	class SetupConductor
	{
	  public:
		using USBSIE				= pUSBSIE;
		using DeviceStringsType		= pDeviceStrings;
		using InputEndpoint			= typename USBSIE::InputEndpoint;
		using InputBuffer			= pInputBuffer;
		using OutputEndpoint		= typename USBSIE::OutputEndpoint;
		using OutputBuffer			= pOutputBuffer;
		using ControlTransferType	= ControlTransfer<InputEndpoint, InputBuffer, OutputEndpoint, OutputBuffer, 64>; // TODO user-configurable buffer-size

		static constexpr USBSIE const&	usb_sie	= vUSBSIE;

	  public:
		// Ctor
		explicit constexpr
		SetupConductor (Device const& device, DeviceStringsType const&, InputEndpoint&, InputBuffer&, OutputEndpoint&, OutputBuffer&);

		/**
		 * Check endpoints and handle setup packets as needed.
		 */
		template<class Debug>
			void
			handle_interrupt (Debug&&);

		/**
		 * Reset to initial state.
		 */
		void
		reset() noexcept;

	  private:
		/**
		 * Handle a setup packet, return true if processed OK.
		 */
		template<class Debug>
			bool
			handle_setup_packet (SetupPacket const&, ControlTransferType&, Debug&& debug);

	  private:
		Device const&				_device;
		DeviceStringsType const&	_device_strings;
		InputEndpoint&				_input_endpoint;
		InputBuffer&				_input_buffer;
		OutputEndpoint&				_output_endpoint;
		OutputBuffer&				_output_buffer;
		uint8_t						_address_to_set		{ 0 };
		BufferedTransfer<ControlTransferType>
									_transfer			{ _input_endpoint, _input_buffer, _output_endpoint, _output_buffer };
	};


template<class U, U const& vU, class DS, class IB, class OB>
	constexpr
	SetupConductor<U, vU, DS, IB, OB>::SetupConductor (Device const& device, DeviceStringsType const& device_strings,
													   InputEndpoint& input_endpoint, InputBuffer& input_buffer,
													   OutputEndpoint& output_endpoint, OutputBuffer& output_buffer):
		_device (device),
		_device_strings (device_strings),
		_input_endpoint (input_endpoint),
		_input_buffer (input_buffer),
		_output_endpoint (output_endpoint),
		_output_buffer (output_buffer)
	{ }


template<class U, U const& vU, class DS, class IB, class OB>
	template<class Debug>
		inline void
		SetupConductor<U, vU, DS, IB, OB>::handle_interrupt (Debug&& debug)
		{
			if (_input_endpoint.is_stalled() || _output_endpoint.is_stalled())
			{
				debug ("!!! stalled\n");
				// TODO
			}

			if (_input_endpoint.is_crc_error() || _output_endpoint.is_crc_error())
			{
				debug ("!!! CRC error\n");
				// TODO stall the endpoint and report to the user
			}

			if (usb_sie.triggered (std::remove_reference_t<decltype (usb_sie)>::InterruptFlag::Reset))
				reset();

			auto on_setup = [&] (SetupPacket const& setup, [[maybe_unused]] Span<uint8_t> output_data) {
				// XXX remove debug
				debug ("  Got setup packet:\n");
				debug ("    transfer_direction  %u  %s\n", setup.transfer_direction,
					(setup.transfer_direction == SetupPacket::TransferDirection::HostToDevice)
						? "-> host to device"
						: "<- device to host");
				debug ("    type                %u\n", setup.type);
				debug ("    recipient           %u\n", setup.recipient);
				debug ("    length              %u\n", setup.length);

				return handle_setup_packet (setup, _transfer, debug);
			};

			auto on_finished = [&] {
				if (_address_to_set != 0)
					usb_sie.set_address (std::exchange (_address_to_set, 0));
				debug ("  Finished\n");
			};

			auto on_error = [&] (typename ControlTransferType::Error error) {
				debug ("ControlTransfer::Error %u\n", static_cast<size_t> (error));
			};

			_transfer.handle_interrupt (on_setup, on_finished, on_error);
		}


template<class U, U const& vU, class DS, class IB, class OB>
	inline void
	SetupConductor<U, vU, DS, IB, OB>::reset() noexcept
	{
		_address_to_set = 0;
		_transfer.reset();
	}


template<class U, U const& vU, class DS, class IB, class OB>
	template<class Debug>
		inline bool
		SetupConductor<U, vU, DS, IB, OB>::handle_setup_packet (SetupPacket const& setup, ControlTransferType& transfer, Debug&& debug)
		{
			// By default, don't return anything:
			transfer.set_transfer_size (0);

			switch (setup.type)
			{
				case SetupPacket::Type::Standard:
					switch (setup.recipient)
					{
						case SetupPacket::Recipient::Device:
						{
							using Type = SetupPacket::DeviceRequest::Type;

							debug ("SETUP: Recipient::Device\n");

							switch (setup.request.device.type)
							{
								case Type::GetStatus:
								{
									debug ("  DeviceRequest::GetStatus\n");
									// Bit 0 is SelfPowered (1 if true)
									// Bit 1 is RemoteWakeup (1 if true)
									uint8_t lsb = 0;

									transfer.buffer()[0] = lsb;
									transfer.buffer()[1] = 0;
									transfer.set_transfer_size (2);
									break;
								}

								case Type::GetDescriptor:
									switch (setup.request.device.get_descriptor.type)
									{
										case DescriptorType::Device:
										{
											auto const desc = make_device_descriptor (_device, _device_strings);
											transfer.buffer().template as<DeviceDescriptor>() = desc;
											transfer.set_transfer_size (std::min (setup.length, sizeof (desc)));
											break;
										}

										case DescriptorType::Configuration:
										{
											// A request for the configuration descriptor should return the device descriptor and all interface and endpoint
											// descriptors in one request.
											size_t size = make_full_configuration_descriptor (transfer.buffer(), _device, setup.request.device.get_descriptor.index, _device_strings);
											transfer.set_transfer_size (std::min (setup.length, size));
											break;
										}

										case DescriptorType::String:
										{
											auto const index = setup.request.device.get_descriptor.index;

											if (index == 0)
											{
												auto const actual_size = make_string_descriptor_0 (transfer.buffer(), LanguageID::English);
												transfer.set_transfer_size (actual_size);
											}
											else
											{
												auto const& string = _device_strings.string_for_index (setup.request.device.get_descriptor.index);
												auto const actual_size = make_string_descriptor (transfer.buffer(), string);
												transfer.set_transfer_size (actual_size);
											}
											break;
										}

										case DescriptorType::Interface:
											// Ignore. USB protocol doesn't allow asking for individual interface descriptors.
											break;

										case DescriptorType::Endpoint:
											// Ignore. USB protocol doesn't allow asking for individual endpoint descriptors.
											break;

										case DescriptorType::BOS:
											debug ("    DescriptorType::BOS\n");
											break;
									}
									break;

								case Type::GetConfiguration:
									debug ("  DeviceRequest::GetConfiguration\n");
									// Return to the host a byte indicating whether device is configured or not
									// 0 means not configured, other value indicates current configuration index.
									transfer.buffer()[0] = 0;
									transfer.set_transfer_size (1);
									break;

								case Type::ClearFeature:
									debug ("  DeviceRequest::ClearFeature\n");
									break;

								case Type::SetFeature:
									debug ("  DeviceRequest::SetFeature\n");
									break;

								case Type::SetAddress:
									// Address should be set after completion of the status stage, so just remember it for now:
									_address_to_set = setup.request.device.set_address.address;
									break;

								case Type::SetDescriptor:
									debug ("  DeviceRequest::SetDescriptor\n");
									break;

								case Type::SetConfiguration:
									debug ("  DeviceRequest::SetConfiguration\n");
									// TODO set current device configuration
									// TODO call-out user method (set current config index in usb::DeviceState)
									break;
							}
							break;
						}

						case SetupPacket::Recipient::Interface:
						{
							using Type = SetupPacket::InterfaceRequest::Type;

							debug ("SETUP: Recipient::Interface\n");

							switch (setup.request.interface.type)
							{
								case Type::GetStatus:
									debug ("  InterfaceRequest::GetStatus\n");
									break;

								case Type::GetInterface:
									debug ("  InterfaceRequest::GetInterface\n");
									break;

								case Type::ClearFeature:
									debug ("  InterfaceRequest::ClearFeature\n");
									break;

								case Type::SetFeature:
									debug ("  InterfaceRequest::SetFeature\n");
									break;

								case Type::SetInterface:
									debug ("  InterfaceRequest::SetInterface\n");
									break;
							}
							break;
						}

						case SetupPacket::Recipient::Endpoint:
							using Type = SetupPacket::EndpointRequest::Type;

							debug ("SETUP: Recipient::Endpoint\n");

							switch (setup.request.endpoint.type)
							{
								case Type::GetStatus:
									debug ("  EndpointRequest::GetStatus\n");
									break;

								case Type::SynchFrame:
									debug ("  EndpointRequest::SynchFrame\n");
									break;

								case Type::ClearFeature:
									debug ("  EndpointRequest::ClearFeature\n");
									break;

								case Type::SetFeature:
									debug ("  EndpointRequest::SetFeature\n");
									break;
							}
							break;

						default:
							break;
					}
					break;

				default:
					break;
			}

			return true;
		}

} // namespace usb
} // namespace avr
} // namespace mulabs

#endif

