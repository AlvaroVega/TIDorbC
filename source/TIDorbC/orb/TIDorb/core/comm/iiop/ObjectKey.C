//////////////////////////////////////////////////////////////////////////////////
//
// File:        ObjectKey.C
// Description: 
//
// Rel:         01.00
// Created:     September, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
//
// Revised:
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/comm/iiop/ObjectKey.h"

TIDorb::core::comm::iiop::ObjectKey::ObjectKey()
throw (TIDThr::SystemException) 
{
  _key = NULL;

  _marshaled_key= NULL;

  _url = NULL;
};

TIDorb::core::comm::iiop::ObjectKey::ObjectKey(TIDorb::core::cdr::CDRInputStream* key)
 throw (TIDThr::SystemException)
{
  _key = NULL;
  _marshaled_key = key;
  _url = NULL;
};
  
TIDorb::core::comm::iiop::ObjectKey::ObjectKey(TIDorb::core::poa::POAKey* key)
 throw (TIDThr::SystemException)
{
  _key = key;
  _marshaled_key = NULL;
  _url = NULL;
};

TIDorb::core::comm::iiop::ObjectKey::~ObjectKey()
 throw (TIDThr::SystemException) 
{
  delete _marshaled_key;
  delete _url;
};

	
bool TIDorb::core::comm::iiop::ObjectKey::operator==(const TIDorb::core::comm::iiop::ObjectKey& other) const
{   
  return getMarshaledKey()->get_buffer()->equal(*(other.getMarshaledKey()->get_buffer()));
};
	
void TIDorb::core::comm::iiop::ObjectKey::read(TIDorb::core::cdr::CDRInputStream& input)
{
  if (_marshaled_key != NULL)
    delete _marshaled_key; 

  _marshaled_key = input.read_encapsulation();

};
	
void 
TIDorb::core::comm::iiop::ObjectKey::write(TIDorb::core::cdr::CDROutputStream& output) const
{
  TIDorb::core::cdr::BufferCDR_ref buffer = getMarshaledKey()->get_buffer();
  output.write_buffer(*buffer);
};

const TIDorb::core::cdr::CDRInputStream* 
TIDorb::core::comm::iiop::ObjectKey::getMarshaledKey() const
{

  if(_marshaled_key == NULL) {

    TIDThr::Synchronized sync(*((ObjectKey*)this));

    if (_marshaled_key != NULL)
      return _marshaled_key;

    if(_key.is_null())
      throw CORBA::INTERNAL("Malformed Objectkey");
    
    TIDorb::core::cdr::CDROutputStream encapsulation (NULL);
    
    encapsulation.write_boolean(encapsulation.get_byte_order());
    
    _key->write(encapsulation);

    delete ((ObjectKey *) this)->_marshaled_key; 

    ((ObjectKey *) this)->_marshaled_key = 
      (TIDorb::core::cdr::CDRInputStream*) (encapsulation.create_input_stream());
    //dynamic_cast <TIDorb::core::cdr::CDRInputStream* > (encapsulation.create_input_stream());
  }
  
  return _marshaled_key;
};

TIDorb::core::poa::POAKey* TIDorb::core::comm::iiop::ObjectKey::get_key() const
{
  
  if(_key.is_null()) {
    
    TIDThr::Synchronized sync(*((ObjectKey*)this));

    if (!_key.is_null())
      return _key;

    getMarshaledKey();
    
    TIDorb::core::cdr::CDRInputStream encapsulation (*(_marshaled_key));
    
    ((ObjectKey*)this)->_key = new TIDorb::core::poa::POAKey();
    
    encapsulation.set_version(Version::VERSION_1_2);
    
    /* Already seetted when the encapsulation was readen	
       CORBA::Boolean byte_order;
       
       encapsulation.read_boolean(byte_order);
       
       encapsulation.set_byte_order(byte_order);
    */
    _key->read(encapsulation);
  }
  
  return _key;
};


char* TIDorb::core::comm::iiop::ObjectKey::get_url()
{
  if (_url != NULL)
    return strdup(_url);
  
  if(_marshaled_key != NULL) {
    
    _marshaled_key->rewind();
    
    // Recover length string
    CORBA::ULong url_len = _marshaled_key->buffer_available();

    // Read string
    _url = CORBA::string_alloc(url_len);
    _marshaled_key->read_octet_array((::CORBA::Octet*)_url, 0, url_len);
    _url[url_len] = '\0';

    return strdup(_url);
  }
  else 
    return 0;
}
