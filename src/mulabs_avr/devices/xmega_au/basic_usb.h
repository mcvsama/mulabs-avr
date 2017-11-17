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

#ifndef MULABS_AVR__DEVICES__XMEGA_AU__BASIC_USB_H__INCLUDED
#define MULABS_AVR__DEVICES__XMEGA_AU__BASIC_USB_H__INCLUDED

// Mulabs:
#include <mulabs_avr/devices/xmega_au/interrupt_system.h>
#include <mulabs_avr/support/protocols/usb/device_definition.h>

// Local:
#include "basic_usb_endpoint.h"


namespace mulabs {
namespace avr {
namespace xmega_au {

template<class pMCU>
	class BasicUSB
	{
	  public:
		using MCU				= pMCU;
		using Register8			= typename MCU::Register8;
		using Endpoint			= BasicUSBEndpoint<MCU>;
		using InputEndpoint		= BasicUSBInputEndpoint<MCU>;
		using OutputEndpoint	= BasicUSBOutputEndpoint<MCU>;

		enum class Speed
		{
			Low,
			Full,
		};

		enum class Interrupt: uint16_t
		{
			// INTCTRLA:
			StartOfFrame				= 1u << (0 + 7),	// InterruptFlag::StartOfFrame
			BusEvent					= 1u << (0 + 6),	// InterruptFlag::Suspend, Resume, Reset
			BusError					= 1u << (0 + 5),	// InterruptFlag::IsochronousCRCError, Underflow, Overflow
			Stall						= 1u << (0 + 4),	// InterruptFlag::Stall
			// INTCTRLB:
			TransactionComplete			= 1u << (8 + 1),	// InterruptFlag::TransactionComplete
			SetupTransactionComplete	= 1u << (8 + 0),	// InterruptFlag::SetupTransactionComplete
		};

		enum class InterruptFlag: uint16_t
		{
			// INTFLAGSA
			StartOfFrame				= 1u << (0 + 7),	// Interrupt::StartOfFrame
			Suspend						= 1u << (0 + 6),	// Interrupt::BusEvent
			Resume						= 1u << (0 + 5),	// Interrupt::BusEvent
			Reset						= 1u << (0 + 4),	// Interrupt::BusEvent
			IsochronousCRCError			= 1u << (0 + 3),	// Interrupt::BusError
			Underflow					= 1u << (0 + 2),	// Interrupt::BusError
			Overflow					= 1u << (0 + 1),	// Interrupt::BusError
			Stall						= 1u << (0 + 0),	// Interrupt::Stall
			// INTFLAGSB
			TransactionComplete			= 1u << (8 + 1),	// Interrupt::TransactionComplete
			SetupTransactionComplete	= 1u << (8 + 0),	// Interrupt::SetupTransactionComplete
		};

		/**
		 * Helper buffer class, aligned as required by the MCU.
		 *
		 * \param	EndpointsNumber
		 *			number of endpoints pairs (OUT and IN) in the table.
		 */
		template<uint8_t EndpointsNumber>
			struct alignas(16) EndpointsTable
			{
			  public:
				// Thrown when endpoint number defined by this table doesn't satisfy requirements given by
				// the usb::Device definition.
				enum WrongEndpointsNumber
				{ };

			  public:
				static constexpr size_t kSingleDirectionEndpointSize = 8;

			  public:
				// Ctor
				explicit constexpr
				EndpointsTable();

				// Ctor
				explicit constexpr
				EndpointsTable (usb::Device const&);

				/**
				 * Return number of signle-direction endpoints held by this buffer.
				 */
				constexpr uint8_t
				single_direction_number() const;

				/**
				 * Return number of endpoints (pairs of IN and OUT descriptors) held by this buffer.
				 */
				constexpr uint8_t
				number() const;

				/**
				 * Return the maximum endpoint index held by this buffer.
				 */
				constexpr uint8_t
				maximum_endpoint_id() const;

				/**
				 * Return pointer to the endpoints table (be used for EPPTR register).
				 */
				constexpr uint8_t*
				table();

				/**
				 * Return pointer to Nth endpoint.
				 */
				constexpr uint8_t*
				operator[] (uint8_t n);

				/**
				 * Return pointer to the output of the Nth pair.
				 */
				constexpr uint8_t*
				nth_output (uint8_t n);

