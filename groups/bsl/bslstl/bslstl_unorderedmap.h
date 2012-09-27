// bslstl_unorderedmap.h                                              -*-C++-*-
#ifndef INCLUDED_BSLSTL_UNORDEREDMAP
#define INCLUDED_BSLSTL_UNORDEREDMAP

#ifndef INCLUDED_BSLS_IDENT
#include <bsls_ident.h>
#endif
BSLS_IDENT("$Id: $")

//@PURPOSE:Provide an STL-compliant unordered_map class.
//
//@CLASSES:
//   bsl::unordered_map : STL-compatible unordered map container
//
//@AUTHOR: Alisdair Meredith (ameredith1) Stefano Pacifico (spacifico1)
//
//@SEE_ALSO: bsl+stdhdrs
//
//@DESCRIPTION: This component defines a single class template 'unordered_map',
// implementing the standard container holding a collection of unique keys with
// no guarantees on ordering, each mapped to an associated value.
//
// An instantiation of 'unordered_map' is an allocator-aware, value-semantic
// type whose salient attributes are its size (number of keys) and the set of
// values the 'unordered_map' contains, without regard to their order.  If
// 'unordered_map' is instantiated with a key type or associated value type
// that is not itself value-semantic, then it will not retain all of its
// value-semantic qualities.  In particular, if the key or value type cannot be
// tested for equality, then an 'unordered_map' containing that type cannot be
// tested for equality.  It is even possible to instantiate 'unordered_map'
// with type that do not have an accessible copy-constructor, in which case the
// 'unordered_map' will not be copyable.  Note that the equality operator for
// each element is used to determine when two 'unordered_map' objects have the
// same value, and not the equality comparator supplied at construction.
//
// An 'unordered_map' meets the requirements of an unordered associative
// container with forward iterators in the C++11 standard [unord].  The
// 'unordered_map' implemented here adheres to the C++11 standard, except that
// it does not have interfaces that take rvalue references, 'initializer_list',
// 'emplace', or operations taking a variadic number of template parameters.
// Note that excluded C++11 features are those that require (or are greatly
// simplified by) C++11 compiler support.
//
///Requirements on 'KEY' and 'VALUE'
///--------------------------------------------
// An 'unordered_map' instantiation is a fully "Value-Semantic Type" (see
// {'bsldoc_glossary'}) only if the supplied 'KEY' and 'VALUE'
// template parameters are fully value-semantic.  It is possible to instantiate
// an 'unoredered_map' with 'KEY' and 'VALUE' parameter arguments
// that do not provide a full set of value-semantic operations, but
// then some methods of the container may not be instantiable.  The following
// terminology, adopted from the C++11 standard, is used in the function
// documentation of 'unordered_map' to describe a function's requirements for
// the 'KEY' and 'VALUE' template parameters.  These terms are also
// defined in section [utility.arg.requirements] of the C++11 standard.  Note
// that, in the context of an 'unordered_map' instantiation, the requirements
// apply specifically to the 'unordered_map's entry type, 'value_type', which
// is an alias for std::pair<const KEY, VALUE_TYPE>'.
//
//: "default-constructible": The type provides an accessible default
//:                          constructor.
//:
//: "copy-constructible": The type provides an accessible copy constructor.
//:
//: "equality-comparable": The type provides an equality-comparison operator
//:     that defines an equivalence relationship and is both reflexive and
//:     transitive.
//
///Memory Allocation
///-----------------
// The type supplied as the 'ALLOCATOR' template parameter determines how
// memory will be allocated.  The 'unordered_map' template supports
// allocators meeting the requirements of the C++11 standard
// [allocator.requirements], and in addition it supports scoped-allocators
// derived from the 'bslma::Allocator' memory allocation protocol.  Clients
// intending to use 'bslma' style allocators should use the template's default
// 'ALLOCATOR' type: The default type for the 'ALLOCATOR' template parameter,
// 'bsl::allocator', provides a C++11 standard-compatible adapter for a
// 'bslma::Allocator' object.
//
///'bslma'-Style Allocators
/// - - - - - - - - - - - -
// If the parameterized 'ALLOCATOR' type of an 'unordered_map' instantiation
// is 'bsl::allocator', then objects of that set type will conform to the
// standard behavior of a 'bslma'-allocator-enabled type.  Such a set accepts
// an optional 'bslma::Allocator' argument at construction.  If the address of
// a 'bslma::Allocator' object is explicitly supplied at construction, it will
// be used to supply memory for the 'unordered_map' throughout its lifetime;
// otherwise, the 'unordered_map' will use the default allocator installed at
// the time of the 'unordered_map's construction (see 'bslma_default').  In
// addition to directly allocating memory from the indicated
// 'bslma::Allocator', an 'unordered_map' supplies that allocator's address to
// the constructors of contained objects of the parameterized 'KEY' types
// with the 'bslalg::TypeTraitUsesBslmaAllocator' trait.
//
///Operations
///----------
// This section describes the run-time complexity of operations on instances
// of 'unordered_map':
//..
//  Legend
//  ------
//  'K'             - parameterized 'KEY' type of the unordered map
//  'V'             - parameterized 'VALUE' type of the unordered map
//  'a', 'b'        - two distinct objects of type 'unordered_map<K, V>'
//  'n', 'm'        - number of elements in 'a' and 'b' respectively
//  'value_type'    - unordered_map<K, V>::value_type
//  'c'             - comparator providing an ordering for objects of type 'K'
//  'al             - an STL-style memory allocator
//  'i1', 'i2'      - two iterators defining a sequence of 'value_type' objects
//  'k'             - an object of type 'K'
//  'v'             - an object of type 'value_type'
//  'p1', 'p2'      - two iterators belonging to 'a'
//  distance(i1,i2) - the number of elements in the range [i1, i2)
//  distance(p1,p2) - the number of elements in the range [p1, p2)
//
//  +----------------------------------------------------+--------------------+
//  | Operation                                          | Complexity         |
//  +====================================================+====================+
//  | unordered_map<K, V> a;    (default construction)   | O[1]               |
//  | unordered_map<K, V> a(al);                         |                    |
//  | unordered_map<K, V> a(c, al);                      |                    |
//  +----------------------------------------------------+--------------------+
//  | unordered_map<K, V> a(b); (copy construction)      | Average: O[n]      |
//  | unordered_map<K, V> a(b, al);                      | Worst:   O[n^2]    |
//  +----------------------------------------------------+--------------------+
//  | unoredered_map<K, V> a(n);                         | O[n]               |
//  | unoredered_map<K, V> a(n, hf);                     |                    |
//  | unoredered_map<K, V> a(n, hf, eq);                 |                    |
//  | unoredered_map<K, V> a(n, hf, eq, al);             |                    |
//  +----------------------------------------------------+--------------------+
//  | unoredered_map<K, V> a(i1, i2);                    | Average: O[        |
//  | unoredered_map<K, V> a(i1, i2, n)                  |   distance(i1, i2)]|
//  | unoredered_map<K, V> a(i1, i2, n, hf);             | Worst:   O[n^2]    |
//  | unoredered_map<K, V> a(i1, i2, n, hf, eq);         |                    |
//  | unordered_map<K, V> a(i1, i2, n, hf, eq, al);      |                    |
//  |                                                    |                    |
//  | a.~unordered_map<K, V>(); (destruction)            | O[n]               |
//  +----------------------------------------------------+--------------------+
//  | a = b;          (assignment)                       | Average: O[n]      |
//  |                                                    | Worst:   O[n^2]    |
//  +----------------------------------------------------+--------------------+
//  | a.begin(), a.end(), a.cbegin(), a.cend(),          | O[1]               |
//  +----------------------------------------------------+--------------------+
//  | a == b, a != b                                     | Best:  O[n]        |
//  |                                                    | Worst: O[n^2]      |
//  +----------------------------------------------------+--------------------+
//  | a.swap(b), swap(a,b)                               | O[1] if 'a' and    |
//  |                                                    | 'b' use the same   |
//  |                                                    | allocator,         |
//  |                                                    | O[n + m] otherwise |
//  +----------------------------------------------------+--------------------+
//  | a.key_eq()                                         | O[1]               |
//  +----------------------------------------------------+--------------------+
//  | a.hash_function()                                  | O[1]               |
//  +----------------------------------------------------+--------------------+
//  | a.size()                                           | O[1]               |
//  +----------------------------------------------------+--------------------+
//  | a.max_size()                                       | O[1]               |
//  +----------------------------------------------------+--------------------+
//  | a.empty()                                          | O[1]               |
//  +----------------------------------------------------+--------------------+
//  | get_allocator()                                    | O[1]               |
//  +----------------------------------------------------+--------------------+
//  | a[k]                                               | O[n]               |
//  +----------------------------------------------------+--------------------+
//  | a.at(k)                                            | O[n]               |
//  +----------------------------------------------------+--------------------+
//  | a.insert(v)                                        | Average: O[1]      |
//  |                                                    | Worst:   O[n]      |
//  +----------------------------------------------------+--------------------+
//  | a.insert(p1, v)                                    | Average: O[1]      |
//  |                                                    | Worst:   O[n]      |
//  +----------------------------------------------------+--------------------+
//  | a.insert(i1, i2)                                   | Average O[         |
//  |                                                    |   distance(i1, i2)]|
//  |                                                    | Worst:  O[ n *     |
//  |                                                    |   distance(i1, i2)]|
//  +----------------------------------------------------+--------------------+
//  | a.erase(p1)                                        | Average: O[1]      |
//  |                                                    | Worst:   O[n]      |
//  +----------------------------------------------------+--------------------+
//  | a.erase(k)                                         | Average: O[        |
//  |                                                    |         a.count(k)]|
//  |                                                    | Worst:   O[n]      |
//  +----------------------------------------------------+--------------------+
//  | a.erase(p1, p2)                                    | Average: O[        |
//  |                                                    |   distance(p1, p2)]|
//  |                                                    | Worst:   O[n]      |
//  +----------------------------------------------------+--------------------+
//  | a.clear()                                          | O[n]               |
//  +----------------------------------------------------+--------------------+
//  | a.find(k)                                          | Average: O[1]      |
//  |                                                    | Worst:   O[n]      |
//  +----------------------------------------------------+--------------------+
//  | a.count(k)                                         | Average: O[1]      |
//  |                                                    | Worst:   O[n]      |
//  +----------------------------------------------------+--------------------+
//  | a.equal_range(k)                                   | Average: O[        |
//  |                                                    |         a.count(k)]|
//  |                                                    | Worst:   O[n]      |
//  +----------------------------------------------------+--------------------+
//  | a.bucket_count()                                   | O[1]               |
//  +----------------------------------------------------+--------------------+
//  | a.max_bucket_count()                               | O[1]               |
//  +----------------------------------------------------+--------------------+
//  | a.bucket(k)                                        | O[1]               |
//  +----------------------------------------------------+--------------------+
//  | a.bucket_size(k)                                   | O[a.bucket_size(k)]|
//  +----------------------------------------------------+--------------------+
//  | a.load_factor()                                    | O[1]               |
//  +----------------------------------------------------+--------------------+
//  | a.max_load_factor()                                | O[1]               |
//  | a.max_load_factor(z)                               | O[1]               |
//  +----------------------------------------------------+--------------------+
//  | a.rehash(k)                                        | Average: O[n]      |
//  |                                                    | Worst:   O[n^2]    |
//  +----------------------------------------------------+--------------------+
//  | a.resize(k)                                        | Average: O[n]      |
//  |                                                    | Worst:   O[n^2]    |
//  +----------------------------------------------------+--------------------+
//..
///Usage
///-----


