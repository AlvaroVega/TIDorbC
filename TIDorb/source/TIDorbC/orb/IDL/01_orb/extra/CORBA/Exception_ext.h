#include "CORBA.h"


#ifndef _CORBA_EXCEPTION_EXT_H_
#define _CORBA_EXCEPTION_EXT_H_

#ifdef TIDORB_HAVE_IOSTREAM
  #include <iostream>
#else
  #include <iostream.h>
#endif

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif


void operator<<=(::CORBA::Any& any, const ::CORBA::Exception& e); // copying
void operator<<=(::CORBA::Any& any, ::CORBA::Exception* e); // non-copying

ostream& operator<<(ostream& os, const ::CORBA::Exception& e);
ostream& operator<<(ostream& os, const ::CORBA::Exception* e);


#endif