				/**
				 * Return pointer to the input of the Nth pair.
				 */
				constexpr uint8_t*
				nth_input (uint8_t n);

				/**
				 * Return value of the framenum register.
				 */
				uint16_t
				framenum() const;

			  private:
				uint8_t		_buffer[2 * EndpointsNumber * kSingleDirectionEndpointSize];
				uint16_t	_framenum;
			};

	  public:
		// Ctor
		explicit constexpr
		BasicUSB (size_t base_address);

		// Equality operator
		constexpr bool
		operator== (BasicUSB const& other) const;

		// Inequality operator
		constexpr bool
		operator!= (BasicUSB const& other) const;

		/**
		 * Reads calibration registers from the production signature row and setups USB calibration registers.
		 */
		void
		calibrate() const;

		/**
		 * Enable/disable USB module.
		 */
		void
		set_enabled (bool) const;

		/**
		 * Select USB speed.
		 */
		void
		set (Speed) const;

		/**
		 * Enable/disable FIFO.
		 */
		void
		set_fifo_enabled (bool) const;

		/**
		 * Enable Store-Frame-Number.
		 */
		void
		set_store_framenum_enabled (bool) const;

		/**
		 * Enable/disable pull-up-during-reset of USB lines.
		 */
		void
		set_pullrst_enabled (bool) const;

		/**
		 * Enable/disable remote-wake-up - sending upstream resume on the USB lines if the bus is in the suspend state
		 * for at least 5 ms.
		 */
		void
		set_rwakeup_enabled (bool) const;

		/**
		 * Enable global NACK - sending NACK to all incoming transactions.
		 */
		void
		set_gnack_enabled (bool) const;

		/**
		 * Set maximum number of endpoints.
		 * Valid values are 1…16.
		 */
		template<uint8_t EndpointsNumber>
			void
			set_endpoints_number() const;

		/**
		 * Set endpoint table pointer and also calls set_endpoints_number().
		 */
		template<uint8_t EndpointsNumber>
			void
			set_endpoints_table (uint8_t*) const;

		/**
		 * Set endpoint table pointer to existing EndpointsTable object.
		 */
		template<uint8_t EndpointsNumber>
			void
			set_endpoints_table (EndpointsTable<EndpointsNumber>&) const;

		/**
		 * Set the USB address the device will respond to.
		 * Valid values are 0…127.
		 */
		void
		set_address (uint8_t) const;

		/**
		 * Attach/detach the USB device from the bus.
		 */
		void
		set_attached (bool) const;

		/**
		 * Return true if upstream resume was sent.
		 */
		bool
		is_upstream_resume() const;

		/**
		 * Return true if downstream resume was sent.
		 */
		bool
		is_downstream_resume() const;

		/**
		 * Return true if the bus is in suspended state.
		 */
		bool
		is_suspended() const;

		/**
		 * Return true if bus-reset condition was detected.
		 */
		bool
		is_bus_reset() const;

		/**
		 * Enable given interrupt types.
		 */
		template<class ...Interrupts>
			void
			enable (Interrupts ...interrupts) const;

		/**
		 * Disable given interrupt types.
		 */
		template<class ...Interrupts>
			void
			disable (Interrupts ...interrupts) const;

		/**
		 * Trigger given interrupt types.
		 */
		template<class ...InterruptFlags>
			void
			trigger (InterruptFlags ...interrupt_flags) const;

		/**
		 * Clear given interrupt flags.
		 */
		template<class ...InterruptFlags>
			void
			clear (InterruptFlags ...interrupt_flags) const;

		/**
		 * Clear all bus-event interrupt flags.
		 */
		void
		clear_all_bus_event_interrupt_flags() const;

		/**
		 * Clear all transaction-complete interrupt flags.
		 */
		void
		clear_all_transaction_complete_interrupt_flags() const;

		/**
		 * Return true if given interrupt was triggered.
		 */
		bool
		triggered (InterruptFlag interrupt_flag) const;

		/**
		 * Set interrupts level.
		 */
		void
		set (InterruptSystem::Level) const;

		/**
		 * Call the callback if given interrupt was triggered, then clear
		 * the interrupt flag.
		 */
		template<class Function>
			void
			on (InterruptFlag flag, Function) const;

		// TODO FIFOWP
		// TODO FIFORP

