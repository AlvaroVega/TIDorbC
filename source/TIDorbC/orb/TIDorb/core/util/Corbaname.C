/////////////////////////////////////////////////////////////////////////
//
// File:        Corbaname.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
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
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/util.h"

#include <string>

#ifdef TIDORB_HAVE_NAMESPACE_STD
using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
using std::string;
#endif


CORBA::Object_ptr
TIDorb::core::util::Corbaname::to_object(TIDorb::core::TIDORB* tidorb,
                                         const char* corbaname_URL)
throw (CosNaming::NamingContext::InvalidName,
       CosNaming::NamingContext::NotFound,
       CosNaming::NamingContext::CannotProceed,
       CORBA::ORB::InvalidName)
{
  string url = corbaname_URL;

  if (url.find(TIDorb::core::util::URL::CORBANAME_PROTOCOL) == string::npos) {
    throw CosNaming::NamingContext::InvalidName(); // "Invalid corbaname"
  }

  string corbaname = url.substr(strlen(TIDorb::core::util::URL::CORBANAME_PROTOCOL));

  string::size_type nameposition = corbaname.find('#');
  if (nameposition == string::npos) {
    CosNaming::NamingContext_var ncontext =
      parse_protocol(tidorb, corbaname.c_str());
    return CosNaming::NamingContext::_duplicate(ncontext);
  }

  CosNaming::NamingContext_var ncontext =
    parse_protocol(tidorb, corbaname.substr(0, nameposition).c_str());

  CosNaming::Name_var name_path =
    to_name(corbaname.substr(nameposition + 1).c_str());

  return ncontext->resolve(name_path);
}




CosNaming::Name* TIDorb::core::util::Corbaname::to_name(const char* sn)
  throw (CosNaming::NamingContext::InvalidName)
{
  CORBA::String_var name_str = TIDorb::util::TranslateURLEscapes::put_escapes(sn);

  string name = (const char*) name_str;

  if (name.find("//") != string::npos) {
    throw CosNaming::NamingContext::InvalidName(); // "Invalid corbaname"
  }

  size_t ntokens = 1;
  string::size_type position = name.find('/');

  while(position != string::npos) {
    ntokens++;
    position = name.find('/', position + 1);
  }

  CosNaming::Name* n = new CosNaming::Name();
  n->length(ntokens);

  string::size_type start_position = 0;
  string::size_type end_position   = 0;
  string::size_type dot_position   = 0;

  for (size_t i = 0; i<ntokens; i++) {
    end_position = name.find('/', start_position);

    if (end_position == string::npos) {
      end_position = name.size();
    }

    dot_position = name.find('.', start_position);

    if ((dot_position != string::npos) && (dot_position < end_position)) {
      (*n)[i].id = TIDorb::util::TranslateURLEscapes::resolve_escapes(
                       name.substr(start_position, dot_position - start_position).c_str());
      (*n)[i].kind = TIDorb::util::TranslateURLEscapes::resolve_escapes(
                       name.substr(dot_position + 1, end_position - dot_position - 1).c_str());
    } else {
      (*n)[i].id = TIDorb::util::TranslateURLEscapes::resolve_escapes(
                       name.substr(start_position, end_position - start_position).c_str());
      (*n)[i].kind = CORBA::string_dup("");
    }

    start_position = end_position + 1;
  }

  return n;
}




char* TIDorb::core::util::Corbaname::to_string(const CosNaming::Name* n)
{
  TIDorb::util::StringBuffer buffer;

  CORBA::String_var id;
  CORBA::String_var kind;

  CORBA::ULong length = n->length();

  for (CORBA::ULong i = 0; i < length; i++) {
    id = TIDorb::util::TranslateURLEscapes::stringfy((*n)[i].id);
    kind = TIDorb::util::TranslateURLEscapes::stringfy((*n)[i].kind);

    buffer << (const char*) id;

    // FIX #388: Corbaname::to_string doesn't include kind field of Name
    if (strlen(kind) > 0)
      buffer << "." << kind;

    if (i < length - 1)
      buffer << "/";
  }

  return CORBA::string_dup(buffer.str().data());
}




CosNaming::NamingContext_ptr
TIDorb::core::util::Corbaname::parse_protocol(TIDorb::core::TIDORB* tidorb, const char* protocol)
  throw (CosNaming::NamingContext::InvalidName, CORBA::ORB::InvalidName)
{
  if((strcmp(protocol, "rir:") == 0) ||
     (strcmp(protocol, "rir:/NameService") == 0))
  {
    CORBA::Object_var naming_ref = tidorb->resolve_initial_references("NameService");
    CosNaming::NamingContext_var naming_context = NULL;
    try {
      naming_context = CosNaming::NamingContext::_narrow(naming_ref);
    } catch (const CORBA::SystemException& ex) {
      throw CosNaming::NamingContext::InvalidName();
    }
    return CosNaming::NamingContext::_duplicate(naming_context);
  }
  else if (strncmp(protocol, "iiop:",5) == 0) 
  {
    TIDorb::util::StringBuffer corbaloc_aux;
    corbaloc_aux << "corbaloc:" << protocol;

    CORBA::Object_var naming_ref =
      tidorb->string_to_object(corbaloc_aux.str().data());
    
    CosNaming::NamingContext_var naming_context = NULL;
    try {
      naming_context = CosNaming::NamingContext::_narrow(naming_ref);
    } catch (const CORBA::SystemException& ex) {
      throw CosNaming::NamingContext::InvalidName();
    }
    return CosNaming::NamingContext::_duplicate(naming_context);
  }
  else if (strncmp(protocol, ":",1) == 0) 
  { 
    TIDorb::util::StringBuffer corbaloc_aux;
    corbaloc_aux << "corbaloc:iiop" << protocol;

    CORBA::Object_var naming_ref =
      tidorb->string_to_object(corbaloc_aux.str().data());
    
    CosNaming::NamingContext_var naming_context = NULL;
    try {
      naming_context = CosNaming::NamingContext::_narrow(naming_ref);
    } catch (const CORBA::SystemException& ex) {
      throw CosNaming::NamingContext::InvalidName();
    }
    return CosNaming::NamingContext::_duplicate(naming_context);
  }
  else {
    throw CosNaming::NamingContext::InvalidName(); //" Invalid corbaname"
  }
}
