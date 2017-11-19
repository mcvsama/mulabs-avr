/* vim:ts=4
 *
 * Copyleft 2014  Michał Gawron
 * Marduk Unix Labs, http://mulabs.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Visit http://www.gnu.org/licenses/gpl-3.0.html for more information on licensing.
 */

// Standard:
#include <stdio.h>


namespace std {

struct type_info;

} // namespace std

namespace __cxxabiv1 {

struct __class_type_info
{
	virtual void unused()
	{ }
} class_type_info;

struct __si_class_type_info
{
	virtual void unused()
	{ }
} si_class_type_info;

} // namespace __cxxabiv1


__extension__ typedef int __guard __attribute__((mode (__DI__)));


extern "C" void
__cxa_pure_virtual (void)
{ }


extern "C" int
__cxa_guard_acquire (__guard* g)
{
	return !*reinterpret_cast<char*> (g);
}


extern "C" void
__cxa_guard_release (__guard* g)
{
	*reinterpret_cast<char*> (g) = 1;
}


extern "C" void
__cxa_guard_abort (__guard*)
{ }


extern "C" void*
__cxa_allocate_exception ([[maybe_unused]] size_t thrown_size) noexcept
{
	// If memory can not be allocated, call std::terminate().
	while (true)
		continue;
}


extern "C" void
__cxa_throw ([[maybe_unused]] void* thrown_exception, [[maybe_unused]] struct std::type_info* tinfo, [[maybe_unused]] void (*dest)(void*))
{ }

