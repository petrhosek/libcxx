//===------------------------- chrono.cpp ---------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "chrono"
#include "cerrno"        // errno
#include "system_error"  // __throw_system_error
#include <time.h>        // clock_gettime, CLOCK_MONOTONIC and CLOCK_REALTIME

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define VC_EXTRA_LEAN
#include <Windows.h>
#if _WIN32_WINNT >= _WIN32_WINNT_WIN8
#include <winapifamily.h>
#endif
#else
#if !defined(CLOCK_REALTIME)
#include <sys/time.h>        // for gettimeofday and timeval
#endif
#endif

#if !defined(_LIBCPP_HAS_NO_MONOTONIC_CLOCK)
#if __APPLE__
#include <mach/mach_time.h>  // mach_absolute_time, mach_timebase_info_data_t
#elif !defined(_WIN32) && !defined(CLOCK_MONOTONIC)
#error "Monotonic clock not implemented"
#endif
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

namespace chrono
{

// system_clock

const bool system_clock::is_steady;

system_clock::time_point
system_clock::now() _NOEXCEPT
{
#if defined(_WIN32)
  // The Windows epoch is Jan 1 1601, the Unix epoch Jan 1 1970.  The difference
  // in nanoseconds is the windows epoch offset.
  static const constexpr __int64 kWindowsEpochOffset = 0x19db1ded53e8000;

  FILETIME ftSystemTime;
#if _WIN32_WINNT >= _WIN32_WINNT_WIN8
#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
  GetSystemTimePreciseAsFileTime(&ftSystemTime);
#else
  GetSystemTimeAsFileTime(&ftSystemTime);
#endif
#else
  GetSystemTimeAsFileTime(&ftSystemTime);
#endif
  __int64 llWinTimeNS =
      ((static_cast<__int64>(ftSystemTime.dwHighDateTime) << 32) |
       static_cast<__int64>(ftSystemTime.dwLowDateTime)) *
      100;
  return time_point(duration_cast<duration>(
      (nanoseconds(llWinTimeNS - kWindowsEpochOffset))));
#else
#ifdef CLOCK_REALTIME
    struct timespec tp;
    if (0 != clock_gettime(CLOCK_REALTIME, &tp))
        __throw_system_error(errno, "clock_gettime(CLOCK_REALTIME) failed");
    return time_point(seconds(tp.tv_sec) + microseconds(tp.tv_nsec / 1000));
#else  // !CLOCK_REALTIME
    timeval tv;
    gettimeofday(&tv, 0);
    return time_point(seconds(tv.tv_sec) + microseconds(tv.tv_usec));
#endif  // CLOCK_REALTIME
#endif
}

time_t
system_clock::to_time_t(const time_point& t) _NOEXCEPT
{
    return time_t(duration_cast<seconds>(t.time_since_epoch()).count());
}

system_clock::time_point
system_clock::from_time_t(time_t t) _NOEXCEPT
{
    return system_clock::time_point(seconds(t));
}

#ifndef _LIBCPP_HAS_NO_MONOTONIC_CLOCK
// steady_clock
//
// Warning:  If this is not truly steady, then it is non-conforming.  It is
//  better for it to not exist and have the rest of libc++ use system_clock
//  instead.

const bool steady_clock::is_steady;

#if defined(__APPLE__)

//   mach_absolute_time() * MachInfo.numer / MachInfo.denom is the number of
//   nanoseconds since the computer booted up.  MachInfo.numer and MachInfo.denom
//   are run time constants supplied by the OS.  This clock has no relationship
//   to the Gregorian calendar.  It's main use is as a high resolution timer.

// MachInfo.numer / MachInfo.denom is often 1 on the latest equipment.  Specialize
//   for that case as an optimization.

static
steady_clock::rep
steady_simplified()
{
    return static_cast<steady_clock::rep>(mach_absolute_time());
}

static
double
compute_steady_factor()
{
    mach_timebase_info_data_t MachInfo;
    mach_timebase_info(&MachInfo);
    return static_cast<double>(MachInfo.numer) / MachInfo.denom;
}

static
steady_clock::rep
steady_full()
{
    static const double factor = compute_steady_factor();
    return static_cast<steady_clock::rep>(mach_absolute_time() * factor);
}

typedef steady_clock::rep (*FP)();

static
FP
init_steady_clock()
{
    mach_timebase_info_data_t MachInfo;
    mach_timebase_info(&MachInfo);
    if (MachInfo.numer == MachInfo.denom)
        return &steady_simplified;
    return &steady_full;
}

steady_clock::time_point
steady_clock::now() _NOEXCEPT
{
    static FP fp = init_steady_clock();
    return time_point(duration(fp()));
}

#elif defined(_WIN32)

steady_clock::time_point
steady_clock::now() _NOEXCEPT
{
  static LARGE_INTEGER freq;
  static BOOL initialized = FALSE;
  if (!initialized)
    initialized = QueryPerformanceFrequency(&freq); // always succceeds

  LARGE_INTEGER counter;
  QueryPerformanceCounter(&counter);
  return time_point(duration(counter.QuadPart * nano::den / freq.QuadPart));
}

#elif defined(CLOCK_MONOTONIC)

steady_clock::time_point
steady_clock::now() _NOEXCEPT
{
    struct timespec tp;
    if (0 != clock_gettime(CLOCK_MONOTONIC, &tp))
        __throw_system_error(errno, "clock_gettime(CLOCK_MONOTONIC) failed");
    return time_point(seconds(tp.tv_sec) + nanoseconds(tp.tv_nsec));
}

#else
#error "Monotonic clock not implemented"
#endif

#endif // !_LIBCPP_HAS_NO_MONOTONIC_CLOCK

}

_LIBCPP_END_NAMESPACE_STD
