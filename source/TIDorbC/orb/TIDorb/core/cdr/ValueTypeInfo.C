/////////////////////////////////////////////////////////////////////////
//
// File:        ValueTypeInfo.C
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

#include "TIDorb/core.h"

#include <string.h>


namespace TIDorb{
namespace core {
namespace cdr {


ValueTypeInfo::ValueTypeInfo() 
//: m_value_tag(NULL_TAG)
: m_value_tag(MIN_VALUE_TAG)
{
    m_repository_ids.length(0);
}

ValueTypeInfo::ValueTypeInfo(const ValueTypeInfo& info) 
: m_code_base_url(CORBA::string_dup(info.m_code_base_url)),
  m_repository_ids(info.m_repository_ids),
  m_value_tag(info.m_value_tag),
  m_value(info.m_value)
{ 
}

ValueTypeInfo& ValueTypeInfo::operator= (const ValueTypeInfo& info)
{
  m_value_tag = info.m_value_tag;
  m_value = info.m_value;
  m_code_base_url = CORBA::string_dup(info.m_code_base_url);
  m_repository_ids = info.m_repository_ids;
  return *this;
}

void ValueTypeInfo::set_null()
{
    m_value_tag = NULL_TAG;
    m_code_base_url = "";
    m_repository_ids.length(0);
    m_value = NULL;
}

bool ValueTypeInfo::isFragmented()
{
    return ((!isIndirection()) 
            && ((m_value_tag & FRAGMENTED_BIT) == FRAGMENTED_BIT));
}

const char* ValueTypeInfo::get_id() const
{
    if (m_repository_ids.length() == 0) {
        return NULL;
    }
    
    return m_repository_ids[0];
}

bool ValueTypeInfo::is_truncable(const char* rep_id) const
{
    const char* value_id = NULL;

    CORBA::ULong size = m_repository_ids.length();
    
    for (CORBA::ULong i = 0; i < size; i++) {
        value_id = m_repository_ids[i];

        if (strcmp(rep_id,value_id) == 0) {
            return true;
        }
    }
    
    return false;
}

void ValueTypeInfo::set_repository_ids(const char** ids, size_t length)
{
    m_repository_ids.length(length);
    
    for (size_t i = 0; i < length; i++) {
        m_repository_ids[i] = CORBA::string_dup(ids[i]);
    }

    if (length == 0) {
        m_value_tag &= ~REPOSITORY_ID_LIST;
    } else if (length == 1) {
        m_value_tag &= ~REPOSITORY_ID_LIST;
        m_value_tag |= SINGLE_REPOSITORY_ID;
    } else {
        m_value_tag |= REPOSITORY_ID_LIST;
    }
}

void ValueTypeInfo::set_code_base_url(char* url)
{
    m_code_base_url = url;

    if (url == NULL) {
        m_value_tag &= ~CODE_BASE_BIT;
    } else {
        m_value_tag |= CODE_BASE_BIT;
    }
}

void ValueTypeInfo::set_fragmented(bool value)
{
    if (value) {
        m_value_tag |= FRAGMENTED_BIT;
    } else {
        m_value_tag &= ~FRAGMENTED_BIT;
    }
}

void ValueTypeInfo::read(CDRInputStream& in)
{
    m_value = NULL;
    
    in.read_long(m_value_tag);
    
    if ((m_value_tag != INDIRECTION_TAG) && (m_value_tag != NULL_TAG)) {
        read_code_base_url(in);
        read_type_info(in);
    }
 
}

void ValueTypeInfo::write(CDROutputStream& out)
{
    if(m_value_tag == NULL_TAG) {
        write_null(out);
    } else {
        out.write_long(m_value_tag);
        write_code_base_url(out);
        write_type_info(out);
    }
}


// protected methods


void ValueTypeInfo::read_code_base_url(CDRInputStream& input)
{
    m_code_base_url = (char*) NULL;
    
    if ((m_value_tag & CODE_BASE_BIT) == 0x0)
        return;

    input.alignment(CDR::LONG_SIZE);

    CDR::AbsolutePosition this_url_position = input.get_absolute_position();

    input.read_indirected_string(m_code_base_url);  
    
}

void ValueTypeInfo::write_code_base_url(CDROutputStream& out)
{
    const char* url = (const char*) m_code_base_url;
    
    if (url) {
        out.write_referenceable_string(url);
    }
}

void ValueTypeInfo::read_type_info(CDRInputStream& input)
{
    if ((m_value_tag & REPOSITORY_ID_LIST) == REPOSITORY_ID_LIST) {
        // read ids sequence
        CORBA::Long length;
        input.read_long(length);
        
        m_repository_ids.length(length);
        
        char* id = NULL;

        for (int i = 0; i < length; i++) {
            input.read_indirected_string(id);
            m_repository_ids[i] = id;
        }

    } else if ((m_value_tag & SINGLE_REPOSITORY_ID) == SINGLE_REPOSITORY_ID) {
        //  int length = input.read_long();

        //  if(length != 1)
        //    throw MARSHAL("Invalid RepositoryId seq length != 1");

        m_repository_ids.length(1);

        char* id = NULL;
        input.read_indirected_string(id);
        m_repository_ids[0] = id;
    } // else there is no type data
}



void ValueTypeInfo::write_type_info(CDROutputStream& out)
{

/* Already done in previous write_code_base_url() call

    const char* url = (const char*) m_code_base_url;
    if (url) {
        out.write_referenceable_string(url);
    }
*/

    if (m_repository_ids.length() > 0) {
        if ((m_value_tag & REPOSITORY_ID_LIST) == REPOSITORY_ID_LIST) {
            CORBA::ULong length = m_repository_ids.length();
            out.write_long(length);
            for (CORBA::ULong i = 0; i < length; i++)
                out.write_referenceable_string(m_repository_ids[i]);
        } else { // SINGLE_REPOSITORY_ID
            out.write_referenceable_string(m_repository_ids[0]);
        }
    }
}

} // namespace cdr
} // core 
} // TIDorb
