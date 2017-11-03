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

#ifndef MULABS_AVR__SUPPORT__PROTOCOLS__BUFFERED_TRANSFER_H__INCLUDED
#define MULABS_AVR__SUPPORT__PROTOCOLS__BUFFERED_TRANSFER_H__INCLUDED

namespace mulabs {
namespace avr {
namespace usb {

/**
 * Wraps a transfer object and provides its own buffer.
 */
template<class pTransfer>
	class BufferedTransfer: pTransfer
	{
	  public:
		using Transfer			= pTransfer;
		using InputEndpoint		= typename Transfer::InputEndpoint;
		using InputBuffer		= typename Transfer::InputBuffer;
		using OutputEndpoint	= typename Transfer::OutputEndpoint;
		using OutputBuffer		= typename Transfer::OutputBuffer;
		using Buffer			= typename Transfer::Buffer;

	  public:
		// Ctor
		explicit constexpr
		BufferedTransfer (InputEndpoint&, InputBuffer&, OutputEndpoint&, OutputBuffer&);

	  private:
		Buffer	_proper_buffer;
	};


template<class T>
	constexpr
	BufferedTransfer<T>::BufferedTransfer (InputEndpoint& input_endpoint, InputBuffer& input_buffer, OutputEndpoint& output_endpoint, OutputBuffer& output_buffer):
		Transfer (input_endpoint, input_buffer, output_endpoint, output_buffer, _proper_buffer)
	{ }

} // namespace usb
} // namespace avr
} // namespace mulabs

#endif

