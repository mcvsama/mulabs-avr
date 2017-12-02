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

#ifndef MULABS_AVR__DEVICES__XMEGA_AU__BASIC_USB_SIE_ENDPOINT_H__INCLUDED
#define MULABS_AVR__DEVICES__XMEGA_AU__BASIC_USB_SIE_ENDPOINT_H__INCLUDED

// Mulabs:
#include <mulabs_avr/devices/xmega_au/interrupt_system.h>
#include <mulabs_avr/utility/atomic.h>


namespace mulabs {
namespace avr {
namespace xmega_au {

template<class pMCU>
	class alignas(2) BasicUSBSIEEndpoint
	{
	  public:
		using MCU			= pMCU;
		using Register8		= typename MCU::Register8;
		using Register16	= typename MCU::Register16;

		static constexpr size_t	kEndpointSize	= 8;

		enum class Type: uint8_t
		{
			Disabled		= 0b00,
			Control			= 0b01,
			Bulk			= 0b10,
			Isochronous		= 0b11,
		};

		enum class ControlBulkBufferSize: uint8_t
		{
			_8				= 0b00 << 0,
			_16				= 0b01 << 0,
			_32				= 0b10 << 0,
			_64				= 0b11 << 0,
		};

		enum class IsochronousBufferSize: uint8_t
		{
			_8				= 0b000 << 0,
			_16				= 0b001 << 0,
			_32				= 0b010 << 0,
			_64				= 0b011 << 0,
			_128			= 0b100 << 0,
			_256			= 0b101 << 0,
			_512			= 0b110 << 0,
			_1023			= 0b111 << 0,
		};

		enum class Buffer
		{
			_0,	// No ping-pong is used. This endpoint is buffer 0.
			_1,	// Ping-pong is used and the next endpoint is the buffer 1.
		};

		enum class Data
		{
			_0,	// DATA0 packet
			_1,	// DATA1 packet
		};

	  protected:
		static constexpr uint8_t kStall						= bit<7>;
		static constexpr uint8_t kCRCError					= bit<7>;
		static constexpr uint8_t kUnderflowOverflow			= bit<6>;
		static constexpr uint8_t kTransactionComplete		= bit<5>;
		static constexpr uint8_t kSetupTransactionComplete	= bit<4>;
		static constexpr uint8_t kTransactionComplete1		= bit<4>;
		static constexpr uint8_t kBank						= bit<3>;
		static constexpr uint8_t kBusNack1					= bit<2>;
		static constexpr uint8_t kBusNack0					= bit<1>;
		static constexpr uint8_t kData						= bit<0>;

	  protected:
		// Ctor
		explicit constexpr
		BasicUSBSIEEndpoint (uint8_t* base_address);

	  public:
		// Equality operator
		constexpr bool
		operator== (BasicUSBSIEEndpoint const& other) const;

		// Inequality operator
		constexpr bool
		operator!= (BasicUSBSIEEndpoint const& other) const;

		/**
		 * Initialize endpoint in-memory.
		 */
		void
		initialize();

		/**
		 * Return endpoint type.
		 */
		Type
		type() const;

		/**
		 * Set endpoint type.
		 */
		void
		set (Type);

		/**
		 * Return true if multipacket transfers are enabled.
		 */
		bool
		multipacket_enabled() const;

		/**
		 * Enable/disable multipacket transfers.
		 */
		void
		set_multipacket_enabled (bool);

		/**
		 * Return true if ping-pong operation is enabled.
		 */
		bool
		ping_pong_enabled() const;

		/**
		 * Enable/disable ping-pong operation.
		 * If enabled, the other endpoint needs to be disabled. Ping-pong is not possible for control endpoints.
		 */
		void
		set_ping_pong_enabled (bool);

		/**
		 * Return true if interrupts for this endpoint are blocked.
		 */
		bool
		interrupts_blocked() const;

		/**
		 * Block/unblock all interrupts for this endpoint.
		 * If blocked, only the interrupt flags get updated, but no interrupt handler is called.
		 */
		void
		set_interrupts_blocked (bool);

		/**
		 * Enable/disable STALL of the endpoint.
		 * This can be used only on endpoints configured to Control or Bulk type.
		 */
		void
		set_stalled (bool);

		/**
		 * Set pointer to the data buffer.
		 */
		void
		set_buffer (uint8_t* buffer);

		/**
		 * Set data buffer size for Control and Bulk endpoints.
		 */
		void
		set (ControlBulkBufferSize);

		/**
		 * Set data buffer size for isochronous endpoints.
		 */
		void
		set (IsochronousBufferSize);

		/**
		 * Set buffer and size in one call.
		 */
		void
		set_buffer (uint8_t* buffer, ControlBulkBufferSize);

		/**
		 * Set buffer and size in one call.
		 */
		void
		set_buffer (uint8_t* buffer, IsochronousBufferSize);

