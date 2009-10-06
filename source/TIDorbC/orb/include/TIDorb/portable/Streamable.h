/*/////////////////////////////////////////////////////////////////////////
//
// File:        Streamable.h
// Description: 
//              
// Rel:         01.00
// Created:     November, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
//
// Revised:
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////*/

#include "TIDorb/portable.h"

#ifndef _TIDORB_PORTABLE_STREAMABLE_H_
#define _TIDORB_PORTABLE_STREAMABLE_H_ 


namespace TIDorb {
namespace portable {

class Streamable : public virtual TIDorb::portable::RefCounter {

  public:


    virtual ~Streamable() {}
    
    virtual void _read(InputStream& input) = 0;
    virtual void _write(OutputStream& out) const = 0;
    virtual CORBA::TypeCode_ptr _type() const = 0;

  protected:
    Streamable() {_count(1);}
};
  
}
}

#endif
