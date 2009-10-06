/*//////////////////////////////////////////////////////////////////////////
//
// File:        ErrnoStr.h
// Description: 
//             
// Rel:         01.00
// Created:     September, 2001
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
//
// Revised:
//
// Copyright 2001 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
//////////////////////////////////////////////////////////////////////////*/

#ifndef __TID_THR_ERRNO_STR_H__
#define __TID_THR_ERRNO_STR_H__

#include <string>
#include <stdio.h>
#include <sys/errno.h>

namespace TIDThr {

const char *enostr(int a);

} // namespace TIDThr 

#endif // __TID_THR_ERRNO_STR_H__
