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

#ifndef MULABS_AVR__SUPPORT__PROTOCOLS__DEVICE_DEFINITION_H__INCLUDED
#define MULABS_AVR__SUPPORT__PROTOCOLS__DEVICE_DEFINITION_H__INCLUDED

// Mulabs:
#include <mulabs_avr/std/algorithm.h>
#include <mulabs_avr/std/initializer_list.h>
#include <mulabs_avr/support/protocols/usb/types.h>
#include <mulabs_avr/utility/constring.h>
#include <mulabs_avr/utility/strong_type.h>


namespace mulabs {
namespace avr {
namespace usb {

using String		= Constring<char16_t>;
using Manufacturer	= StrongType<String, struct ManufacturerType>;
using Product		= StrongType<String, struct ProductType>;
using Serial		= StrongType<String, struct SerialType>;

enum class Direction: uint8_t
{
	Out					= 0b0,
	In					= 0b1,
};

enum class TransferType: uint8_t
{
	Control				= 0b00,
	Isochronous			= 0b01,
	Bulk				= 0b10,
	Interrupt			= 0b11,
};

enum class SyncType: uint8_t
{
	NoSync				= 0b00,
	Async				= 0b01,
	Sync				= 0b11,
	Adaptive			= 0b10,
};

enum class UsageType: uint8_t
{
	Data				= 0b00,
	Feedback			= 0b01,
	ExplicitFeedback	= 0b10,
};


/**
 * Base class for all exceptions here.
 */
class Exception
{ };


class Endpoint
{
  public:
	// Thrown when Endpoint is Bulk or Control type and has Interval value != 0.
	class Use0ForInterval: Exception
	{ };

	// Thrown when Endpoint is Isochronous and Interval value != 1.
	class Use1ForInterval: Exception
	{ };

	// Thrown when SyncType is set to anything else than NoSync for TransferType other than Isochronous.
	class MustBeNoSync: Exception
	{ };

	// Thrown when user tried to define default control endpoint here.
	class ControlEndpoint0MustNotBeDefinedHere: Exception
	{ };

	// Thrown when endpoint index is not within range 1..15.
	class InvalidEndpointIndex: Exception
	{ };

  public:
	// Ctor
	explicit constexpr
	Endpoint (Index, Direction, TransferType, SyncType, UsageType, MaxPacketSize, Interval = Interval (0));

  public:
	Index			index;
	Direction		direction;
	TransferType	transfer_type;
	SyncType		sync_type;
	UsageType		usage_type;
	MaxPacketSize	max_packet_size;
	Interval		interval;
};


class Interface
{
  public:
	// Thrown when DeviceClass is invalid (like InterfaceSpecified here).
	class BadDeviceClass: Exception
	{ };

  public:
	// Ctor
	explicit constexpr
	Interface (Index, AlternateIndex, DeviceClass, DeviceSubClass, DeviceProtocol, String description, std::initializer_list<Endpoint>);

  public:
	Index							index;
	AlternateIndex					alternate_index;
	DeviceClass						device_class;
	DeviceSubClass					device_sub_class;
	DeviceProtocol					device_protocol;
	String							description;
	std::initializer_list<Endpoint>	endpoints;
};


class Configuration
{
  public:
	// Thrown when configuration index is 0, which is forbidden. // TODO is it?
	class ConfigurationIndexMustNotBe0: Exception
	{ };

	// Thrown when interface indices are not sequentially ordered starting from 0.
	class InvalidIndexSequence: Exception
	{ };

	// Thrown when interface specified in alternate_index isn't defined.
	class AlternateInterfaceIndexNotFound: Exception
	{ };

	// Thrown when within single configuration there are two or more endpoints with the same index.
	class ConflictingEndpointIndices: Exception
	{ };

  public:
	// Ctor
	explicit constexpr
	Configuration (Index, String description, SelfPowered, RemoteWakeup, MaxPowerMilliAmps, std::initializer_list<Interface>);

  public:
	Index								index;
	String								description;
	SelfPowered							self_powered;
	RemoteWakeup						remote_wakeup;
	MaxPowerMilliAmps					max_power_milli_amps;
	std::initializer_list<Interface>	interfaces;
};


class Device
{
  public:
	// Thrown when configuration indices are found not to be unique numbers.
	class ConfigurationIndexNotUnique: Exception
	{ };

  public:
	// Ctor
	explicit constexpr
	Device (USBVersion, VendorID, ProductID, ReleaseID, DeviceClass, DeviceSubClass, DeviceProtocol,
			Manufacturer, Product, Serial, MaxPacketSize max_packet_size_0, std::initializer_list<Configuration> configurations);

	/**
	 * Return maximum endpoint address.
	 */
	constexpr uint8_t
	maximum_endpoint_id() const;

	// TODO
#if 0
	constexpr String
	string_for_descriptor (uint8_t descriptor_index) const;
#endif

