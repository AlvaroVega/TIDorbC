#include "TIDorb/core/typecode.h"

#ifndef _TIDorb_core_typecode_SequenceTypeCode_H_
#define _TIDorb_core_typecode_SequenceTypeCode_H_

#include "CORBA.h"
#include "TIDThr.h"


#ifdef TIDORB_HAVE_IOSTREAM
  #include <iostream>
#else
  #include <iostream.h>
#endif

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif


namespace TIDorb {
namespace core {
namespace typecode {


class SequenceTypeCode : public ArrayTypeCode
{
  public:
    SequenceTypeCode()
       throw (TIDThr::SystemException);

    SequenceTypeCode(CORBA::TypeCode_ptr element_type, CORBA::ULong length)
       throw (TIDThr::SystemException);

    ~SequenceTypeCode()
       throw (TIDThr::SystemException);

    /**
    * TypeCode marshalling.
    */

    void skip_params(TIDorb::core::cdr::CDRInputStream& input) const;

    bool skip_value(TIDorb::core::cdr::CDRInputStream& input) const;

    void remarshal_value(TIDorb::core::cdr::CDRInputStream& input,
                                 TIDorb::core::cdr::CDROutputStream& output) const;

    bool values_equal (TIDorb::core::cdr::CDRInputStream& a_input,
                       TIDorb::core::cdr::CDRInputStream& b_input) const;

    void dump (ostream& output) const;

    bool dump_value(TIDorb::core::cdr::CDRInputStream& input, ostream& output) const;

};


}
}
}

#endif
