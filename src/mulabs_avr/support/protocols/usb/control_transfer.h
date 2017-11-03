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

#ifndef MULABS_AVR__SUPPORT__PROTOCOLS__CONTROL_TRANSFER_H__INCLUDED
#define MULABS_AVR__SUPPORT__PROTOCOLS__CONTROL_TRANSFER_H__INCLUDED

namespace mulabs {
namespace avr {
namespace usb {

/**
 * Handles and buffers control transfers (Setup packets and responses).
 */
template<class pInputEndpoint, class pInputBuffer, class pOutputEndpoint, class pOutputBuffer, size_t pSize>
	class ControlTransfer
	{
	  public:
		using InputEndpoint				= pInputEndpoint;
		using InputBuffer				= pInputBuffer;
		using OutputEndpoint			= pOutputEndpoint;
		using OutputBuffer				= pOutputBuffer;
		using Buffer					= uint8_t[pSize];

		static constexpr size_t kSize	= pSize;

	  public:
		// Ctor
		explicit constexpr
		ControlTransfer (InputEndpoint&, InputBuffer&, OutputEndpoint&, OutputBuffer&, Buffer&);

		/**
		 * Check enpoints state, read buffers when necessary and update state of the transfer.
		 */
		void
		handle_interrupt();

		/**
		 * Return true if transfer has finished.
		 */
		bool
		finished() const;

		/**
		 * Reset state (reinitialize) so that new transfer can be done.
		 */
		void
		reset();

	  private:
		InputEndpoint&		_input_endpoint;
		InputBuffer&		_input_buffer;
		OutputEndpoint&		_output_endpoint;
		OutputBuffer&		_output_buffer;
		Buffer&				_buffer;
		size_t				_position	{ 0 };
		bool				_finished	{ false };
	};


template<class IE, class IB, class OE, class OB, size_t S>
	constexpr
	ControlTransfer<IE, IB, OE, OB, S>::ControlTransfer (InputEndpoint& input_endpoint, InputBuffer& input_buffer, OutputEndpoint& output_endpoint, OutputBuffer& output_buffer, Buffer& buffer):
		_input_endpoint (input_endpoint),
		_input_buffer (input_buffer),
		_output_endpoint (output_endpoint),
		_output_buffer (output_buffer),
		_buffer (buffer)
	{ }


template<class IE, class IB, class OE, class OB, size_t S>
	inline void
	ControlTransfer<IE, IB, OE, OB, S>::handle_interrupt()
	{
		// TODO
	}


template<class IE, class IB, class OE, class OB, size_t S>
	inline bool
	ControlTransfer<IE, IB, OE, OB, S>::finished() const
	{
		return _finished;
	}


template<class IE, class IB, class OE, class OB, size_t S>
	inline void
	ControlTransfer<IE, IB, OE, OB, S>::reset()
	{
		_position = 0;
		_finished = false;
	}

} // namespace usb
} // namespace avr
} // namespace mulabs

#endif

