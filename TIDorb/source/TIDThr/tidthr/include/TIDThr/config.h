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
// (C) Copyright 2009 Telefonica Investigacion y Desarrollo
//     S.A.Unipersonal (Telefonica I+D)
//
// This file is part of Morfeo CORBA Platform.
//
// Morfeo CORBA Platform is free software: you can redistribute it and/or
// modify it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the License,
// or (at your option) any later version.
//
// Morfeo CORBA Platform is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with Morfeo CORBA Platform. If not, see
//
//   http://www.gnu.org/licenses
//
// Info about members and contributors of the MORFEO project
// is available at
//
//   http://morfeo-project.org
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
