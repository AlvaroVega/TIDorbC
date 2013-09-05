/////////////////////////////////////////////////////////////////////////
//
// File:        IOR.C
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


static TIDorb::core::iop::IOR* create_null_ior()
{
  TIDorb::core::iop::VectorTaggedProfile profiles;
  return new TIDorb::core::iop::IOR("", &profiles);
}



TIDorb::core::iop::IOR_ref TIDorb::core::iop::IOR::_null_ior(create_null_ior());



TIDorb::core::iop::IOR::IOR() 
  throw (TIDThr::SystemException)
{

  has_hash_code = false;
  hash_code = 0;

  _type_id = NULL;
  _ior_string = NULL;
  _profile_IIOP = NULL;
  _profile_IIOP_searched = false;
  _profile_IIOP_position = 0;

  // MIOP extensions
  _profile_MIOP = NULL;
  _profile_MIOP_searched = false;
  _profile_MIOP_position = 0;

  _ZIOP_searched = false;
  _has_ZIOP_policies = false;
}




TIDorb::core::iop::IOR::~IOR() 
  throw (TIDThr::SystemException)
{
  CORBA::string_free(_type_id);
  CORBA::string_free(_ior_string);
}



    
TIDorb::core::iop::IOR::IOR(const char* id, const VectorTaggedProfile* profiles)
  throw (TIDThr::SystemException)
{
  has_hash_code = false;
  hash_code = 0;

  _profiles = *profiles;
  _type_id = CORBA::string_dup(id);
  _ior_string = NULL;

  _profile_IIOP = NULL;
  _profile_IIOP_searched = false;
  _profile_IIOP_position = 0;
  // MIOP extensions
  _profile_MIOP = NULL;
  _profile_MIOP_searched = false;
  _profile_MIOP_position = 0;

  _ZIOP_searched = false;
  _has_ZIOP_policies = false;

  // Profile IIOP
  {
    CORBA::ULong count = member_count();
    ((IOR*) this)->_profile_IIOP_searched = true;
    for (CORBA::ULong number = 0; number < count; number++) {
      if (get_profile(number)->_tag == TIDorb::core::iop::TAG_INTERNET_IOP){
        ((IOR*) this)->_profile_IIOP_searched = false;
        ((IOR*) this)->_profile_IIOP_position = number;
        ((IOR*) this)->_profile_IIOP = 
          (TIDorb::core::comm::iiop::ProfileIIOP*)(get_profile(number));
      }
    }

  }

  // Profile MIOP
  {
    CORBA::ULong count = member_count();
    ((IOR*) this)->_profile_MIOP_searched = true;
    for (CORBA::ULong number = 0; number < count; number++) {
      if (get_profile(number)->_tag == TIDorb::core::iop::TAG_UIPMC) {
        ((IOR*) this)->_profile_MIOP_position = number;
        ((IOR*) this)->_profile_MIOP = 
          (TIDorb::core::comm::miop::ProfileMIOP*)(get_profile(number));
        ((IOR*) this)->_profile_MIOP_searched = false;
      }
    }

  }

}



    
TIDorb::core::iop::IOR* TIDorb::core::iop::IOR::null_ior()
{
  return _null_ior;
}



    
void TIDorb::core::iop::IOR::assign(const TIDorb::core::iop::IOR& ior)
{
  CORBA::string_free(_type_id);
  CORBA::string_free(_ior_string);
	  
  _profiles = ior._profiles;
  _type_id = CORBA::string_dup(ior._type_id);
  _ior_string = CORBA::string_dup(ior._ior_string);
}



    
const char* TIDorb::core::iop::IOR::get_type_id() const
{
  return _type_id;
}



    
CORBA::ULong TIDorb::core::iop::IOR::member_count() const
{
  return _profiles.size();	    
}



    
TIDorb::core::iop::TaggedProfile* TIDorb::core::iop::IOR::get_profile(CORBA::ULong index) const
{
  if((index < 0) || (index >= _profiles.size()))
    return NULL;
  else
    return _profiles[index];
}



    
bool TIDorb::core::iop::IOR::equivalent(const TIDorb::core::iop::IOR& ior) const
{
  if((_profiles.size() == 0) || (ior._profiles.size() == 0))
    return false;
	
  if(strcmp(_type_id, ior._type_id) != 0)
    return false;
	
  TIDorb::core::iop::IOR* ior_max = NULL;
  TIDorb::core::iop::IOR* ior_min = NULL;
	
  if(member_count() > ior.member_count()) {
    ior_max = (TIDorb::core::iop::IOR*) this;
    ior_min = (TIDorb::core::iop::IOR*) &ior;
  } else {
    ior_max = (TIDorb::core::iop::IOR*) &ior;
    ior_min = (TIDorb::core::iop::IOR*) this;
  }
	
  CORBA::ULong size_min = ior_min->member_count();
  CORBA::ULong size_max = ior_max->member_count();
	
  for(CORBA::ULong i = 0; i < size_min; i++) {
    if(ior_min->_profiles[i]->_tag == TAG_INTERNET_IOP) {
      for(CORBA::ULong j = 0; j < size_max; j++) {
        if(ior_max->_profiles[i]->_tag == TAG_INTERNET_IOP) {
          TIDorb::core::comm::iiop::ProfileIIOP* i_profile =
            dynamic_cast< TIDorb::core::comm::iiop::ProfileIIOP* >
              ((TaggedProfile*) ior_min->_profiles[i]);
          TIDorb::core::comm::iiop::ProfileIIOP* j_profile =
            dynamic_cast< TIDorb::core::comm::iiop::ProfileIIOP* >
              ((TaggedProfile*) ior_max->_profiles[j]);

          if(*i_profile == *j_profile)
            return true;
        }
      }
    }
  }

  return false;

}
    
    
    
    
const char* TIDorb::core::iop::IOR::toString() const
{
  return toString(NULL);
	
}



    
CORBA::ULong TIDorb::core::iop::IOR::hashCode() const
{
  if(!has_hash_code) {
    ((IOR*) this)->has_hash_code = true;
    ((IOR*) this)->hash_code = atol(toString());
  }
  	
  return hash_code;  	
}



    
const char* TIDorb::core::iop::IOR::toString(TIDorb::core::TIDORB* orb) const
{
  if(_ior_string == NULL) {
	  
    TIDThr::Synchronized synchro (*((IOR*) this));

    if(_ior_string != NULL)  // May be changed after return of synchro
      return _ior_string;

    TIDorb::core::cdr::CDROutputStream ior_output(orb);
		    
    ior_output.write_boolean(ior_output.get_byte_order());
	    
    write(ior_output);
	  
    TIDorb::core::cdr::BufferCDR* cdr_buffer = ior_output.get_buffer();
	    
    CORBA::ULong available = cdr_buffer->get_available_bytes();
	    
    CORBA::ULong buffer_length=(2* available)+ 5; // "IOR:" plus '/0' at the end
    char* buffer = CORBA::string_alloc(buffer_length);
	    
    CORBA::ULong i = 0;
    buffer[i++]= 'I';
    buffer[i++]= 'O';
    buffer[i++]= 'R';
    buffer[i++]= ':';
	    
    //CORBA::ULong num_chunks = cdr_buffer->get_num_available_chunks();
	    
    TIDorb::core::cdr::ChunkCDR* cdr_chunk = NULL;

    // UMR avoid
    int index1=0, index2=0;    

    CORBA::ULong octet = 0;
    CORBA::ULong chunk_available;
    CORBA::Octet* ior_data = NULL;
	    
    CORBA::ULong chunk_no = 0;
	    
    while (i < buffer_length - 1 ) {
      cdr_chunk = cdr_buffer->get_chunk(chunk_no++);
      chunk_available = cdr_chunk->get_available_bytes();
      ior_data = (CORBA::Octet*)cdr_chunk->get_buffer();
		
      for(CORBA::ULong j = 0; j < chunk_available; j++) {
        octet = ior_data[j];
        
 	index1 = (octet & 0xf0)>>4;
	index2 = octet & 0xf;
       
        buffer[i++] = TIDorb::util::Base16Codec::toBase16[index1];
        buffer[i++] = TIDorb::util::Base16Codec::toBase16[index2];

      }
    }
    
    // end of string

    buffer[i] = '\0';
	    
    ((IOR*) this)->_ior_string = buffer;
  }
	
  return _ior_string;
}

