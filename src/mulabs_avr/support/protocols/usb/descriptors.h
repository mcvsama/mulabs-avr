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
#include <mulabs_avr/utility/span.h>
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
 * Thrown when incorrect endpoint number is given.
 */
class InvalidEndpointNumber
{ };


/**
 * Thrown when buffer size is too small.
 */
class InsufficientBufferSize
{ };


struct DeviceDescriptor
{
	// This layout is required by the USB standard:
	uint8_t						length					= sizeof (DeviceDescriptor);
	DescriptorType				descriptor_type			= DescriptorType::Device;
	USBVersion					usb_version				= USBVersion::_2_0;
	DeviceClass					device_class			= DeviceClass::VendorSpecified;
	DeviceSubClass::Value		device_sub_class		= 0x00;
	DeviceProtocol::Value		device_protocol			= 0x00;
	MaxPacketSize0::Value		max_packet_size_0		= 64;
	VendorID::Value				vendor_id				= 0x0000;
	ProductID::Value			product_id				= 0x0000;
	ReleaseID::Value			release_id				= 0x0000;
	StringID					manufacturer_index		= 0;
	StringID					product_index			= 0;
	StringID					serial_number_index		= 0;
	uint8_t						num_configurations		= 0;
} __attribute__((packed));

static_assert (sizeof (DeviceDescriptor) == 18);


struct ConfigurationDescriptor
{
	// This layout is required by the USB standard:
	uint8_t						length						= sizeof (ConfigurationDescriptor);
	DescriptorType				descriptor_type				= DescriptorType::Configuration;
	uint16_t					total_length				= 0;
	uint8_t						number_of_interfaces		= 0;
	ConfigurationValue::Value	configuration_value			= 0;
	StringID					description_index			= 0;
	uint8_t						flags						= 0;
	uint8_t						max_power_2_milli_amps		= 0;

  public:
	/**
	 * Return value for the 'flags' field.
	 */
	static constexpr uint8_t
	make_flags (USBVersion, SelfPowered, RemoteWakeup);
} __attribute__((packed));

static_assert (sizeof (ConfigurationDescriptor) == 9);


struct InterfaceDescriptor
{
	// This layout is required by the USB standard:
	uint8_t						length						= sizeof (InterfaceDescriptor);
	DescriptorType				descriptor_type				= DescriptorType::Interface;
	Index::Value				index						= 0;
	AlternateIndex::Value		alternate_index				= 0;
	uint8_t						num_endpoints				= 0;
	DeviceClass					interface_class				= DeviceClass::VendorSpecified;
	DeviceSubClass::Value		interface_sub_class			= 0x00;
	DeviceProtocol::Value		interface_protocol			= 0x00;
	StringID					description_index			= 0;
} __attribute__((packed));

static_assert (sizeof (InterfaceDescriptor) == 9);


struct EndpointDescriptor
{
	// This layout is required by the USB standard:
	uint8_t						length						= sizeof (EndpointDescriptor);
	DescriptorType				descriptor_type				= DescriptorType::Endpoint;
	uint8_t						address						= 0x00;
	uint8_t						attributes					= 0x00;
	MaxPacketSize::Value		max_packet_size				= 0;
	Interval::Value				interval					= 0;

  public:
	/**
	 * Return value suitable for the 'address' field.
	 */
	static constexpr uint8_t
	make_address (Index, Direction);

	/**
	 * Return value suitable for the 'attributes' field.
	 */
	static constexpr uint8_t
	make_attributes (TransferType, SyncType, UsageType);
} __attribute__((packed));

static_assert (sizeof (EndpointDescriptor) == 7);


struct StringDescriptorZero
{
	// This layout is required by the USB standard:
	uint8_t						length						= sizeof (length) + sizeof (DescriptorType);
	DescriptorType				descriptor_type				= DescriptorType::String;
	// There should be a flexible array of LanguageIDs here.
} __attribute__((packed));

static_assert (sizeof (StringDescriptorZero) == 2);


struct StringDescriptor
{
	// This layout is required by the USB standard:
	uint8_t						length						= sizeof (length) + sizeof (DescriptorType);
	DescriptorType				descriptor_type				= DescriptorType::String;
	// There should be a flexible array of char16_t's here.
} __attribute__((packed));

static_assert (sizeof (StringDescriptor) == 2);


constexpr uint8_t
ConfigurationDescriptor::make_flags (USBVersion usb_version, SelfPowered self_powered, RemoteWakeup remote_wakeup)
{
	uint8_t result = 0;

	if (usb_version >= USBVersion::_1_1)
		result = 0b10000000;

	if (*self_powered)
		set_bit<6> (result);

	if (*remote_wakeup)
		set_bit<5> (result);

	return result;
}


constexpr uint8_t
EndpointDescriptor::make_address (Index endpoint_number, Direction direction)
{
	if (*endpoint_number >= 16)
		throw InvalidEndpointNumber();

	return (*endpoint_number << 0)
		 | (static_cast<uint8_t> (direction) << 7);
}


