// bdlt_timetable.h                                                   -*-C++-*-
#ifndef INCLUDED_BDLT_TIMETABLE
#define INCLUDED_BDLT_TIMETABLE

#ifndef INCLUDED_BSLS_IDENT
#include <bsls_ident.h>
#endif
BSLS_IDENT("$Id: $")

//@PURPOSE: Provide a repository for accessing timetable information.
//
//@CLASSES:
//  bdlt::Timetable: repository for accessing timetable information
//  bdlt::TimetableTransition: datetime and transition code value
//
//@DESCRIPTION: This component provides a value-semantic class,
// 'bdlt::Timetable', that represents a timetable of state transitions over a
// *valid* *range* of dates, an associated iterator,
// 'bdlt::Timetable::const_iterator', that provides non-modifiable access to
// its elements, and a class 'bdlt::TimetableTransition', that represents a
// change of state at a datetime.
//
// 'bdlt::Timetable' is designed to be especially efficient at determining the
// state in effect at a given 'bdlt::Datetime' value (within the valid range
// for a particular 'bdlt::Timetable' object), and iterating through the state
// transitions.
//
// Default-constructed timetables are empty, and have an empty valid range.
// Timetables can also be constructed with an initial (non-empty) valid range.
// The 'setValidRange' method modifies the valid range of a timetable, and a
// suite of "add" methods can be used to populate a timetable with state
// transitions.
//
// Timetables are value-semantic objects, and, as such, necessarily support all
// of the standard value-semantic operations, such as default construction,
// copy construction and copy assignment, and equality comparison.
//
///Exception-Safety Guarantees
///---------------------------
// All methods of the 'bdlt::Timetable' are exception-safe, but in general
// provide only the basic guarantee (i.e., no guarantee of rollback): If an
// exception occurs (i.e., while attempting to allocate memory), the timetable
// object is left in a coherent state, but (unless otherwise specified) its
// *value* is undefined.
//
///Usage
///-----
// This section illustrates intended use of this component.
//
///Example 1: 'Exchange Schedule'
/// - - - - - - - - - - - - - - -
// Suppose we want to track the open and close times for an exchange.  Most
// Mondays (and Tuesdays, Wednesdays, etc.) will have the same schedule,
// although some may differ.  We can use 'bdlt::Timetable' to efficiently store
// this data.
//
// First, we create an instance of 'bdlt::Timetable' with the desired valid
// range:
//..
//  bdlt::Timetable timetable(bdlt::Date(2018, 1, 1),
//                            bdlt::Date(2018, 12, 31));
//..
// Then, we define the codes for start-of-trading and end-of-trading and
// populate the typical transitions into the timetable:
//..
//  const int TRADING    = 0;
//  const int NO_TRADING = 1;
//
//  timetable.setInitialTransitionCode(NO_TRADING);
//
//  for (int i = 0; i < 5; ++ i) {
//      timetable.addTransition(static_cast<bdlt::DayOfWeek::Enum>(
//                                                 bdlt::DayOfWeek::e_MON + i),
//                              bdlt::Time(8, 30),
//                              TRADING,
//                              timetable.firstDate(),
//                              timetable.lastDate());
//
//      timetable.addTransition(static_cast<bdlt::DayOfWeek::Enum>(
//                                                 bdlt::DayOfWeek::e_MON + i),
//                              bdlt::Time(16, 30),
//                              NO_TRADING,
//                              timetable.firstDate(),
//                              timetable.lastDate());
//  }
//..
// Next, we add a holiday on January 19, 2018:
//..
//  timetable.removeAllTransitions(bdlt::Date(2018, 1, 19));
//..
// Then, we add a half-day on November 23, 2018:
//..
//  timetable.addTransition(bdlt::Datetime(2018, 11, 23, 12, 30),
//                          NO_TRADING);
//
//  timetable.removeTransition(bdlt::Datetime(2018, 11, 23, 16, 30));
//..
// Finally, we verify the transition code in effect at a few datetimes.
//..
//  assert(NO_TRADING == timetable.transitionCodeInEffect(
//                                      bdlt::Datetime(2018,  1, 15,  8,  0)));
//
//  assert(TRADING    == timetable.transitionCodeInEffect(
//                                      bdlt::Datetime(2018,  1, 15,  8, 30)));
//
//  assert(TRADING    == timetable.transitionCodeInEffect(
//                                      bdlt::Datetime(2018,  1, 15, 16,  0)));
//
//  assert(NO_TRADING == timetable.transitionCodeInEffect(
//                                      bdlt::Datetime(2018,  1, 15, 16, 30)));
//
//  assert(NO_TRADING == timetable.transitionCodeInEffect(
//                                      bdlt::Datetime(2018, 11, 23,  8,  0)));
//
//  assert(TRADING    == timetable.transitionCodeInEffect(
//                                      bdlt::Datetime(2018, 11, 23,  8, 30)));
//
//  assert(TRADING    == timetable.transitionCodeInEffect(
//                                      bdlt::Datetime(2018, 11, 23, 12,  0)));
//
//  assert(NO_TRADING == timetable.transitionCodeInEffect(
//                                      bdlt::Datetime(2018, 11, 23, 12, 30)));
//..

#include <bdlscm_version.h>

#include <bdlt_date.h>
#include <bdlt_datetime.h>
#include <bdlt_dayofweek.h>
#include <bdlt_time.h>

#include <bdlc_compactedarray.h>

#include <bslalg_swaputil.h>

#include <bslh_hash.h>

#include <bslma_allocator.h>
#include <bslma_usesbslmaallocator.h>

#include <bsls_assert.h>

#include <bsl_algorithm.h>
#include <bsl_cstddef.h>
#include <bsl_iosfwd.h>
#include <bsl_iterator.h>
#include <bsl_vector.h>

namespace BloombergLP {
namespace bdlt {

// FORWARD DECLARATIONS
class Timetable;
class Timetable_Day;
class Timetable_ConstIterator;

                        // =========================
                        // class TimetableTransition
                        // =========================

class TimetableTransition {
    // This class represents a state transition, implemented as a datetime for
    // when the transition occurs, and a code to indicate the new state.

