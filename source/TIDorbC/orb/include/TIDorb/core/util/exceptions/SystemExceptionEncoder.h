/* NO_PROCESSING */

#include "TIDorb/core/util/exceptions.h"

#ifndef _TIDORB_CORE_UTIL_EXCEPTIONS_SYSTEM_EXCEPTION_ENCODER_H_
#define _TIDORB_CORE_UTIL_EXCEPTIONS_SYSTEM_EXCEPTION_ENCODER_H_

#include "CORBA.h"


namespace TIDorb {
namespace core {
namespace util {
namespace exceptions {


class SystemExceptionEncoder {

  public:


    static SystemExceptionCreatorTable* init_creators();

    static void write(TIDorb::portable::OutputStream& out,
                      const CORBA::SystemException& sys);

    static CORBA::SystemException* read(TIDorb::portable::InputStream& in);
    static CORBA::SystemException* copy(const CORBA::SystemException& se);
    static CORBA::SystemException* copy(const char* rep_id);

  protected:

    static const SystemExceptionCreatorTable* st_creators_table;		

};

} // exceptions
} // util
} // core
} // TIDorb

#endif
