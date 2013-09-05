#ifndef _CORBA_SYSTEM_EXCEPTION_EXT_H_
#define _CORBA_SYSTEM_EXCEPTION_EXT_H_

#include "CORBA/SystemException.h"

#ifdef TIDORB_HAVE_IOSTREAM
#include <iostream>
#else
#include <iostream.h>
#endif

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif


void operator<<=(::CORBA::Any& any, const ::CORBA::SystemException& se);
void operator<<=(::CORBA::Any& any, ::CORBA::SystemException* se);

::CORBA::Boolean operator>>=(const ::CORBA::Any& any, const ::CORBA::SystemException*& se);


ostream& operator<<(ostream& os, const ::CORBA::SystemException& se);
ostream& operator<<(ostream& os, const ::CORBA::SystemException* se);


#endif