// Prevent 'bslstl' headers from being included directly in 'BSL_OVERRIDES_STD'
// mode.  Doing so is unsupported, and is likely to cause compilation errors.
#if defined(BSL_OVERRIDES_STD) && !defined(BSL_STDHDRS_PROLOGUE_IN_EFFECT)
#error "<bslstl_unorderedmap.h> header can't be included directly in \
BSL_OVERRIDES_STD mode"
#endif

#ifndef INCLUDED_BSLSCM_VERSION
#include <bslscm_version.h>
#endif

#ifndef INCLUDED_BSLS_ASSERT
#include <bsls_assert.h>
#endif

#ifndef INCLUDED_BSLSTL_ALLOCATORATOR
#include <bslstl_allocator.h>  // Can probably escape with a fwd-decl, but not
#endif                         // very user friendly

#ifndef INCLUDED_BSLSTL_ALLOCATORATORTRAITS
#include <bslstl_allocatortraits.h>
#endif

#ifndef INCLUDED_BSLSTL_EQUALTO
#include <bslstl_equalto.h>
#endif

#ifndef INCLUDED_BSLSTL_HASH
#include <bslstl_hash.h>
#endif

#ifndef INCLUDED_BSLSTL_HASHTABLE
#include <bslstl_hashtable.h>
#endif

