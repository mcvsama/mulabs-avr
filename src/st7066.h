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

#ifndef MULABS_AVR__ST7066_H__INCLUDED
#define MULABS_AVR__ST7066_H__INCLUDED

// AVR:
#include <avr/io.h>

// Local:
#include "utility.h"
#include "interrupts.h"
#include "mcu.h"
#include "pin.h"


namespace mulabs {
namespace avr {

/**
 * \param	tConfig
 *			Structure describing how MCU pins are connected to the display.
 *			It's expected that this structure contains a set of static constexpr Pins
 *			with the following names:
 *
 *			  * db_4 (data bus line, bit 4)
 *			  * db_5 (data bus line, bit 5)
 *			  * db_6 (data bus line, bit 6)
 *			  * db_7 (data bus line, bit 7)
 *			  * rs (data/instruction signal)
 *			  * rw (read/write signal)
 *			  * e (chip enable signal)
 *
 *			Also it should contain constexpr uint8_t:
 *
 *			  * columns (number of columns in the display)
 */
template<class tConfig>
	class ST7066
	{
	  public:
		typedef tConfig Config;

		enum class WriteMode: uint8_t
		{
			Instruction	= 0,
			Data		= 1,
		};

		enum class ReadMode: uint8_t
		{
			BusyAddress	= 0,
			Data		= 1,
		};

		enum class Interface: uint8_t
		{
			Bits4		= 0,
			// Bits8	= 1, // Disabled since only 4-bit iface is supported now.
		};

		enum class Lines: uint8_t
		{
			OneLine		= 0,
			TwoLines	= 1,
		};

		enum class Font: uint8_t
		{
			Px5x8		= 0,
			Px5x11		= 1,
		};

		enum class Display: uint8_t
		{
			Off			= 0,
			On			= 1,
		};

		enum class Cursor: uint8_t
		{
			Off			= 0,
			On			= 1,
		};

		enum class CursorBlinking: uint8_t
		{
			Off			= 0,
			On			= 1,
		};

		enum class CursorDirection: uint8_t
		{
			Left		= 0,
			Right		= 1,
		};

		enum class Shifting: uint8_t
		{
			Off			= 0,
			On			= 1,
		};

	  public:
		/**
		 * Ctor.
		 * Configures MCU pins. Note that after calling this constructor,
		 * you must also call initialize_4bit() method after display initialization-delay.
		 * Refer to display datasheet.
		 */
		ST7066();

		/**
		 * Wait 40 ms for display to initialize.
		 */
		static void
		initialization_delay();

		/**
		 * First method that must be called after initialization_delay().
		 * Note that LCD needs to initialize itself after poweron, it takes not less than 40 ms.
		 * Don't call any method, including this one, before that time.
		 */
		void
		initialize_4bit (Interface length, Lines lines, Font font);

		/**
		 * Set number of lines displayed.
		 */
		void
		set_lines (Lines lines);

		/**
		 * Set font.
		 */
		void
		set_font (Font font);

		/**
		 * Set lines and font at the same time.
		 */
		void
		set_lines_and_font (Lines, Font);

		/**
		 * Enable/disable display.
		 */
		void
		set_display (Display);

		/**
		 * Set cursor visibility.
		 */
		void
		set_cursor (Cursor);

		/**
		 * Set cursor blinking.
		 */
		void
		set_cursor_blinking (CursorBlinking);

		/**
		 * Set various display settings.
		 */
		void
		set_display_options (Display, Cursor, CursorBlinking);

		/**
		 * Set cursor movement enabled.
		 */
		void
		set_cursor_direction (CursorDirection);

		/**
		 * Enable/disable shifting of display contents.
		 */
		void
		set_shifting (Shifting);

		/**
		 * Set cursor-related options.
		 */
		void
		set_cursor_options (CursorDirection, Shifting);

		/**
		 * Clear display.
		 */
		static void
		clear();

		/**
		 * Set cursor position.
		 */
		void
		locate (uint8_t row, uint8_t column);

		/**
		 * Print string under cursor position.
		 */
		static void
		print (const char* string);

	  protected:
		/**
		 * Return true if the display is busy processing and can't
		 * accept new instructions.
		 */
		static bool
		busy();

		/**
		 * Wait in a loop until display says it's not busy.
		 */
		static void
		wait();

		/**
		 * Prepare for write.
		 */
		static void
		prepare_for_write (WriteMode);

		/**
		 * Execute write instruction on the display.
		 * Assume that data lines are configured as outputs and set correctly.
		 */
		static void
		write();

		/**
		 * Prepare for read.
		 */
		static void
		prepare_for_read (ReadMode);

