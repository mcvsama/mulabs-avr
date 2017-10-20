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

#ifndef MULABS_AVR__DEVICES__XMEGA_AU__BASIC_TIMER_01_H__INCLUDED
#define MULABS_AVR__DEVICES__XMEGA_AU__BASIC_TIMER_01_H__INCLUDED

#include <mulabs_avr/devices/xmega_au/interrupt_system.h>
#include <mulabs_avr/utility/bits.h>


namespace mulabs {
namespace avr {
namespace xmega_au {

// TODO make impossible for Timer1 to use CCC and CCD
template<class pMCU>
	class BasicTimer01
	{
	  public:
		typedef pMCU						MCU;
		typedef typename MCU::Register8		Register8;
		typedef typename MCU::Register16	Register16;

		enum class Mode: uint8_t
		{
			Normal				= 0b000,
			Frequency			= 0b001,
			SingleSlopePWM		= 0b011,
			DualSlopePWMTop		= 0b101,
			DualSlopePWMBoth	= 0b110,
			DualSlopePWMBottom	= 0b111,
		};

		enum class ByteMode: uint8_t
		{
			Normal16bit			= 0b00,
			ByteMode			= 0b01,
			SplitMode			= 0b10,
		};

		enum class EventAction: uint8_t
		{
			None				= 0b000 << 5,
			Capture				= 0b001 << 5,
			UpDown				= 0b010 << 5,
			QuadratureDecode	= 0b011 << 5,
			Restart				= 0b100 << 5,
			FrequencyCapture	= 0b101 << 5,
			PulseWidthCapture	= 0b110 << 5,
		};

		enum class ClockSource: uint8_t
		{
			None				= 0b0000,
			Div1				= 0b0001,
			Div2				= 0b0010,
			Div4				= 0b0011,
			Div8				= 0b0100,
			Div64				= 0b0101,
			Div256				= 0b0110,
			Div1024				= 0b0111,
			Event0				= 0b1000,
			Event1				= 0b1001,
			Event2				= 0b1010,
			Event3				= 0b1011,
			Event4				= 0b1100,
			Event5				= 0b1101,
			Event6				= 0b1110,
			Event7				= 0b1111,
		};

		enum class CompareCaptureChannel: uint8_t
		{
			A					= 0b0001,
			B					= 0b0010,
			C					= 0b0100,
			D					= 0b1000,
		};

		enum PeriodBufferType: uint8_t
		{
			PeriodBuffer,
		};

		enum class Direction
		{
			Up,
			Down,
		};

		enum class InterruptType
		{
			Error,
			Overflow,
		};

	  private:
		enum class Command: uint8_t
		{
			Update				= 0b01 << 2,
			Restart				= 0b10 << 2,
			HardReset			= 0b11 << 2,
		};

	  public:
		// Ctor:
		constexpr
		BasicTimer01 (Register8 ctrla, Register8 ctrlb, Register8 ctrlc, Register8 ctrld, Register8 ctrle,
					  Register8 intctrla, Register8 intctrlb,
					  Register8 ctrlfclr, Register8 ctrlfset,
					  Register8 ctrlgclr, Register8 ctrlgset,
					  Register8 intflags, Register8 temp,
					  Register16 cnt, Register8 cntl, Register8 cnth,
					  Register16 per, Register8 perl, Register8 perh, Register16 perbuf, Register8 perbufl, Register8 perbufh,
					  Register16 cca, Register8 ccal, Register8 ccah, Register16 ccabuf, Register8 ccabufl, Register8 ccabufh,
					  Register16 ccb, Register8 ccbl, Register8 ccbh, Register16 ccbbuf, Register8 ccbbufl, Register8 ccbbufh,
					  Register16 ccc, Register8 cccl, Register8 ccch, Register16 cccbuf, Register8 cccbufl, Register8 cccbufh,
					  Register16 ccd, Register8 ccdl, Register8 ccdh, Register16 ccdbuf, Register8 ccdbufl, Register8 ccdbufh);

		constexpr bool
		operator== (BasicTimer01 const& other) const;

		constexpr bool
		operator!= (BasicTimer01 const& other) const;

		void
		set (Mode) const;

		void
		set (ByteMode) const;

		void
		set (EventAction) const;

		/**
		 * Set counter direction.
		 */
		void
		set (Direction) const;

		template<uint8_t EventBus>
			void
			set_event_source_bus() const;

		void
		disable_event_source_bus() const;

