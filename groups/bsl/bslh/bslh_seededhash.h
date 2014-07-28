// bslh_seededhash.h                                                  -*-C++-*-
#ifndef INCLUDED_BSLH_SEEDEDHASH
#define INCLUDED_BSLH_SEEDEDHASH

#ifndef INCLUDED_BSLS_IDENT
#include <bsls_ident.h>
#endif
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide a struct to run seeded hashing algorithms on any type.
//
//@CLASSES:
//  bslh::Hash: Universal hashing functor that can apply any algorithm to types
//
//@SEE_ALSO: bslh_hash, bslh_seedgenerator
//
//@DESCRIPTION: This component provides a templated struct, 'bslh::SeededHash',
// which provides hashing functionality and is a drop in replacement for
// 'bsl::hash'.  It is similar to 'bslh::Hash', however, it is meant for hashes
// that require a seed. It takes a seed generator and uses that to create seeds
// to give the the hashing algorithsm.  'bslh::SeededHash' is a wrapper which
// adapts hashing algorithms from 'bslh' to match the inteface of 'bsl::hash'.
// 'bslh::SeededHash' is a universal hashing functor that will hash any type
// that implements 'hashAppend' using the hashing algorithm provided as a
// template parameter.  For more details, see:
// https://cms.prod.bloomberg.com/team/pages/viewpage.action?title=
// Modular+Hashing&spaceKey=bde
//
///Relationship to 'bslh::Hash'
/// - - - - - - - - - - - - - -
// 'bslh::SeededHash' is substantially similar to 'bslh::Hash'.
// 'bslh::SeededHash' presents a similar interface to that of 'bslh::Hash',
// however, it adds a constructor that accepts a seed generator. Because of the
// use of seeds, 'bslh::SeededHash' stores data and therefor does not allow the
// empty base optimization like 'bslh::Hash' does.

#ifndef INCLUDED_BSLSCM_VERSION
#include <bslscm_version.h>
#endif

#ifndef INCLUDED_BSLH_HASH
#include <bslh_hash.h>
#endif

#ifndef INCLUDED_STDDEF_H
#include <stddef.h>  // for 'size_t'
#define INCLUDED_STDDEF_H
#endif

namespace BloombergLP {

namespace bslh {

                        // ======================
                        // class bslh::SeededHash
                        // ======================

template <class SEED_GENERATOR, class HASH_ALGORITHM
                                                  = bslh::DefaultHashAlgorithm>
struct SeededHash
{
    // Wraps the parameterized 'HASH_ALGORITHM', which requires a seed, in an
    // interface that is a drop in replacement for 'bsl::hash'.
  private:
    // DATA
    char seed[HASH_ALGORITHM::k_SEED_LENGTH];

  public:
    // TYPES
    typedef size_t result_type;
        // Type of the hash that will be returned.

    // CREATORS
    SeededHash();
        // Create a 'bslh::SeededHash' which will default construct the
        // parameterized 'SEED_GENERATOR' to initialize the seed that will be
        // passed to the parameterized 'HASH_ALGORITHM' when it is used.

    explicit SeededHash(SEED_GENERATOR& seedGenerator);
        // Create a 'bslh::SeededHash' which will use the specified
        // 'seedGenerator' to initialize the seed that will be passed to the
        // parameterized 'HASH_ALGORITHM' when it is used.

    // ACCESSORS
    template <class TYPE>
    result_type operator()(TYPE const& type) const;
        // Returns a hash generated by the parameterized 'HASHALG' for the
        // specified 'type'.  The value returned by the 'HASH_ALGORITHM' is
        // converted to 'size_t' before returning

};

// CREATORS
template <class SEED_GENERATOR, class HASH_ALGORITHM>
SeededHash<SEED_GENERATOR, HASH_ALGORITHM>::SeededHash() {
    SEED_GENERATOR seedGenerator;
    seedGenerator.generateSeed(seed, HASH_ALGORITHM::k_SEED_LENGTH);
}

template <class SEED_GENERATOR, class HASH_ALGORITHM>
SeededHash<SEED_GENERATOR, HASH_ALGORITHM>::SeededHash(
                                               SEED_GENERATOR& seedGenerator) {
    seedGenerator.generateSeed(seed, HASH_ALGORITHM::k_SEED_LENGTH);
}

// ACCESSORS
template <class SEED_GENERATOR, class HASH_ALGORITHM>
template <class TYPE>
typename SeededHash<SEED_GENERATOR, HASH_ALGORITHM>::result_type
SeededHash<SEED_GENERATOR, HASH_ALGORITHM>::operator()(TYPE const& key) const
{
    HASH_ALGORITHM hashAlg(seed);
    hashAppend(hashAlg, key);
    return static_cast<result_type>(hashAlg.computeHash());
}


}  // close namespace bslh

}  // close namespace BloombergLP


#endif

// ----------------------------------------------------------------------------
// Copyright (C) 2014 Bloomberg Finance L.P.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------- END-OF-FILE ----------------------------------
