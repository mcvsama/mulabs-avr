/* vim:ts=4
 *
 * Copyleft 2012…2014  Michał Gawron
 * Marduk Unix Labs, http://mulabs.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Visit http://www.gnu.org/licenses/gpl-3.0.html for more information on licensing.
 */

#ifndef MULABS_AVR__DEVICES__XMEGA_AU__BASIC_USART_H__INCLUDED
#define MULABS_AVR__DEVICES__XMEGA_AU__BASIC_USART_H__INCLUDED

#include <mulabs_avr/devices/xmega_au/interrupt_system.h>


namespace mulabs {
namespace avr {
namespace xmega_au {

template<class pMCU>
	class BasicUSART
	{
	  public:
		typedef pMCU						MCU;
		typedef typename MCU::Register8		Register8;

		enum class Mode: uint8_t
		{
			Asynchronous		= 0b00 << 6,
			Synchronous			= 0b01 << 6,
			IrCOM				= 0b10 << 6,
			MasterSPI			= 0b11 << 6,
		};

		enum class Parity: uint8_t
		{
			None				= 0b00 << 4,
			Even				= 0b10 << 4,
			Odd					= 0b11 << 4,
		};

		enum class Speed: uint8_t
		{
			_1x					= 0b0 << 2,
			_2x					= 0b1 << 2,
		};

	  private:
		enum class DataBits: uint8_t
		{
			_5					= 0b000,
			_6					= 0b001,
			_7					= 0b010,
			_8					= 0b011,
			_9					= 0b111,
		};

		enum class StopBits: uint8_t
		{
			_1					= 0b0 << 3,
			_2					= 0b1 << 3,
		};

		enum class StatusFlags: uint8_t
		{
			RxComplete			= 1 << 7,	// Interrupt flag
			TxComplete			= 1 << 6,	// Interrupt flag
			DataRegisterEmpty	= 1 << 5,	// Interrupt flag
			FrameError			= 1 << 4,
			BufferOverflow		= 1 << 3,
			ParityError			= 1 << 2,
		};

	  public:
		// Ctor:
		constexpr
		BasicUSART (Register8 data, Register8 status,
					Register8 ctrla, Register8 ctrlb, Register8 ctrlc,
					Register8 baudctrla, Register8 baudctrlb);

		constexpr bool
		operator== (BasicUSART const& other) const;

		constexpr bool
		operator!= (BasicUSART const& other) const;

		/**
		 * Wait for possibility to write to the buffer.
		 */
		void
		wait_for_write() const;

		/**
		 * Write the 5…8-bit word to the output register(s).
		 * If using 9-bit mode, use write9() instead.
		 *
		 * The transmit buffer can be written only when DREIF in the STATUS register is set.
		 */
		void
		write (uint8_t) const;

		/**
		 * Wait for possibility to write to the buffer and then write.
		 */
		void
		write_blocking (uint8_t) const;

		/**
		 * Writesthe 9-bit word to the output register(s).
		 */
		void
		write9 (uint16_t) const;

		/**
		 * 9-bit version of the write_blocking().
		 */
		void
		write9_blocking (uint16_t) const;

		/**
		 */
		/**
		 * Wait for possibility to read from the input buffer.
		 */
		void
		wait_for_read() const;

		/**
		 * Read 5…8-bit word from the input register(s).
		 * If using 9-bit mode, use read9() instead.
		 */
		uint8_t
		read() const;

		/**
		 * Wait for possibility to read from the input buffer and then read.
		 */
		uint8_t
		read_blocking() const;

		/**
		 * Read the 9-bit word from the input register(s).
		 */
		uint16_t
		read9() const;

		/**
		 * 9-bit version of the read_blocking().
		 */
		uint16_t
		read9_blocking() const;

		/**
		 * Return true there was an error in the last incoming frame.
		 * It's cleared when data is read from the buffer.
		 */
		bool
		is_frame_error() const;

		/**
		 * Return true if receiver overflow occured.
		 * It's cleared when data is read from the buffer.
		 */
		bool
		is_rx_overflow() const;

		/**
		 * Return true if parity error occured when reading data.
		 * It's cleared when data is read from the buffer.
		 */
		bool
		is_rx_parity_error() const;

