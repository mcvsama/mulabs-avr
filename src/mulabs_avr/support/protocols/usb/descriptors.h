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

#ifndef MULABS_AVR__SUPPORT__PROTOCOLS__DESCRIPTORS_H__INCLUDED
#define MULABS_AVR__SUPPORT__PROTOCOLS__DESCRIPTORS_H__INCLUDED

// Mulabs:
#include <mulabs_avr/support/protocols/usb/device_definition.h>
#include <mulabs_avr/support/protocols/usb/types.h>
#include <mulabs_avr/utility/strong_type.h>


namespace mulabs {
namespace avr {
namespace usb {

enum class DescriptorType: uint8_t
{
	Device				= 0x01,
	Configuration		= 0x02,
	String				= 0x03,
	Interface			= 0x04,
	Endpoint			= 0x05,
	BOS					= 0x0f,
};


using StringID = uint8_t;


struct DeviceDescriptor
{
  public:
	uint8_t					length					= 18;
	DescriptorType			descriptor_type			= DescriptorType::Device;
	USBVersion				usb_version				= USBVersion::_2_0;
	DeviceClass				device_class			= DeviceClass::VendorSpecified;
	DeviceSubClass::Value	device_sub_class		= 0x00;
	DeviceProtocol::Value	device_protocol			= 0x00;
	MaxPacketSize::Value	max_packet_size_0		= 64;
	VendorID::Value			vendor_id				= 0x0000;
	ProductID::Value		product_id				= 0x0000;
	ReleaseID::Value		release_id				= 0x0000;
	StringID				manufacturer_index		= 0;
	StringID				product_index			= 0;
	StringID				serial_number_index		= 0;
	uint8_t					num_configurations		= 0;
} __attribute__((packed));


struct ConfigurationDescriptor
{
	uint8_t				length						= 9;
	DescriptorType		descriptor_type				= DescriptorType::Configuration;
	uint16_t			total_length				= 0;
	uint8_t				number_of_interfaces		= 0;
	Index::Value		configuration_value			= 0;
	StringID			description_index			= 0;
	uint8_t				flags						= 0;
	uint8_t				max_power_2_milli_amps		= 0;

  public:
	static constexpr uint8_t
	make_flags (SelfPowered, RemoteWakeup);
} __attribute__((packed));


// TODO StringDescriptor


// TODO
struct InterfaceDescriptor
{
	uint8_t				length						= 9;
	DescriptorType		descriptor_type				= DescriptorType::Interface;
} __attribute__((packed));


// TODO
struct EndpointDescriptor
{
	uint8_t				length						= 7;
	DescriptorType		descriptor_type				= DescriptorType::Endpoint;
} __attribute__((packed));


constexpr uint8_t
ConfigurationDescriptor::make_flags (SelfPowered self_powered, RemoteWakeup remote_wakeup)
{
	uint8_t result = 0b10000000;

	if (*self_powered)
		set_bit<6> (result);

	if (*remote_wakeup)
		set_bit<5> (result);

	return result;
}


/*
 * Global functions
 */


/**
 * Make a DeviceDescriptor for the provided Device.
 */
constexpr DeviceDescriptor
make_device_descriptor (Device const& device)
{
	DeviceDescriptor descriptor;
	descriptor.usb_version = device.usb_version;
	descriptor.device_class = device.device_class;
	descriptor.device_sub_class = *device.device_sub_class;
	descriptor.device_protocol = *device.device_protocol;
	descriptor.max_packet_size_0 = *device.max_packet_size_0;
	descriptor.vendor_id = *device.vendor_id;
	descriptor.product_id = *device.product_id;
	descriptor.release_id = *device.release_id;
	descriptor.manufacturer_index = 1; // TODO device.manufacturer
	descriptor.product_index = 2; // TODO device.product
	descriptor.serial_number_index = 3; // TODO device.serial
	descriptor.num_configurations = device.configurations.size();
	return descriptor;
}


/**
 * Make a configuration descriptor.
 */
constexpr ConfigurationDescriptor
make_configuration_descriptor (Configuration const& configuration)
{
	ConfigurationDescriptor descriptor;
	descriptor.total_length = 9; // TODO?
	descriptor.number_of_interfaces = configuration.interfaces.size();
	descriptor.configuration_value = *configuration.index;
	descriptor.description_index = 1; // TODO
	descriptor.flags = descriptor.make_flags (configuration.self_powered, configuration.remote_wakeup);
	descriptor.max_power_2_milli_amps = *configuration.max_power_milli_amps / 2;
	return descriptor;
}

} // namespace usb
} // namespace avr
} // namespace mulabs

#endif

