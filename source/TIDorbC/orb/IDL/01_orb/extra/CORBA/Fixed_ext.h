#include "CORBA.h"


#ifndef _CORBA_FIXED_EXT_H_
#define _CORBA_FIXED_EXT_H_

#ifdef TIDORB_HAVE_IOSTREAM
  #include <iostream>
#else
  #include <iostream.h>
#endif

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
//
#endif

inline istream& operator>>(istream& is, CORBA::Fixed& val){return is;}
inline ostream& operator<<(ostream& os, const CORBA::Fixed& val){return os;}


#endif