    // DATA
    Datetime d_datetime;  // datetime of the transition
    int      d_code;      // code in effect at, and after, 'd_datetime'

    // FRIENDS
    friend class Timetable;
    friend class Timetable_Day;

    friend bool operator==(const TimetableTransition&,
                           const TimetableTransition&);
    friend bool operator!=(const TimetableTransition&,
                           const TimetableTransition&);
    friend bool operator< (const TimetableTransition&,
                           const TimetableTransition&);
    friend bool operator< (const TimetableTransition&,
                           const Datetime&);
    friend bool operator< (const Datetime&,
                           const TimetableTransition&);

    template <class HASHALG>
    friend void hashAppend(HASHALG& hashAlg, const TimetableTransition&);

    // NOT IMPLEMENTED
    TimetableTransition();

    // PRIVATE CREATORS
    TimetableTransition(const Datetime& datetime, int code);
        // Create a 'TimetableTransition' having the specified 'datetime' and
        // 'code'.

  public:
    // CREATORS
    TimetableTransition(const TimetableTransition& original);
        // Create a 'TimetableTransition' having the same value as the
        // specified 'original' object.

    //! ~TimetableTransition() = default;
        // Destroy this object.

    // MANIPULATORS
    TimetableTransition& operator=(const TimetableTransition& rhs);
        // Assign to this 'TimetableTransition' the value of the specified
        // 'rhs' timetable transition, and return a reference providing
        // modifiable access to this 'TimetableTransition'.

    // ACCESSORS
    const Datetime& datetime() const;
        // Return the datetime of this transition.

    int transitionCode() const;
        // Return the code of this transition.

                             // Aspects

