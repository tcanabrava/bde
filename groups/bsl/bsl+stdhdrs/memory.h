// memory.h                                                           -*-C++-*-

//@PURPOSE: Provide backwards compatibility for STLPort header
//
//@SEE_ALSO: package bsl+stdhdrs
//
//@AUTHOR: Arthur Chiu (achiu21)
//
//@DESCRIPTION: This component provides backwards compatibility for Bloomberg
// libraries that depend on non-standard STLPort header files.  This header
// includes the corresponding C++ header and provides any additional
// symbols that the STLPort header would define via transitive includes.

/* Coppied from stlport */
/* The header <memory.h> is not part of either the C or C++ standards, but is
 * in widespread use none the less.  It contains a portion of what is included
 * in the standard C header, <string.h>.  In the Sun compiler, <memory.h> is
 * broken wrt the use of namespaces, but <string.h> is fine.  Therefore, the
 * easiest way to provide the functionality of <memory.h> is to simply forward
 * it to <string.h>
 */

#include <bsl_cstring.h>

using std::memmove;
using std::memcpy;
using std::memchr;
using std::memcmp;
using std::memset;

// ---------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2009
//      All Rights Reserved.
//      Property of Bloomberg L.P. (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ----------------------------- END-OF-FILE ---------------------------------