constexpr uint8_t
EndpointDescriptor::make_attributes (TransferType transfer_type, SyncType sync_type, UsageType usage_type)
{
	uint8_t result = static_cast<uint8_t> (transfer_type) << 0;

	if (transfer_type == TransferType::Isochronous)
	{
		result |= static_cast<uint8_t> (sync_type) << 2;
		result |= static_cast<uint8_t> (usage_type) << 4;
	}

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
	make_configuration_descriptor (Device const& device, Configuration const& configuration, pDeviceStrings const& strings)
	{
		ConfigurationDescriptor descriptor;
		descriptor.total_length = descriptor.length;
		descriptor.number_of_interfaces = configuration.interfaces.size();
		descriptor.configuration_value = *configuration.value;
		descriptor.description_index = strings.index_for_string (configuration.description);
		descriptor.flags = descriptor.make_flags (device.usb_version, configuration.self_powered, configuration.remote_wakeup);
		descriptor.max_power_2_milli_amps = *configuration.max_power_milli_amps / 2;
		return descriptor;
	}


template<class pDeviceStrings>
	constexpr InterfaceDescriptor
	make_interface_descriptor (Interface const& interface, pDeviceStrings const& strings)
	{
		InterfaceDescriptor descriptor;
		descriptor.index = *interface.index;
		descriptor.alternate_index = *interface.alternate_index;
		descriptor.num_endpoints = interface.endpoints.size();
		descriptor.interface_class = interface.interface_class;
		descriptor.interface_sub_class = *interface.interface_sub_class;
		descriptor.interface_protocol = *interface.interface_protocol;
		descriptor.description_index = strings.index_for_string (interface.description);
		return descriptor;
	}


constexpr EndpointDescriptor
make_endpoint_descriptor ([[maybe_unused]] Endpoint const& endpoint)
{
	EndpointDescriptor descriptor;
	descriptor.address = EndpointDescriptor::make_address (endpoint.index, endpoint.direction);
	descriptor.attributes = EndpointDescriptor::make_attributes (endpoint.transfer_type, endpoint.sync_type, endpoint.usage_type);
	descriptor.max_packet_size = *endpoint.max_packet_size;
	descriptor.interval = *endpoint.interval;
	return descriptor;
}


/**
 * Make a configuration descriptor and also the rest of the hierarchy for that configuration.
 * Put it all into a buffer.
 */
template<class pDeviceStrings>
	[[nodiscard]]
	constexpr size_t
	make_full_configuration_descriptor (Span<uint8_t> target_buffer, Device const& device, uint8_t configuration_index, pDeviceStrings const& strings)
	{
		Configuration const configuration = device.configuration_for_index (configuration_index);
		auto const initial_pointer = target_buffer.data();

		auto& configuration_descriptor = target_buffer.template as<ConfigurationDescriptor>();
		configuration_descriptor = make_configuration_descriptor (device, configuration, strings);
		target_buffer.remove_prefix (sizeof (configuration_descriptor));

		for (auto const& interface: configuration.interfaces)
		{
			auto& interface_descriptor = target_buffer.template as<InterfaceDescriptor>();
			interface_descriptor = make_interface_descriptor (interface, strings);
			target_buffer.remove_prefix (sizeof (interface_descriptor));

			for (auto const& endpoint: interface.endpoints)
			{
				auto& endpoint_descriptor = target_buffer.template as<EndpointDescriptor>();
				endpoint_descriptor = make_endpoint_descriptor (endpoint);
				target_buffer.remove_prefix (sizeof (endpoint_descriptor));
			}
		}

		size_t const total_length = target_buffer.data() - initial_pointer;
		configuration_descriptor.total_length = total_length;
		return total_length;
	}


namespace detail {

template<class ...pLanguageIDs>
	constexpr void
	fill_in_language_ids (LanguageID* target_lang_id, LanguageID language_id, pLanguageIDs ...remaining_language_ids)
	{
		*target_lang_id = language_id;

		if constexpr (sizeof... (remaining_language_ids) > 0)
			fill_in_language_ids (target_lang_id + 1, remaining_language_ids...);
	}

} // namespace detail


/**
 * Make a string descriptor-0 (the one that lists available languages).
 */
template<class ...pLanguageIDs>
	[[nodiscard]]
	constexpr size_t
	make_string_descriptor_0 (Span<uint8_t> target_buffer, LanguageID language_id, pLanguageIDs ...remaining_language_ids)
	{
		auto& descriptor = target_buffer.template as<StringDescriptorZero>();
		constexpr size_t tags_length = sizeof (LanguageID) * (1 + sizeof... (remaining_language_ids));
		constexpr size_t total_bytes = sizeof (descriptor) + tags_length;

		if (total_bytes > target_buffer.size())
			throw InsufficientBufferSize();
		else
		{
			descriptor = StringDescriptorZero();
			descriptor.length += tags_length;
			LanguageID* const language_ids_array = reinterpret_cast<LanguageID*> (target_buffer.data() + sizeof (descriptor));
			detail::fill_in_language_ids (language_ids_array, language_id, remaining_language_ids...);
		}

		return total_bytes;
	}


/**
 * Make a string descriptor.
 */
[[nodiscard]]
constexpr size_t
make_string_descriptor (Span<uint8_t> target_buffer, String const& string)
{
	auto& descriptor = target_buffer.template as<StringDescriptor>();
	size_t string_size_bytes = string.size() * sizeof (String::Char);
	size_t total_bytes = sizeof (descriptor) + string_size_bytes;

	if (total_bytes > target_buffer.size())
		throw InsufficientBufferSize();
	else
	{
		descriptor = StringDescriptor();
		descriptor.length += string_size_bytes;
		String::Char* strdest = reinterpret_cast<String::Char*> (target_buffer.data() + sizeof (descriptor));

		for (uint8_t i = 0; i < string.size(); ++i, ++strdest)
			*strdest = string[i];
	}

	return total_bytes;
}

} // namespace usb
} // namespace avr
} // namespace mulabs

#endif

