/* -----------------------------------------------------------------------------

 File: TIDorb/core/cdr/ValueTypeInfo.h
  
 Revisions:
 
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
 ------------------------------------------------------------------------------ */

#include "TIDorb/core/cdr.h"

#ifndef __TIDorb_core_cdr_ValueTypeInfo_h_
#define __TIDorb_core_cdr_ValueTypeInfo_h_

#include "CORBA.h"
#include "TIDThr.h"


namespace TIDorb {
namespace core {
namespace cdr {



class ValueTypeInfo
{

    public:

    static const int NULL_TAG = 0;

    /* value tags constants */
    static const CORBA::Long CODE_BASE_BIT        = 0x00000001;

    static const CORBA::Long SINGLE_REPOSITORY_ID = 0x00000002;

    static const CORBA::Long REPOSITORY_ID_LIST   = 0x00000006;

    static const CORBA::Long INDIRECTION_TAG      = 0xffffffff;

    static const CORBA::Long FRAGMENTED_BIT       = 0x00000008;

    static const CORBA::Long MIN_VALUE_TAG        = 0x7fffff00;

    static const CORBA::Long MAX_VALUE_TAG        = 0x7fffffff;

    static const CORBA::Long MIN_CHUNK_SIZE       = 0x00000000;

    static const CORBA::Long MAX_CHUNK_SIZE       = 0x7fffff00;

    

    ValueTypeInfo();
    ValueTypeInfo(const ValueTypeInfo& info);
    virtual ~ValueTypeInfo() {}

    ValueTypeInfo& operator= (const ValueTypeInfo& info);

    bool is_null()
    {
        return m_value_tag == NULL_TAG;
    }
    
    void set_null();

    bool isIndirection()
    {
        return m_value_tag == INDIRECTION_TAG;
    }

    bool isFragmented();

    bool is_state()
    {
        return (m_value_tag != NULL_TAG) && (m_value_tag != INDIRECTION_TAG);
    }

    const CORBA::StringSeq& get_repository_ids() const
    {
        return m_repository_ids;
    }

    const char* get_id() const;

    bool is_truncable(const char* rep_id) const;
    
    
    void set_repository_ids(const char** ids, size_t length);
    

    void set_value(CORBA::ValueBase* val)
    {        
	CORBA::add_ref(val);
        m_value = val;
    }

    CORBA::ValueBase* get_value()
    {
        CORBA::add_ref(m_value);
        return m_value;
    }
    
    /*
     * @return the value without incrementing the reference counting
     */
    const CORBA::ValueBase* get_value_ref() const
    {        
        return m_value;
    }

    bool has_code_base_url() const
    {
        return ((const char*) m_code_base_url) != NULL;
    }

    const char* get_code_base_url()
    {
        return (const char*) m_code_base_url;
    }

    void set_code_base_url(char* url);

    void set_fragmented(bool value);

    static void write_null(TIDorb::portable::OutputStream& os)
    {
        os.write_ulong(NULL_TAG);
    }
   
    /*
     * Marshalling/Demarshalling
     */
    
    void read(CDRInputStream& in); 
    void write(CDROutputStream& out);
    
    protected:

    void read_code_base_url(CDRInputStream& input);

    void write_code_base_url(CDROutputStream& out);

    void read_type_info(CDRInputStream& input);

    char* read_repository_id(CDRInputStream& input);

    void write_type_info(CDROutputStream& out);  
   
    
    private:
    
     CORBA::String_var m_code_base_url;
     CORBA::StringSeq m_repository_ids;
     CORBA::Long m_value_tag;
     CORBA::ValueBase_var m_value;
};

}
}
}

#endif