#ifndef INCLUDED_BSLSTL_HASHTABLEBUCKETITERATOR
#include <bslstl_hashtablebucketiterator.h>
#endif

#ifndef INCLUDED_BSLSTL_HASHTABLEITERATOR
#include <bslstl_hashtableiterator.h>
#endif

#ifndef INCLUDED_BSLSTL_ITERATORUTIL
#include <bslstl_iteratorutil.h>
#endif

#ifndef INCLUDED_BSLSTL_PAIR
#include <bslstl_pair.h>
#endif

#ifndef INCLUDED_BSLSTL_UNORDEREDMAPKEYCONFIGURATION
#include <bslstl_unorderedmapkeyconfiguration.h>
#endif

#ifndef INCLUDED_BSLSTL_STDEXCEPTUTIL
#include <bslstl_stdexceptutil.h> // required to implement 'at'
#endif

#ifndef INCLUDED_CSTDDEF
#include <cstddef>  // for 'std::size_t'
#define INCLUDED_CSTDDEF
#endif

namespace BloombergLP
{
namespace bslalg { class BidirectionalLink; }
}

namespace bsl {
                        // =======================
                        // class bsl::unorderedmap
                        // =======================

template <
        class KEY,
        class VALUE,
        class HASH  = bsl::hash<KEY>,
        class EQUAL = bsl::equal_to<KEY>,
        class ALLOCATOR = bsl::allocator<bsl::pair<const KEY, VALUE> > >
class unordered_map {
    // This class template implements a value-semantic container type holding
    // an unordered sequence of unique keys (of the parameterized type, 'KEY').
    //
    // This class:
    //: o supports a complete set of *value-semantic* operations
    //:   o except for 'bdex' serialization
    //: o is *exception-neutral* (agnostic except for the 'at' method)
    //: o is *alias-safe*
    //: o is 'const' *thread-safe*
    // For terminology see {'bsldoc_glossary'}.

  private:

    // PRIVATE TYPES
    typedef bsl::allocator_traits<ALLOCATOR> AllocatorTraits;
        // This typedef is an alias for the allocator traits type associated
        // with this container.

    typedef bsl::pair<const KEY, VALUE>  ValueType;
        // This typedef is an alias for the type of key-value pair objects
        // maintained by this map.

    typedef ::BloombergLP::bslstl::UnorderedMapKeyPolicy<ValueType> ListPolicy;
        // This typedef is an alias for the policy used internally by this
        // container to extract the 'KEY' value from the key-value pair
        // objects maintained by this map.

    typedef ::BloombergLP::bslstl::HashTable<ListPolicy, HASH, EQUAL, ALLOCATOR>
                                                                    HashTable;
        // This typedef is an alias for the template instantion of the
        // underlying 'bslslt::HashTable' used in this container.

    typedef ::BloombergLP::bslalg::BidirectionalLink HashTableLink;
        // This typedef is an alias for the type of links of the linked list of
        // elements held by the underlying 'bslstl::HashTable'.

    typedef typename HashTable::NodeType HashTableNode;
        // This typedef is an alias for the type of nodes that hold the values
        // in this container.
  
  public:
    // PUBLIC TYPES
    typedef KEY                                        key_type;
    typedef VALUE                                      mapped_type;
    typedef bsl::pair<const KEY, VALUE>                value_type;
    typedef HASH                                       hasher;
    typedef EQUAL                                      key_equal;
    typedef ALLOCATOR                                  allocator_type;

    typedef typename allocator_type::reference         reference;
    typedef typename allocator_type::const_reference   const_reference;

    typedef typename AllocatorTraits::size_type        size_type;
    typedef typename AllocatorTraits::difference_type  difference_type;
    typedef typename AllocatorTraits::pointer          pointer;
    typedef typename AllocatorTraits::const_pointer    const_pointer;

    typedef ::BloombergLP::bslstl::HashTableIterator<
                                         value_type, difference_type> iterator;
    typedef ::BloombergLP::bslstl::HashTableIterator<
                             const value_type, difference_type> const_iterator;
    typedef ::BloombergLP::bslstl::HashTableBucketIterator<
                                   value_type, difference_type> local_iterator;
    typedef ::BloombergLP::bslstl::HashTableBucketIterator<
                       const value_type, difference_type> const_local_iterator;

  private:
    // DATA
    HashTable d_impl;  // underlying hash table of this container