  public:
	USBVersion								usb_version;
	VendorID								vendor_id;
	ProductID								product_id;
	ReleaseID								release_id;
	DeviceClass								device_class;
	DeviceSubClass							device_sub_class;
	DeviceProtocol							device_protocol;
	Manufacturer							manufacturer;
	Product									product;
	Serial									serial;
	MaxPacketSize							max_packet_size_0;
	std::initializer_list<Configuration>	configurations;
	// String indices are offset by 1, so that string for string-descriptor 1 is at strings[0], etc:
	std::initializer_list<String>			strings;
};


constexpr
Endpoint::Endpoint (Index index, Direction direction, TransferType transfer_type, SyncType sync_type,
					UsageType usage_type, MaxPacketSize max_packet_size, Interval interval):
	index (index),
	direction (direction),
	transfer_type (transfer_type),
	sync_type (sync_type),
	usage_type (usage_type),
	max_packet_size (max_packet_size),
	interval (interval)
{
	// Ensure that endpoint 0 is not specified here:
	if (*index == 0)
		throw ControlEndpoint0MustNotBeDefinedHere();

	// Make sure endpoint number is valid:
	if (*index < 1 || *index > 15)
		throw InvalidEndpointIndex();

	switch (transfer_type)
	{
		case TransferType::Control:
		case TransferType::Bulk:
			// Bulk and control endpoints should use interval 0:
			if (*interval != 0)
				throw Use0ForInterval();
			break;

		case TransferType::Isochronous:
			// Isochronous must use interval 1:
			if (*interval != 1)
				throw Use1ForInterval();

			if (sync_type != SyncType::NoSync)
				throw MustBeNoSync();
			break;

		case TransferType::Interrupt:
			break;
	}
}


constexpr
Interface::Interface (Index index, AlternateIndex alternate_index,
					  DeviceClass device_class, DeviceSubClass device_sub_class, DeviceProtocol device_protocol, String description,
					  std::initializer_list<Endpoint> endpoints):
	index (index),
	alternate_index (alternate_index),
	device_class (device_class),
	device_sub_class (device_sub_class),
	device_protocol (device_protocol),
	description (description),
	endpoints (endpoints)
{
	// Ensure that interface doesn't say the device class is interface-specified, because we're the interface:
	if (device_class == DeviceClass::InterfaceSpecified)
		throw BadDeviceClass();
}


constexpr
Configuration::Configuration (Index index, String description, SelfPowered self_powered, RemoteWakeup remote_wakeup,
							  MaxPowerMilliAmps max_power_milli_amps, std::initializer_list<Interface> interfaces):
	index (index),
	description (description),
	self_powered (self_powered),
	remote_wakeup (remote_wakeup),
	max_power_milli_amps (max_power_milli_amps),
	interfaces (interfaces)
{
	if (*index == 0)
		throw ConfigurationIndexMustNotBe0();

	// Ensure that interfaces are numbered sequentially from 0 as required by the USB standard:
	auto interface = interfaces.begin();

	for (size_t i = 0; i < interfaces.size(); ++i, ++interface)
	{
		if (*interface->index != i)
			throw InvalidIndexSequence();
	}

	// Ensure that this->alternate_index refers to an existing interface:
	for (auto const& interface: interfaces)
	{
		bool const found = std::any_of (interfaces.begin(), interfaces.end(), [&](auto const& other) {
			return *interface.alternate_index == *other.index;
		});

		if (!found)
			throw AlternateInterfaceIndexNotFound();
	}

	// Ensure that endpoints with the same direction have unique indices within configuration:
	for (auto const& interface: interfaces)
	{
		for (auto const& endpoint: interface.endpoints)
		{
			// Count all endpoints with this index, result must be 1:
			size_t number = 0;

			for (auto const& other_interface: interfaces)
				for (auto const& other_endpoint: other_interface.endpoints)
					if (*endpoint.index == *other_endpoint.index && endpoint.direction == other_endpoint.direction)
						++number;

			if (number != 1)
				throw ConflictingEndpointIndices();
		}
	}
}


constexpr
Device::Device (USBVersion usb_version, VendorID vendor_id, ProductID product_id, ReleaseID release_id,
				DeviceClass device_class, DeviceSubClass device_sub_class, DeviceProtocol device_protocol,
				Manufacturer manufacturer, Product product, Serial serial, MaxPacketSize max_packet_size_0,
				std::initializer_list<Configuration> configurations):
	usb_version (usb_version),
	vendor_id (vendor_id),
	product_id (product_id),
	release_id (release_id),
	device_class (device_class),
	device_sub_class (device_sub_class),
	device_protocol (device_protocol),
	manufacturer (manufacturer),
	product (product),
	serial (serial),
	max_packet_size_0 (max_packet_size_0),
	configurations (configurations)
{
	// Ensure that configurations identifiers are unique):
	for (auto const& configuration: configurations)
	{
		size_t num = 0;

		for (auto const& other: configurations)
			if (*configuration.index == *other.index)
				++num;

		if (num > 1)
			throw ConfigurationIndexNotUnique();
	}
}


constexpr uint8_t
Device::maximum_endpoint_id() const
{
	uint8_t max = 0;

	for (auto const& configuration: configurations)
		for (auto const& interface: configuration.interfaces)
			for (auto const& endpoint: interface.endpoints)
				if (max < *endpoint.index)
					max = *endpoint.index;

	return max;
}


#if 0
constexpr String
Device::string_for_descriptor (uint8_t descriptor_index) const
{
	return strings[descriptor_index - 1];
}
#endif

} // namespace usb
} // namespace avr
} // namespace mulabs

#endif

