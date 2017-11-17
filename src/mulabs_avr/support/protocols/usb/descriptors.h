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

using StringID = uint8_t;


enum class DescriptorType: uint8_t
{
	Device				= 0x01,
	Configuration		= 0x02,
	String				= 0x03,
	Interface			= 0x04,
	Endpoint			= 0x05,
	BOS					= 0x0f,
};


/**
 * Thrown when buffer size is too small.
 */
class InsufficientBufferSize
{ };


struct DeviceDescriptor
{
	// This layout is required by the USB standard:
	uint8_t					length					= sizeof (DeviceDescriptor);
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

static_assert (sizeof (DeviceDescriptor) == 18);


struct ConfigurationDescriptor
{
	// This layout is required by the USB standard:
	uint8_t				length						= sizeof (ConfigurationDescriptor);
	DescriptorType		descriptor_type				= DescriptorType::Configuration;
	uint16_t			total_length				= 0;
	uint8_t				number_of_interfaces		= 0;
	Index::Value		configuration_value			= 0;
	StringID			description_index			= 0;
	uint8_t				flags						= 0;
	uint8_t				max_power_2_milli_amps		= 0;

  public:
	/**
	 * Return value for the 'flags' field.
	 */
	static constexpr uint8_t
	make_flags (SelfPowered, RemoteWakeup);
} __attribute__((packed));

static_assert (sizeof (ConfigurationDescriptor) == 9);


struct StringDescriptorZero
{
	// This layout is required by the USB standard:
	uint8_t				length						= sizeof (length) + sizeof (DescriptorType);
	DescriptorType		descriptor_type				= DescriptorType::String;
	// There should be a flexible array of LanguageIDs here.
} __attribute__((packed));

static_assert (sizeof (StringDescriptorZero) == 2);


struct StringDescriptor
{
	// This layout is required by the USB standard:
	uint8_t				length						= sizeof (length) + sizeof (DescriptorType);
	DescriptorType		descriptor_type				= DescriptorType::String;
	// There should be a flexible array of char16_t's here.
} __attribute__((packed));

static_assert (sizeof (StringDescriptor) == 2);


// TODO
struct InterfaceDescriptor
{
	// This layout is required by the USB standard:
	uint8_t				length						= sizeof (InterfaceDescriptor); // TODO supposed to be 9
	DescriptorType		descriptor_type				= DescriptorType::Interface;
} __attribute__((packed));

// TODO static_assert (sizeof (EndpointDescriptor) == 9);


// TODO
struct EndpointDescriptor
{
	// This layout is required by the USB standard:
	uint8_t				length						= sizeof (EndpointDescriptor); // TODO supposed to be 7
	DescriptorType		descriptor_type				= DescriptorType::Endpoint;
} __attribute__((packed));

// TODO static_assert (sizeof (EndpointDescriptor) == 7);


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
template<class pDeviceStrings>
	constexpr DeviceDescriptor
	make_device_descriptor (Device const& device, pDeviceStrings const& strings)
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
		descriptor.manufacturer_index = strings.index_for_string (*device.manufacturer);
		descriptor.product_index = strings.index_for_string (*device.product);
		descriptor.serial_number_index = strings.index_for_string (*device.serial);
		descriptor.num_configurations = device.configurations.size();
		return descriptor;
	}


/**
 * Make a configuration descriptor.
 */
template<class pDeviceStrings>
	constexpr ConfigurationDescriptor
	make_configuration_descriptor (Configuration const& configuration, pDeviceStrings const& strings)
	{
		ConfigurationDescriptor descriptor;
		descriptor.total_length = 9; // TODO?
		descriptor.number_of_interfaces = configuration.interfaces.size();
		descriptor.configuration_value = *configuration.index;
		descriptor.description_index = strings.index_for_string (configuration.description);
		descriptor.flags = descriptor.make_flags (configuration.self_powered, configuration.remote_wakeup);
		descriptor.max_power_2_milli_amps = *configuration.max_power_milli_amps / 2;
		return descriptor;
	}


namespace detail {

template<class ...pLanguageIDs>
	constexpr void
	fill_in_language_ids (LanguageID* target, LanguageID language_id, pLanguageIDs ...remaining_language_ids)
	{
		*target = language_id;

		if constexpr (sizeof... (remaining_language_ids) > 0)
			fill_in_language_ids (target + 1, remaining_language_ids...);
	}

} // namespace detail


/**
 * Make a string descriptor-0 (the one that lists available languages).
 */
template<class ...pLanguageIDs>
	[[nodiscard]]
	constexpr size_t
	make_string_descriptor_0 (StringDescriptorZero& target, size_t buffer_size, LanguageID language_id, pLanguageIDs ...remaining_language_ids)
	{
		constexpr size_t tags_length = sizeof (LanguageID) * (1 + sizeof... (remaining_language_ids));
		constexpr size_t total_bytes = sizeof (target) + tags_length;

		if (total_bytes > buffer_size)
			throw InsufficientBufferSize();
		else
		{
			target = StringDescriptorZero();
			target.length += tags_length;
			LanguageID* const language_ids_array = reinterpret_cast<LanguageID*> (reinterpret_cast<uint8_t*> (&target) + sizeof (target));
			detail::fill_in_language_ids (language_ids_array, language_id, remaining_language_ids...);
		}

		return total_bytes;
	}


/**
 * Make a string descriptor.
 */
[[nodiscard]]
constexpr size_t
make_string_descriptor (StringDescriptor& target, size_t buffer_size, String const& string)
{
	size_t string_size_bytes = string.size() * sizeof (String::Char);
	size_t total_bytes = sizeof (target) + string_size_bytes;

	if (total_bytes > buffer_size)
		throw InsufficientBufferSize();
	else
	{
		target = StringDescriptor();
		target.length += string_size_bytes;
		String::Char* strdest = reinterpret_cast<String::Char*> (reinterpret_cast<uint8_t*> (&target) + sizeof (target));

		for (uint8_t i = 0; i < string.size(); ++i, ++strdest)
			*strdest = string[i];
	}

	return total_bytes;
}

} // namespace usb
} // namespace avr
} // namespace mulabs

#endif

