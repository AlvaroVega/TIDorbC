#include "TIDorb/core/typecode.h"

#ifndef _TIDorb_core_typecode_RecursiveTypeCode_H_
#define _TIDorb_core_typecode_RecursiveTypeCode_H_

#include "CORBA.h"
#include "TIDThr.h"


namespace TIDorb {
namespace core {
namespace typecode {


class RecursiveTypeCode : public TypeCodeImpl
{
  public:
    RecursiveTypeCode()
       throw (TIDThr::SystemException);

    RecursiveTypeCode(const char* id)
       throw (TIDThr::SystemException);

    ~RecursiveTypeCode()
       throw (TIDThr::SystemException);

    CORBA::Boolean equal(CORBA::TypeCode_ptr) const;

    const char* id() const;

    // TIDorb operations

    bool isSimple()const;

    /**
    * TypeCode marshalling.
    */

    //void marshal(TIDorb::core::cdr::CDROutputStream& output);

    void partial_unmarshal(TIDorb::core::cdr::CDRInputStream& input);

//     bool skip_value(TIDorb::core::cdr::CDRInputStream& input) const;

//     void remarshal_value(TIDorb::core::cdr::CDRInputStream& input,
//                          TIDorb::core::cdr::CDROutputStream& output) const;

//     bool values_equal (TIDorb::core::cdr::CDRInputStream& a_input,
//                        TIDorb::core::cdr::CDRInputStream& b_input) const;

    void dump (ostream& output) const;

    bool dump_value(TIDorb::core::cdr::CDRInputStream& input, ostream& output) const;

  protected:

    char* m_id;

};


}
}
}

#endif
