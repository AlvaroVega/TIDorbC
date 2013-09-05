#include "CORBA.h"


#ifndef _CORBA_VALUE_BASE_EXT_H_
#define _CORBA_VALUE_BASE_EXT_H_


inline void CORBA::add_ref(CORBA::ValueBase* vb)
{
  if (vb) vb->_add_ref();
}

inline void CORBA::remove_ref(CORBA::ValueBase* vb)
{
  if (vb) vb->_remove_ref();
}


#endif //_CORBA_VALUE_BASE_EXT_H_
