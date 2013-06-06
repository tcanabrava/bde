// bcemt_saturatedtimeconversionimputil.cpp                           -*-C++-*-
#include <bcemt_saturatedtimeconversionimputil.h>

#include <bdes_ident.h>
BDES_IDENT_RCSID(bcemt_barrier_cpp,"$Id$ $CSID$")

#ifdef BCES_PLATFORM_WIN32_THREADS
#include <windows.h>
#endif

#if (6 == sizeof(long))
#error !!!
#endif

#if defined(BCES_PLATFORM_POSIX_THREADS) && defined(BSLS_PLATFORM_CPU_64_BIT)
// Note 'long' is always 32 bit on 64 bit Windows.
#define BCEMT_SATURATEDTIMECONVERSION_LONG_IS_64_BIT 1
#endif

namespace BloombergLP {

// PRIVATE CONSTANTS
enum { 
    NANOSEC_PER_MILLISEC = 1000000,
    MILLISEC_PER_SEC     = 1000
};

// PRIVATE CLASS METHODS
template <typename TYPE>
static inline
TYPE maxOf(const TYPE&)
    // Return the maximum representable value of 'TYPE'
{
    return bsl::numeric_limits<TYPE>::max();
};

template <typename TYPE>
static inline
TYPE minOf(const TYPE&)
    // Return the minimum representable value of 'TYPE'
{
    return bsl::numeric_limits<TYPE>::min();
};

static inline
void toTimeTImp(int *dst, bsls::Types::Int64 src)
    // Assign to the specified 'dst' the value of the sepcified 'src', and if
    // 'src' is less than the lowest representable 'time_t' value, set 'dst' to
    // the minimum 'time_t' value, and if 'src' is greater than the highest
    // representable 'time_t' value, set 'dst' to the maximum 'time_t' value.
{
    *dst = src > maxOf(*dst) ? maxOf(*dst)
                             : (src < minOf(*dst) ? minOf(*dst) : (int) src);
}

static inline
void toTimeTImp(long *dst, bsls::Types::Int64 src)
    // Assign to the specified 'dst' the value of the sepcified 'src', and if
    // 'src' is less than the lowest representable 'time_t' value, set 'dst' to
    // the minimum 'time_t' value, and if 'src' is greater than the highest
    // representable 'time_t' value, set 'dst' to the maximum 'time_t' value.
{
#ifdef BCEMT_SATURATEDTIMECONVERSION_LONG_IS_64_BIT
    *dst = src;
#else
    *dst = src > maxOf(*dst) ? maxOf(*dst)
                             : (src < minOf(*dst) ? minOf(*dst)
                                                  : (long) src);
#endif
}

static inline
void toTimeTImp(long long *dst, bsls::Types::Int64 src)
    // Assign to the specified 'dst' the value of the sepcified 'src', and if
    // 'src' is less than the lowest representable 'time_t' value, set 'dst' to
    // the minimum 'time_t' value, and if 'src' is greater than the highest
    // representable 'time_t' value, set 'dst' to the maximum 'time_t' value.
{
    *dst = src;
}

static inline
void toTimeTImp(unsigned int *dst, bsls::Types::Int64 src)
    // Assign to the specified 'dst' the value of the sepcified 'src', and if
    // 'src' is less than the lowest representable 'time_t' value, set 'dst' to
    // the minimum 'time_t' value, and if 'src' is greater than the highest
    // representable 'time_t' value, set 'dst' to the maximum 'time_t' value.
{
    *dst = src > maxOf(*dst) ? maxOf(*dst)
                             : (src < 0 ? 0 : (unsigned int) src);
}

static inline
void toTimeTImp(unsigned long *dst, bsls::Types::Int64 src)
    // Assign to the specified 'dst' the value of the sepcified 'src', and if
    // 'src' is less than the lowest representable 'time_t' value, set 'dst' to
    // the minimum 'time_t' value, and if 'src' is greater than the highest
    // representable 'time_t' value, set 'dst' to the maximum 'time_t' value.
{
#ifdef BCEMT_SATURATEDTIMECONVERSION_LONG_IS_64_BIT
    *dst = src < 0 ? 0 : src;
#else
    *dst = src > maxOf(*dst) ? maxOf(*dst)
                             : (src < 0 ? 0 : (unsigned long) src);
#endif
}

static inline
void toTimeTImp(unsigned long long *dst, bsls::Types::Int64 src)
    // Assign to the specified 'dst' the value of the sepcified 'src', and if
    // 'src' is less than the lowest representable 'time_t' value, set 'dst' to
    // the minimum 'time_t' value, and if 'src' is greater than the highest
    // representable 'time_t' value, set 'dst' to the maximum 'time_t' value.
{
    *dst = src < 0 ? 0 : (unsigned long long) src;
}

// PUBLIC CLASS METHODS
void bcemt_SaturatedTimeConversionImpUtil::toTimeSpec(
                                                 TimeSpec                 *dst,
                                                 const bdet_TimeInterval&  src)
{
    enum { MAX_NANOSECONDS = 1000 * 1000 * 1000 - 1 };

    // In the test driver, in the test case "ASSERTS ABOUT 'TimeSpec' and
    // 'mach_timespec_t'" we verify that 'TimeSpec' can contain negative
    // values.

    toTimeTImp(&dst->tv_sec, src.seconds());

    dst->tv_nsec = src.seconds() > (bsls::Types::Int64) dst->tv_sec
                 ? MAX_NANOSECONDS
                 : src.seconds() < (bsls::Types::Int64) dst->tv_sec
                 ? -MAX_NANOSECONDS
                 : src.nanoseconds();
}

#ifdef BSLS_PLATFORM_OS_DARWIN
void bcemt_SaturatedTimeConversionImpUtil::toTimeSpec(
                                                 mach_timespec_t          *dst,
                                                 const bdet_TimeInterval&  src)
{
    enum { MAX_NANOSECONDS = 1000 * 1000 * 1000 - 1 };

    // 'dst->tv_sec' is unsigned, which is verified in the test driver in test
    // case "ASSERTS ABOUT 'TimeSpec' and 'mach_timespec_t'.".

    toTimeTImp(&dst->tv_sec, src.seconds());

    dst->tv_nsec = src.seconds() > (bsls::Types::Int64) dst->tv_sec
                 ? MAX_NANOSECONDS
                 : src.nanoseconds() < 0
                 ? 0
                 : src.nanoseconds();
}
#endif

void bcemt_SaturatedTimeConversionImpUtil::toTimeT(
                                                 bsl::time_t              *dst,
                                                 const bsls::Types::Int64  src)
{
    toTimeTImp(dst, src);
}

void bcemt_SaturatedTimeConversionImpUtil::toMillisec(
                                                 unsigned int             *dst,
                                                 const bdet_TimeInterval&  src)
{
    BSLS_ASSERT(dst);

    const int nanoMilliSeconds = src.nanoseconds() / NANOSEC_PER_MILLISEC;

    if (src.seconds() < 0 || (0 == src.seconds() && nanoMilliSeconds <= 0)) {
        *dst = 0;
    }
    else if (src.seconds() > maxOf(*dst)) {
        *dst = maxOf(*dst);
    }
    else {
        // 'src.seconds() < 2^32', therefore
        // 'src.seconds() * 1000 + 999 < 2^63', so this will work.  We also
        // know that 'src' is a poitive time interval.

        toTimeTImp(dst, src.seconds() * 1000 + nanoMilliSeconds);
    }
}

void bcemt_SaturatedTimeConversionImpUtil::toMillisec(
                                                 unsigned long            *dst,
                                                 const bdet_TimeInterval&  src)
{
    BSLS_ASSERT_SAFE(dst);

    typedef bsl::conditional<sizeof(unsigned int) == sizeof(unsigned long),
                             unsigned int, 
                             Uint64>::type LongAlias;

    LongAlias result;

    toMillisec(&result, src);

    *dst = result;
}

void bcemt_SaturatedTimeConversionImpUtil::toMillisec(
                                                 bsls::Types::UInt64      *dst,
                                                 const bdet_TimeInterval&  src)
{
    BSLS_ASSERT(dst);

    const int nanoMilliSeconds = src.nanoseconds() / NANOSEC_PER_MILLISEC;

    if (src.seconds() < 0 || (0 == src.seconds() && nanoMilliSeconds <= 0)) {
        *dst = 0;
    }
    else if (src.seconds() >= maxOf(src.secconds()) / 1000 - nanoMillseconds) {
        *dst = maxOf(*dst);
    }
    else {              
        toTimeTImp(dst, src.seconds() * 1000 + nanoMilliSeconds);
    }
}

#endif


}  // close namespace BloombergLP

// ---------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2013
//      All Rights Reserved.
//      Property of Bloomberg L.P. (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ----------------------------- END-OF-FILE ---------------------------------