		/**
		 * Select clock source for the timer.
		 * None disables the timer.
		 */
		void
		set (ClockSource) const;

		template<class ...Channels>
			void
			enable (Channels...) const;

		template<class ...Channels>
			void
			disable (Channels...) const;

		void
		set (InterruptType, InterruptSystem::Level) const;

		void
		set (CompareCaptureChannel, InterruptSystem::Level) const;

		/**
		 * Directly set pin output level when timer is in waveform-generator mode, but is not running
		 * (clock source is None).
		 */
		void
		set (CompareCaptureChannel, bool output) const;

		// TODO INTFLAGS

		/**
		 * This operation needs to be done atomically (interrupts disabled).
		 */
		void
		set_value (uint16_t value) const;

		void
		set_value_lsb (uint8_t value) const;

		void
		set_value_msb (uint8_t value) const;

		/**
		 * This operation needs to be done atomically (interrupts disabled).
		 */
		void
		set_period (uint16_t value) const;

		/**
		 * This operation needs to be done atomically (interrupts disabled).
		 */
		void
		set_period_buffered (uint16_t value) const;

		void
		set_period_lsb (uint8_t value) const;

		void
		set_period_lsb_buffered (uint8_t value) const;

		void
		set_period_msb (uint8_t value) const;

		void
		set_period_msb_buffered (uint8_t value) const;

		/**
		 * This operation needs to be done atomically (interrupts disabled).
		 */
		void
		set_cc_value (CompareCaptureChannel channel, uint16_t value) const;

		/**
		 * This operation needs to be done atomically (interrupts disabled).
		 */
		void
		set_cc_value_buffered (CompareCaptureChannel channel, uint16_t value) const;

		void
		set_cc_value_lsb (CompareCaptureChannel channel, uint8_t value) const;

		void
		set_cc_value_lsb_buffered (CompareCaptureChannel channel, uint8_t value) const;

		void
		set_cc_value_msb (CompareCaptureChannel channel, uint8_t value) const;

		void
		set_cc_value_msb_buffered (CompareCaptureChannel channel, uint8_t value) const;

		/**
		 * Indicate that buffers listed in arguments (and set by set_*_buffered functions) are valid and should be used.
		 * Buffer can be CompareCaptureChannel or PeriodBufferType.
		 */
		template<class ...Buffers>
			void
			mark_buffers_valid (Buffers...) const;

		/**
		 * Indicate that buffers listed in arguments are INvalid.
		 * Buffer can be CompareCaptureChannel or PeriodBufferType.
		 */
		template<class ...Buffers>
			void
			mark_buffers_invalid (Buffers...) const;

		/**
		 * Force update condition.
		 */
		void
		update() const;

		/**
		 * Restart the waveform period.
		 */
		void
		restart() const;

		/**
		 * Reset the timer to initial settings.
		 */
		void
		reset() const;

		template<uint8_t Divider>
			static constexpr ClockSource
			clock_source_for_divider();

		template<uint8_t EventBus>
			static constexpr ClockSource
			clock_source_for_event_bus();

	  private:
		/**
		 * Helper for enable()/disable() of CompareCaptureChannels.
		 * Collect list of channels and prepare value for the CTRLB register.
		 */
		template<class Channel, class ...Channels>
			constexpr uint8_t
			make_channels_list (Channel channel, Channels ...channels) const
			{
				return make_channels_list (channel) | make_channels_list (channels...);
			}

		/**
		 * Recursive stop-condition for make_channels_list().
		 */
		constexpr uint8_t
		make_channels_list (CompareCaptureChannel channel) const
		{
			return static_cast<uint8_t> (channel) << 4;
		}

		/**
		 * Helper for mark_buffers_(in)valid().
		 * Collect list of buffers and prepare value for the CTRLGSET register.
		 */
		template<class Buffer, class ...Buffers>
			constexpr uint8_t
			make_buffers_list (Buffer buffer, Buffers ...buffers) const
			{
				return make_buffers_list (buffer) | make_buffers_list (buffers...);
			}

		/**
		 * Recursive stop-condition for make_buffers_list().
		 */
		constexpr uint8_t
		make_buffers_list (CompareCaptureChannel channel) const
		{
			return static_cast<uint8_t> (channel) << 1;
		}

		/**
		 * Recursive stop-condition for make_buffers_list().
		 */
		constexpr uint8_t
		make_buffers_list (PeriodBufferType) const
		{
			return 0b1;
		}

