///////////////////////////////////////////////////////////////////////////
//
// File:        TIDorb/config.h
// Description: 
// Rel:         01.00
// Created:     May, 2001
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

#ifndef _TIDORB_CONFIG_H_
#define _TIDORB_CONFIG_H_

/* define if the compiler implements 'std' namespace. */
#if (defined(__SUNPRO_CC) && (__SUNPRO_CC_COMPAT==5) && !defined(__SUNPRO_LIBRARY_IOSTREAM)) || \
    (defined(__HP_aCC)    && defined(_HP_NAMESPACE_STD)) || \
    (defined(__linux)) || \
    (defined(__sun)       && ((__GNUC__ > 2) || (__GNUC__ == 2 &&  __GNUC_MINOR__ >= 95)))
#ifndef TIDORB_HAVE_NAMESPACE_STD
#define TIDORB_HAVE_NAMESPACE_STD 1
#endif
#endif


/* define if you have the <iostream> header file. */
#if (defined(__SUNPRO_CC) && (__SUNPRO_CC_COMPAT==5) && !defined(__SUNPRO_LIBRARY_IOSTREAM)) || \
    (defined(__HP_aCC)    && defined(_HP_NAMESPACE_STD)) || \
    (defined(__linux)     && ((__GNUC__ > 2) || (__GNUC__ == 2 &&  __GNUC_MINOR__ >= 95))) || \
    (defined(__sun)       && ((__GNUC__ > 2) || (__GNUC__ == 2 &&  __GNUC_MINOR__ >= 95)))
#ifndef TIDORB_HAVE_IOSTREAM
#define TIDORB_HAVE_IOSTREAM 1
#endif
#endif


/* define if you have the <fstream> header file. */
#if (defined(__SUNPRO_CC) && (__SUNPRO_CC_COMPAT==5) && !defined(__SUNPRO_LIBRARY_IOSTREAM)) || \
    (defined(__HP_aCC)    && defined(_HP_NAMESPACE_STD)) || \
    (defined(__linux)     && ((__GNUC__ > 2) || (__GNUC__ == 2 &&  __GNUC_MINOR__ >= 95))) || \
    (defined(__sun)       && ((__GNUC__ > 2) || (__GNUC__ == 2 &&  __GNUC_MINOR__ >= 95)))
#ifndef TIDORB_HAVE_FSTREAM
#define TIDORB_HAVE_FSTREAM 1
#endif
#endif


/* define if you have the <sstream> header file. */
#if (defined(__SUNPRO_CC) && (__SUNPRO_CC_COMPAT==5) && !defined(__SUNPRO_LIBRARY_IOSTREAM)) || \
    (defined(__HP_aCC)    && defined(_HP_NAMESPACE_STD))  || \
    (defined(__linux)     && ((__GNUC__ > 2) || (__GNUC__ == 2 &&  __GNUC_MINOR__ >= 95))) || \
    (defined(__sun)       && ((__GNUC__ > 2) || (__GNUC__ == 2 &&  __GNUC_MINOR__ >= 95)))
#ifndef TIDORB_HAVE_SSTREAM
#define TIDORB_HAVE_SSTREAM 1
#endif
#endif


/* define if you have the <strstream> header file. */
#if (defined(__SUNPRO_CC) && (__SUNPRO_CC_COMPAT==5) && !defined(__SUNPRO_LIBRARY_IOSTREAM)) || \
    (defined(__HP_aCC)    && defined(_HP_NAMESPACE_STD)) || \
    defined(__linux) || \
    (defined(__sun)       && ((__GNUC__ > 2) || (__GNUC__ == 2 &&  __GNUC_MINOR__ >= 95)))
#ifndef TIDORB_HAVE_STRSTREAM
#define TIDORB_HAVE_STRSTREAM 1
#endif
#endif


/* Version number of package */
#define __TIDorbC__ 060000L


/* CORBA Standard Compliance */
#define __CORBA_VERSION__ 0x0206


#endif // _TIDORB_CONFIG_H_