TIDorb::core::iop::IOR& 
TIDorb::core::iop::IOR::operator=(TIDorb::core::iop::IOR* ior) 
  throw (TIDThr::SystemException)
{
  const TIDorb::core::iop::IOR& mior = *ior;
  
  has_hash_code = false;
  hash_code = 0;
  assign(mior);
 
  _profile_IIOP = NULL;
  _profile_IIOP_searched = false;
  _profile_IIOP_position = 0;
  // MIOP extensions
  _profile_MIOP = NULL;
  _profile_MIOP_searched = false;
  _profile_MIOP_position = 0;

  _ZIOP_searched = false;
  _has_ZIOP_policies = false;
  
  return *this;
}



TIDorb::core::iop::IOR* 
TIDorb::core::iop::IOR::fromString(TIDorb::core::TIDORB* orb, const char* ior_str)
{
  int length = strlen(ior_str);
  if (((length%2) != 0) || (length < 14))
    throw CORBA::INV_OBJREF("IOR format error: invalid ior length.", 0, CORBA::COMPLETED_NO); 

  if ((ior_str[0]!='I') || (ior_str[1]!='O') || (ior_str[2]!= 'R') || (ior_str[3] != ':')) 
    throw CORBA::INV_OBJREF("Invalid IOR format id: \"IOR:\".",0, CORBA::COMPLETED_NO); 
	
  CORBA::ULong buffer_length = (length-4) / 2;
	
  TIDorb::core::cdr::ChunkCDR * chunk = new TIDorb::core::cdr::ChunkCDR (buffer_length);
	
  chunk->set_available_bytes(buffer_length);
	
  unsigned char* buffer = (unsigned char*) chunk->get_buffer();
	
  //se supone que convierte el string en un nuevo array de caracteres
  int j = 4;
  try {
    for (CORBA::ULong i = 0; i < buffer_length; i++) 
      //buffer[i]= (byte) (16* (Base16Codec.getValue(ior_chars[j++])) + 
      //		   Base16Codec.getValue(ior_chars[j++]));
      
      buffer[i]= (unsigned char) (16* (TIDorb::util::Base16Codec::getValue(ior_str[j++])) + 
                                  TIDorb::util::Base16Codec::getValue(ior_str[j++]));
		
  } catch (const exception& e) {
    throw CORBA::INV_OBJREF(e.what(), 0, CORBA::COMPLETED_NO);
  } 


  // create the the ior with the new buffer

  TIDorb::core::iop::IOR* ior = new TIDorb::core::iop::IOR();

  TIDorb::core::cdr::CDRInputStream internet_cdr(orb, 
                                                 new TIDorb::core::cdr::BufferCDR(chunk));
  		
  CORBA::Boolean flag;
  internet_cdr.read_boolean(flag);
  internet_cdr.set_byte_order(flag);
	
  ior->read(internet_cdr);
	
  ior->_ior_string = CORBA::string_dup(ior_str);
	
  return ior;
}



    
void TIDorb::core::iop::IOR::write(TIDorb::core::cdr::CDROutputStream& out) const
{
  // write type_id
  out.write_string(_type_id);
	
  // write profiles sequence
  size_t size = _profiles.size();
  out.write_ulong(size);
      
  for(size_t i = 0; i < size; i++)
    _profiles[i]->write(out);
}


    
    