		/**
		 * Enable/disable receiver.
		 */
		void
		set_rx_enabled (bool enabled) const;

		/**
		 * Enable/disable transmitter.
		 */
		void
		set_tx_enabled (bool enabled) const;

		/**
		 * Set rx complete interrupt level.
		 */
		void
		set_rx_complete (InterruptSystem::Level) const;

		/**
		 * Set rx complete interrupt level.
		 */
		void
		set_tx_complete (InterruptSystem::Level) const;

		/**
		 * Set data-register-empty interrupt level.
		 */
		void
		set_data_register_empty (InterruptSystem::Level) const;

		/**
		 * Enable/disable double transmission speed.
		 *
		 * This must be set to 1x when using IrCOM mode or synchronous modes.
		 * It's irrelevant in MasterSPI mode.
		 */
		void
		set (Speed) const;

		/**
		 * Enable/disable multiprocessor communication mode.
		 */
		void
		set_mpcm_enabled (bool enabled) const;

		/**
		 * Set communication mode.
		 */
		void
		set (Mode) const;

		/**
		 * Set parity generation and checking mode.
		 * Unused in MasterSPI mode.
		 */
		void
		set (Parity) const;

		/**
		 * Set number of data bits in the word.
		 *
		 * Accepted values are 5, 6, 7, 8 and 9.
		 * If 9 bits word is used, write9() instead of write() must be used.
		 */
		void
		set_data_bits (uint8_t) const;

		/**
		 * Set number of data bits in the word.
		 *
		 * Performs compile-time check if the number is valid.
		 */
		template<uint8_t DataBits>
			void
			set_data_bits() const;

		/**
		 * Set number of stop bits inserted by the transmitter.
		 *
		 * Valid values are 1 or 2.
		 * Unused in MasterSPI mode.
		 */
		void
		set_stop_bits (uint8_t) const;

		/**
		 * Set number of stop bits inserted by the transmitter.
		 *
		 * Performs compile-time check if the number is valid.
		 */
		template<uint8_t StopBits>
			void
			set_stop_bits() const;

		// TODO CTRLC.UDORD
		// TODO CTRLC.UCPHA

		/**
		 * Set baud rate (in bits per second) for given mode.
		 */
		template<Mode pMode, Speed pSpeed = Speed::_1x>
			void
			set_baud_rate (uint32_t baud_rate, uint32_t peripheral_frequency) const;

		/**
		 * Set period directly in fractional baud rate generator.
		 */
		void
		set_baud_rate_period (uint16_t period) const;

		/**
		 * Set scale directly in fractional baud rate generator.
		 * This makes sense only in asynchronous mode.
		 */
		void
		set_baud_rate_scale (int8_t scale) const;