	  private:
		Register8	_ctrla, _ctrlb, _ctrlc, _ctrld, _ctrle;
		Register8	_intctrla, _intctrlb;
		Register8	_ctrlfclr, _ctrlfset;
		Register8	_ctrlgclr, _ctrlgset;
		Register8	_intflags, _temp;
		Register16	_cnt;
		Register8	_cntl, _cnth;
		Register16	_per;
		Register8	_perl, _perh;
		Register16	_perbuf;
		Register8	_perbufl, _perbufh;
		Register16	_cca;
		Register8	_ccal, _ccah;
		Register16	_ccabuf;
		Register8	_ccabufl, _ccabufh;
		Register16	_ccb;
		Register8	_ccbl, _ccbh;
		Register16	_ccbbuf;
		Register8	_ccbbufl, _ccbbufh;
		Register16	_ccc;
		Register8	_cccl, _ccch;
		Register16	_cccbuf;
		Register8	_cccbufl, _cccbufh;
		Register16	_ccd;
		Register8	_ccdl, _ccdh;
		Register16	_ccdbuf;
		Register8	_ccdbufl, _ccdbufh;
	};


template<class M>
	constexpr
	BasicTimer01<M>::BasicTimer01 (Register8 ctrla, Register8 ctrlb, Register8 ctrlc, Register8 ctrld, Register8 ctrle,
								   Register8 intctrla, Register8 intctrlb,
								   Register8 ctrlfclr, Register8 ctrlfset,
								   Register8 ctrlgclr, Register8 ctrlgset,
								   Register8 intflags, Register8 temp,
								   Register16 cnt, Register8 cntl, Register8 cnth,
								   Register16 per, Register8 perl, Register8 perh, Register16 perbuf, Register8 perbufl, Register8 perbufh,
								   Register16 cca, Register8 ccal, Register8 ccah, Register16 ccabuf, Register8 ccabufl, Register8 ccabufh,
								   Register16 ccb, Register8 ccbl, Register8 ccbh, Register16 ccbbuf, Register8 ccbbufl, Register8 ccbbufh,
								   Register16 ccc, Register8 cccl, Register8 ccch, Register16 cccbuf, Register8 cccbufl, Register8 cccbufh,
								   Register16 ccd, Register8 ccdl, Register8 ccdh, Register16 ccdbuf, Register8 ccdbufl, Register8 ccdbufh):
		_ctrla (ctrla), _ctrlb (ctrlb), _ctrlc (ctrlc), _ctrld (ctrld), _ctrle (ctrle),
		_intctrla (intctrla), _intctrlb (intctrlb),
		_ctrlfclr (ctrlfclr), _ctrlfset (ctrlfset),
		_ctrlgclr (ctrlgclr), _ctrlgset (ctrlgset),
		_intflags (intflags), _temp (temp),
		_cnt (cnt), _cntl (cntl), _cnth (cnth),
		_per (per), _perl (perl), _perh (perh), _perbuf (perbuf), _perbufl (perbufl), _perbufh (perbufh),
		_cca (cca), _ccal (ccal), _ccah (ccah), _ccabuf (ccabuf), _ccabufl (ccabufl), _ccabufh (ccabufh),
		_ccb (ccb), _ccbl (ccbl), _ccbh (ccbh), _ccbbuf (ccbbuf), _ccbbufl (ccbbufl), _ccbbufh (ccbbufh),
		_ccc (ccc), _cccl (cccl), _ccch (ccch), _cccbuf (cccbuf), _cccbufl (cccbufl), _cccbufh (cccbufh),
		_ccd (ccd), _ccdl (ccdl), _ccdh (ccdh), _ccdbuf (ccdbuf), _ccdbufl (ccdbufl), _ccdbufh (ccdbufh)
	{ }


template<class M>
	constexpr bool
	BasicTimer01<M>::operator== (BasicTimer01 const& other) const
	{
#define BASIC_TIMER_EQ(n) (&_##n == &other._##n)
		return BASIC_TIMER_EQ (ctrla) && BASIC_TIMER_EQ (ctrlb) && BASIC_TIMER_EQ (ctrlc) && BASIC_TIMER_EQ (ctrld) && BASIC_TIMER_EQ (ctrle)
			&& BASIC_TIMER_EQ (intctrla) && BASIC_TIMER_EQ (intctrlb)
			&& BASIC_TIMER_EQ (ctrlfclr) && BASIC_TIMER_EQ (ctrlfset)
			&& BASIC_TIMER_EQ (ctrlgclr) && BASIC_TIMER_EQ (ctrlgset)
			&& BASIC_TIMER_EQ (intflags) && BASIC_TIMER_EQ (temp)
			&& BASIC_TIMER_EQ (cnt) && BASIC_TIMER_EQ (cntl) && BASIC_TIMER_EQ (cnth)
			&& BASIC_TIMER_EQ (per) && BASIC_TIMER_EQ (perl) && BASIC_TIMER_EQ (perh) && BASIC_TIMER_EQ (perbuf) && BASIC_TIMER_EQ (perbufl) && BASIC_TIMER_EQ (perbufh)
			&& BASIC_TIMER_EQ (cca) && BASIC_TIMER_EQ (ccal) && BASIC_TIMER_EQ (ccah) && BASIC_TIMER_EQ (ccabuf) && BASIC_TIMER_EQ (ccabufl) && BASIC_TIMER_EQ (ccabufh)
			&& BASIC_TIMER_EQ (ccb) && BASIC_TIMER_EQ (ccbl) && BASIC_TIMER_EQ (ccbh) && BASIC_TIMER_EQ (ccbbuf) && BASIC_TIMER_EQ (ccbbufl) && BASIC_TIMER_EQ (ccbbufh)
			&& BASIC_TIMER_EQ (ccc) && BASIC_TIMER_EQ (cccl) && BASIC_TIMER_EQ (ccch) && BASIC_TIMER_EQ (cccbuf) && BASIC_TIMER_EQ (cccbufl) && BASIC_TIMER_EQ (cccbufh)
			&& BASIC_TIMER_EQ (ccd) && BASIC_TIMER_EQ (ccdl) && BASIC_TIMER_EQ (ccdh) && BASIC_TIMER_EQ (ccdbuf) && BASIC_TIMER_EQ (ccdbufl) && BASIC_TIMER_EQ (ccdbufh);
#undef BASIC_TIMER_EQ
	}


template<class M>
	constexpr bool
	BasicTimer01<M>::operator!= (BasicTimer01 const& other) const
	{
		return !(*this == other);
	}


template<class M>
	inline void
	BasicTimer01<M>::set (Mode mode) const
	{
		_ctrlb = (_ctrlb.read() & 0b11110000) | static_cast<uint8_t> (mode);
	}


template<class M>
	inline void
	BasicTimer01<M>::set (ByteMode mode) const
	{
		_ctrle = static_cast<uint8_t> (mode);
	}


template<class M>
	inline void
	BasicTimer01<M>::set (EventAction event_action) const
	{
		_ctrld = (_ctrld.read() & 0b0001'1111) | static_cast<uint8_t> (event_action);
	}


template<class M>
	inline void
	BasicTimer01<M>::set (Direction direction) const
	{
		switch (direction)
		{
			case Direction::Up:
				_ctrlfclr = 0b1;
				break;

			case Direction::Down:
				_ctrlfset = 0b1;
				break;
		}
	}


template<class M>
	template<uint8_t EventBus>
		inline void
		BasicTimer01<M>::set_event_source_bus() const
		{
			static_assert (EventBus >= 0 && EventBus <= 7);

			_ctrld = (_ctrld.read() & 0b1111'0000) | 0b1000 | EventBus;
		}


template<class M>
	inline void
	BasicTimer01<M>::disable_event_source_bus() const
	{
		_ctrld = (_ctrld.read() & 0b1111'0000);
	}


template<class M>
	inline void
	BasicTimer01<M>::set (ClockSource clock_source) const
	{
		_ctrla = (_ctrla.read() & 0b1111'0000) | static_cast<uint8_t> (clock_source);
	}


template<class M>
	template<class ...Channels>
		inline void
		BasicTimer01<M>::enable (Channels ...channels) const
		{
			_ctrlb = _ctrlb.read() | make_channels_list (channels...);
		}


template<class M>
	template<class ...Channels>
		inline void
		BasicTimer01<M>::disable (Channels ...channels) const
		{
			_ctrlb = _ctrlb.read() & ~make_channels_list (channels...);
		}


template<class M>
	inline void
	BasicTimer01<M>::set (InterruptType type, InterruptSystem::Level level) const
	{
		uint8_t const int_level = static_cast<uint8_t> (level);

		switch (type)
		{
			case InterruptType::Error:
				_intctrla = (_intctrla.read() & 0b11110011) | (int_level << 2);
				break;

			case InterruptType::Overflow:
				_intctrla = (_intctrla.read() & 0b11110011) | (int_level << 0);
				break;
		}
	}


template<class M>
	inline void
	BasicTimer01<M>::set (CompareCaptureChannel channel, InterruptSystem::Level level) const
	{
		uint8_t const int_level = static_cast<uint8_t> (level);

		switch (channel)
		{
			case CompareCaptureChannel::A:
				_intctrlb = (_intctrlb.read() & 0b11111100) | (int_level << 0);
				break;

			case CompareCaptureChannel::B:
				_intctrlb = (_intctrlb.read() & 0b11110011) | (int_level << 2);
				break;

			case CompareCaptureChannel::C:
				_intctrlb = (_intctrlb.read() & 0b11001111) | (int_level << 4);
				break;

			case CompareCaptureChannel::D:
				_intctrlb = (_intctrlb.read() & 0b00111111) | (int_level << 6);
				break;
		}
	}


template<class M>
	void
	BasicTimer01<M>::set (CompareCaptureChannel channel, bool output) const
	{
		switch (channel)
		{
			case CompareCaptureChannel::A:
				_ctrlc = (_ctrlc.read() & 0b00001110) | ((output ? 1 : 0) << 0);
				break;

			case CompareCaptureChannel::B:
				_ctrlc = (_ctrlc.read() & 0b00001101) | ((output ? 1 : 0) << 1);
				break;

			case CompareCaptureChannel::C:
				_ctrlc = (_ctrlc.read() & 0b00001011) | ((output ? 1 : 0) << 2);
				break;

			case CompareCaptureChannel::D:
				_ctrlc = (_ctrlc.read() & 0b00000111) | ((output ? 1 : 0) << 3);
				break;
		}
	}


template<class M>
	inline void
	BasicTimer01<M>::set_value (uint16_t value) const
	{
		uint8_t const l = (value >> 0) & 0xff;
		uint8_t const h = (value >> 8) & 0xff;
		_cntl = l;
		_cnth = h;
	}


template<class M>
	inline void
	BasicTimer01<M>::set_value_lsb (uint8_t value) const
	{
		_cntl = value;
	}


template<class M>
	inline void
	BasicTimer01<M>::set_value_msb (uint8_t value) const
	{
		_cnth = value;
	}


template<class M>
	inline void
	BasicTimer01<M>::set_period (uint16_t period) const
	{
		uint8_t const l = (period >> 0) & 0xff;
		uint8_t const h = (period >> 8) & 0xff;
		_perl = l;
		_perh = h;
	}


template<class M>
	inline void
	BasicTimer01<M>::set_period_buffered (uint16_t period) const
	{
		uint8_t const l = (period >> 0) & 0xff;
		uint8_t const h = (period >> 8) & 0xff;
		_perbufl = l;
		_perbufh = h;
	}


template<class M>
	inline void
	BasicTimer01<M>::set_period_lsb (uint8_t value) const
	{
		_perl = value;
	}


template<class M>
	inline void
	BasicTimer01<M>::set_period_lsb_buffered (uint8_t value) const
	{
		_perbufl = value;
	}


template<class M>
	inline void
	BasicTimer01<M>::set_period_msb (uint8_t value) const
	{
		_perh = value;
	}


template<class M>
	inline void
	BasicTimer01<M>::set_period_msb_buffered (uint8_t value) const
	{
		_perbufh = value;
	}


template<class M>
	inline void
	BasicTimer01<M>::set_cc_value (CompareCaptureChannel channel, uint16_t value) const
	{
		uint8_t const l = (value >> 0) & 0xff;
		uint8_t const h = (value >> 8) & 0xff;

		switch (channel)
		{
			case CompareCaptureChannel::A:	_ccal = l; _ccah = h; break;
			case CompareCaptureChannel::B:	_ccbl = l; _ccbh = h; break;
			case CompareCaptureChannel::C:	_cccl = l; _ccch = h; break;
			case CompareCaptureChannel::D:	_ccdl = l; _ccdh = h; break;
		}
	}


template<class M>
	inline void
	BasicTimer01<M>::set_cc_value_buffered (CompareCaptureChannel channel, uint16_t value) const
	{
		uint8_t const l = (value >> 0) & 0xff;
		uint8_t const h = (value >> 8) & 0xff;

		switch (channel)
		{
			case CompareCaptureChannel::A:	_ccabufl = l; _ccabufh = h; break;
			case CompareCaptureChannel::B:	_ccbbufl = l; _ccbbufh = h; break;
			case CompareCaptureChannel::C:	_cccbufl = l; _cccbufh = h; break;
			case CompareCaptureChannel::D:	_ccdbufl = l; _ccdbufh = h; break;
		}
	}


template<class M>
	inline void
	BasicTimer01<M>::set_cc_value_lsb (CompareCaptureChannel channel, uint8_t value) const
	{
		switch (channel)
		{
			case CompareCaptureChannel::A:	_ccal = value; break;
			case CompareCaptureChannel::B:	_ccbl = value; break;
			case CompareCaptureChannel::C:	_cccl = value; break;
			case CompareCaptureChannel::D:	_ccdl = value; break;
		}
	}


template<class M>
	inline void
	BasicTimer01<M>::set_cc_value_lsb_buffered (CompareCaptureChannel channel, uint8_t value) const
	{
		switch (channel)
		{
			case CompareCaptureChannel::A:	_ccabufl = value; break;
			case CompareCaptureChannel::B:	_ccbbufl = value; break;
			case CompareCaptureChannel::C:	_cccbufl = value; break;
			case CompareCaptureChannel::D:	_ccdbufl = value; break;
		}
	}


template<class M>
	inline void
	BasicTimer01<M>::set_cc_value_msb (CompareCaptureChannel channel, uint8_t value) const
	{
		switch (channel)
		{
			case CompareCaptureChannel::A:	_ccah = value; break;
			case CompareCaptureChannel::B:	_ccbh = value; break;
			case CompareCaptureChannel::C:	_ccch = value; break;
			case CompareCaptureChannel::D:	_ccdh = value; break;
		}
	}


template<class M>
	inline void
	BasicTimer01<M>::set_cc_value_msb_buffered (CompareCaptureChannel channel, uint8_t value) const
	{
		switch (channel)
		{
			case CompareCaptureChannel::A:	_ccabufh = value; break;
			case CompareCaptureChannel::B:	_ccbbufh = value; break;
			case CompareCaptureChannel::C:	_cccbufh = value; break;
			case CompareCaptureChannel::D:	_ccdbufh = value; break;
		}
	}


template<class M>
	inline void
	BasicTimer01<M>::update() const
	{
		_ctrlfset = static_cast<uint8_t> (Command::Update);
	}


template<class M>
	inline void
	BasicTimer01<M>::restart() const
	{
		_ctrlfset = static_cast<uint8_t> (Command::Restart);
	}


template<class M>
	inline void
	BasicTimer01<M>::reset() const
	{
		_ctrlfset = static_cast<uint8_t> (Command::Reset);
	}


template<class M>
	template<class ...Buffers>
		inline void
		BasicTimer01<M>::mark_buffers_valid (Buffers ...buffers) const
		{
			_ctrlgset = make_buffers_list (buffers...);
		}


template<class M>
	template<class ...Buffers>
		inline void
		BasicTimer01<M>::mark_buffers_invalid (Buffers ...buffers) const
		{
			_ctrlgclr = make_buffers_list (buffers...);
		}


template<class M>
	template<uint8_t Divider>
		constexpr typename BasicTimer01<M>::ClockSource
		BasicTimer01<M>::clock_source_for_divider()
		{
			static_assert (Divider == 1 || Divider == 2 || Divider == 4 || Divider == 8 ||
						   Divider == 64 || Divider == 256 || Divider == 1024);

			switch (Divider)
			{
				case 1:		return ClockSource::Div1;
				case 2:		return ClockSource::Div2;
				case 4:		return ClockSource::Div4;
				case 8:		return ClockSource::Div8;
				case 64:	return ClockSource::Div64;
				case 256:	return ClockSource::Div256;
				case 1024:	return ClockSource::Div1024;
			}
		}


template<class M>
	template<uint8_t EventBus>
		constexpr typename BasicTimer01<M>::ClockSource
		BasicTimer01<M>::clock_source_for_event_bus()
		{
			static_assert (EventBus >= 0 && EventBus <= 7);

			switch (EventBus)
			{
				case 0:		return ClockSource::EventBus0;
				case 1:		return ClockSource::EventBus1;
				case 2:		return ClockSource::EventBus2;
				case 3:		return ClockSource::EventBus3;
				case 4:		return ClockSource::EventBus4;
				case 5:		return ClockSource::EventBus5;
				case 6:		return ClockSource::EventBus6;
				case 7:		return ClockSource::EventBus7;
			}
		}

} // namespace xmega_au
} // namespace avr
} // namespace mulabs

#endif