    bsl::ostream& print(bsl::ostream& stream,
                        int           level = 0,
                        int           spacesPerLevel = 4) const;
        // Format this object to the specified output 'stream' at the (absolute
        // value of) the optionally specified indentation 'level' and return a
        // reference to the modifiable 'stream'.  If 'level' is specified,
        // optionally specify 'spacesPerLevel', the number of spaces per
        // indentation level for this and all of its nested objects.  If
        // 'level' is negative, suppress indentation of the first line.  If
        // 'spacesPerLevel' is negative, format the entire output on one line,
        // suppressing all but the initial indentation (as governed by
        // 'level').  If 'stream' is not valid on entry, this operation has no
        // effect.
};

// FREE OPERATORS
bool operator==(const TimetableTransition& lhs,
                const TimetableTransition& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' timetable transitions
    // have the same value, and 'false' otherwise.  Two timetable transitions
    // have the same value if they have the same datetime and transition code.

bool operator!=(const TimetableTransition& lhs,
                const TimetableTransition& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' timetable transitions do
    // not have the same value, and 'false' otherwise.  Two timetable
    // transitions do not have the same value if they do not have the same
    // datetime and transition code.

bool operator<(const TimetableTransition& lhs,
               const TimetableTransition& rhs);
    // Return 'true' if the specified 'lhs' has a value less than the specified
    // 'rhs', and 'false' otherwise.  Timetable transition 'lhs' has a value
    // less than timetable transition 'rhs' if
    // 'lhs.datetime() < rhs.datetime()', or 'lhs.datetime() == rhs.datetime()'
    // and 'lhs.transitionCode() < rhs.transitionCode()'.

bool operator<(const TimetableTransition& lhs, const Datetime& rhs);
    // Return 'true' if the specified 'lhs' has a value less than the specified
    // 'rhs', and 'false' otherwise.  Timetable transition 'lhs' has a value
    // less than datetime 'rhs' if 'lhs.datetime() < rhs'.

bool operator<(const Datetime& lhs, const TimetableTransition& rhs);
    // Return 'true' if the specified 'lhs' has a value less than the specified
    // 'rhs', and 'false' otherwise.  Datetime 'lhs' has a value less than
    // timetable transition 'rhs' if 'lhs < rhs.datetime()'.

// HASH SPECIALIZATIONS
template <class HASHALG>
void hashAppend(HASHALG& hashAlg, const TimetableTransition& object);
    // Pass the specified 'object' to the specified 'hashAlg'.  This function
    // integrates with the 'bslh' modular hashing system and effectively
    // provides a 'bsl::hash' specialization for 'TimetableTransition'.

                           // ===================
                           // class Timetable_Day
                           // ===================

class Timetable_Day {
    // This class implements one day of a timetable.

    // DATA
    int                              d_initialTransitionCode;
                                             // transition code in effect at
                                             // the start of this day

    bsl::vector<TimetableTransition> d_transition;
                                             // ordered vector of transitions

    // FRIENDS
    friend class Timetable_ConstIterator;

    friend bool operator==(const Timetable_Day&, const Timetable_Day&);
    friend bool operator!=(const Timetable_Day&, const Timetable_Day&);
    friend bool operator< (const Timetable_Day&, const Timetable_Day&);

    template <class HASHALG>
    friend void hashAppend(HASHALG& hashAlg, const Timetable_Day&);

  public:
    // CLASS DATA
    static const int k_UNSET_TRANSITION_CODE = -1;  // value representing an
                                                    // unset transition code

    // CREATORS
    Timetable_Day(bslma::Allocator *basicAllocator);
        // Create a 'Timetable_Day'.  The specified 'basicAllocator' is used to
        // supply memory.  The behavior is undefined unless
        // '0 != basicAllocator'.

    Timetable_Day(const Timetable_Day&  original,
                  bslma::Allocator     *basicAllocator);
        // Create a 'Timetable_Day' having the same value as the specified
        // 'original' object.  The specified 'basicAllocator' is used to supply
        // memory.  The behavior is undefined unless '0 != basicAllocator'.

    //! ~Timetable_Day() = default;
        // Destroy this object.

    // MANIPULATORS
    Timetable_Day& operator=(const Timetable_Day& rhs);
        // Assign to this 'Timetable_Day' the value of the specified 'rhs'
        // daily timetable, and return a reference providing modifiable access
        // to this 'Timetable_Day'.

    bool setInitialTransitionCode(int transitionCode);
        // Set the transition code in effect at the start of this timetable to
        // the specified 'transitionCode'.  Return 'true' if the value returned
        // by 'finalTransitionCode()' prior to this operations is not equal to
        // the value returned by 'finalTransitionCode()' after this operation.
        // The behavior is undefined unless
        // '0 <= transitionCode || k_UNSET_TRANSITION_CODE == transitionCode'.

    bool addTransition(const Datetime& datetime, int transitionCode);
        // Add a transition at the specified 'datetime' having the specified
        // 'transitionCode'.  If 'datetime' is already a transition point,
        // replace the existing code with the provided 'transitionCode'.
        // Return 'true' if the value returned by 'finalTransitionCode()' prior
        // to this operations is not equal to the value returned by
        // 'finalTransitionCode()' after this operation.  The behavior is
        // undefined unless '24 > datetime.hour()',
        // '0 <= transitionCode || k_UNSET_TRANSITION_CODE == transitionCode',
        // and all the resultant transitions in this 'Timetable_Day' have the
        // same date.

    bool removeAllTransitions();
        // Remove all transitions.  Return 'true' if the value returned by
        // 'finalTransitionCode()' prior to this operations is not equal to the
        // value returned by 'finalTransitionCode()' after this operation.

    bool removeTransition(const Datetime& datetime);
        // If a transition occurs at the specified 'datetime', remove the
        // transition from this daily timetable.  Otherwise, return without
        // modifying this daily timetable.  Return 'true' if the value returned
        // by 'finalTransitionCode()' prior to this operations is not equal to
        // the value returned by 'finalTransitionCode()' after this operation.
        // The behavior is undefined unless '24 > datetime.hour()', and there
        // are no transitions in this 'Timetable_Day' or
        // 'd_transition[0].datetime().date() == datetime.date()'.

    // ACCESSORS
    int finalTransitionCode() const;
        // Return the transition code at the end of this daily timetable.  If
        // this daily timetable has no such transition, return
        // 'k_UNSET_TRANSITION_CODE'.

    int initialTransitionCode() const;
        // Return the transition code at the start of this daily timetable (see
        // 'setInitialTransitionCode').  If this daily timetable has no such
        // transition, return 'k_UNSET_TRANSITION_CODE'.

    bsl::size_t size() const;
        // Return the number of transitions in this daily timetable.

    int transitionCodeInEffect(const Datetime& datetime) const;
        // Return the transition code associated with the first transition that
        // occurs on or before the specified 'datetime' in this timetable.  If
        // this timetable has no such transition, return
        // 'k_UNSET_TRANSITION_CODE'.  The behavior is undefined unless
        // '24 > datetime.hour()', and there are no transitions in this
        // 'Timetable_Day' or
        // 'd_transition[0].datetime().date() == datetime.date()'.
};

// FREE OPERATORS
bool operator==(const Timetable_Day& lhs, const Timetable_Day& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' daily timetables have the
    // same value, and 'false' otherwise.  Two daily timetables have the same
    // value if they have the same initial transition code, transitions, and
    // each corresponding pair of transitions has the same transition code.

bool operator!=(const Timetable_Day& lhs, const Timetable_Day& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' daily timetables do not
    // have the same value, and 'false' otherwise.  Two daily timetables do not
    // have the same value if they do not have the same initial transition
    // code, transitions, or, for at least one corresponding pair of
    // transitions, do not have the same transition code.

bool operator<(const Timetable_Day& lhs, const Timetable_Day& rhs);
    // Return 'true' if the specified 'lhs' daily timetable is
    // lexicographically less than the specified 'rhs' daily timetable, and
    // 'false' otherwise.

// HASH SPECIALIZATIONS
template <class HASHALG>
void hashAppend(HASHALG& hashAlg, const Timetable_Day& object);
    // Pass the specified 'object' to the specified 'hashAlg'.  This function
    // integrates with the 'bslh' modular hashing system and effectively
    // provides a 'bsl::hash' specialization for 'Timetable_Day'.

                             // ===============
                             // class Timetable
                             // ===============

class Timetable {
    // This class implements a value-semantic repository of datetime indexed
    // state transitions over a *valid* *range* of dates.  This valid range,
    // '[firstDate() .. lastDate()]', spans the first and last dates of a
    // timetable's accessible contents.  A timetable can be "populated" with
    // state transitions via a suite of "add" methods.  Note that the behavior
    // of requesting *any* timetable information for a supplied date whose
    // value is outside the current *valid* *range* for that timetable is
    // undefined.

    // DATA
    Date                                d_firstDate;  // start of valid range

    Date                                d_lastDate;   // end of valid range

    int                                 d_initialTransitionCode;
                                                      // transition code in
                                                      // effect *before* the
                                                      // valid range

    bdlc::CompactedArray<Timetable_Day> d_timetable;  // daily timetables

    // FRIENDS
    friend class Timetable_ConstIterator;

    friend bool operator==(const Timetable&, const Timetable&);

    friend bool operator!=(const Timetable&, const Timetable&);

    template <class HASHALG>
    friend void hashAppend(HASHALG& hashAlg, const Timetable&);

  public:
    // CLASS DATA
    static const int k_UNSET_TRANSITION_CODE =
                                       Timetable_Day::k_UNSET_TRANSITION_CODE;

    // TYPES
    typedef Timetable_ConstIterator const_iterator;

    // CREATORS
    explicit Timetable(bslma::Allocator *basicAllocator = 0);
        // Create an empty timetable, i.e., a timetable having an empty valid
        // range.  Optionally specify a 'basicAllocator' used to supply memory.
        // If 'basicAllocator' is 0, the currently installed default allocator
        // is used.

    Timetable(
             const Date&       firstDate,
             const Date&       lastDate,
             int               initialTransitionCode = k_UNSET_TRANSITION_CODE,
             bslma::Allocator *basicAllocator = 0);
        // Create a timetable having a valid range from the specified
        // 'firstDate' through the specified 'lastDate' and having the
        // optionally specified 'initialTransitionCode'.  Optionally specify a
        // 'basicAllocator' used to supply memory.  If 'basicAllocator' is 0,
        // the currently installed default allocator is used.  The behavior is
        // undefined unless 'firstDate <= lastDate'.

    Timetable(const Timetable& original, bslma::Allocator *basicAllocator = 0);
        // Create a timetable having the value of the specified 'original'
        // timetable.  Optionally specify a 'basicAllocator' used to supply
        // memory.  If 'basicAllocator' is 0, the currently installed default
        // allocator is used.

    ~Timetable();
        // Destroy this object.

    // MANIPULATORS
    Timetable& operator=(const Timetable& rhs);
        // Assign to this timetable the value of the specified 'rhs' timetable,
        // and return a reference providing modifiable access to this
        // timetable.

    void addTransition(const Date& date, const Time& time, int transitionCode);
        // Add a transition on the specified 'date' at the specified 'time'
        // having the specified 'transitionCode'.  If 'time' is already a
        // transition point on 'date', replace the existing code with the
        // provided 'transitionCode'.  The addition of a transition, but not
        // the replacement of the code at the transition, invalidates all
        // iterators.  The behavior is undefined unless '24 > time.hour()',
        // 'date' is within the valid range of this timetable, and
        // '0 <= transitionCode || k_UNSET_TRANSITION_CODE == transitionCode'.

    void addTransition(const Datetime& datetime, int transitionCode);
        // Add a transition at the specified 'datetime' having the specified
        // 'transitionCode'.  If 'datetime' is already a transition point,
        // replace the existing code with the provided 'transitionCode'.  The
        // addition of a transition, but not the replacement of the code at the
        // transition, invalidates all iterators.  The behavior is undefined
        // unless '24 > datetime.hour()', 'datetime.date()' is within the valid
        // range of this timetable, and
        // '0 <= transitionCode || k_UNSET_TRANSITION_CODE == transitionCode'.

    void addTransition(const DayOfWeek::Enum& dayOfWeek,
                       const Time&            time,
                       int                    transitionCode,
                       const Date&            firstDate,
                       const Date&            lastDate);
        // Add transitions that occur at the specified 'time', having the
        // specified 'transitionCode', to all dates that are of the specified
        // 'dayOfWeek' within the closed interval of dates from the specified
        // 'firstDate' to the specified 'lastDate'.  For every date where this
        // transition will occur, if 'time' is already a transition point,
        // replace the existing code with the provided 'transitionCode'.  The
        // addition of a transition, but not the replacement of the code at the
        // transition, invalidates all iterators.  The behavior is undefined
        // unless '24 > time.hour()', 'firstDate <= lastDate', 'firstDate' and
        // 'lastDate' are within the valid range of this timetable, and
        // '0 <= transitionCode || k_UNSET_TRANSITION_CODE == transitionCode'.

    void removeAll();
        // Remove all information from this timetable, leaving it with its
        // default constructed "empty" value.  The removal of a transition
        // invalidates all iterators.

    void removeAllTransitions(const Date& date);
        // Remove all transitions from the specified 'date'.  The removal of a
        // transition invalidates all iterators.  The behavior is undefined
        // unless 'date' is within the valid range of this timetable.

    void removeTransition(const DayOfWeek::Enum& dayOfWeek,
                          const Time&            time,
                          const Date&            firstDate,
                          const Date&            lastDate);
        // Remove all transitions that occur at the specified 'time' to all
        // dates that are of the specified 'dayOfWeek' within the closed
        // interval of dates from the specified 'firstDate' to the specified
        // 'lastDate'.  The removal of a transition invalidates all iterators.
        // The behavior is undefined unless '24 > time.hour()',
        // 'firstDate <= lastDate', and 'firstDate' and 'lastDate' are within
        // the valid range of this timetable.

    void removeTransition(const Date& date, const Time& time);
        // If a transition occurs on the specified 'date' at the specified
        // 'time', remove the transition from this timetable.  Otherwise,
        // return without modifying this timetable.  The removal of a
        // transition invalidates all iterators.  The behavior is undefined
        // unless '24 > time.hour()' and 'date' is within the valid range of
        // this timetable.

    void removeTransition(const Datetime& datetime);
        // If a transition occurs at the specified 'datetime', remove the
        // transition from this timetable.  Otherwise, return without modifying
        // this timetable.  The removal of a transition invalidates all
        // iterators.  The behavior is undefined unless '24 > datetime.hour()'
        // and 'datetime.date()' is within the valid range of this timetable.

    void setInitialTransitionCode(int transitionCode);
        // Set the transition code in effect at the start of this timetable to
        // the specified 'transitionCode'.  The behavior is undefined unless
        // '0 <= transitionCode || k_UNSET_TRANSITION_CODE == transitionCode'.

    void setValidRange(const Date& firstDate, const Date& lastDate);
        // Set the range of this timetable using the specified 'firstDate' and
        // 'lastDate' as, respectively, the first date and the last date of the
        // timetable.  Any transitions, and associated transition codes, that
        // are outside of the new range are removed.  The removal of a
        // transition invalidates all iterators.  The behavior is undefined
        // unless 'firstDate <= lastDate'.

                                  // Aspects

    void swap(Timetable& other);
        // Efficiently exchange the value of this object with the value of the
        // specified 'other' object.  This method provides the no-throw
        // exception-safety guarantee.  The behavior is undefined unless this
        // object was created with the same allocator as 'other'.

    // ACCESSORS
    const_iterator begin() const;
        // Return an iterator referring to the first transition in this
        // timetable, or the past-the-end iterator if this array is empty.  The
        // iterator remains valid as long as this array exists.

    const_iterator end() const;
        // Return the past-the-end iterator for this array.  The iterator
        // remains valid as long as this array exists, and its length does not
        // decrease.

    const Date& firstDate() const;
        // Return a reference providing non-modifiable access to the earliest
        // date in the valid range of this timetable.  The behavior is
        // undefined if this timetable does not have a valid range (i.e.,
        // default constructed or constructed from a calendar not having a
        // valid range).

    int initialTransitionCode() const;
        // Return the transition code at the start of this timetable (see
        // 'setInitialTransitionCode').  If this timetable has no such
        // transition, return 'k_UNSET_TRANSITION_CODE'.

    bool isInRange(const Date& date) const;
        // Return 'true' if the specified 'date' is within the valid range of
        // this timetable, and 'false' otherwise.

    const Date& lastDate() const;
        // Return a reference providing non-modifiable access to the latest
        // date in the valid range of this timetable.  The behavior is
        // undefined if this timetable does not have a valid range (i.e.,
        // default constructed or constructed from a calendar not having a
        // valid range).

    int length() const;
        // Return the number of days in the valid range of this timetable,
        // which is defined to be 0 if this timetable is empty, and
        // 'lastDate() - firstDate() + 1' otherwise.

    int transitionCodeInEffect(const Date& date, const Time& time) const;
        // Return the transition code associated with the first transition that
        // occurs on or before the specified 'date' at the specified 'time' in
        // this timetable.  If this timetable has no such transition, return
        // 'k_UNSET_TRANSITION_CODE'.  The behavior is undefined unless
        // '24 > time.hour()' and 'date' is within the valid range of this
        // timetable.

    int transitionCodeInEffect(const Datetime& datetime) const;
        // Return the transition code associated with the first transition that
        // occurs on or before the specified 'datetime' in this timetable.  If
        // this timetable has no such transition, return
        // 'k_UNSET_TRANSITION_CODE'.  The behavior is undefined unless
        // '24 > datetime.hour()' and 'datetime.date()' is within the valid
        // range of this timetable.

                                  // Aspects

    bslma::Allocator *allocator() const;
        // Return the allocator used by this object to supply memory.

    bsl::ostream& print(bsl::ostream& stream,
                        int           level = 0,
                        int           spacesPerLevel = 4) const;
        // Format this object to the specified output 'stream' at the (absolute
        // value of) the optionally specified indentation 'level' and return a
        // reference to the modifiable 'stream'.  If 'level' is specified,
        // optionally specify 'spacesPerLevel', the number of spaces per
        // indentation level for this and all of its nested objects.  If
        // 'level' is negative, suppress indentation of the first line.  If
        // 'spacesPerLevel' is negative, format the entire output on one line,
        // suppressing all but the initial indentation (as governed by
        // 'level').  If 'stream' is not valid on entry, this operation has no
        // effect.
};

// FREE OPERATORS
bool operator==(const Timetable& lhs, const Timetable& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' timetables have the same
    // value, and 'false' otherwise.  Two timetables have the same value if
    // they have the same initial transition code, valid range (or are both
    // empty), transitions, and each corresponding pair of transitions has the
    // same transition code.

bool operator!=(const Timetable& lhs, const Timetable& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' timetables do not have
    // the same value, and 'false' otherwise.  Two timetables do not have the
    // same value if they do not have the same initial transition code, valid
    // range (and are not both empty), transitions, or, for at least one
    // corresponding pair of transitions, do not have the same transition code.

bsl::ostream& operator<<(bsl::ostream& stream, const Timetable& timetable);
    // Write the value of the specified 'timetable' to the specified output
    // 'stream', and return a reference to the modifiable 'stream'.

// FREE FUNCTIONS
void swap(Timetable& a, Timetable& b);
    // Efficiently exchange the values of the specified 'a' and 'b' objects.
    // This function provides the no-throw exception-safety guarantee.  The
    // behavior is undefined unless the two objects were created with the same
    // allocator.

// HASH SPECIALIZATIONS
template <class HASHALG>
void hashAppend(HASHALG& hashAlg, const Timetable& object);
    // Pass the specified 'object' to the specified 'hashAlg'.  This function
    // integrates with the 'bslh' modular hashing system and effectively
    // provides a 'bsl::hash' specialization for 'Timetable'.

                      // =============================
                      // class Timetable_ConstIterator
                      // =============================

class Timetable_ConstIterator {
    // Provide read-only, sequential access in increasing (chronological) order
    // to the transitions in a 'Timetable' object.

    // DATA
    const Timetable *d_timetable_p;  // A pointer to the 'Timetable' into which
                                     // this iterator references.

    bsl::size_t      d_dayIndex;     // The index of the referenced day within
                                     // the timetable.

    bsl::size_t      d_timeIndex;    // The index of the referenced time on the
                                     // referenced day within the timetable.

    // FRIENDS
    friend class Timetable;

    friend bool operator==(const Timetable_ConstIterator&,
                           const Timetable_ConstIterator&);
    friend bool operator!=(const Timetable_ConstIterator&,
                           const Timetable_ConstIterator&);

  private:
    // PRIVATE CREATORS
    Timetable_ConstIterator(const Timetable& timetable,
                            bsl::size_t      dayIndex,
                            bsl::size_t      timeIndex);
        // Create a transition iterator for the specified 'timetable'.  This
        // iterator references the time at the specified 'timeIndex' on the day
        // at the specified 'dayIndex' in the 'timetable'.

  public:
    // CREATORS
    Timetable_ConstIterator();
        // Create a default iterator.  Note that the use of most methods - as
        // indicated in their documentation - upon this iterator will result in
        // undefined behavior.

    Timetable_ConstIterator(const Timetable_ConstIterator& original);
        // Create an iterator having the value of the specified 'original'
        // iterator.

    //! ~Timetable_ConstIterator() = default;
        // Destroy this object.

    // MANIPULATORS
    Timetable_ConstIterator& operator=(const Timetable_ConstIterator& rhs);
        // Assign to this iterator the value of the specified 'rhs' iterator,
        // and return a reference providing modifiable access to this object.

    Timetable_ConstIterator& operator++();
        // Advance this iterator to refer to the next transition in the
        // associated timetable, and return a reference providing modifiable
        // access to this object.  The behavior is undefined unless, on entry,
        // this iterator references a valid transition.

    Timetable_ConstIterator& operator--();
        // Regress this iterator to refer to the previous transition in the
        // associated timetable, and return a reference providing modifiable
        // access to this object.  The behavior is undefined unless, on entry,
        // this iterator references a valid transition that is not the first
        // transition of the associated timetable.

    // ACCESSORS
    const TimetableTransition& operator*() const;
        // Return the element value referenced by this iterator.  The behavior
        // is undefined unless this iterator references a valid transition in
        // the timetable.

    const TimetableTransition *operator->() const;
        // Return a pointer to the element value referenced by this iterator.
        // The behavior is undefined unless this iterator references a valid
        // transition in the timetable.
};

// FREE OPERATORS
Timetable_ConstIterator operator++(Timetable_ConstIterator& iterator, int);
    // Advance the specified 'iterator' to refer to the next transition in the
    // referenced timetable, and return an iterator referring to the original
    // element (*before* the advancement).  The behavior is undefined unless,
    // on entry, 'iterator' references a valid transition.

Timetable_ConstIterator operator--(Timetable_ConstIterator& iterator, int);
    // Decrement the specified 'iterator' to refer to the previous transition
    // in the referenced timetable, and return an iterator referring to the
    // original element (*before* the decrementation).  The behavior is
    // undefined unless, on entry, 'iterator' references a valid transition.

bool operator==(const Timetable_ConstIterator& lhs,
                const Timetable_ConstIterator& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' iterators have the same
    // value, and 'false' otherwise.  Two 'Timetable_ConstIterator' iterators
    // have the same value if they refer to the same timetable, and the same
    // transition.

bool operator!=(const Timetable_ConstIterator& lhs,
                const Timetable_ConstIterator& rhs);
    // Return 'true' if the specified 'lhs' and 'rhs' iterators do not have the
    // same value, and 'false' otherwise.  Two 'Timetable_ConstIterator'
    // iterators do not have the same value if they do not refer to the same
    // timetable, or do not refer to the same transition.

// ============================================================================
//                             INLINE DEFINITIONS
// ============================================================================

                        // -------------------------
                        // class TimetableTransition
                        // -------------------------

// PRIVATE CREATORS
inline
TimetableTransition::TimetableTransition(const Datetime& datetime, int code)
: d_datetime(datetime)
, d_code(code)
{
}

// CREATORS
inline
TimetableTransition::TimetableTransition(const TimetableTransition& original)
: d_datetime(original.d_datetime)
, d_code(original.d_code)
{
}

// MANIPULATORS
inline
TimetableTransition& TimetableTransition::operator=(
                                                const TimetableTransition& rhs)
{
    d_datetime = rhs.d_datetime;
    d_code     = rhs.d_code;

    return *this;
}

// ACCESSORS
inline
const Datetime& TimetableTransition::datetime() const
{
    return d_datetime;
}

inline
int TimetableTransition::transitionCode() const
{
    return d_code;
}

}  // close package namespace

// FREE OPERATORS
inline
bool bdlt::operator==(const TimetableTransition& lhs,
                      const TimetableTransition& rhs)
{
    return lhs.d_datetime == rhs.d_datetime && lhs.d_code == rhs.d_code;
}

inline
bool bdlt::operator!=(const TimetableTransition& lhs,
                      const TimetableTransition& rhs)
{
    return lhs.d_datetime != rhs.d_datetime || lhs.d_code != rhs.d_code;
}

inline
bool bdlt::operator<(const TimetableTransition& lhs,
                     const TimetableTransition& rhs)
{
    return lhs.d_datetime < rhs.d_datetime
        || (lhs.d_datetime == rhs.d_datetime && lhs.d_code < rhs.d_code);
}

inline
bool bdlt::operator<(const TimetableTransition& lhs, const Datetime& rhs)
{
    return lhs.d_datetime < rhs;
}

inline
bool bdlt::operator<(const Datetime& lhs, const TimetableTransition& rhs)
{
    return lhs < rhs.d_datetime;
}

// HASH SPECIALIZATIONS
template <class HASHALG>
inline
void bdlt::hashAppend(HASHALG& hashAlg, const TimetableTransition& object)
{
    using ::BloombergLP::bslh::hashAppend;

    hashAppend(hashAlg, object.d_datetime);
    hashAppend(hashAlg, object.d_code);
}

namespace bdlt {

                           // --------------------
                           // struct Timetable_Day
                           // --------------------

// CREATORS
inline
Timetable_Day::Timetable_Day(bslma::Allocator *basicAllocator)
: d_initialTransitionCode(k_UNSET_TRANSITION_CODE)
, d_transition(basicAllocator)
{
    BSLS_ASSERT_SAFE(0 != basicAllocator);
}

inline
Timetable_Day::Timetable_Day(const Timetable_Day&  original,
                             bslma::Allocator     *basicAllocator)
: d_initialTransitionCode(original.d_initialTransitionCode)
, d_transition(original.d_transition, basicAllocator)
{
    BSLS_ASSERT_SAFE(0 != basicAllocator);
}

// MANIPULATORS
inline
Timetable_Day& Timetable_Day::operator=(const Timetable_Day& rhs)
{
    d_initialTransitionCode = rhs.d_initialTransitionCode;
    d_transition            = rhs.d_transition;

    return *this;
}

inline
bool Timetable_Day::setInitialTransitionCode(int transitionCode)
{
    bool rv = d_initialTransitionCode != transitionCode
           && d_transition.empty();

    d_initialTransitionCode = transitionCode;

    return rv;
}

inline
bool Timetable_Day::addTransition(const Datetime& datetime, int transitionCode)
{
    BSLS_ASSERT_SAFE(24 > datetime.hour());

    BSLS_ASSERT_SAFE(                                    0 <= transitionCode
                     || Timetable::k_UNSET_TRANSITION_CODE == transitionCode);

    BSLS_ASSERT_SAFE(   d_transition.empty()
                     || d_transition[0].datetime().date() == datetime.date());

    int code = finalTransitionCode();

    bsl::vector<TimetableTransition>::iterator iter =
                                         bsl::lower_bound(d_transition.begin(),
                                                          d_transition.end(),
                                                          datetime);
    if (iter == d_transition.end() || iter->d_datetime != datetime) {
        d_transition.insert(iter, TimetableTransition(datetime,
                                                      transitionCode));
    }
    else {
        iter->d_code = transitionCode;
    }

    return code != finalTransitionCode();
}

inline
bool Timetable_Day::removeAllTransitions()
{
    int code = finalTransitionCode();

    d_transition.clear();

    return code != d_initialTransitionCode;
}

inline
bool Timetable_Day::removeTransition(const Datetime& datetime)
{
    BSLS_ASSERT_SAFE(   d_transition.empty()
                     || d_transition[0].datetime().date() == datetime.date());

    bsl::vector<TimetableTransition>::iterator iter =
                                         bsl::lower_bound(d_transition.begin(),
                                                          d_transition.end(),
                                                          datetime);

    if (iter == d_transition.end() || iter->d_datetime != datetime) {
        return false;                                                 // RETURN
    }

    int code = finalTransitionCode();

    d_transition.erase(iter);

    return finalTransitionCode() != code;
}

// ACCESSORS
inline
int Timetable_Day::finalTransitionCode() const
{
    bsl::vector<TimetableTransition>::const_reverse_iterator iter =
                                                         d_transition.rbegin();

    return iter != d_transition.rend()
         ? iter->d_code
         : d_initialTransitionCode;
}

inline
int Timetable_Day::initialTransitionCode() const
{
    return d_initialTransitionCode;
}

inline
bsl::size_t Timetable_Day::size() const
{
    return d_transition.size();
}

inline
int Timetable_Day::transitionCodeInEffect(const Datetime& datetime) const
{
    BSLS_ASSERT_SAFE(24 > datetime.hour());

    BSLS_ASSERT_SAFE(   d_transition.empty()
                     || d_transition[0].datetime().date() == datetime.date());

    bsl::vector<TimetableTransition>::const_iterator iter =
                                         bsl::upper_bound(d_transition.begin(),
                                                          d_transition.end(),
                                                          datetime);
    if (iter == d_transition.end()) {
        return finalTransitionCode();                                 // RETURN
    }
    else if (iter == d_transition.begin()) {
        return d_initialTransitionCode;                               // RETURN
    }

    --iter;

    return iter->d_code;
}

}  // close package namespace

// FREE OPERATORS
inline
bool bdlt::operator==(const Timetable_Day& lhs, const Timetable_Day& rhs)
{
    return lhs.d_initialTransitionCode == rhs.d_initialTransitionCode
        && lhs.d_transition            == rhs.d_transition;
}

inline
bool bdlt::operator!=(const Timetable_Day& lhs, const Timetable_Day& rhs)
{
    return lhs.d_initialTransitionCode != rhs.d_initialTransitionCode
        || lhs.d_transition            != rhs.d_transition;
}

inline
bool bdlt::operator<(const Timetable_Day& lhs, const Timetable_Day& rhs)
{
    return lhs.d_initialTransitionCode < rhs.d_initialTransitionCode
        || (   lhs.d_initialTransitionCode == rhs.d_initialTransitionCode
            && lhs.d_transition < rhs.d_transition);
}

// HASH SPECIALIZATIONS
template <class HASHALG>
inline
void bdlt::hashAppend(HASHALG& hashAlg, const Timetable_Day& object)
{
    using ::BloombergLP::bslh::hashAppend;

    hashAppend(hashAlg, object.d_initialTransitionCode);
    hashAppend(hashAlg, object.d_transition);
}

namespace bdlt {

                             // ---------------
                             // class Timetable
                             // ---------------

// MANIPULATORS
inline
Timetable& Timetable::operator=(const Timetable& rhs)
{
    Timetable(rhs, allocator()).swap(*this);

    return *this;
}

inline
void Timetable::addTransition(const Date& date,
                              const Time& time,
                              int         transitionCode)
{
    addTransition(Datetime(date, time), transitionCode);
}

inline
void Timetable::removeAll()
{
    d_initialTransitionCode = k_UNSET_TRANSITION_CODE;

    d_firstDate = Date(9999, 12, 31);
    d_lastDate  = Date(   1,  1,  1);

    d_timetable.removeAll();
}

inline
void Timetable::removeTransition(const Date& date, const Time& time)
{
    removeTransition(Datetime(date, time));
}

                                  // Aspects

inline
void Timetable::swap(Timetable& other)
{
    // 'swap' is undefined for objects with non-equal allocators.

    BSLS_ASSERT_SAFE(allocator() == other.allocator());

    bslalg::SwapUtil::swap(&d_initialTransitionCode,
                           &other.d_initialTransitionCode);

    bslalg::SwapUtil::swap(&d_firstDate, &other.d_firstDate);
    bslalg::SwapUtil::swap(&d_lastDate,  &other.d_lastDate);
    bslalg::SwapUtil::swap(&d_timetable, &other.d_timetable);
}

// ACCESSORS
inline
Timetable::const_iterator Timetable::begin() const
{
    bsl::size_t dayIndex = 0;

    while (dayIndex < d_timetable.length()
        && 0 == d_timetable[dayIndex].size()) {
        ++dayIndex;
    }
    return Timetable_ConstIterator(*this, dayIndex, 0);
}

inline
Timetable::const_iterator Timetable::end() const
{
    return Timetable_ConstIterator(*this, d_timetable.length(), 0);
}

inline
const Date& Timetable::firstDate() const
{
    BSLS_ASSERT_SAFE(0 < length());

    return d_firstDate;
}

inline
int Timetable::initialTransitionCode() const
{
    return d_initialTransitionCode;
}

inline
bool Timetable::isInRange(const Date& date) const
{
    return date >= d_firstDate && date <= d_lastDate;
}

inline
const Date& Timetable::lastDate() const
{
    BSLS_ASSERT_SAFE(0 < length());

    return d_lastDate;
}

inline
int Timetable::length() const
{
    return d_firstDate <= d_lastDate ? d_lastDate - d_firstDate + 1 : 0;
}

inline
int Timetable::transitionCodeInEffect(const Date& date, const Time& time) const
{
    return transitionCodeInEffect(Datetime(date, time));
}

inline
int Timetable::transitionCodeInEffect(const Datetime& datetime) const
{
    BSLS_ASSERT_SAFE(24 > datetime.hour());
    BSLS_ASSERT_SAFE(isInRange(datetime.date()));

    bsl::size_t          index = datetime.date() - d_firstDate;
    const Timetable_Day& daily = d_timetable[index];

    return daily.transitionCodeInEffect(datetime);
}

                                  // Aspects

inline
bslma::Allocator *Timetable::allocator() const
{
    return d_timetable.allocator();
}

}  // close package namespace

// FREE OPERATORS
inline
bool bdlt::operator==(const Timetable& lhs, const Timetable& rhs)
{
    return lhs.d_initialTransitionCode == rhs.d_initialTransitionCode
        && lhs.d_firstDate             == rhs.d_firstDate
        && lhs.d_lastDate              == rhs.d_lastDate
        && lhs.d_timetable             == rhs.d_timetable;
}

inline
bool bdlt::operator!=(const Timetable& lhs, const Timetable& rhs)
{
    return lhs.d_initialTransitionCode != rhs.d_initialTransitionCode
        || lhs.d_firstDate             != rhs.d_firstDate
        || lhs.d_lastDate              != rhs.d_lastDate
        || lhs.d_timetable             != rhs.d_timetable;
}

inline
bsl::ostream& bdlt::operator<<(bsl::ostream&    stream,
                               const Timetable& timetable)
{
    timetable.print(stream, 0, -1);
    return stream;
}

// FREE FUNCTIONS
inline
void bdlt::swap(Timetable& a, Timetable& b)
{
    a.swap(b);
}

// HASH SPECIALIZATIONS
template <class HASHALG>
inline
void bdlt::hashAppend(HASHALG& hashAlg, const Timetable& object)
{
    using ::BloombergLP::bslh::hashAppend;

    hashAppend(hashAlg, object.d_firstDate);
    hashAppend(hashAlg, object.d_lastDate);
    hashAppend(hashAlg, object.d_initialTransitionCode);
    hashAppend(hashAlg, object.d_timetable);
}

namespace bdlt {

                      // =============================
                      // class Timetable_ConstIterator
                      // =============================

// PRIVATE CREATORS
inline
Timetable_ConstIterator::Timetable_ConstIterator(const Timetable& timetable,
                                                 bsl::size_t      dayIndex,
                                                 bsl::size_t      timeIndex)
: d_timetable_p(&timetable)
, d_dayIndex(dayIndex)
, d_timeIndex(timeIndex)
{
}

// CREATORS
inline
Timetable_ConstIterator::Timetable_ConstIterator()
: d_timetable_p(0)
, d_dayIndex(0)
, d_timeIndex(0)
{
}

inline
Timetable_ConstIterator::Timetable_ConstIterator(
                                       const Timetable_ConstIterator& original)
: d_timetable_p(original.d_timetable_p)
, d_dayIndex(original.d_dayIndex)
, d_timeIndex(original.d_timeIndex)
{
}

// MANIPULATORS
inline
Timetable_ConstIterator& Timetable_ConstIterator::
                                  operator=(const Timetable_ConstIterator& rhs)
{
    d_timetable_p = rhs.d_timetable_p;
    d_dayIndex    = rhs.d_dayIndex;
    d_timeIndex   = rhs.d_timeIndex;

    return *this;
}

inline
Timetable_ConstIterator& Timetable_ConstIterator::operator++()
{
    BSLS_ASSERT_SAFE(d_dayIndex < d_timetable_p->d_timetable.length());

    ++d_timeIndex;
    while (d_dayIndex < d_timetable_p->d_timetable.length()
        && d_timeIndex == d_timetable_p->d_timetable[d_dayIndex].size()) {
        d_timeIndex = 0;
        ++d_dayIndex;
    }

    return *this;
}

inline
Timetable_ConstIterator& Timetable_ConstIterator::operator--()
{
    BSLS_ASSERT_SAFE(0 < d_dayIndex || 0 < d_timeIndex);

    if (d_timeIndex) {
        --d_timeIndex;
    }
    else {
        --d_dayIndex;

        while (d_dayIndex
            && 0 == d_timetable_p->d_timetable[d_dayIndex].size()) {
            --d_dayIndex;
        }

        BSLS_ASSERT_SAFE(0 < d_timetable_p->d_timetable[d_dayIndex].size());

        d_timeIndex = d_timetable_p->d_timetable[d_dayIndex].size() - 1;
    }

    return *this;
}

// ACCESSORS
inline
const TimetableTransition& Timetable_ConstIterator::operator*() const
{
    BSLS_ASSERT_SAFE(d_timetable_p);
    BSLS_ASSERT_SAFE(d_dayIndex
                          < static_cast<bsl::size_t>(d_timetable_p->length()));

    return d_timetable_p->d_timetable[d_dayIndex].d_transition[d_timeIndex];
}

inline
const TimetableTransition *Timetable_ConstIterator::operator->() const
{
    BSLS_ASSERT_SAFE(d_timetable_p);
    BSLS_ASSERT_SAFE(d_dayIndex
                          < static_cast<bsl::size_t>(d_timetable_p->length()));

    return &d_timetable_p->d_timetable[d_dayIndex].d_transition[d_timeIndex];
}

}  // close package namespace

// FREE OPERATORS
inline
bdlt::Timetable_ConstIterator bdlt::operator++(
                                             Timetable_ConstIterator& iterator,
                                             int)
{
    const Timetable_ConstIterator curr = iterator;
    ++iterator;
    return curr;
}

inline
bdlt::Timetable_ConstIterator bdlt::operator--(
                                             Timetable_ConstIterator& iterator,
                                             int)
{
    const Timetable_ConstIterator curr = iterator;
    --iterator;
    return curr;
}

inline
bool bdlt::operator==(const Timetable_ConstIterator& lhs,
                      const Timetable_ConstIterator& rhs)
{
    return lhs.d_timetable_p == rhs.d_timetable_p
        && lhs.d_dayIndex    == rhs.d_dayIndex
        && lhs.d_timeIndex   == rhs.d_timeIndex;
}

inline
bool bdlt::operator!=(const Timetable_ConstIterator& lhs,
                      const Timetable_ConstIterator& rhs)
{
    return lhs.d_timetable_p != rhs.d_timetable_p
        || lhs.d_dayIndex    != rhs.d_dayIndex
        || lhs.d_timeIndex   != rhs.d_timeIndex;
}

}  // close enterprise namespace

// TRAITS

namespace BloombergLP {
namespace bslma {

template <>
struct UsesBslmaAllocator<bdlt::Timetable_Day> : bsl::true_type {};

template <>
struct UsesBslmaAllocator<bdlt::Timetable> : bsl::true_type {};

}  // close namespace bslma
}  // close enterprise namespace

#endif

// ----------------------------------------------------------------------------
// Copyright 2018 Bloomberg Finance L.P.
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