	  private:
		Register8	_data, _status;
		Register8	_ctrla, _ctrlb, _ctrlc;
		Register8	_baudctrla, _baudctrlb;
	};


template<class M>
	constexpr
	BasicUSART<M>::BasicUSART (Register8 data, Register8 status,
							   Register8 ctrla, Register8 ctrlb, Register8 ctrlc,
							   Register8 baudctrla, Register8 baudctrlb):
		_data (data), _status (status),
		_ctrla (ctrla), _ctrlb (ctrlb), _ctrlc (ctrlc),
		_baudctrla (baudctrla), _baudctrlb (baudctrlb)
	{ }


template<class M>
	constexpr bool
	BasicUSART<M>::operator== (BasicUSART const& other) const
	{
#define BASIC_USART_EQ(n) (&_##n == &other._##n)
		return BASIC_USART_EQ (data) && BASIC_USART_EQ (status)
			&& BASIC_USART_EQ (ctrla) && BASIC_USART_EQ (ctrlb) && BASIC_USART_EQ (ctrlc)
			&& BASIC_USART_EQ (baudctrla) && BASIC_USART_EQ (baudctrlb);
#undef BASIC_USART_EQ
	}


template<class M>
	constexpr bool
	BasicUSART<M>::operator!= (BasicUSART const& other) const
	{
		return !(*this == other);
	}


template<class M>
	inline void
	BasicUSART<M>::wait_for_write() const
	{
		// Wait for DREIF flag to be set:
		while (!(_status.read() & static_cast<uint8_t> (StatusFlags::DataRegisterEmpty)))
			continue;
	}


template<class M>
	inline void
	BasicUSART<M>::write (uint8_t data) const
	{
		_data = data;
	}


template<class M>
	inline void
	BasicUSART<M>::write_blocking (uint8_t data) const
	{
		wait_for_write();
		write (data);
	}


template<class M>
	inline void
	BasicUSART<M>::write9 (uint16_t data) const
	{
		_ctrlb = (_ctrlb.read() & 0b00011110) | ((data >> 8) & 1);
		write (data & 0xff);
	}


template<class M>
	inline void
	BasicUSART<M>::write9_blocking (uint16_t data) const
	{
		wait_for_write();
		write9 (data);
	}


template<class M>
	inline void
	BasicUSART<M>::wait_for_read() const
	{
		// Wait for RXCIF (receive complete interrupt flag):
		while (!(_status.read() & static_cast<uint8_t> (StatusFlags::RxComplete)))
			continue;
	}


template<class M>
	inline uint8_t
	BasicUSART<M>::read() const
	{
		return _data.read();
	}


template<class M>
	inline uint8_t
	BasicUSART<M>::read_blocking() const
	{
		wait_for_read();
		return read();
	}


template<class M>
	inline uint16_t
	BasicUSART<M>::read9() const
	{
		// TODO
		return 0;
	}


template<class M>
	inline uint16_t
	BasicUSART<M>::read9_blocking() const
	{
		wait_for_read();
		return read9();
	}


template<class M>
	inline bool
	BasicUSART<M>::is_frame_error() const
	{
		return _status.read() & static_cast<uint8_t> (StatusFlags::FrameError);
	}


template<class M>
	inline bool
	BasicUSART<M>::is_rx_overflow() const
	{
		return _status.read() & static_cast<uint8_t> (StatusFlags::BufferOverflow);
	}


template<class M>
	inline bool
	BasicUSART<M>::is_rx_parity_error() const
	{
		return _status.read() & static_cast<uint8_t> (StatusFlags::ParityError);
	}


template<class M>
	inline void
	BasicUSART<M>::set_rx_enabled (bool enabled) const
	{
		_ctrlb = (_ctrlb.read() & 0b00001111) | ((enabled ? 1 : 0) << 4);
	}


template<class M>
	inline void
	BasicUSART<M>::set_tx_enabled (bool enabled) const
	{
		_ctrlb = (_ctrlb.read() & 0b00010111) | ((enabled ? 1 : 0) << 3);
	}


template<class M>
	inline void
	BasicUSART<M>::set_rx_complete (InterruptSystem::Level level) const
	{
		_ctrla = (_ctrla.read() & 0b00001111) | (static_cast<uint8_t> (level) << 4);
	}


template<class M>
	inline void
	BasicUSART<M>::set_tx_complete (InterruptSystem::Level level) const
	{
		_ctrla = (_ctrla.read() & 0b00110011) | (static_cast<uint8_t> (level) << 2);
	}


template<class M>
	inline void
	BasicUSART<M>::set_data_register_empty (InterruptSystem::Level level) const
	{
		_ctrla = (_ctrla.read() & 0b00111100) | (static_cast<uint8_t> (level) << 0);
	}


template<class M>
	inline void
	BasicUSART<M>::set (Speed speed) const
	{
		_ctrlb = (_ctrlb.read() & 0b00011011) | static_cast<uint8_t> (speed);
	}


template<class M>
	inline void
	BasicUSART<M>::set_mpcm_enabled (bool enabled) const
	{
		_ctrlb = (_ctrlb.read() & 0b00011101) | ((enabled ? 1 : 0) << 1);
	}


template<class M>
	inline void
	BasicUSART<M>::set (Mode mode) const
	{
		_ctrlc = (_ctrlc.read() & 0b00111111) | static_cast<uint8_t> (mode);
	}


template<class M>
	inline void
	BasicUSART<M>::set (Parity parity) const
	{
		_ctrlc = (_ctrlc.read() & 0b11001111) | static_cast<uint8_t> (parity);
	}


template<class M>
	inline void
	BasicUSART<M>::set_data_bits (uint8_t data_bits) const
	{
		switch (data_bits)
		{
			case 5:
				set_data_bits<5>();
				break;

			case 6:
				set_data_bits<6>();
				break;

			case 7:
				set_data_bits<7>();
				break;

			case 8:
				set_data_bits<8>();
				break;

			case 9:
				set_data_bits<9>();
				break;
		}
	}


template<class M>
	template<uint8_t pDataBits>
		inline void
		BasicUSART<M>::set_data_bits() const
		{
			static_assert (pDataBits >= 5 && pDataBits <= 9);

			switch (pDataBits)
			{
				case 5:
					_ctrlc = (_ctrlc.read() & 0b11111000) | static_cast<uint8_t> (DataBits::_5);
					break;

				case 6:
					_ctrlc = (_ctrlc.read() & 0b11111000) | static_cast<uint8_t> (DataBits::_6);
					break;

				case 7:
					_ctrlc = (_ctrlc.read() & 0b11111000) | static_cast<uint8_t> (DataBits::_7);
					break;

				case 8:
					_ctrlc = (_ctrlc.read() & 0b11111000) | static_cast<uint8_t> (DataBits::_8);
					break;

				case 9:
					_ctrlc = (_ctrlc.read() & 0b11111000) | static_cast<uint8_t> (DataBits::_9);
					break;
			}
		}


template<class M>
	inline void
	BasicUSART<M>::set_stop_bits (uint8_t stop_bits) const
	{
		switch (stop_bits)
		{
			case 1:
				set_stop_bits<1>();
				break;

			case 2:
				set_stop_bits<2>();
				break;
		}
	}


template<class M>
	template<uint8_t pStopBits>
		inline void
		BasicUSART<M>::set_stop_bits() const
		{
			static_assert (pStopBits >= 1 && pStopBits <= 2);

			switch (pStopBits)
			{
				case 1:
					_ctrlc = (_ctrlc.read() & 0b11110111) | static_cast<uint8_t> (StopBits::_1);
					break;

				case 2:
					_ctrlc = (_ctrlc.read() & 0b11110111) | static_cast<uint8_t> (StopBits::_2);
					break;
			}
		}


template<class M>
	template<typename BasicUSART<M>::Mode pMode, typename BasicUSART<M>::Speed pSpeed>
		inline void
		BasicUSART<M>::set_baud_rate (uint32_t baud_rate, uint32_t peripheral_frequency) const
		{
			static_assert (pMode == Mode::Synchronous || pMode == Mode::Asynchronous || pMode == Mode::MasterSPI);
			static_assert (pMode == Mode::Asynchronous || pSpeed == Speed::_1x, "Speed must be _1x in Synchronous and MasterSPI modes");

			auto pow2 = [](uint8_t power) {
				return 1 << power;
			};

			switch (pMode)
			{
				case Mode::Synchronous:
					set_baud_rate_period (peripheral_frequency / (2 * baud_rate) - 1);
					break;

				case Mode::Asynchronous:
				{
					constexpr int8_t scale = 0;
					uint32_t x = 0;

					switch (pSpeed)
					{
						case Speed::_1x:	x = 16;	break;
						case Speed::_2x:	x = 8;	break;
					}

					if (scale >= 0)
						set_baud_rate_period (1.0f * peripheral_frequency / (pow2 (scale) * x * baud_rate) - 1);
					else
						set_baud_rate_period (1.0f * pow2 (-scale) * (peripheral_frequency / (x * baud_rate) - 1));

					set_baud_rate_scale (scale);
					break;
				}
			}
		}


template<class M>
	inline void
	BasicUSART<M>::set_baud_rate_period (uint16_t period) const
	{
		_baudctrla = period & 0x0ff;
		_baudctrlb = (_baudctrlb.read() & 0b11110000) | ((period & 0xf00) >> 8);
	}


template<class M>
	inline void
	BasicUSART<M>::set_baud_rate_scale (int8_t scale) const
	{
		_baudctrlb = (_baudctrlb.read() & 0b00001111) | (scale << 4);
	}

} // namespace xmega_au
} // namespace avr
} // namespace mulabs

#endif