		/**
		 * Return true if endpoint was responded with a stall handshake to a transaction.
		 *
		 * Not applicable for isochronous endpoints.
		 */
		bool
		is_stalled() const;

		/**
		 * Reset the stall-flag.
		 *
		 * Not applicable for isochronous endpoints.
		 */
		void
		reset_stall();

		/**
		 * Return true if there was a CRC error in an incoming data packet.
		 *
		 * Applies only to isochronous output endpoints.
		 */
		bool
		is_crc_error() const;

		/**
		 * Reset the CRC error flag.
		 *
		 * Applies only to isochronous output endpoints.
		 */
		void
		reset_crc_error();

		/**
		 * Return true if IN or OUT transaction completed successfully.
		 */
		bool
		transaction_complete() const;

		/**
		 * Reset the in/out-transaction-complete flag.
		 */
		void
		reset_transaction_complete();

		/**
		 * Return true if SETUP, IN or OUT transaction completed successfully.
		 */
		bool
		setup_transaction_complete() const;

		/**
		 * Reset the setup/in/out-transaction-complete flag.
		 */
		void
		reset_setup_transaction_complete();

		/**
		 * Wait for transaction-complete flag (in a loop).
		 */
		void
		wait_for_transaction_complete() const;

		// TODO STATUS<3> BANK

		/**
		 * Return true if endpoint is NACK-ing all transactions.
		 * Buffer selects buffer if ping-pong operation is enabled. Otherwise use Buffer::_0.
		 */
		bool
		is_nack_all (Buffer) const;

		/**
		 * If nack is true, the endpoint will NACK all transactions from the host.
		 * Buffer selects buffer if ping-pong operation is enabled. Otherwise use Buffer::_0.
		 */
		void
		set_nack_all (Buffer, bool nack);

		/**
		 * Return what next packet is supposed to be (DATA0 or DATA1).
		 */
		Data
		next_data_packet() const;

		/**
		 * Set next packet to be sent as DATA0/DATA1.
		 */
		void
		set_next_data_packet (Data);

		/**
		 * Clear transaction_complete flags (both normal and setup), clear nack_all flag and clear the overflow or overflow condition.
		 */
		void
		set_ready();

	  protected:
		size_t const		_base_address;
		// There registers are not memory-mapped device registers, but regular SRAM cells,
		// so atomic_* instructions are allowed on them:
		Register8 const		_status;
		Register8 const		_ctrl;
		Register16 const	_cnt;
		Register8 const		_cnth;
		Register16 const	_dataptr;
		Register16 const	_auxdata;
	};


template<class pMCU>
	class alignas(2) BasicUSBSIEOutputEndpoint: public BasicUSBSIEEndpoint<pMCU>
	{
	  public:
		using Endpoint = BasicUSBSIEEndpoint<pMCU>;

	  public:
		// Ctor
		explicit constexpr
		BasicUSBSIEOutputEndpoint (uint8_t* base_address);

		/**
		 * Get number of bytes received in the last OUT or SETUP transaction.
		 */
		size_t
		transaction_size() const;

		/**
		 * Set total data size for a complete transfer. Must be multiple of the maximum packet size
		 * except for isochronous 1023-byte endpoints.
		 */
		void
		set_total_transfer_size (size_t);

		/**
		 * Return true if the endpoint is not yet ready to receive new data from host after an OUT packet.
		 */
		bool
		is_overflow() const;

		/**
		 * Reset overflow condition.
		 */
		void
		reset_overflow();
	};


template<class pMCU>
	class alignas(2) BasicUSBSIEInputEndpoint: public BasicUSBSIEEndpoint<pMCU>
	{
	  public:
		using Endpoint = BasicUSBSIEEndpoint<pMCU>;

	  public:
		// Ctor
		explicit constexpr
		BasicUSBSIEInputEndpoint (uint8_t* base_address);

		/**
		 * Set number of bytes to be sent in the next IN transaction.
		 */
		void
		set_transaction_size (size_t);

		/**
		 * Set automatic Zero Length Packet to be managed by hardware.
		 */
		void
		set_azlp_enabled (bool enabled);

		/**
		 * Get total number of bytes sent.
		 * Used in multipacket transfers. Should be reset with reset_total_transfer_size() when setting up a new
		 * transfer.
		 */
		size_t
		total_transfer_size() const;

		/**
		 * Reset the total transfer size to 0.
		 */
		void
		reset_total_transfer_size();

		/**
		 * Return true if the endpoint is not yet ready to send data to host after sending an IN packet.
		 */
		bool
		is_underflow() const;

		/**
		 * Reset underflow condition.
		 */
		void
		reset_underflow();

