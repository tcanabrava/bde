// s_baltst_simplerequest.cpp        *DO NOT EDIT*         @generated -*-C++-*-

#include <bsls_ident.h>
BSLS_IDENT_RCSID(s_baltst_simplerequest_cpp,"$Id$ $CSID$")

#include <s_baltst_simplerequest.h>

#include <bdlat_formattingmode.h>
#include <bdlat_valuetypefunctions.h>
#include <bdlb_print.h>
#include <bdlb_printmethods.h>
#include <bdlb_string.h>

#include <bsl_string.h>
#include <s_baltst_address.h>
#include <s_baltst_basicrecord.h>
#include <s_baltst_customint.h>
#include <s_baltst_customstring.h>
#include <s_baltst_customizedstring.h>
#include <s_baltst_enumerated.h>
#include <s_baltst_mychoice.h>
#include <s_baltst_myenumeration.h>
#include <s_baltst_mysequence.h>
#include <s_baltst_mysequencewithanonymouschoicechoice.h>
#include <s_baltst_mysequencewitharray.h>
#include <s_baltst_mysequencewithattributes.h>
#include <s_baltst_mysequencewithnillable.h>
#include <s_baltst_mysequencewithnullable.h>
#include <s_baltst_mysimplecontent.h>
#include <s_baltst_mysimpleintcontent.h>
#include <s_baltst_rawdata.h>
#include <s_baltst_rawdataswitched.h>
#include <s_baltst_rawdataunformatted.h>
#include <s_baltst_sequencewithanonymitychoice1.h>
#include <bslim_printer.h>
#include <bsls_assert.h>

#include <bsl_iomanip.h>
#include <bsl_limits.h>
#include <bsl_ostream.h>

namespace BloombergLP {
namespace s_baltst {

                            // -------------------
                            // class SimpleRequest
                            // -------------------

// CONSTANTS

const char SimpleRequest::CLASS_NAME[] = "SimpleRequest";

const bdlat_AttributeInfo SimpleRequest::ATTRIBUTE_INFO_ARRAY[] = {
    {
        ATTRIBUTE_ID_DATA,
        "data",
        sizeof("data") - 1,
        "",
        bdlat_FormattingMode::e_TEXT
    },
    {
        ATTRIBUTE_ID_RESPONSE_LENGTH,
        "responseLength",
        sizeof("responseLength") - 1,
        "",
        bdlat_FormattingMode::e_DEC
    }
};

// CLASS METHODS

const bdlat_AttributeInfo *SimpleRequest::lookupAttributeInfo(
        const char *name,
        int         nameLength)
{
    for (int i = 0; i < 2; ++i) {
        const bdlat_AttributeInfo& attributeInfo =
                    SimpleRequest::ATTRIBUTE_INFO_ARRAY[i];

        if (nameLength == attributeInfo.d_nameLength
        &&  0 == bsl::memcmp(attributeInfo.d_name_p, name, nameLength))
        {
            return &attributeInfo;
        }
    }

    return 0;
}

const bdlat_AttributeInfo *SimpleRequest::lookupAttributeInfo(int id)
{
    switch (id) {
      case ATTRIBUTE_ID_DATA:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_DATA];
      case ATTRIBUTE_ID_RESPONSE_LENGTH:
        return &ATTRIBUTE_INFO_ARRAY[ATTRIBUTE_INDEX_RESPONSE_LENGTH];
      default:
        return 0;
    }
}

// CREATORS

SimpleRequest::SimpleRequest(bslma::Allocator *basicAllocator)
: d_data(basicAllocator)
, d_responseLength()
{
}

SimpleRequest::SimpleRequest(const SimpleRequest& original,
                             bslma::Allocator *basicAllocator)
: d_data(original.d_data, basicAllocator)
, d_responseLength(original.d_responseLength)
{
}

#if defined(BSLS_COMPILERFEATURES_SUPPORT_RVALUE_REFERENCES) \
 && defined(BSLS_COMPILERFEATURES_SUPPORT_NOEXCEPT)
SimpleRequest::SimpleRequest(SimpleRequest&& original) noexcept
: d_data(bsl::move(original.d_data))
, d_responseLength(bsl::move(original.d_responseLength))
{
}

SimpleRequest::SimpleRequest(SimpleRequest&& original,
                             bslma::Allocator *basicAllocator)
: d_data(bsl::move(original.d_data), basicAllocator)
, d_responseLength(bsl::move(original.d_responseLength))
{
}
#endif

SimpleRequest::~SimpleRequest()
{
}

// MANIPULATORS

SimpleRequest&
SimpleRequest::operator=(const SimpleRequest& rhs)
{
    if (this != &rhs) {
        d_data = rhs.d_data;
        d_responseLength = rhs.d_responseLength;
    }

    return *this;
}

#if defined(BSLS_COMPILERFEATURES_SUPPORT_RVALUE_REFERENCES) \
 && defined(BSLS_COMPILERFEATURES_SUPPORT_NOEXCEPT)
SimpleRequest&
SimpleRequest::operator=(SimpleRequest&& rhs)
{
    if (this != &rhs) {
        d_data = bsl::move(rhs.d_data);
        d_responseLength = bsl::move(rhs.d_responseLength);
    }

    return *this;
}
#endif

void SimpleRequest::reset()
{
    bdlat_ValueTypeFunctions::reset(&d_data);
    bdlat_ValueTypeFunctions::reset(&d_responseLength);
}

// ACCESSORS

bsl::ostream& SimpleRequest::print(
        bsl::ostream& stream,
        int           level,
        int           spacesPerLevel) const
{
    bslim::Printer printer(&stream, level, spacesPerLevel);
    printer.start();
    printer.printAttribute("data", d_data);
    printer.printAttribute("responseLength", d_responseLength);
    printer.end();
    return stream;
}


}  // close package namespace
}  // close enterprise namespace

// GENERATED BY BLP_BAS_CODEGEN_2020.03.30
// USING bas_codegen.pl s_baltst.xsd -m msg -p s_baltst -C tmp_singles --msgSplit 1 --noExternalization --noHashSupport --noAggregateConversion
// ----------------------------------------------------------------------------
// NOTICE:
//      Copyright 2020 Bloomberg Finance L.P. All rights reserved.
//      Property of Bloomberg Finance L.P. (BFLP)
//      This software is made available solely pursuant to the
//      terms of a BFLP license agreement which governs its use.
// ------------------------------- END-OF-FILE --------------------------------