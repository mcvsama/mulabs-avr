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

#ifndef MULABS_AVR__SUPPORT__PROTOCOLS__SETUP_PACKET_H__INCLUDED
#define MULABS_AVR__SUPPORT__PROTOCOLS__SETUP_PACKET_H__INCLUDED

// Mulabs:
#include <mulabs_avr/support/protocols/usb/descriptors.h>


namespace mulabs {
namespace avr {
namespace usb {

struct SetupPacket
{
  public:
	// TODO should be strongly typed when https://gcc.gnu.org/bugzilla/show_bug.cgi?id=51242 is fixed
	enum TransferDirection
	{
		HostToDevice	= 0b0,
		DeviceToHost	= 0b1,
	};

	// TODO should be strongly typed when https://gcc.gnu.org/bugzilla/show_bug.cgi?id=51242 is fixed
	enum Type
	{
		Standard		= 0b00,
		Class			= 0b01,
		Vendor			= 0b10,
	};

	// TODO should be strongly typed when https://gcc.gnu.org/bugzilla/show_bug.cgi?id=51242 is fixed
	enum Recipient
	{
		Device			= 0b00000,
		Interface		= 0b00001,
		Endpoint		= 0b00010,
		Other			= 0b00011,
	};

	enum class Feature: uint16_t
	{
		SelfPowered		= 1u << 0,
		RemoteWakeup	= 1u << 1,
	};

	struct DeviceRequest
	{
	  public:
		enum class Type: uint8_t
		{
			// Device to host:
			GetStatus			= 0x00,
			GetDescriptor		= 0x06,
			GetConfiguration	= 0x08,
			// Host to device:
			ClearFeature		= 0x01,
			SetFeature			= 0x03,
			SetAddress			= 0x05,
			SetDescriptor		= 0x07,
			SetConfiguration	= 0x09,
		};

		struct GetStatus
		{
			uint16_t			reserved[2];
			// SetupPacket::data is two-byte status.
		} __attribute__((packed));

		struct SetClearFeature
		{
			Feature				feature;
			uint16_t			reserved[1];
		} __attribute__((packed));

		struct SetAddress
		{
			uint16_t			address;
			uint16_t			reserved[1];
		} __attribute__((packed));

		struct SetGetDescriptor
		{
			uint8_t				index;
			DescriptorType		type;
			uint16_t			language_id;
			// SetupPacket::data is the descriptor.
		} __attribute__((packed));

		struct SetConfiguration
		{
			uint16_t			configuration_value;
			uint16_t			reserved[1];
		} __attribute__((packed));

		struct GetConfiguration
		{
			uint16_t			reserved[2];
			// SetupPacket::data is 1-byte configuration value.
		} __attribute__((packed));

	  public:
		Type					type;
		union {
			// Device to host:
			GetStatus			get_status;
			SetGetDescriptor	get_descriptor;
			GetConfiguration	get_configuration;
			// Host to device:
			SetClearFeature		clear_feature;
			SetClearFeature		set_feature;
			SetAddress			set_address;
			SetGetDescriptor	set_descriptor;
			SetConfiguration	set_configuration;
		};
	} __attribute__((packed));

	struct InterfaceRequest
	{
	  public:
		enum class Type: uint8_t
		{
			// Device to host:
			GetStatus			= 0x00,
			GetInterface		= 0x0a,
			// Host to device:
			ClearFeature		= 0x01,
			SetFeature			= 0x03,
			SetInterface		= 0x11,
		};

		struct GetStatus
		{
			uint16_t			reserved[1];
			uint16_t			interface;
		} __attribute__((packed));

		struct SetClearFeature
		{
			Feature				feature;
			uint16_t			interface;
		} __attribute__((packed));

		struct GetInterface
		{
			uint16_t			reserved[1];
			uint16_t			interface;
		} __attribute__((packed));

		struct SetInterface
		{
			uint16_t			alternate_index;
			uint16_t			interface;
		} __attribute__((packed));

	  public:
		Type					type;
		union {
			// Device to host:
			GetStatus			get_status;
			GetInterface		get_interface;
			// Host to device:
			SetClearFeature		clear_feature;
			SetClearFeature		set_feature;
			SetInterface		set_interface;
		};
	} __attribute__((packed));

	struct EndpointRequest
	{
	  public:
		enum class Type: uint8_t
		{
			// Device to host:
			GetStatus			= 0x00,
			SynchFrame			= 0x12,
			// Host to device:
			ClearFeature		= 0x01,
			SetFeature			= 0x03,
		};

		struct Endpoint
		{
			uint16_t			direction:1;
			uint16_t			reserved_1:3;
			uint16_t			index:4;
		} __attribute__((packed));

		struct GetStatus
		{
			uint16_t			reserved[1];
			Endpoint			endpoint;
		} __attribute__((packed));

		struct SynchFrame
		{
			uint16_t			reserved[1];
			Endpoint			endpoint;
		} __attribute__((packed));

		struct SetClearFeature
		{
			Feature				feature;
			uint16_t			interface;
		} __attribute__((packed));

	  public:
		Type					type;
		union {
			// Device to host:
			GetStatus			get_status;
			SynchFrame			synch_frame;
			// Host to device:
			SetClearFeature		clear_feature;
			SetClearFeature		set_feature;
		};
	} __attribute__((packed));

	union Request
	{
		DeviceRequest			device;
		InterfaceRequest		interface;
		EndpointRequest			endpoint;
	};

  public:
	Recipient					recipient:5;
	Type						type:2;
	TransferDirection			transfer_direction:1;
	Request						request;
	// Length of the data[] array:
	uint16_t					length;
} __attribute__((packed));

} // namespace usb
} // namespace avr
} // namespace mulabs

#endif