		/**
		 * Call set_transaction_size (bytes) and set_ready().
		 */
		void
		set_ready (size_t bytes);
	};


template<class M>
	inline constexpr
	BasicUSBSIEEndpoint<M>::BasicUSBSIEEndpoint (uint8_t* base_address):
		_base_address (reinterpret_cast<size_t> (base_address)),
		_status (_base_address + 0x00),
		_ctrl (_base_address + 0x01),
		_cnt (_base_address + 0x02),
		_cnth (_base_address + 0x03),
		_dataptr (_base_address + 0x04),
		_auxdata (_base_address + 0x06)
	{ }


template<class M>
	constexpr bool
	BasicUSBSIEEndpoint<M>::operator== (BasicUSBSIEEndpoint const& other) const
	{
		return _base_address == other._base_address;
	}


template<class M>
	constexpr bool
	BasicUSBSIEEndpoint<M>::operator!= (BasicUSBSIEEndpoint const& other) const
	{
		return !(*this == other);
	}


template<class M>
	inline void
	BasicUSBSIEEndpoint<M>::initialize()
	{
		_status.write (0);
		_ctrl.write (0);
		_cnt.write (0);
		_dataptr.write (0);
		_auxdata.write (0);
	}


template<class M>
	inline auto
	BasicUSBSIEEndpoint<M>::type() const -> Type
	{
		return static_cast<Type> (_ctrl.template get_bits_value<7, 6>());
	}


template<class M>
	inline void
	BasicUSBSIEEndpoint<M>::set (Type type)
	{
		_ctrl.template set_bits_value<7, 6> (static_cast<uint8_t> (type));
	}


template<class M>
	inline bool
	BasicUSBSIEEndpoint<M>::multipacket_enabled() const
	{
		return _ctrl.template get_bit<5>();
	}


template<class M>
	inline void
	BasicUSBSIEEndpoint<M>::set_multipacket_enabled (bool enabled)
	{
		_ctrl.template set_bit_value<5> (enabled);
	}


template<class M>
	inline bool
	BasicUSBSIEEndpoint<M>::ping_pong_enabled() const
	{
		return _ctrl.template get_bit<4>();
	}


template<class M>
	inline void
	BasicUSBSIEEndpoint<M>::set_ping_pong_enabled (bool enabled)
	{
		_ctrl.template set_bit_value<4> (enabled);
	}


template<class M>
	inline bool
	BasicUSBSIEEndpoint<M>::interrupts_blocked() const
	{
		return _ctrl.template get_bit<3>();
	}


template<class M>
	inline void
	BasicUSBSIEEndpoint<M>::set_interrupts_blocked (bool blocked)
	{
		_ctrl.template set_bit_value<3> (blocked);
	}


template<class M>
	inline void
	BasicUSBSIEEndpoint<M>::set_stalled (bool enabled)
	{
		_ctrl.template set_bit_value<2> (enabled);
	}


template<class M>
	inline void
	BasicUSBSIEEndpoint<M>::set_buffer (uint8_t* buffer)
	{
		_dataptr.write (reinterpret_cast<size_t> (buffer));
	}


template<class M>
	inline void
	BasicUSBSIEEndpoint<M>::set (ControlBulkBufferSize buffer_size)
	{
		_ctrl.template set_bits_value<1, 0> (static_cast<uint8_t> (buffer_size));
	}


template<class M>
	inline void
	BasicUSBSIEEndpoint<M>::set (IsochronousBufferSize buffer_size)
	{
		_ctrl.template set_bits_value<2, 0> (static_cast<uint8_t> (buffer_size));
	}


template<class M>
	inline void
	BasicUSBSIEEndpoint<M>::set_buffer (uint8_t* buffer, ControlBulkBufferSize buffer_size)
	{
		set_buffer (buffer);
		set (buffer_size);
	}


template<class M>
	inline void
	BasicUSBSIEEndpoint<M>::set_buffer (uint8_t* buffer, IsochronousBufferSize buffer_size)
	{
		set_buffer (buffer);
		set (buffer_size);
	}


template<class M>
	inline bool
	BasicUSBSIEEndpoint<M>::is_stalled() const
	{
		return this->_status.template get_bit<7>();
	}


template<class M>
	inline void
	BasicUSBSIEEndpoint<M>::reset_stall()
	{
		atomic_sram_set_bit<7> (this->_status.ref());
	}


template<class M>
	inline bool
	BasicUSBSIEEndpoint<M>::is_crc_error() const
	{
		return this->_status.template get_bit<7>();
	}


template<class M>
	inline void
	BasicUSBSIEEndpoint<M>::reset_crc_error()
	{
		atomic_sram_set_bit<7> (this->_status.ref());
	}


template<class M>
	inline bool
	BasicUSBSIEEndpoint<M>::transaction_complete() const
	{
		return this->_status.template get_bit<5>();
	}


template<class M>
	inline void
	BasicUSBSIEEndpoint<M>::reset_transaction_complete()
	{
		atomic_sram_clear_bit<5> (this->_status.ref());
	}


template<class M>
	inline bool
	BasicUSBSIEEndpoint<M>::setup_transaction_complete() const
	{
		return this->_status.template get_bit<4>();
	}


template<class M>
	inline void
	BasicUSBSIEEndpoint<M>::reset_setup_transaction_complete()
	{
		atomic_sram_clear_bit<4> (this->_status.ref());
	}


template<class M>
	inline void
	BasicUSBSIEEndpoint<M>::wait_for_transaction_complete() const
	{
		while (!transaction_complete())
			continue;
	}


template<class M>
	inline bool
	BasicUSBSIEEndpoint<M>::is_nack_all (Buffer buffer) const
	{
		switch (buffer)
		{
			case Buffer::_0:
				return this->_status.template get_bit<1>();

			case Buffer::_1:
				return this->_status.template get_bit<2>();

			default:
				return false;
		}
	}


template<class M>
	inline void
	BasicUSBSIEEndpoint<M>::set_nack_all (Buffer buffer, bool nack)
	{
		switch (buffer)
		{
			case Buffer::_0:
				atomic_sram_set_bit_value<1> (_status.ref(), nack);
				break;

			case Buffer::_1:
				atomic_sram_set_bit_value<2> (_status.ref(), nack);
				break;
		}
	}


template<class M>
	inline auto
	BasicUSBSIEEndpoint<M>::next_data_packet() const -> Data
	{
		return this->_status.template get_bit<0>() ? Data::_1 : Data::_0;
	}


template<class M>
	inline void
	BasicUSBSIEEndpoint<M>::set_next_data_packet (Data data)
	{
		switch (data)
		{
			case Data::_0:
				atomic_sram_clear_bit<0> (_status.ref());
				break;

			case Data::_1:
				atomic_sram_set_bit<0> (_status.ref());
				break;
		}
	}


template<class M>
	inline void
	BasicUSBSIEEndpoint<M>::set_ready()
	{
		atomic_sram_clear_bitmask<kUnderflowOverflow | kTransactionComplete | kTransactionComplete1 | kBusNack1 | kBusNack0> (this->_status.ref());
	}


template<class M>
	constexpr
	BasicUSBSIEOutputEndpoint<M>::BasicUSBSIEOutputEndpoint (uint8_t* base_address):
		BasicUSBSIEEndpoint<M> (base_address)
	{ }


template<class M>
	inline size_t
	BasicUSBSIEOutputEndpoint<M>::transaction_size() const
	{
		return this->_cnt.read();
	}


template<class M>
	inline void
	BasicUSBSIEOutputEndpoint<M>::set_total_transfer_size (size_t bytes)
	{
		this->_auxdata = bytes;
	}


template<class M>
	inline bool
	BasicUSBSIEOutputEndpoint<M>::is_overflow() const
	{
		return this->_status.template get_bit<6>();
	}


template<class M>
	inline void
	BasicUSBSIEOutputEndpoint<M>::reset_overflow()
	{
		atomic_sram_clear_bit<6> (this->_status.ref());
	}


template<class M>
	constexpr
	BasicUSBSIEInputEndpoint<M>::BasicUSBSIEInputEndpoint (uint8_t* base_address):
		BasicUSBSIEEndpoint<M> (base_address)
	{ }


template<class M>
	inline void
	BasicUSBSIEInputEndpoint<M>::set_transaction_size (size_t bytes)
	{
		this->_cnt.write ((this->_cnt.read() & 0b10000000'00000000) | (bytes & 0b00000011'11111111));
	}


template<class M>
	inline void
	BasicUSBSIEInputEndpoint<M>::set_azlp_enabled (bool enabled)
	{
		this->_cnth.template set_bit_value<7> (enabled);
	}


template<class M>
	inline size_t
	BasicUSBSIEInputEndpoint<M>::total_transfer_size() const
	{
		return this->_auxdata.read();
	}


template<class M>
	inline void
	BasicUSBSIEInputEndpoint<M>::reset_total_transfer_size()
	{
		this->_auxdata = 0;
	}


template<class M>
	inline bool
	BasicUSBSIEInputEndpoint<M>::is_underflow() const
	{
		return this->_status.template get_bit<6>();
	}


template<class M>
	inline void
	BasicUSBSIEInputEndpoint<M>::reset_underflow()
	{
		atomic_sram_clear_bit<6> (this->_status.ref());
	}


template<class M>
	inline void
	BasicUSBSIEInputEndpoint<M>::set_ready (size_t bytes)
	{
		set_transaction_size (bytes);
		Endpoint::set_ready();
	}

} // namespace xmega_au
} // namespace avr
} // namespace mulabs

#endif