  public:
    // CREATORS
    explicit unordered_map(size_type n = 0,
                           const hasher& hash = hasher(),
                           const key_equal& equal = key_equal(),
                           const allocator_type& allocator = allocator_type());
        // Construct an empty unordered map.  Optionally specify a 'hasher'
        // used to generate the hash values associated to the key-value pairs
        // contained in this object.  If 'hash' is not supplied, a
        // default-constructed object of type 'hasher' is used.  Optionally
        // specify a key-equality functor 'equal' used to verify that two
        // key values are the same.  If 'key_equal' is not supplied, a
        // default-constructed object of type 'key_equal' is used. Optionally
        // specify an 'allocator' used to supply memory.  If 'allocator' is not
        // supplied, a default-constructed object of the (template parameter)
        // type 'allocator_type' is used.  If the 'allocator_type' is
        // 'bsl::allocator' (the default), then 'allocator' shall be
        // convertible to 'bslma::Allocator *'.  If the 'ALLOCATOR' is
        // 'bsl::allocator' and 'allocator' is not supplied, the currently
        // installed default allocator will be used to supply memory.

    explicit unordered_map(const allocator_type& allocator);
        // Construct an empty unordered map that uses the specified 'allocator'
        // to supply memory. Use a default-constructed object of type 'hasher'
        // to generate the hash values associated to the key-value pairs
        // contained in this object. Also, use a default-constructed object of
        // type 'key_equal' to verify that two key values are the same.  If the
        // 'allocator_type' is 'bsl::allocator' (the default), then 'allocator'
        // shall be convertible to 'bslma::Allocator *'.

    unordered_map(const unordered_map&  original);
    unordered_map(const unordered_map&  original,
                  const allocator_type& allocator);
        // Construct an unordered map having the same value as that of the
        // specified 'original'. Use a default-constructed object of type
        // 'hasher' to generate the hash values associated to the key-value
        // pairs contained in this object. Also, use a default-constructed
        // object of type 'key_equal' to verify that two key values are the
        // same.  Optionally specify an 'allocator' used to supply memory.  If
        // 'allocator' is not supplied, a default-constructed object of type
        // 'allocator_type' is used.  If the 'allocator_type' is
        // 'bsl::allocator' (the default), then 'allocator' shall be
        // convertible to 'bslma::Allocator *'.

    template <class InputIterator>
    unordered_map(InputIterator first, InputIterator last,
                  size_type n = 0,
                  const hasher& hash = hasher(),
                  const key_equal& equal = key_equal(),
                  const allocator_type& allocator = allocator_type());
        // Construct an empty unordered map and insert each 'value_type' object
        // in the sequence starting at the specified 'first' element, and
        // ending immediately before the specified 'last' element, ignoring
        // those pairs having a key that appears earlier in the sequence. .
        // Optionally specify a 'hasher' used to generate the hash values
        // associated to the key-value pairs contained in this object.  If
        // 'hash' is not supplied, a default-constructed object of type
        // 'hasher' is used.  Optionally specify a key-equality functor
        // 'equal' used to verify that two key values are the same.  If
        // 'equal' is not supplied, a default-constructed object of type
        // 'key_equal' is used. Optionally specify an 'allocator' used to
        // supply memory.  If 'allocator' is not supplied, a
        // default-constructed object of the (template parameter) type
        // 'allocator_type' is used.  If the 'allocator_type' is
        // 'bsl::allocator' (the default), then 'allocator' shall be
        // convertible to 'bslma::Allocator *'.  If the 'allocator_type' is
        // 'bsl::allocator' and 'allocator' is not supplied, the currently
        // installed default allocator will be used to supply memory.
        //  The (template parameter) type 'INPUT_ITERATOR' shall meet the
        //  requirements of an input iterator defined in the C++11 standard
        //  [24.2.3] providing access to values of a type convertible to
        //  'value_type'.  The behavior is undefined unless 'first' and 'last'
        //  refer to a sequence of valid values where 'first' is at a position
        //  at or before 'last'.  This method requires that the (template
        //  parameter) types 'KEY' and 'VALUE' both be "copy-constructible"
        //  (see {Requirements on 'KEY' and 'VALUE'}).

    ~unordered_map();
        // Destroy this object.

    // MANIPULATORS
    unordered_map& operator=(const unordered_map&);
        // Assign to this object the value, hasher, and key-equality functor of
        // the specified 'rhs' object, propagate to this object the allocator
        // of 'rhs' if the
        // 'ALLOCATOR' type has trait 'propagate_on_container_copy_assignment',
        // and return a reference providing modifiable access to this object.
        // This method requires that the (template parameter types) 'KEY'
        // and 'VALUE' both be "copy-constructible" (see {Requirements on
        // 'KEY' and 'VALUE'}).

    mapped_type& operator[](const key_type& key);
        // Return a reference providing modifiable access to the mapped-value
        // associated with the specified 'key'; if this container does not
        // already contain a 'value_type' object with 'key', first insert a new
        // 'value_type' object having 'key' and a default-constructed
        // 'VALUE' object, and return a reference to the mapped value.
        // This method requires that the (template parameter) types 'KEY'
        // and 'VALUE' both be "default-constructible" (see {Requirements
        // on 'KEY' and 'VALUE'}).

    mapped_type& at(const key_type& key);
        // Return a reference providing modifiable access to the mapped-value
        // associated with the specified 'key', if such an entry exists;
        // otherwise throw an 'std::out_of_range' exception.  Note that this
        // method is not exception agnostic.

    iterator begin();
        // Return an iterator providing modifiable access to the first
        // 'value_type' object in the sequence of 'value_type' objects
        // maintained by this map, or the 'end' iterator if this map is empty.

    iterator end();
        // Return an iterator providing modifiable access to the past-the-end
        // element in the sequence of 'value_type' objects maintained by this
        // map.

