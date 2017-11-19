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

// Mulabs:
#include <mulabs_avr/utility/span.h>


namespace mulabs {
namespace avr {
namespace usb {

/**
 * Handles and buffers control transfers (Setup packets and responses).
 *
 * Has three main states:
 *   • waiting for Setup packet (initial state)
 *   • waiting for output transaction(s) to complete
 *   • waiting for input transaction(s) to complete
 */
template<class pInputEndpoint, class pInputBuffer, class pOutputEndpoint, class pOutputBuffer, size_t pSize>
	class ControlTransfer
	{
	  public:
		using InputEndpoint				= pInputEndpoint;
		using InputBuffer				= pInputBuffer;
		using OutputEndpoint			= pOutputEndpoint;
		using OutputBuffer				= pOutputBuffer;
		using Buffer					= Array<uint8_t, pSize>;

		static constexpr size_t kSize	= pSize;

		static_assert (kSize >= 8, "buffer size must be at least 8 bytes (for Setup packets)");

		enum class Error
		{
			SetupTransactionNot8Bytes,
			ExpectedZLPAck,
			BufferOverflow,
		};

	  private:
		enum class State
		{
			Wait4SetupToken,
			Wait4OutputToken,
			Wait4HostToDeviceAck,
			Wait4InputToken,
			Wait4DeviceToHostAck,
		};

		enum class NextOutputResult
		{
			More,
			Finished,
			Overflow,
		};

		enum class NextInputResult
		{
			More,
			Finished,
		};

	  public:
		// Ctor
		explicit constexpr
		ControlTransfer (InputEndpoint&, InputBuffer&, OutputEndpoint&, OutputBuffer&, Buffer&) noexcept;

		/**
		 * Check enpoints state, read buffers when necessary and update state of the transfer.
		 *
		 * \param	on_setup (Setup const&) -> bool
		 *			Called when setup request is received.
		 *			If a host-to-device request was received, then also OUT transaction was received and is buffered
		 *			and ready to use. If on_setup returns true, ACK (ZLP) is sent to the host.
		 *			If a device-to-host request was received, when the on_setup function returns true, it's expected
		 *			that the buffer is filled with data and transaction size is set, so that it will be sent back to host.
		 * \param	on_finished
		 *			Called when transfer is complete, that is ACK was sent or received as necessary.
		 * \param	on_error (Error) -> void
		 *			Called on errors with appropriate error argument.
		 */
		template<class OnSetup, class OnFinished, class OnError>
			void
			handle_interrupt (OnSetup&& on_setup, OnFinished&& on_finished, OnError&& on_error) noexcept;

		/**
		 * Get ready for handling next OUT token.
		 */
		[[nodiscard]]
		NextOutputResult
		post_output() noexcept;

		/**
		 * Get ready for handling next IN token.
		 */
		[[nodiscard]]
		NextInputResult
		pre_input() noexcept;

		/**
		 * Reset to initial state.
		 */
		void
		reset() noexcept;

		/**
		 * Reset to given state so that fresh transfer can be done.
		 * Performs necessary endpoints setup for requested state.
		 */
		void
		reset (State) noexcept;

		/**
		 * Access the buffer object.
		 */
		Buffer&
		buffer() noexcept;

		/**
		 * Access the buffer object.
		 */
		Buffer const&
		buffer() const noexcept;

		/**
		 * Set IN transfer size.
		 */
		void
		set_transfer_size (size_t bytes) noexcept;

