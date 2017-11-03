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

#ifndef MULABS_AVR__SUPPORT__PROTOCOLS__TYPES_H__INCLUDED
#define MULABS_AVR__SUPPORT__PROTOCOLS__TYPES_H__INCLUDED

// Mulabs:
#include <mulabs_avr/utility/strong_type.h>


namespace mulabs {
namespace avr {
namespace usb {

using Index				= StrongType<uint8_t, struct IndexType>;
using AlternateIndex	= StrongType<uint8_t, struct AlternateIndexType>;
using MaxPacketSize		= StrongType<uint8_t, struct MaxPacketSizeType>;
using VendorID			= StrongType<uint16_t, struct VendorIDType>;
using ProductID			= StrongType<uint16_t, struct ProductIDType>;
using ReleaseID			= StrongType<uint16_t, struct ReleaseIDType>;
using DeviceSubClass	= StrongType<uint8_t, struct DeviceSubClassType>;
using DeviceProtocol	= StrongType<uint8_t, struct DeviceProtocolType>;
using MaxPowerMilliAmps	= StrongType<uint8_t, struct MaxPowerMilliAmpsType>;
using SelfPowered		= StrongType<bool, struct SelfPoweredType>;
using RemoteWakeup		= StrongType<bool, struct RemoteWakeupType>;
using Interval			= StrongType<uint8_t, struct IntervalType>;

enum class USBVersion: uint16_t
{
	_1_0				= 0x0100,
	_1_1				= 0x0110,
	_2_0				= 0x2000,
};

enum class DeviceClass: uint8_t
{
	// Means that each interface specifies its own class:
	InterfaceSpecified	= 0x00,
	// Means that class is vendor-specific:
	VendorSpecified		= 0xff,
};

} // namespace usb
} // namespace avr
} // namespace mulabs

#endif