    local_iterator begin(size_type index);
        // Return a local iterator providing modifiable access to the first
        // 'value_type' object in the sequence of 'value_type' objects
        // of the bucket having the specified 'index' in the array of buckets
        // maintained by this map, or the 'end(index)' otherwise.

    local_iterator end(size_type n);
        // Return a local iterator providing modifiable access to the
        // past-the-end element in the sequence of 'value_type' objects
        // of the bucket having the specified 'index' in the array of buckets
        // maintained by this unordered map.

    pair<iterator, bool> insert(const value_type& value);
        // Insert the specified 'value' into this unprdered map if the key (the
        // 'first' element) of the 'value' does not already exist in this map;
        // otherwise, if a 'value_type' object having the same key as 'value'
        // already exists in this map, this method has no effect.  Return a
        // pair whose 'first' member is an iterator referring to the (possibly
        // newly inserted) 'value_type' object in this map whose key is the
        // same as that of 'value', and whose 'second' member is 'true' if a
        // new value was inserted, and 'false' if the value was already
        // present.  This method requires that the (template parameter) types
        // 'KEY' and 'VALUE_TYPE' types both be "copy-constructible" (see
        // {Requirements on 'KEY' and 'VALUE_TYPE'}).

    iterator insert(const_iterator hint, const value_type& obj);
        // Insert the specified 'value' into this map (in constant
        // time if the specified 'hint' is a valid element f the bucket to
        // which 'value' belongs), if the key (the 'first' element) of the
        // 'value' does not already exist in this map; otherwise, if a
        // 'value_type' object having the same key as 'value' already exists in
        // this map, this method has no effect.  Return an iterator referring
        // to the (possibly newly inserted) 'value_type' object in this map
        // whose key is the same as that of 'value'.  If 'hint' is not a valid
        // immediate successor to the key of 'value', this operation will have
        // worst case O[N] and average case constant time complexity, where 'N'
        // is the size of this map.  The behavior is undefined unless 'hint' is
        // a valid iterator into this unordered map.  This method requires that
        // the (template parameter) types 'KEY' and 'VALUE_TYPE' both be
        // "copy-constructible" (see {Requirements on 'KEY' and
        // 'VALUE_TYPE'}).

    template <class INPUT_ITERATOR>
    void insert(INPUT_ITERATOR first, INPUT_ITERATOR last);
        // Insert into this map the value of each 'value_type' object in the
        // range starting at the specified 'first' iterator and ending
        // immediately before the specified 'last' iterator, whose key is not
        // already contained in this map.  The (template parameter) type
        // 'INPUT_ITERATOR' shall meet the requirements of an input iterator
        // defined in the C++11 standard [24.2.3] providing access to values of
        // a type convertible to 'value_type'.  This method requires that
        // the (template parameter) types 'KEY' and 'VALUE_TYPE' both be
        // "copy-constructible" (see {Requirements on 'KEY' and
        // 'VALUE_TYPE'}).

    iterator erase(const_iterator position);
        // Remove from this unordered map the 'value_type' object at the
        // specified 'position', and return an iterator referring to the
        // element immediately following the removed element, or to the
        // past-the-end position if the removed element was the last element in
        // the sequence of elements maintained by this map.  The behavior is
        // undefined unless 'position' refers to a 'value_type' object in this
        // unordered map.

    size_type erase(const key_type& key);
        // Remove from this map the 'value_type' object having the specified
        // 'key', if it exists, and return 1; otherwise, if there is no
        // 'value_type' object having 'key', return 0 with no other
        // effect.

    iterator erase(const_iterator first, const_iterator last);
        // Remove from this map the 'value_type' objects starting at the
        // specified 'first' position up to, but including the specified 'last'
        // position, and return 'last'.  The behavior is undefined unless
        // 'first' and 'last' either refer to elements in this map or are the
        // 'end' iterator, and the 'first' position is at or before the 'last'
        // position in the ordered sequence provided by this container.

    void swap(unordered_map&);
        // Exchange the value of this object as well as its hasher and
        // key-equality functor with those of the specified 'other' object.
        // Additionally if
        // 'bslstl::AllocatorTraits<ALLOCATOR>::propagate_on_container_swap' is
        // 'true' then exchange the allocator of this object with that of the
        // 'other' object, and do not modify either allocator otherwise.  This
        // method provides the no-throw exception-safety guarantee and
        // guarantees O[1] complexity.  The behavior is undefined is unless
        // either this object was created with the same allocator as 'other' or
        // 'propagate_on_container_swap' is 'true'.

    void clear();
        // Remove all entries from this container.  Note that the container is
        // empty after this call, but allocated memory may be retained for
        // future use.

    iterator find(const key_type& key);
        // Return an iterator providing modifiable access to the 'value_type'
        // object in this map having the specified 'key', if such an entry
        // exists, and the past-the-end ('end') iterator otherwise.

    pair<iterator, iterator> equal_range(const key_type& k);
        // Return a pair of iterators providing modifiable access to the
        // sequence of 'value_type' objects in this unordered map having the
        // specified 'key', where the the first iterator is positioned at the
        // start of  the sequence, and the second is positioned one past the
        // end of the sequence.  If this unordered map contains no 'value_type'
        // objects having 'key', then the two returned iterators will have the
        // same value.  Note that since a map maintains unique keys, the range
        // will contain at most one element.

    // ACCESSORS
    allocator_type get_allocator() const;
        // Return (a copy of) the allocator used for memory allocation by this
        // map.

    const_iterator begin() const;
        // Return an iterator providing non-modifiable access to the first
        // 'value_type' object in the sequence of 'value_type' objects
        // maintained by this map, or the 'end' iterator if this map is empty.

    const_iterator end() const;
        // Return an iterator providing non-modifiable access to the
        // past-the-end element in the sequence of 'value_type' objects
        // maintained by this map.
    