void TIDorb::core::iop::IOR::read(TIDorb::core::cdr::CDRInputStream& input) 
{
  input.read_string(_type_id);
	
  CORBA::ULong size;
  input.read_ulong(size);
	
  if(size < 0)
    throw CORBA::INV_OBJREF("Invalid Profile sequence length.", 0, CORBA::COMPLETED_NO);
	
  _profiles.resize(size);
	

	
  for(size_t i = 0; i < size; i++) {	
    _profiles[i] = TaggedProfileReader::read(input);
  }

  // Ya que esta llamada no es simultanea aprovechamos para leer aqui los profiles 
  // y tenerlos precarcargados sin condiciones de carrera

  // Profile IIOP
  {
    CORBA::ULong count = member_count();
    ((IOR*) this)->_profile_IIOP_searched = true;
    for (CORBA::ULong number = 0; number < count; number++) {
      if (get_profile(number)->_tag == TIDorb::core::iop::TAG_INTERNET_IOP){
        ((IOR*) this)->_profile_IIOP_searched = false;
        ((IOR*) this)->_profile_IIOP_position = number;
        ((IOR*) this)->_profile_IIOP = 
          (TIDorb::core::comm::iiop::ProfileIIOP*)(get_profile(number));
      }
    }
  
  }

  // Profile MIOP
  {
    CORBA::ULong count = member_count();

    ((IOR*) this)->_profile_MIOP_searched = true;
    for (CORBA::ULong number = 0; number < count; number++) {
      if (get_profile(number)->_tag == TIDorb::core::iop::TAG_UIPMC) {
        ((IOR*) this)->_profile_MIOP_position = number;

        ((IOR*) this)->_profile_MIOP = 
          (TIDorb::core::comm::miop::ProfileMIOP*)(get_profile(number));
        ((IOR*) this)->_profile_MIOP_searched = false;
      }
    }
  }

}



    
void TIDorb::core::iop::IOR::skip(TIDorb::core::cdr::CDRInputStream& input) 
{
  input.skip_string();
	
  CORBA::ULong size;
  input.read_ulong(size);
	
  if(size < 0)
    throw CORBA::INV_OBJREF();
  //("Invalid Profile sequence length.",0, CompletionStatus.COMPLETED_NO);
	
  for(size_t i = 0; i < size; i++) {
    TaggedProfile::skip(input);
  }
}

