///////////////////////////////////////////////////////////////////////////
//
// File:        TIDThr_ErrnoStr.C
// Description: This file contains the Errno errors
// Rel:         01.00
// Created:     May, 2001
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
//
// Revised:
//   02/29/2008 by Alvaro Polo <apv@tid.es>
//      * Some errno values not defined in Darwin OS (Mac OS X) included
//        conditionally
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
///////////////////////////////////////////////////////////////////////////

#include "TIDThr.h"


namespace TIDThr {
    
    

#define ENT(n)  {n, #n}

struct e_ent {
  int   eno;
  const char    *estr;
};


/* Table of errno CODES
 * taken from Osr5.0.5 /usr/include/sys/errno.h
 */
static struct e_ent etbl[] =  { 
  /* Error codes */
  ENT(EPERM),
  ENT(ENOENT),
  ENT(ESRCH),
  ENT(EINTR),
  ENT(EIO),
  ENT(ENXIO),
  ENT(E2BIG),
  ENT(ENOEXEC),
  ENT(EBADF),
  ENT(ECHILD),
  ENT(EAGAIN),
  ENT(ENOMEM),
  ENT(EACCES),
  ENT(EFAULT),
  ENT(ENOTBLK),
  ENT(EBUSY),
  ENT(EEXIST),
  ENT(EXDEV),
  ENT(ENODEV),
  ENT(ENOTDIR),
  ENT(EISDIR),
  ENT(EINVAL),
  ENT(ENFILE),
  ENT(EMFILE),
  ENT(ENOTTY),
  ENT(ETXTBSY),
  ENT(EFBIG),
  ENT(ENOSPC),
  ENT(ESPIPE),
  ENT(EROFS),
  ENT(EMLINK),
  ENT(EPIPE),
  ENT(EDOM),
  ENT(ERANGE),
  ENT(ENOMSG),
  ENT(EIDRM),
#ifndef __darwin
  ENT(ECHRNG),
  ENT(EL2NSYNC),
  ENT(EL3HLT),
  ENT(EL3RST),
  ENT(ELNRNG),
  ENT(EUNATCH),
  ENT(ENOCSI),
  ENT(EL2HLT),
#endif
  ENT(EDEADLK),
  ENT(ENOLCK),
#ifndef __HP_aCC
#ifndef __darwin
  ENT(EBADE),
  ENT(EBADR),
  ENT(EXFULL),
  ENT(ENOANO),
  ENT(EBADRQC),
  ENT(EBADSLT),
  ENT(EDEADLOCK),
  ENT(EBFONT),
#endif
#endif
  /* stream problems */
  ENT(ENOSTR),
  ENT(ENODATA),
  ENT(ETIME),
  ENT(ENOSR),
#ifndef __darwin
  ENT(ENONET),
  ENT(ENOPKG),
  ENT(EREMOTE),
  ENT(ENOLINK),
  ENT(EADV),
  ENT(ESRMNT),
  ENT(ECOMM),
#endif
  ENT(EPROTO),
  ENT(EMULTIHOP),
#ifdef ELBIN
  ENT(ELBIN),
#endif  
#ifndef __sun
#ifndef __darwin
  ENT(EDOTDOT),
#endif
#endif
  ENT(EBADMSG),
  ENT(ENAMETOOLONG),
  ENT(EOVERFLOW),
#ifndef __HP_aCC
#ifndef __darwin
  ENT(ENOTUNIQ),
  ENT(EBADFD),
  ENT(EREMCHG),
  /* shared library problems */
  ENT(ELIBACC),
  ENT(ELIBBAD),
  ENT(ELIBSCN),
  ENT(ELIBMAX),
  ENT(ELIBEXEC),
#endif
#endif
  ENT(EILSEQ),
  ENT(ENOSYS),
  /* non-blocking and interrupt i/o */
  ENT(EWOULDBLOCK),
  ENT(EINPROGRESS),
  ENT(EALREADY),
  /* ipc/network software arg errors */
  ENT(ENOTSOCK),
  ENT(EDESTADDRREQ),
  ENT(EMSGSIZE),
  ENT(EPROTOTYPE),
  ENT(EPROTONOSUPPORT),
  ENT(ESOCKTNOSUPPORT),
  ENT(EOPNOTSUPP),
  ENT(EPFNOSUPPORT),
  ENT(EAFNOSUPPORT),
  ENT(EADDRINUSE),
  ENT(EADDRNOTAVAIL),
  /* ipc/network software arg -  operational errors */
  ENT(ENETDOWN),
  ENT(ENETUNREACH),
  ENT(ENETRESET),
  ENT(ECONNABORTED),
  ENT(ECONNRESET),
  ENT(ENOBUFS),
  ENT(EISCONN),
  ENT(ENOTCONN),
  ENT(ESHUTDOWN),
  ENT(ETOOMANYREFS),
  ENT(ETIMEDOUT),
  ENT(ECONNREFUSED),
  ENT(EHOSTDOWN),
  ENT(EHOSTUNREACH),
  ENT(ENOPROTOOPT),
  /* Xenix */
#ifndef __HP_aCC
#ifndef __sun
#ifndef __darwin
  ENT(EUCLEAN),
  ENT(ENOTNAM),

  ENT(ENAVAIL),
  ENT(EISNAM),
  ENT(EREMOTEIO),
#endif
#endif
#endif
#ifdef EINIT
  ENT(EINIT),
#endif
#ifdef EREMDEV
  ENT(EREMDEV),
#endif
  ENT(ENOTEMPTY),
  ENT(ELOOP),
  ENT(ESTALE),
#ifndef __HP_aCC
#ifndef __darwin
  ENT(ERESTART),
  ENT(ESTRPIPE),
#endif
#endif
#ifdef EIORESID
  ENT(EIORESID)
#endif
#ifdef __linux
  ENT(EWOULDBLOCK), /*  41  mapped to EAGAIN */
  ENT(EDEADLOCK),   /*  58  mapped to EDEADLK */
  ENT(EUSERS),  /*  87  Too many users */
  ENT(EDQUOT),  /* 122  Quota exceeded */
  ENT(ENOMEDIUM),   /* 123  No medium found */
  ENT(EMEDIUMTYPE), /* 124  Wrong medium type */
#endif

};    

/* lookup the string for a given error number */ 

const char*
enostr(int a)
{    
    for (size_t i = 0; i < sizeof(etbl)/sizeof(etbl[0]); i++) {
        if (a == etbl[i].eno) {
            return etbl[i].estr;
        }
    }
    
    return "unknown";	
}
  
} // namespace TIDThr 