	  private:
		/**
		 * Helper for enable()/disable() of Interrupts.
		 */
		template<class ...Interrupts>
			static constexpr uint16_t
			make_interrupts_list (Interrupt interrupt, Interrupts ...interrupts)
			{
				return make_interrupts_list (interrupt) | make_interrupts_list (interrupts...);
			}

		/**
		 * Recursive stop-condition for make_interrupts_list().
		 */
		static constexpr uint16_t
		make_interrupts_list (Interrupt interrupt)
		{
			return static_cast<uint16_t> (interrupt);
		}

		/**
		 * Helper for enable()/disable() of Interrupts.
		 */
		template<class ...InterruptFlags>
			static constexpr uint16_t
			make_interrupt_flags_list (InterruptFlag interrupt_flag, InterruptFlags ...interrupt_flags)
			{
				return make_interrupt_flags_list (interrupt_flag) | make_interrupt_flags_list (interrupt_flags...);
			}

		/**
		 * Recursive stop-condition for make_interrupts_list().
		 */
		static constexpr uint16_t
		make_interrupt_flags_list (InterruptFlag interrupt_flag)
		{
			return static_cast<uint16_t> (interrupt_flag);
		}

	  private:
		size_t const	_base_address;
		Register8 const	_ctrla, _ctrlb;
		Register8 const	_status;
		Register8 const	_addr;
		Register8 const	_fifowp, _fiforp;
		Register8 const	_epptrl, _epptrh;
		Register8 const	_intctrla, _intctrlb;
		Register8 const	_intflagsaclr, _intflagsaset;
		Register8 const	_intflagsbclr, _intflagsbset;
		Register8 const	_cal0, _cal1;
	};


template<class M>
	template<uint8_t EndpointsNumber>
		constexpr
		BasicUSB<M>::EndpointsTable<EndpointsNumber>::EndpointsTable()
		{ }


template<class M>
	template<uint8_t EndpointsNumber>
		constexpr
		BasicUSB<M>::EndpointsTable<EndpointsNumber>::EndpointsTable (usb::Device const& device)
		{
			if (device.maximum_endpoint_id() != maximum_endpoint_id())
				throw WrongEndpointsNumber();
		}


template<class M>
	template<uint8_t EndpointsNumber>
		constexpr uint8_t
		BasicUSB<M>::EndpointsTable<EndpointsNumber>::single_direction_number() const
		{
			return 2 * EndpointsNumber;
		}


template<class M>
	template<uint8_t EndpointsNumber>
		constexpr uint8_t
		BasicUSB<M>::EndpointsTable<EndpointsNumber>::number() const
		{
			return EndpointsNumber;
		}


template<class M>
	template<uint8_t EndpointsNumber>
		constexpr uint8_t
		BasicUSB<M>::EndpointsTable<EndpointsNumber>::maximum_endpoint_id() const
		{
			return EndpointsNumber - 1;
		}


template<class M>
	template<uint8_t EndpointsNumber>
		constexpr uint8_t*
		BasicUSB<M>::EndpointsTable<EndpointsNumber>::table()
		{
			return _buffer;
		}


template<class M>
	template<uint8_t EndpointsNumber>
		constexpr uint8_t*
		BasicUSB<M>::EndpointsTable<EndpointsNumber>::operator[] (uint8_t n)
		{
			return _buffer + n * kSingleDirectionEndpointSize;
		}


template<class M>
	template<uint8_t EndpointsNumber>
		constexpr uint8_t*
		BasicUSB<M>::EndpointsTable<EndpointsNumber>::nth_output (uint8_t n)
		{
			return _buffer + (2 * n) * kSingleDirectionEndpointSize;
		}


template<class M>
	template<uint8_t EndpointsNumber>
		constexpr uint8_t*
		BasicUSB<M>::EndpointsTable<EndpointsNumber>::nth_input (uint8_t n)
		{
			return _buffer + (2 * n + 1) * kSingleDirectionEndpointSize;
		}


template<class M>
	template<uint8_t EndpointsNumber>
		inline uint16_t
		BasicUSB<M>::EndpointsTable<EndpointsNumber>::framenum() const
		{
			return _framenum;
		}


template<class M>
	constexpr
	BasicUSB<M>::BasicUSB (size_t base_address):
		_base_address (base_address),
		_ctrla (base_address + 0x00),
		_ctrlb (base_address + 0x01),
		_status (base_address + 0x02),
		_addr (base_address + 0x03),
		_fifowp (base_address + 0x04),
		_fiforp (base_address + 0x05),
		_epptrl (base_address + 0x06),
		_epptrh (base_address + 0x07),
		_intctrla (base_address + 0x08),
		_intctrlb (base_address + 0x09),
		_intflagsaclr (base_address + 0x0a),
		_intflagsaset (base_address + 0x0b),
		_intflagsbclr (base_address + 0x0c),
		_intflagsbset (base_address + 0x0d),
		_cal0 (base_address + 0x3a),
		_cal1 (base_address + 0x3b)
	{ }


template<class M>
	constexpr bool
	BasicUSB<M>::operator== (BasicUSB const& other) const
	{
		return _base_address == other._base_address;
	}


template<class M>
	constexpr bool
	BasicUSB<M>::operator!= (BasicUSB const& other) const
	{
		return !(*this == other);
	}


template<class M>
	inline void
	BasicUSB<M>::calibrate() const
	{
		_cal0 = MCU::read (MCU::SignatureRegister::USBCAL0);
		_cal1 = MCU::read (MCU::SignatureRegister::USBCAL1);
	}


template<class M>
	inline void
	BasicUSB<M>::set_enabled (bool enabled) const
	{
		_ctrla.template set_bit_value<7> (enabled);
	}


template<class M>
	inline void
	BasicUSB<M>::set (Speed speed) const
	{
		switch (speed)
		{
			case Speed::Low:
				_ctrla.template clear_bit<6>();
				break;

			case Speed::Full:
				_ctrla.template set_bit<6>();
				break;
		}
	}


template<class M>
	inline void
	BasicUSB<M>::set_fifo_enabled (bool enabled) const
	{
		_ctrla.template set_bit_value<5> (enabled);
	}


template<class M>
	inline void
	BasicUSB<M>::set_store_framenum_enabled (bool enabled) const
	{
		_ctrla.template set_bit_value<4> (enabled);
	}


template<class M>
	inline void
	BasicUSB<M>::set_pullrst_enabled (bool enabled) const
	{
		_ctrlb.template set_bit_value<4> (enabled);
	}


template<class M>
	inline void
	BasicUSB<M>::set_rwakeup_enabled (bool enabled) const
	{
		_ctrlb.template set_bit_value<2> (enabled);
	}


template<class M>
	inline void
	BasicUSB<M>::set_gnack_enabled (bool enabled) const
	{
		_ctrlb.template set_bit_value<1> (enabled);
	}


template<class M>
	template<uint8_t EndpointsNumber>
		inline void
		BasicUSB<M>::set_endpoints_number() const
		{
			static_assert (1 <= EndpointsNumber && EndpointsNumber <= 16);

			_ctrla.template set_bits_value<3, 0> (EndpointsNumber - 1);
		}


template<class M>
	template<uint8_t EndpointsNumber>
		inline void
		BasicUSB<M>::set_endpoints_table (uint8_t* endpoints) const
		{
			_epptrl = static_cast<uint8_t> (reinterpret_cast<size_t> (endpoints) >> 0) & 0xff;
			_epptrh = static_cast<uint8_t> (reinterpret_cast<size_t> (endpoints) >> 8) & 0xff;
			set_endpoints_number<EndpointsNumber>();
		}


template<class M>
	template<uint8_t EndpointsNumber>
		inline void
		BasicUSB<M>::set_endpoints_table (EndpointsTable<EndpointsNumber>& endpoints_table) const
		{
			set_endpoints_table<EndpointsNumber> (endpoints_table.table());
		}


template<class M>
	inline void
	BasicUSB<M>::set_address (uint8_t address) const
	{
		_addr = address;
	}


template<class M>
	inline void
	BasicUSB<M>::set_attached (bool attached) const
	{
		_ctrlb.template set_bit_value<0> (attached);
	}


template<class M>
	inline bool
	BasicUSB<M>::is_upstream_resume() const
	{
		return _status.template get_bit<3>();
	}


template<class M>
	inline bool
	BasicUSB<M>::is_downstream_resume() const
	{
		return _status.template get_bit<2>();
	}


template<class M>
	inline bool
	BasicUSB<M>::is_suspended() const
	{
		return _status.template get_bit<1>();
	}


template<class M>
	inline bool
	BasicUSB<M>::is_bus_reset() const
	{
		return _status.template get_bit<0>();
	}


template<class M>
	template<class ...Interrupts>
		inline void
		BasicUSB<M>::enable (Interrupts ...interrupts) const
		{
			uint16_t const bits = make_interrupts_list (interrupts...);

			if (bits & 0x00ff)
				_intctrla = _intctrla.read() | ((bits >> 0) & 0xff);

			if (bits & 0xff00)
				_intctrlb = _intctrlb.read() | ((bits >> 8) & 0xff);
		}


template<class M>
	template<class ...Interrupts>
		inline void
		BasicUSB<M>::disable (Interrupts ...interrupts) const
		{
			uint16_t const bits = make_interrupts_list (interrupts...);

			if (bits & 0x00ff)
				_intctrla = _intctrla.read() & ~((bits >> 0) & 0xff);

			if (bits & 0xff00)
				_intctrlb = _intctrlb.read() & ~((bits >> 8) & 0xff);
		}


template<class M>
	template<class ...InterruptFlags>
		inline void
		BasicUSB<M>::trigger (InterruptFlags ...interrupt_flags) const
		{
			uint16_t const bits = make_interrupt_flags_list (interrupt_flags...);

			if (bits & 0x00ff)
				_intflagsaset = (bits >> 0) & 0xff;

			if (bits & 0xff00)
				_intflagsbset = (bits >> 8) & 0xff;
		}


template<class M>
	template<class ...InterruptFlags>
		inline void
		BasicUSB<M>::clear (InterruptFlags ...interrupt_flags) const
		{
			uint16_t const bits = make_interrupt_flags_list (interrupt_flags...);
			uint16_t const transaction_complete = static_cast<decltype (bits)> (InterruptFlag::TransactionComplete);

			if (bits & 0x00ff)
				_intflagsaclr = (bits >> 0) & 0xff;

			// Exception for TransactionComplete, it's handler another way.
			if ((bits & ~transaction_complete) & 0xff00)
				_intflagsbclr = (bits >> 8) & 0xff;

			// There's a documentation error in manual for XMEGA128 (at least) regarding clearing the TRNIF flag.
			// The proper way to do this is as follows:
			//   • When the FIFO is enabled, the TRNIF interrupt flag is cleared:
			//     • by writing to FIFORP or FIFOWP any value,
			//     • by reading one or several times to FIFORP depending on the size of the fifo.
			//   • When the FIFO is disabled, the TRNIF interrupt flag is cleared:
			//     • by writing to FIFORP or FIFOWP any value.
			if (bits & transaction_complete)
				_fiforp = 0x00;
		}


template<class M>
	inline void
	BasicUSB<M>::clear_all_bus_event_interrupt_flags() const
	{
		clear (
			InterruptFlag::StartOfFrame,
			InterruptFlag::Suspend,
			InterruptFlag::Resume,
			InterruptFlag::Reset,
			InterruptFlag::IsochronousCRCError,
			InterruptFlag::Underflow,
			InterruptFlag::Overflow,
			InterruptFlag::Stall
		);
	}


template<class M>
	inline void
	BasicUSB<M>::clear_all_transaction_complete_interrupt_flags() const
	{
		clear (
			InterruptFlag::TransactionComplete,
			InterruptFlag::SetupTransactionComplete
		);
	}


template<class M>
	inline bool
	BasicUSB<M>::triggered (InterruptFlag flag) const
	{
		uint16_t const bits = make_interrupt_flags_list (flag);

		if (bits & 0x00ff)
			return _intflagsaclr & ((bits >> 0) & 0xff);

		if (bits & 0xff00)
			return _intflagsbclr & ((bits >> 8) & 0xff);

		return false;
	}


template<class M>
	template<class Function>
		inline void
		BasicUSB<M>::on (InterruptFlag flag, Function callback) const
		{
			if (triggered (flag))
			{
				callback();
				clear (flag);
			}
		}


template<class M>
	inline void
	BasicUSB<M>::set (InterruptSystem::Level level) const
	{
		_intctrla.template set_bits_value<1, 0> (static_cast<uint8_t> (level));
	}

} // namespace xmega_au
} // namespace avr
} // namespace mulabs

#endif