bool TIDorb::core::iop::IOR::is_IIOP() const
{
  return !_profile_IIOP_searched;
}

TIDorb::core::comm::iiop::ProfileIIOP*
TIDorb::core::iop::IOR::profile_IIOP() const
{
  if(_profile_IIOP_searched) {
    throw CORBA::INV_OBJREF();
  }
  return _profile_IIOP;
}


bool TIDorb::core::iop::IOR::use_another_Profile_IIOP()
{
  CORBA::ULong count = member_count();
  bool _profile_IIOP_changed = false;
  for (CORBA::ULong number = 0; number < count; number++) {
    if ( (get_profile(number)->_tag == TIDorb::core::iop::TAG_INTERNET_IOP) &&
         ((IOR*) this)->_profile_IIOP_position != number ) {
      _profile_IIOP_changed = true;
      ((IOR*) this)->_profile_IIOP_position = number;
      ((IOR*) this)->_profile_IIOP = 
        (TIDorb::core::comm::iiop::ProfileIIOP*)(get_profile(number));
      break;
    }
  }
  return _profile_IIOP_changed;
}



//
// MIOP extensions
//

TIDorb::core::comm::miop::ProfileMIOP*
TIDorb::core::iop::IOR::profile_MIOP() const
{
  if (_profile_MIOP_searched) {
    throw CORBA::INV_OBJREF();
  }
  
  return _profile_MIOP;
}




bool TIDorb::core::iop::IOR::is_ZIOP()
{
  if (!_ZIOP_searched) {
    TIDorb::core::PolicyContext* context = profile_IIOP()->getPolicies();
    if (context != NULL) {
      _has_ZIOP_policies = 
        TIDorb::core::ziop::Ziop::checkCompressionEnabled(*context);
    } else
      _has_ZIOP_policies = false;
    _ZIOP_searched = true;
  }
  return _has_ZIOP_policies;
}


bool TIDorb::core::iop::IOR::is_group_reference() const
{
  return !_profile_MIOP_searched;
}


const TIDorb::core::comm::iiop::Version&
TIDorb::core::iop::IOR::GIOPVersion() const
{
  if (!_profile_MIOP_searched) {

    // Currently, MIOP always relies on GIOP 1.2  	
    return TIDorb::core::comm::iiop::Version::VERSION_1_2;

  } else {

    // Use same GIOP and IIOP versions
    if (!_profile_IIOP_searched) {
      if (_profile_IIOP == NULL)
        throw CORBA::INTERNAL("Cannot get IIOP Profile.");
      return _profile_IIOP->getVersion();
    }
    else 
      throw CORBA::INV_OBJREF();

  }
}

//
// end MIOP extensions
//




TIDorb::core::comm::iiop::ObjectKey* TIDorb::core::iop::IOR::object_key() const
{
  // MIOP extensions
  if (!_profile_MIOP_searched)
    throw CORBA::MARSHAL();
  // end MIOP extensions

  if (!_profile_IIOP_searched) 
    return _profile_IIOP->getObjectKey();
  else 
    throw CORBA::INV_OBJREF();
}


TIDorb::core::PolicyContext* TIDorb::core::iop::IOR::policies() const
{
  if (!_profile_IIOP_searched) 
    return _profile_IIOP->getPolicies();
  else 
    throw CORBA::INV_OBJREF();
  
}


SSLIOP::SSL* TIDorb::core::iop::IOR::get_SSL() const
{
  if (!_profile_IIOP_searched) 
    return _profile_IIOP->get_SSL();
  else 
    throw CORBA::INV_OBJREF(); 
}

CSIIOP::CompoundSecMechList* TIDorb::core::iop::IOR::get_CompoundSecMechList() const
{
  if (!_profile_IIOP_searched) 
    return _profile_IIOP->get_CompoundSecMechList();
  else 
    throw CORBA::INV_OBJREF(); 
}

