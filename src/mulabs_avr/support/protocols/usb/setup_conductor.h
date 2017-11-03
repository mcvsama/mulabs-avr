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


namespace mulabs {
namespace avr {
namespace usb {

// TODO
// The DATA stage is optional. If present, it always starts with a transaction containing a DATA1 packet. Then, the transaction type alternates between DATA0 and DATA1 until all required data have been transferred.
// The STATUS stage is a transaction containing a zero-length DATA1 packet. If the DATA stage was IN, then the STATUS stage is OUT, and vice-versa.
//
// Above means that TOGGLE must always be set to 1 before sending data.
// TODO add USB::InputEndpoint::set_next_data_packet_index (1);


/**
 * This class handles the setup packets on the USB bus and replies according to the provided USB device definition.
 */
template<class pUSBModule, class pInputEndpoint, class pInputBuffer, class pOutputEndpoint, class pOutputBuffer>
	class SetupConductor
	{
	  public:
		using USBModule				= pUSBModule;
		using InputEndpoint			= pInputEndpoint;
		using InputBuffer			= pInputBuffer;
		using OutputEndpoint		= pOutputEndpoint;
		using OutputBuffer			= pOutputBuffer;

	  public:
		// Ctor
		explicit constexpr
		SetupConductor (Device const& device, USBModule&, InputEndpoint&, InputBuffer&, OutputEndpoint&, OutputBuffer&);

		/**
		 * Check endpoints and handle setup packets as needed.
		 */
		template<class DebugFunc>
			void
			handle_interrupt (DebugFunc);

	  private:
		Device const&		_device;
		USBModule&			_usb;
		InputEndpoint&		_input_endpoint;
		InputBuffer&		_input_buffer;
		OutputEndpoint&		_output_endpoint;
		OutputBuffer&		_output_buffer;
		uint8_t				_address_to_set		{ 0x00 };
		BufferedTransfer<ControlTransfer<InputEndpoint, InputBuffer, OutputEndpoint, OutputBuffer, 64>> // TODO user-configurable buffer-size
							_transfer			{ _input_endpoint, _input_buffer, _output_endpoint, _output_buffer };
	};


template<class U, class IE, class IB, class OE, class OB>
	constexpr
	SetupConductor<U, IE, IB, OE, OB>::SetupConductor (Device const& device, USBModule& usb,
													   InputEndpoint& input_endpoint, InputBuffer& input_buffer,
													   OutputEndpoint& output_endpoint, OutputBuffer& output_buffer):
		_device (device),
		_usb (usb),
		_input_endpoint (input_endpoint),
		_input_buffer (input_buffer),
		_output_endpoint (output_endpoint),
		_output_buffer (output_buffer)
	{ }


template<class U, class IE, class IB, class OE, class OB>
	template<class DebugFunc>
		inline void
		SetupConductor<U, IE, IB, OE, OB>::handle_interrupt (DebugFunc debug)
		{
			bool respond = false;
			size_t respond_bytes = 0;

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

			if (_input_endpoint.transaction_complete())
			{
				if (_address_to_set != 0)
				{
					debug ("SETTING ADDRESS %d\n", (int)_address_to_set);
					_usb.set_address (_address_to_set);
					_address_to_set = 0;
				}
			}

			if (_output_endpoint.transaction_complete() && _output_endpoint.transaction_size() == 0)
			{
				// Host sends ZLP in the status transaction to confirm successful reception.
				debug ("ZLP from HOST\n");
			}

#if 0
			if (_output_endpoint.transaction_size() > 0)
			{
				debug ("BUFFERING\n");
				// Add to buffer:
				// TODO prevent buffer overflow
				for (size_t i = 0; i < _output_endpoint.transaction_size(); ++i, ++_buffer_size)
					_buffer[_buffer_size] = output_buffer[i];
			}
#endif

			if (_output_endpoint.setup_transaction_complete())
			{
				// Handle what's in buffer.
				auto const& setup = _output_buffer.template as<usb::SetupPacket>();

				debug ("  Got setup packet:\n");
				debug ("    transfer_direction  %u\n", setup.transfer_direction);
				debug ("    type                %u\n", setup.type);
				debug ("    recipient           %u\n", setup.recipient);
				debug ("    length              %u\n", setup.length);

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
										debug ("  DeviceRequest::GetStatus\n");
										// Bit 0 is SelfPowered (1 if true)
										// Bit 1 is RemoteWakeup (1 if true)
										// TODO get these bits from the _device:
										_input_buffer.fill (0);
										_input_buffer[0] = 0b00000001;
										_input_buffer[1] = 0;
										respond = true;
										respond_bytes = 2;
										break;

									case Type::GetDescriptor:
										debug ("  DeviceRequest::GetDescriptor\n");

										switch (setup.request.device.get_descriptor.type)
										{
											case DescriptorType::Device:
											{
												debug ("    DescriptorType::Device\n");
												_input_buffer.fill (0);
												_input_buffer.template as<DeviceDescriptor>() = make_device_descriptor (_device);
												respond = true;
												respond_bytes = sizeof (DeviceDescriptor);
												break;
											}

											case DescriptorType::Configuration:
												debug ("    DescriptorType::Configuration\n");
												// Return the Device descriptor and all the rest of the configuration
												// tree in one request.
												break;

											case DescriptorType::String:
												debug ("    DescriptorType::String\n");
												break;

											case DescriptorType::Interface:
												debug ("    DescriptorType::Interface\n");
												break;

											case DescriptorType::Endpoint:
												debug ("    DescriptorType::Endpoint\n");
												break;

											case DescriptorType::BOS:
												debug ("    DescriptorType::BOS\n");
												break;
										}
										break;

									case Type::GetConfiguration:
										debug ("  DeviceRequest::GetConfiguration\n");
										// TODO return a byte indicating whether device is configured or not
										// 0 means not confiugred, 1 or more indicates current configuration index.
										break;

									case Type::ClearFeature:
										debug ("  DeviceRequest::ClearFeature\n");
										break;

									case Type::SetFeature:
										debug ("  DeviceRequest::SetFeature\n");
										break;

									case Type::SetAddress:
										debug ("  DeviceRequest::SetAddress %u\n", (unsigned)setup.request.device.set_address.address);
										// Address should be set after completion of the status stage, so just remember it
										// for now:
										_address_to_set = setup.request.device.set_address.address;
										respond = true;
										respond_bytes = 0;
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
			}

			if (respond)
			{
				_input_endpoint.set_ready (respond_bytes);
				debug ("<IN %u B\n", (unsigned)respond_bytes);
				_input_endpoint.wait_for_transaction_complete();
				debug ("<IN done\n");
			}
			else
			{
				_input_endpoint.reset_transaction_complete();
				_input_endpoint.reset_setup_transaction_complete();
			}

			if (_output_endpoint.transaction_complete() || _output_endpoint.setup_transaction_complete())
			{
				debug("PREP OUT\n");
				_output_endpoint.set_ready();
			}

			// TODO _buffer_size = 0;
		}

} // namespace usb
} // namespace avr
} // namespace mulabs

#endif

