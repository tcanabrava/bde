// bsl_cfenv.h                                                        -*-C++-*-
#ifndef INCLUDED_BSL_CFENV
#define INCLUDED_BSL_CFENV

#include <bsls_ident.h>
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide functionality of the corresponding C++ Standard header.
//
//@SEE_ALSO: package bos+stdhdrs in the bos package group
//
//@DESCRIPTION: Provide types, in the 'bsl' namespace, equivalent to those
// defined in the corresponding C++ standard header.  Include the native
// compiler-provided standard header, and also directly include Bloomberg's
// implementation of the C++ standard type (if one exists).  Finally, place the
// included symbols from the 'std' namespace (if any) into the 'bsl' namespace.

#include <bsls_nativestd.h>

#include <cfenv>

namespace bsl {

    using native_std::feclearexcept;
    using native_std::feraiseexcept;
    using native_std::fegetexceptflag;
    using native_std::fesetexceptflag;
    using native_std::fegetround;
    using native_std::fesetround;
    using native_std::fegetenv;
    using native_std::fesetenv;
    using native_std::feholdexcept;
    using native_std::feupdateenv;
    using native_std::fetestexcept;
    using native_std::fenv_t;
    using native_std::fexcept_t;

}  // close package namespace

#endif

// ----------------------------------------------------------------------------
// Copyright 2015 Bloomberg Finance L.P.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ----------------------------- END-OF-FILE ----------------------------------