    const_iterator cbegin() const;
        // Return an iterator providing non-modifiable access to the first
        // 'value_type' object in the sequence of 'value_type' objects
        // maintained by this map, or the 'end' iterator if this map is empty.

    const_iterator cend() const;
        // Return an iterator providing non-modifiable access to the
        // past-the-end element in the sequence of 'value_type' objects
        // maintained by this map.

    const_local_iterator begin(size_type index) const;
        // Return a local iterator providing non-modifiable access to the first
        // 'value_type' object in the sequence of 'value_type' objects of the
        // bucket having the specified 'index' in the array of buckets
        // maintained by this map, or the 'end(index)' otherwise.

    const_local_iterator end(size_type n) const;
        // Return a local iterator providing non-modifiable access to the
        // past-the-end element in the sequence of 'value_type' objects of the
        // bucket having the specified 'index' in the array of buckets
        // maintained by this unordered map.

    const_local_iterator cbegin(size_type index) const;
        // Return a local iterator providing non-modifiable access to the first
        // 'value_type' object in the sequence of 'value_type' objects of the
        // bucket having the specified 'index' in the array of buckets
        // maintained by this map, or the 'end(index)' otherwise.

    const_local_iterator cend(size_type n) const;
        // Return a local iterator providing non-modifiable access to the
        // past-the-end element in the sequence of 'value_type' objects of the
        // bucket having the specified 'index' in the array of buckets
        // maintained by this unordered map.

    bool empty() const;
        // Return 'true' if this map contains no elements, and 'false'
        // otherwise.

    size_type size() const;
        // Return the number of elements in this map.

    size_type max_size() const;
        // Return a theoretical upper bound on the largest number of elements
        // that this map could possibly hold.  Note that there is no guarantee
        // that the map can successfully grow to the returned size, or even
        // close to that size without running out of resources.

    const mapped_type& at(const key_type& key) const;
        // Return a reference providing non-modifiable access to the
        // mapped-value associated with the specified 'key', if such an entry
        // exists; otherwise throw an 'std::out_of_range' exception.  Note
        // that this method is not exception agnostic.

    hasher hash_function() const;
        // Return (a copy of) the hash unary functor (used in this unordered
        // map) that returns the 'size_t' hash value of a 'value_type' object.
    
    key_equal key_eq() const;
        // Return (a copy of) the key-equality binary functor that returns true
        // if the value of two 'key_type' objects is the same, and false
        // otherwise.

    const_iterator find(const key_type& key) const;
        // Return an iterator providing non-modifiable access to the
        // 'value_type' object in this map having the specified 'key', if such
        // an entry exists, and the past-the-end ('end') iterator otherwise.

    size_type count(const key_type& key) const;
        // Return the number of 'value_type' objects within this container
        // having the specified 'key'.  Note that since an unordered map
        // maintains unique keys, the returned value will be either 0 or 1.
    
    pair<const_iterator, const_iterator> equal_range(
                                                    const key_type& key) const;
        // Return a pair of iterators providing non-modifiable access to the
        // sequence of 'value_type' objects in this container having the
        // specified 'key', where the the first iterator is positioned at the
        // start of the sequence and the second iterator is positioned one past
        // the end of the sequence.  If this map contains no 'value_type'
        // objects having 'key' then the two returned iterators will have the
        // same value.  Note that since a map maintains unique keys, the range
        // will contain at most one element.

    // bucket interface
    size_type      bucket_count() const;
        // Return the number of buckets in the array of buckets of this
        // unordered map.
    
    size_type max_bucket_count() const;
        // Return a theoretical upper bound on the largest number of buckets
        // that this container could possibly hold.  Note that there is no
        // guarantee that the map can successfully grow to the returned size,
        // or even close to that size without running out of resources.
    
    size_type bucket_size(size_type index) const;
        // Return the number of elements contained (hashed) in the bucket at
        // the specified 'index' in the array of buckets of this container.
    
    size_type bucket(const key_type& key) const;
        // Return the index of the bucket, in the array of buckets of this
        // container, where values having the specified 'key' are inserted
        // (hashed).

    // hash policy
    float load_factor() const;
        // Return the current ratio between the 'size' of this container and
        // the number of buckets.  The 'load_factor' is a measure of how full
        // the container is, and a higher load factor leads to an increased
        // number of collisions, thus resulting in a loss performance. 
    
    float max_load_factor() const;
        // Return the maximum load factor allowed for this container.  If
        // 'load_factor' exceeds 'max_load_factor' an increase of the number of
        // buckets and rehash of the elements of the container in the new array
        // of buckets is required (see rehash). 
    
    void  max_load_factor(float newLoadFactor);
        // Set the maximum load factor of this container to the specified
        // 'newLoadFactor'.
    
    void  rehash(size_type numBuckets);
        // Change the size of the array of buckets of this container to the
        // specified 'numBuckets', and redistribute all the elements contained
        // by this object into the new sequence of buckets, according to their
        // hash values. 
    
    void  reserve(size_type numElements);
        // Rehash the container with a number of buckets such that the
        // 'load_factor' of this container does not exceed the
        // 'max_load_factor' value for the specified 'numElements'.

