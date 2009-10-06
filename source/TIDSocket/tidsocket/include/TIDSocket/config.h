///////////////////////////////////////////////////////////////////////////
//
// File:        config.h
// Description: 
// Rel:         01.00
// Created:     May, 2001
// Author:      Pablo Rodriguez Archilla      ( pra@tid.es   )
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
///////////////////////////////////////////////////////////////////////////

#ifndef __TID_SOCKET_CONFIG_H__
#define __TID_SOCKET_CONFIG_H__


/* define if the Sun Workshop C++ compiler uses "-library=iostream" option. */
#if (defined(__SUNPRO_CC) && (__SUNPRO_CC_COMPAT==5))
#include <iostream.h>
#if (defined(_IOSTREAM_H_))
#ifndef __SUNPRO_LIBRARY_IOSTREAM
#define __SUNPRO_LIBRARY_IOSTREAM 1
#endif
#endif
#endif

/* define if the compiler implements 'std' namespace. */
#if (defined(__SUNPRO_CC) && (__SUNPRO_CC_COMPAT==5)) || \
    (defined(__HP_aCC)    && defined(_HP_NAMESPACE_STD)) || \
    (defined(__linux)) || \
    (defined(__sun)       && ((__GNUC__ > 2) || (__GNUC__ == 2 &&  __GNUC_MINOR__ >= 95)))
//    (defined(__GNUC__))
#ifndef TIDSOCKET_HAVE_NAMESPACE_STD
#define TIDSOCKET_HAVE_NAMESPACE_STD 1
#endif
#endif

/* define if iostream classes are defined into 'std' namespace. */
#if (defined(TIDSOCKET_HAVE_NAMESPACE_STD) && \
     !defined(__SUNPRO_LIBRARY_IOSTREAM))
#ifndef TIDSOCKET_HAVE_STD_IOSTREAM_CLASSES
#define TIDSOCKET_HAVE_STD_IOSTREAM_CLASSES 1
#endif
#endif


#endif //__TID_SOCKET_CONFIG_H__
