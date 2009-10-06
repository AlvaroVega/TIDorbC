//////////////////////////////////////////////////////////////////////////
//
// File:        config.h
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
//////////////////////////////////////////////////////////////////////////

#ifndef __TID_THR_CONFIG_H__
#define __TID_THR_CONFIG_H__



/* define if the compiler implements 'std' namespace. */
#if (defined(__SUNPRO_CC) && (__SUNPRO_CC_COMPAT==5) && !defined(__SUNPRO_LIBRARY_IOSTREAM)) || \
    (defined(__HP_aCC)    && defined(_HP_NAMESPACE_STD)) || \
    (defined(__linux)) || \
    (defined(__sun)       && ((__GNUC__ > 2) || (__GNUC__ == 2 &&  __GNUC_MINOR__ >= 95)))
#ifndef TIDTHR_HAVE_NAMESPACE_STD
#define TIDTHR_HAVE_NAMESPACE_STD 1
#endif
#endif


/* define if you have the <iostream> header file. */
#if (defined(__SUNPRO_CC) && (__SUNPRO_CC_COMPAT==5) && !defined(__SUNPRO_LIBRARY_IOSTREAM)) || \
    (defined(__HP_aCC)    && defined(_HP_NAMESPACE_STD)) || \
    (defined(__linux)     && ((__GNUC__ > 2) || (__GNUC__ == 2 &&  __GNUC_MINOR__ >= 95))) || \
    (defined(__sun)       && ((__GNUC__ > 2) || (__GNUC__ == 2 &&  __GNUC_MINOR__ >= 95)))
#ifndef TIDTHR_HAVE_IOSTREAM
#define TIDTHR_HAVE_IOSTREAM 1
#endif
#endif


#endif //__TID_THR_CONFIG_H__