    friend // must be defined inline, as no syntax to declare out-of-line
    bool operator==(const unordered_map& lhs, const unordered_map& rhs) {
        return lhs.d_impl == rhs.d_impl;
    }
        // Return 'true' if the specified 'lhs' and 'rhs' objects have the same
        // value, and 'false' otherwise.  Two 'unoredered_map' objects have the
        // same value if they have the same number of key-value pairs, and each
        // key-value pair that is contained in one of the objects is also
        // contained in the other object.  This method requires that the
        // (template parameter) types 'KEY' and 'VALUE' both be
        // "equality-comparable" (see {Requirements on 'KEY' and
        // 'VALUE'}).

};

// FREE FUNCTIONS
template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
void swap(unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>& x,
          unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>& y);

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
bool operator!=(
          const unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>& lhs,
          const unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' objects do not have the
    // same value, and 'false' otherwise.  Two 'unordered_map' objects do not
    // have the same value if they do not have the same number of key-value
    // pairs, or some key-value pair that is contained in one of the objects is
    // not also contained in the other object.  This method requires that the
    // (template parameter) types 'KEY' and 'VALUE' both be
    // "equality-comparable" (see {Requirements on 'KEY' and
    // 'VALUE'}).


// ===========================================================================
//                  TEMPLATE AND INLINE FUNCTION DEFINITIONS
// ===========================================================================


                        //--------------------
                        // class unordered_map
                        //--------------------

// CREATORS
template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::
unordered_map(size_type             n,
              const hasher&         hash,
              const key_equal&      equal,
              const allocator_type& allocator)
: d_impl(hash, equal, n, allocator)
{
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
template <class InputIterator>
inline
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::unordered_map(
                                               InputIterator         first,
                                               InputIterator         last,
                                               size_type             n,
                                               const hasher&         hash,
                                               const key_equal&      equal,
                                               const allocator_type& allocator)
: d_impl(hash, equal, n, allocator)
{
    this->insert(first, last);
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::unordered_map(
                                                    const unordered_map& other)
: d_impl(other.d_impl,
         AllocatorTraits::select_on_container_copy_construction(
                                                        other.get_allocator()))
{
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::unordered_map(
                                                       const allocator_type& a)
: d_impl(HASH(), EQUAL(), 0, a)
{
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::unordered_map(
                                               const unordered_map&  other, 
                                               const allocator_type& allocator)
: d_impl(other.d_impl, allocator)
{
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::~unordered_map()
{
    // All memory management is handled by the base 'd_impl' member.
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>&
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::operator=(
                                                    const unordered_map& other)
{
    unordered_map(other, this->get_allocator()).swap(*this);
    return *this;
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
ALLOCATOR
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::get_allocator() const
{
    return d_impl.allocator();
}

    // size and capacity
template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
bool
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::empty() const
{
    return 0 == d_impl.size();
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
typename unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::size_type
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::size() const
{
    return d_impl.size();
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
typename unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::size_type
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::max_size() const
{
    return d_impl.maxSize();
}

    // iterators
template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
typename unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::iterator
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::begin()
{
    return iterator(d_impl.elementListRoot());
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
typename unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::const_iterator
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::begin() const
{
    return const_iterator(d_impl.elementListRoot());
}


template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
typename unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::iterator
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::end()
{
    return iterator();
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
typename unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::const_iterator
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::end() const
{
    return const_iterator();
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
typename unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::const_iterator
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::cbegin() const
{
    return const_iterator(d_impl.elementListRoot());
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
typename unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::const_iterator
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::cend() const
{
    return const_iterator();
}

    // modifiers
template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
bsl::pair<typename unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::iterator,
          bool>
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::insert(
                                                       const value_type& value)
{
    typedef bsl::pair<iterator, bool> ResultType;

    bool isInsertedFlag = false;

    HashTableLink *result = d_impl.insertIfMissing(&isInsertedFlag, value);

    return ResultType(iterator(result), isInsertedFlag);
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
typename unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::iterator
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::insert(
                                                       const_iterator    hint, 
                                                       const value_type& value)
{   // There is no realistic use-case for the 'hint' in an unordered_map of
    // unique values.  We could quickly test for a duplicate key, and have a
    // fast return path for when the method fails, but in the typical use case
    // where a new element is inserted, we are adding an extra key-check for no
    // benefit.  In order to insert an element into a bucket, we need to walk
    // the whole bucket looking for duplicates, and the hint is no help in
    // finding the start of a bucket.
    return this->insert(value);
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
template <class InputIterator>
inline
void
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::insert(
                                                          InputIterator first, 
                                                          InputIterator last)
{
    if (size_t maxInsertions =
            ::BloombergLP::bslstl::IteratorUtil::insertDistance(first, last)) {
        this->reserve(this->size() + maxInsertions);
    }

    // This loop could be clean up with fewer temporaries, we are discarding a
    // 'pair' returned from each call to 'insert'.
    while (first != last) {
        this->insert(*first++);
    }
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
typename unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::iterator
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::erase(
                                                       const_iterator position)
{
    BSLS_ASSERT(position != this->end());
    return iterator(d_impl.remove(position.node()));
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
typename unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::size_type
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::erase(const key_type& key)
{
    if (HashTableLink *target = d_impl.find(key)) {
        d_impl.remove(target);
        return 1;
    }
    else {
        return 0;
    }
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
typename unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::iterator
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::erase(
                                                         const_iterator first, 
                                                         const_iterator last)
{   // bad answer here, need to turn 'first' into a non-const iterator

    // 7 Most of the library's algorithmic templates that operate on data
    // structures have interfaces that use ranges.  A range is a pair of
    // iterators that designate the beginning and end of the computation.  A
    // range [i,i) is an empty range; in general, a range [i,j) refers to the
    // elements in the data structure starting with the element pointed to by i
    // and up to but not including the element pointed to by j.  Range [i,j) is
    // valid if and only if j is reachable from i.  The result of the
    // application of functions in the library to invalid ranges is undefined.
#if defined BDE_BUILD_TARGET_SAFE2
    // Check that 'first' and 'last' are valid iterators referring to this
    // container.
    if (first == last) {
        iterator it = this->begin();
        while(it != first) {
            BSLS_ASSERT(it != this->end());
        }
        while(it != last) {
            BSLS_ASSERT(it != this->end());
        }
    }
#endif

    while (first != last) {
        first = this->erase(first);
    }

    return iterator(first.node()); // convert from const_iterator
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
void
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::clear()
{
    d_impl.removeAll();
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
void
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::swap(unordered_map& other)
{
    // assert that allocators are compatible
    // TBD
    d_impl.swap(other.d_impl);
}

    // observers
template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
typename unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::hasher
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::hash_function() const
{
    return d_impl.hasher();
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
typename unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::key_equal
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::key_eq() const
{
    return d_impl.comparator();
}


// lookup
template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
typename unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::iterator
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::find(
                                                             const key_type& k)
{
    return iterator(d_impl.find(k));
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
typename
       unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::const_iterator
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::find(
                                                     const key_type& key) const
{
    return const_iterator(d_impl.find(key));
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
typename unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::size_type
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::count(
                                                     const key_type& key) const
{
    // bool conversion to size_type gives the correct result
    // Simple implementation could be optimized with a list-walk
    // Might have list-walk search for value as LinkUtil algorithm?
    return d_impl.find(key) != 0;
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
bsl::pair<typename unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::iterator,
          typename unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::iterator>
inline
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::equal_range(
                                                           const key_type& key)
{
    typedef bsl::pair<iterator, iterator> ResultType;

    HashTableLink *first = d_impl.find(key);
    return first
         ? ResultType(iterator(first), iterator(first->nextLink()))
         : ResultType(iterator(0),     iterator(0));
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
bsl::pair<typename unordered_map<KEY,
                                 VALUE,
                                 HASH,
                                 EQUAL,
                                 ALLOCATOR>::const_iterator,
          typename unordered_map<KEY,
                                 VALUE,
                                 HASH,
                                 EQUAL,
                                 ALLOCATOR>::const_iterator>
inline
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::equal_range(
                                                     const key_type& key) const
{
    typedef bsl::pair<const_iterator, const_iterator> ResultType;

    HashTableLink *first = d_impl.find(key);
    return first
         ? ResultType(const_iterator(first), const_iterator(first->nextLink()))
         : ResultType(const_iterator(0),     const_iterator(0));
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
typename unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::mapped_type&
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::operator[](
                                                           const key_type& key)
{
    HashTableLink *node = d_impl.insertIfMissing(key);
    return static_cast<HashTableNode *>(node)->value().second;
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
typename unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::mapped_type&
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::at(const key_type& key)
{
    HashTableLink *node = d_impl.find(key);

    if (!node) {
        BloombergLP::bslstl::StdExceptUtil::throwOutOfRange("Boo!");
    }

    return static_cast<HashTableNode *>(node)->value().second;
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
const typename
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::mapped_type&
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::at(
                                                     const key_type& key) const
{
    HashTableLink *target = d_impl.find(key);
    if (!target ){
        BloombergLP::bslstl::StdExceptUtil::throwOutOfRange("Boo!");
    }
    return static_cast<HashTableNode *>(target)->value().second;
}

    // bucket interface
template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
typename unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::size_type
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::bucket_count() const
{
    return d_impl.numBuckets();
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
typename unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::size_type
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::max_bucket_count() const
{
    return d_impl.maxNumOfBuckets();
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
typename unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::size_type
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::bucket_size(
                                                         size_type index) const
{
    BSLS_ASSERT_SAFE(index < this->bucket_count());
    return d_impl.countElementsInBucket(index);
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
typename unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::size_type
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::bucket(
                                                     const key_type& key) const
{
    return d_impl.bucketIndexForKey(key);
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
typename
       unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::local_iterator
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::begin(size_type index)
{
    BSLS_ASSERT_SAFE(index < this->bucket_count());
    return local_iterator(&d_impl.bucketAtIndex(index));
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
typename
 unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::const_local_iterator
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::begin(size_type index) const
{
    BSLS_ASSERT_SAFE(index < this->bucket_count());
    return const_local_iterator(&d_impl.bucketAtIndex(index));
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
typename
       unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::local_iterator
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::end(size_type index)
{
    BSLS_ASSERT_SAFE(index < this->bucket_count());
    return local_iterator(0, &d_impl.bucketAtIndex(index));
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
typename
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::const_local_iterator
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::end(size_type index) const
{
    BSLS_ASSERT_SAFE(index < this->bucket_count());
    return const_local_iterator(0, &d_impl.bucketAtIndex(index));
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
typename
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::const_local_iterator
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::cbegin(
                                                         size_type index) const
{
    BSLS_ASSERT_SAFE(index < this->bucket_count());
    return const_local_iterator(&d_impl.bucketAtIndex(index));
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
typename
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::const_local_iterator
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::cend(size_type index) const
{
    BSLS_ASSERT_SAFE(index < this->bucket_count());
    return const_local_iterator(0, &d_impl.bucketAtIndex(index));
}

    // hash policy
template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
float
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::load_factor() const
{
    return d_impl.loadFactor();
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
float
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::max_load_factor() const
{
    return d_impl.maxLoadFactor();
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
void
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::max_load_factor(
                                                           float newLoadFactor)

{
    d_impl.maxLoadFactor(newLoadFactor);
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
void
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::rehash(
                                                          size_type numBuckets)
{
    return d_impl.rehash(numBuckets);
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
void
unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>::reserve(
                                                         size_type numElements)
{
    return d_impl.rehashForNumElements(numElements);
}

}  // close namespace bsl

//----------------------------------------------------------------------------
//                  free functions and operators
//----------------------------------------------------------------------------

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
void
bsl::swap(bsl::unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>& x,
          bsl::unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>& y)
{
    x.swap(y);
}

template <class KEY, class VALUE, class HASH, class EQUAL, class ALLOCATOR>
inline
bool bsl::operator!=(
      const bsl::unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>& lhs,
      const bsl::unordered_map<KEY, VALUE, HASH, EQUAL, ALLOCATOR>& rhs)
{
    return !(lhs == rhs);
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
