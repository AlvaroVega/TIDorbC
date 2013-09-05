/////////////////////////////////////////////////////////////////////////
//
// File:        DefaultValueRefCountBase.C
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

#include "CORBA.h"

namespace CORBA {


DefaultValueRefCountBase::DefaultValueRefCountBase()
{ 
  __ref_count_ = 1;

  int ret = pthread_mutex_init(&__ref_mutex_,NULL);

  if(ret)
    throw CORBA::NO_RESOURCES();
}

DefaultValueRefCountBase::~DefaultValueRefCountBase()
{
  pthread_mutex_destroy(&__ref_mutex_);
}

CORBA::ULong DefaultValueRefCountBase::_refcount_value()
{
  return __ref_count_;
}

void DefaultValueRefCountBase::_count(CORBA::ULong v)
{
  __ref_count_ = v;
}

ValueBase* DefaultValueRefCountBase::_add_ref()
{
  int ret;
  
  if(ret = pthread_mutex_lock(&__ref_mutex_))
    throw CORBA::UNKNOWN();
  
  __ref_count_++;
  
  if(ret = pthread_mutex_unlock(&__ref_mutex_)) 
    throw CORBA::UNKNOWN();

  return this;
}

void DefaultValueRefCountBase::_remove_ref()
{
  CORBA::ULong value;
  int ret;
  
  if(ret = pthread_mutex_lock(&__ref_mutex_))
    throw CORBA::UNKNOWN();

  value = --__ref_count_;

  if(ret = pthread_mutex_unlock(&__ref_mutex_)) 
    throw CORBA::UNKNOWN();

  if(value == 0)
    delete this;
}

ValueBase* DefaultValueRefCountBase::_copy_value()
{
  throw CORBA::NO_IMPLEMENT();
}

} // namespace CORBA	