		/**
		 * Execute read instruction on the display.
		 *
		 * WARNING: Before issuing this command, pins must be configured as inputs,
		 * otherwise you may burn your microcontroller.
		 *
		 * After this the data lines should be ready to read.
		 */
		static void
		read();

		/**
		 * Configure data lines as inputs.
		 */
		static void
		configure_as_input();

		/**
		 * Configure data lines as outputs.
		 */
		static void
		configure_as_output();

	  private:
		Interface		_interface;
		Lines			_lines;
		Font			_font;
		Display			_display;
		Cursor			_cursor;
		CursorBlinking	_cursor_blinking;
		CursorDirection	_cursor_direction;
		Shifting		_shifting;
		uint8_t			_ddram_addr;
	};


template<class P>
	inline
	ST7066<P>::ST7066()
	{
		Config::e.configure_as_input();
		Config::e.set_tri_state();
		Config::rs.configure_as_input();
		Config::rs.set_tri_state();
		Config::rw.configure_as_input();
		Config::rw.set_tri_state();
		configure_as_input();
		Config::db_4.pull_up();
		Config::db_5.pull_up();
		Config::db_6.pull_up();
		Config::db_7.pull_up();
	}


template<class P>
	inline void
	ST7066<P>::initialization_delay()
	{
		MCU::sleep_ms (40);
	}


template<class P>
	inline void
	ST7066<P>::initialize_4bit (Interface interface, Lines lines, Font font)
	{
		_interface = interface;
		_lines = lines;
		_font = font;

		Config::e.set_low();
		Config::rs.set_low();
		Config::rw.set_low();
		Config::e.configure_as_output();
		Config::rs.configure_as_output();
		Config::rw.configure_as_output();

		prepare_for_write (WriteMode::Instruction);

		// Three times according to doc:
		for (int i = 0; i < 3; ++i)
		{
			Config::db_4.set (1);
			Config::db_5.set (1);
			Config::db_6.set (0);
			Config::db_7.set (0);
			write();
			MCU::sleep_ms<5>();
		}

		// Select 4-bit interface:
		Config::db_4.set (!!static_cast<uint8_t> (interface));
		write();
		MCU::sleep_ms<5>();

		set_lines_and_font (lines, font);
		set_display_options (Display::Off, Cursor::Off, CursorBlinking::Off);
		set_cursor_options (CursorDirection::Right, Shifting::Off);
		clear();
	}


template<class P>
	inline void
	ST7066<P>::set_lines (Lines lines)
	{
		set_lines_and_font (lines, _font);
	}


template<class P>
	inline void
	ST7066<P>::set_font (Font font)
	{
		set_lines_and_font (_lines, font);
	}


template<class P>
	inline void
	ST7066<P>::set_lines_and_font (Lines lines, Font font)
	{
		_lines = lines;
		_font = font;

		wait();
		prepare_for_write (WriteMode::Instruction);
		Config::db_4.set (!!static_cast<uint8_t> (_interface));
		Config::db_5.set (1);
		Config::db_6.set (0);
		Config::db_7.set (0);
		write();
		Config::db_6.set (!!static_cast<uint8_t> (font));
		Config::db_7.set (!!static_cast<uint8_t> (lines));
		write();
	}


template<class P>
	inline void
	ST7066<P>::set_display (Display display)
	{
		set_display_options (display, _cursor, _cursor_blinking);
	}


template<class P>
	inline void
	ST7066<P>::set_cursor (Cursor cursor)
	{
		set_display_options (_display, cursor, _cursor_blinking);
	}


template<class P>
	inline void
	ST7066<P>::set_cursor_blinking (CursorBlinking cursor_blinking)
	{
		set_display_options (_display, _cursor, cursor_blinking);
	}


template<class P>
	inline void
	ST7066<P>::set_display_options (Display display, Cursor cursor, CursorBlinking cursor_blinking)
	{
		_display = display;
		_cursor = cursor;
		_cursor_blinking = cursor_blinking;

		wait();
		prepare_for_write (WriteMode::Instruction);
		Config::db_4.set (0);
		Config::db_5.set (0);
		Config::db_6.set (0);
		Config::db_7.set (0);
		write();
		Config::db_4.set (!!static_cast<uint8_t> (cursor_blinking));
		Config::db_5.set (!!static_cast<uint8_t> (cursor));
		Config::db_6.set (!!static_cast<uint8_t> (display));
		Config::db_7.set (1);
		write();
	}


template<class P>
	inline void
	ST7066<P>::set_cursor_direction (CursorDirection cursor_direction)
	{
		set_cursor_options (cursor_direction, _shifting);
	}


template<class P>
	inline void
	ST7066<P>::set_shifting (Shifting shifting)
	{
		set_cursor_options (_cursor_direction, shifting);
	}


template<class P>
	inline void
	ST7066<P>::set_cursor_options (CursorDirection cursor_direction, Shifting shifting)
	{
		_cursor_direction = cursor_direction;
		_shifting = shifting;

		wait();
		prepare_for_write (WriteMode::Instruction);
		Config::db_4.set (0);
		Config::db_5.set (0);
		Config::db_6.set (0);
		Config::db_7.set (0);
		write();
		Config::db_4.set (!!static_cast<uint8_t> (shifting));
		Config::db_5.set (!!static_cast<uint8_t> (cursor_direction));
		Config::db_6.set (!!static_cast<uint8_t> (1));
		write();
	}


template<class P>
	inline void
	ST7066<P>::clear()
	{
		wait();
		prepare_for_write (WriteMode::Instruction);
		Config::db_4.set (0);
		Config::db_5.set (0);
		Config::db_6.set (0);
		Config::db_7.set (0);
		write();
		Config::db_4.set (1);
		write();
	}


template<class P>
	inline void
	ST7066<P>::locate (uint8_t row, uint8_t column)
	{
		_ddram_addr = Config::row_pitch * row + column;
		wait();
		prepare_for_write (WriteMode::Instruction);
		Config::db_4.set (get_bit (_ddram_addr, 4));
		Config::db_5.set (get_bit (_ddram_addr, 5));
		Config::db_6.set (get_bit (_ddram_addr, 6));
		Config::db_7.set (1);
		write();
		Config::db_4.set (get_bit (_ddram_addr, 0));
		Config::db_5.set (get_bit (_ddram_addr, 1));
		Config::db_6.set (get_bit (_ddram_addr, 2));
		Config::db_7.set (get_bit (_ddram_addr, 3));
		write();
	}


template<class P>
	inline void
	ST7066<P>::print (const char* string)
	{
		for (const char* c = string; *c != 0; ++c)
		{
			wait();
			prepare_for_write (WriteMode::Data);
			Config::db_4.set (get_bit<uint8_t> (*c, 4));
			Config::db_5.set (get_bit<uint8_t> (*c, 5));
			Config::db_6.set (get_bit<uint8_t> (*c, 6));
			Config::db_7.set (get_bit<uint8_t> (*c, 7));
			write();
			Config::db_4.set (get_bit<uint8_t> (*c, 0));
			Config::db_5.set (get_bit<uint8_t> (*c, 1));
			Config::db_6.set (get_bit<uint8_t> (*c, 2));
			Config::db_7.set (get_bit<uint8_t> (*c, 3));
			write();
		}
	}


template<class P>
	inline bool
	ST7066<P>::busy()
	{
		prepare_for_read (ReadMode::BusyAddress);
		read();
		return Config::db_7.read();
	}


template<class P>
	inline void
	ST7066<P>::wait()
	{
		MCU::sleep_ms<2>();
		while (busy())
			continue;
	}


template<class P>
	inline void
	ST7066<P>::prepare_for_write (WriteMode mode)
	{
		Config::rw.set_low();
		Config::rs.set (!!static_cast<uint8_t> (mode));
		configure_as_output();
	}


template<class P>
	inline void
	ST7066<P>::write()
	{
		// The data line must be stable for at least 40 ns (according to doc)
		// before executing instruction.
		MCU::sleep_us<1>();
		Config::e.set_high();
		// Minimum time for data to be read by the display
		// is 10 ns.
		MCU::sleep_us<1>();
		Config::e.set_low();
		// According to docs, minimum 'e' pin high time is 140 ns.
		// Also the total cycle time between subsequent e.set_high()
		// must be minimum 1.2 µs. It'll be for sure thanks to above
		// 1 µs delays.
	}


template<class P>
	inline void
	ST7066<P>::prepare_for_read (ReadMode mode)
	{
		Config::rw.set_high();
		Config::rs.set (!!static_cast<uint8_t> (mode));
		configure_as_input();
	}


template<class P>
	inline void
	ST7066<P>::read()
	{
		Config::e.set_high();
		MCU::sleep_us<1>();
		Config::e.set_low();
		// Maximum delay before data line is stable is 100 ns.
	}


template<class P>
	inline void
	ST7066<P>::configure_as_input()
	{
		Config::db_4.configure_as_input();
		Config::db_5.configure_as_input();
		Config::db_6.configure_as_input();
		Config::db_7.configure_as_input();
	}


template<class P>
	inline void
	ST7066<P>::configure_as_output()
	{
		Config::db_4.configure_as_output();
		Config::db_5.configure_as_output();
		Config::db_6.configure_as_output();
		Config::db_7.configure_as_output();
	}

} // namespace avr
} // namespace mulabs

#endif

