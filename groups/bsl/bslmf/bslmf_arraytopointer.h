// bslmf_arraytopointer.h                                             -*-C++-*-
#ifndef INCLUDED_BSLMF_ARRAYTOPOINTER
#define INCLUDED_BSLMF_ARRAYTOPOINTER

#ifndef INCLUDED_BSLS_IDENT
#include <bsls_ident.h>
#endif
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide a meta-function to convert array types to pointer types.
//
//@CLASSES:
//  bslmf::ArrayToPointer: convert an array type to a pointer type
//  bslmf::ArrayToConstPointer: convert an array type to a 'const' pointer type
//
//@SEE_ALSO: bslmf_isarray, bslmf_forwardingtype
//
//@AUTHOR: Ilougino Rocha (irocha)
//
//@DESCRIPTION: This component provides a meta function for converting array
// types to pointer types.  The utility is generally used for in templates that
// require forwarding or storage of arguments that are passed a arrays(e.g.,
// "string literals").  For non array types, the type is left unmodified.  Note
// that 'bslmf::ArrayToPointer' and 'bslmf::ArrayToConstPointer' retain the CV
// qualifiers of the original type.  In other words, if the original array type
// was 'const' or 'volatile', or 'const volatile', the converted pointer type
// will also be 'const', 'volatile', or 'const volatile' respectively.
//
// When an explicit const pointer pointer type is needed(such as when accepting
// as argument, then 'bslmf::ArrayToConstPointer' should be used.
//
///Usage
///-----
// For example:
//..
//  assert(1 == bslmf::IsSame<bslmf::ArrayToPointer<int[5]>::Type
//                          , int *>::VALUE);
//  assert(1 == bslmf::IsSame<bslmf::ArrayToPointer<int *>::Type
//                          , int *>::VALUE);
//  assert(0 == bslmf::IsSame<bslmf::ArrayToPointer<int (*)[5]>::Type]
//                          , int **>::VALUE);
//..

#ifndef INCLUDED_BSLSCM_VERSION
#include <bslscm_version.h>
#endif

#ifndef INCLUDED_CSTDDEF
#include <cstddef>       // for 'std::size_t'
#define INCLUDED_CSTDDEF
#endif

namespace BloombergLP {

namespace bslmf {

                            // =====================
                            // struct ArrayToPointer
                            // =====================

template <typename TYPE, typename ORIGINAL_TYPE>
struct ArrayToPointer_Imp;

template <typename TYPE>
struct ArrayToPointer {
    typedef typename ArrayToPointer_Imp<TYPE, TYPE>::Type Type;
};

template <typename TYPE>
struct ArrayToPointer<TYPE &> {
    typedef typename ArrayToPointer_Imp<TYPE, TYPE &>::Type Type;
};

                          // ==========================
                          // struct ArrayToConstPointer
                          // ==========================

template <typename TYPE>
struct ArrayToConstPointer {
    typedef typename ArrayToPointer_Imp<const TYPE, TYPE>::Type Type;
};

template <typename TYPE>
struct ArrayToConstPointer<TYPE &> {
    typedef typename ArrayToPointer_Imp<const TYPE, TYPE &>::Type Type;
};

                         // =========================
                         // struct ArrayToPointer_Imp
                         // =========================

template <typename TYPE, typename ORIGINAL_TYPE>
struct ArrayToPointer_Imp {
    typedef ORIGINAL_TYPE Type;
};

template <typename TYPE, std::size_t NUM_ELEMENTS, typename UNUSED>
struct ArrayToPointer_Imp<TYPE [NUM_ELEMENTS], UNUSED> {
    typedef TYPE *Type;
};

template <typename TYPE, typename UNUSED>
struct ArrayToPointer_Imp<TYPE [], UNUSED> {
    typedef TYPE *Type;
};

}  // close package namespace

// ===========================================================================
//                           BACKWARD COMPATIBILITY
// ===========================================================================

#ifdef bslmf_ArrayToConstPointer
#undef bslmf_ArrayToConstPointer
#endif
#define bslmf_ArrayToConstPointer bslmf::ArrayToConstPointer
    // This alias is defined for backward compatibility.

#ifdef bslmf_ArrayToPointer
#undef bslmf_ArrayToPointer
#endif
#define bslmf_ArrayToPointer bslmf::ArrayToPointer
    // This alias is defined for backward compatibility.

}  // close enterprise namespace

#endif

// ---------------------------------------------------------------------------
// NOTICE:
//      Copyright (C) Bloomberg L.P., 2005
//      All Rights Reserved.
//      Property of Bloomberg L.P. (BLP)
//      This software is made available solely pursuant to the
//      terms of a BLP license agreement which governs its use.
// ----------------------------- END-OF-FILE ---------------------------------
