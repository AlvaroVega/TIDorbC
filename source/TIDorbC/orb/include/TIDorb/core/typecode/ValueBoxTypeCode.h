#include "TIDorb/core/typecode.h"

#ifndef _TIDorb_core_typecode_ValueBoxTypeCode_H_
#define _TIDorb_core_typecode_ValueBoxTypeCode_H_

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


class ValueBoxTypeCode : public ComplexTypeCode
{
  public:
    ValueBoxTypeCode()
       throw (TIDThr::SystemException);

    ValueBoxTypeCode(const char* id, const char* name, ::CORBA::TypeCode_ptr boxed_type)
       throw (TIDThr::SystemException);

    ~ValueBoxTypeCode()
       throw (TIDThr::SystemException);

    CORBA::Boolean equal(CORBA::TypeCode_ptr tc) const;
    CORBA::Boolean equivalent(CORBA::TypeCode_ptr) const;
    CORBA::TypeCode_ptr content_type() const;

    /**
    * TypeCode marshalling.
    */

    virtual bool skip_value(TIDorb::core::cdr::CDRInputStream& input) const;

    virtual void remarshal_value(TIDorb::core::cdr::CDRInputStream& input,
                                 TIDorb::core::cdr::CDROutputStream& output) const;

    virtual bool values_equal (TIDorb::core::cdr::CDRInputStream& a_input,
                               TIDorb::core::cdr::CDRInputStream& b_input) const;

    virtual void dump (ostream& output) const;

    virtual bool dump_value(TIDorb::core::cdr::CDRInputStream& input, ostream& output) const;

    void write_params(TIDorb::core::cdr::CDROutputStream& output);
    void read_params(TIDorb::core::cdr::CDRInputStream& input);

  protected:
    CORBA::TypeCode_ptr m_boxed_type;

};


}
}
}

#endif