	  private:
		InputEndpoint&		_input_endpoint;
		InputBuffer&		_input_buffer;
		OutputEndpoint&		_output_endpoint;
		OutputBuffer&		_output_buffer;
		SetupPacket			_setup;
		Buffer&				_buffer;
		State				_state			{ State::Wait4SetupToken };
		size_t				_position		{ 0 };
		size_t				_total_size		{ 0 };
	};


template<class IE, class IB, class OE, class OB, size_t S>
	constexpr
	ControlTransfer<IE, IB, OE, OB, S>::ControlTransfer (InputEndpoint& input_endpoint, InputBuffer& input_buffer, OutputEndpoint& output_endpoint, OutputBuffer& output_buffer, Buffer& buffer) noexcept:
		_input_endpoint (input_endpoint),
		_input_buffer (input_buffer),
		_output_endpoint (output_endpoint),
		_output_buffer (output_buffer),
		_buffer (buffer)
	{
		reset();
	}


template<class IE, class IB, class OE, class OB, size_t S>
	template<class OnSetup, class OnFinished, class OnError>
		inline void
		ControlTransfer<IE, IB, OE, OB, S>::handle_interrupt (OnSetup&& on_setup, OnFinished&& on_finished, OnError&& on_error) noexcept
		{
			// Setup token always has priority:
			if (_output_endpoint.setup_transaction_complete())
			{
				_total_size = _output_endpoint.transaction_size();

				// Setup transaction must be 8 bytes, if it's not, discard:
				if (_total_size != sizeof (usb::SetupPacket))
				{
					on_error (Error::SetupTransactionNot8Bytes);
					_output_endpoint.set_ready();
				}
				// If valid size:
				else
				{
					_setup = _output_buffer.template as<usb::SetupPacket>();

					switch (_setup.transfer_direction)
					{
						case SetupPacket::TransferDirection::HostToDevice:
							_total_size = _setup.length;

							if (_total_size > 0)
								reset (State::Wait4OutputToken);
							else
							{
								if (on_setup (_setup, Span<uint8_t>()))
									reset (State::Wait4HostToDeviceAck);
								else
									reset (State::Wait4SetupToken);
							}
							break;

						case SetupPacket::TransferDirection::DeviceToHost:
							if (on_setup (_setup, Span<uint8_t>()))
								reset (State::Wait4InputToken);
							else
								reset (State::Wait4SetupToken);
							break;
					}
				}

				_output_endpoint.reset_setup_transaction_complete();
				_input_endpoint.reset_setup_transaction_complete();
			}
			// If not a setup transaction:
			else
			{
				switch (_state)
				{
					case State::Wait4SetupToken:
						if (_output_endpoint.transaction_complete())
							// Discard received data and prepare for setup again:
							reset (State::Wait4SetupToken);
						break;

					case State::Wait4OutputToken:
						if (_output_endpoint.transaction_complete())
						{
							switch (post_output())
							{
								case NextOutputResult::Finished:
									if (on_setup (_setup, Span (_buffer.data(), _position)))
										reset (State::Wait4HostToDeviceAck);
									break;

								case NextOutputResult::More:
									break;

								case NextOutputResult::Overflow:
									on_error (Error::BufferOverflow);
									break;
							}
						}
						break;

					case State::Wait4HostToDeviceAck:
						if (_input_endpoint.transaction_complete())
						{
							on_finished();
							reset (State::Wait4SetupToken);
						}
						break;

					case State::Wait4InputToken:
						if (_input_endpoint.transaction_complete())
						{
							switch (pre_input())
							{
								case NextInputResult::Finished:
									reset (State::Wait4DeviceToHostAck);
									break;

								case NextInputResult::More:
									break;
							}
						}
						break;

					case State::Wait4DeviceToHostAck:
						if (_output_endpoint.transaction_complete())
						{
							if (_output_endpoint.transaction_size() == 0)
								on_finished();
							else
								on_error (Error::ExpectedZLPAck);

							reset (State::Wait4SetupToken);
						}
						break;
				}
			}
		}


template<class IE, class IB, class OE, class OB, size_t S>
	inline auto
	ControlTransfer<IE, IB, OE, OB, S>::post_output() noexcept -> NextOutputResult
	{
		size_t const transaction_size = _output_endpoint.transaction_size();

		// If there's still room for input:
		if (_position + transaction_size <= _total_size || transaction_size == 0)
		{
			for (size_t i = 0; i < transaction_size; ++i, ++_position)
				_buffer[_position] = _output_buffer[i];

			// If last packet:
			if (transaction_size < _output_buffer.size())
				return NextOutputResult::Finished;
			else
			{
				_output_endpoint.set_ready();
				return NextOutputResult::More;
			}
		}
		else
			return NextOutputResult::Overflow;
	}


template<class IE, class IB, class OE, class OB, size_t S>
	inline auto
	ControlTransfer<IE, IB, OE, OB, S>::pre_input() noexcept -> NextInputResult
	{
		if (_position < _total_size)
		{
			size_t to_send = std::min (_total_size - _position, _input_buffer.size());

			for (size_t i = 0; i < to_send; ++i, ++_position)
				_input_buffer[i] = _buffer[_position];

			_input_endpoint.set_ready (to_send);

			return NextInputResult::More;
		}
		else
			return NextInputResult::Finished;
	}


template<class IE, class IB, class OE, class OB, size_t S>
	inline void
	ControlTransfer<IE, IB, OE, OB, S>::reset() noexcept
	{
		reset (State::Wait4SetupToken);
	}


template<class IE, class IB, class OE, class OB, size_t S>
	inline void
	ControlTransfer<IE, IB, OE, OB, S>::reset (State state) noexcept
	{
		_state = state;
		_position = 0;

		if (state != State::Wait4InputToken)
			_total_size = 0;

		switch (state)
		{
			case State::Wait4SetupToken:
				// Setup tokens always come as DATA0:
				_output_endpoint.set_next_data_packet (InputEndpoint::Endpoint::Data::_0);
				_output_endpoint.set_ready();
				break;

			case State::Wait4OutputToken:
				_output_endpoint.set_ready();
				break;

			case State::Wait4HostToDeviceAck:
				// Send ZLP aka status stage (always needs to go as DATA1):
				_input_endpoint.set_next_data_packet (InputEndpoint::Endpoint::Data::_1);
				_input_endpoint.set_ready (0);
				break;

			case State::Wait4InputToken:
				static_cast<void> (pre_input());
				break;

			case State::Wait4DeviceToHostAck:
				// Send ZLP aka status stage (always needs to go as DATA1):
				_output_endpoint.set_next_data_packet (OutputEndpoint::Endpoint::Data::_1);
				_output_endpoint.set_ready();
				break;
		}
	}


template<class IE, class IB, class OE, class OB, size_t S>
	inline auto
	ControlTransfer<IE, IB, OE, OB, S>::buffer() noexcept -> Buffer&
	{
		return _buffer;
	}


template<class IE, class IB, class OE, class OB, size_t S>
	inline auto
	ControlTransfer<IE, IB, OE, OB, S>::buffer() const noexcept -> Buffer const&
	{
		return const_cast<ControlTransfer<IE, IB, OE, OB, S>*> (this)->buffer();
	}


template<class IE, class IB, class OE, class OB, size_t S>
	inline void
	ControlTransfer<IE, IB, OE, OB, S>::set_transfer_size (size_t bytes) noexcept
	{
		_total_size = bytes;
	}

} // namespace usb
} // namespace avr
} // namespace mulabs

#endif

