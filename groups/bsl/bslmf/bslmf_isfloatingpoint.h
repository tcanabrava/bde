// bslmf_isfloatingpoint.h                                            -*-C++-*-
#ifndef INCLUDED_BSLMF_ISFLOATINGPOINT
#define INCLUDED_BSLMF_ISFLOATINGPOINT

#ifndef INCLUDED_BSLS_IDENT
#include <bsls_ident.h>
#endif
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide a compile-time check for floating-point types.
//
//@CLASSES:
//  bsl::is_floating_point: meta-function for determining floating-point types
//
//@SEE_ALSO: bslmf_integralconstant
//
//@AUTHOR:
//
//@DESCRIPTION: This component defines a meta-function,
// 'bsl::is_floating_point' , which may be used to query whether a type is a
// (possibly cv-qualified) floating-point type as defined in section 3.9.1.8 of
// the C++11 standard [basic.fundamental].
//
// 'bsl::is_floating_point' meets the requirements of the 'is_floating_point'
// template defined in the C++11 standard [meta.unary.cat].
//
///Usage
///-----
// In this section we show intended use of this component.
//
///Example 1: Verify Floating-Point Types
///- - - - - - - - - - - - - - - -
// Suppose that we want to assert whether a particular type is a floating-point
// type.
//
// First, we create two 'typedef's -- a floating-point type and a
// non-floating-point type:
//..
//  typedef void MyType;
//  typedef float  MyFloatingPointType;
//..
// Now, we instantiate the 'bsl::is_floating_point' template for each of the
// 'typedef's and assert the 'value' static data member of each instantiation:
//..
//  assert(false == bsl::is_floating_point<MyType>::value);
//  assert(true == bsl::is_floating_point<MyFloatingPointType>::value);
//..

#ifndef INCLUDED_BSLSCM_VERSION
#include <bslscm_version.h>
#endif

#ifndef INCLUDED_BSLMF_INTEGRALCONSTANT
#include <bslmf_integralconstant.h>
#endif

#ifndef INCLUDED_BSLMF_REMOVECV
#include <bslmf_removecv.h>
#endif

namespace BloombergLP {
namespace bslmf {

                         // ==========================
                         // struct IsFloatingPoint_Imp
                         // ==========================

template <typename TYPE>
struct IsFloatingPoint_Imp : bsl::false_type {
    // This 'struct' template provides a meta-function to determine whether the
    // (template parameter) 'TYPE' is a floating-point type.  This generic
    // default template derives from 'bsl::false_type'.  Template
    // specializations are provided (below) that derives from 'bsl::true_type'.
};

template <>
struct IsFloatingPoint_Imp<float> : bsl::true_type {
     // This specialization of 'IsFloatingPoint_Imp' derives from
     // 'bsl::true_type' for when the (template parameter) 'TYPE' is a
     // floating-point type.
};

template <>
struct IsFloatingPoint_Imp<double> : bsl::true_type {
     // This specialization of 'IsFloatingPoint_Imp' derives from
     // 'bsl::true_type' for when the (template parameter) 'TYPE' is a
     // floating-point type.
};

template <>
struct IsFloatingPoint_Imp<long double> : bsl::true_type {
     // This specialization of 'IsFloatingPoint_Imp' derives from
     // 'bsl::true_type' for when the (template parameter) 'TYPE' is a
     // floating-point type.
};

}
}

namespace bsl {

                         // ========================
                         // struct is_floating_point
                         // ========================

template <typename TYPE>
struct is_floating_point
    : BloombergLP::bslmf::IsFloatingPoint_Imp<
                                typename remove_cv<TYPE>::type>::type {
    // This 'struct' template implements the 'is_floating_point' meta-function
    // defined in the C++11 standard [meta.unary.cat] to determine if the
    // (template parameter) 'TYPE' is a floating-point type.  This 'struct'
    // derives from 'bsl::true_type' if the 'TYPE' is a floating-point type and
    // 'bsl::false_type' otherwise.
};

}

#endif

// ---------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2012
//      All Rights Reserved.
//      Property of Bloomberg L.P. (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ----------------------------- END-OF-FILE ---------------------------------
