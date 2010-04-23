/////////////////////////////////////////////////////////////////////////
//
// File:        CSIComponent.C
// Description:
//              
// Rel:         01.00
// Created:     June, 2009
//
// Revised:    
//
// Copyright 2009 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/security.h"




TIDorb::core::security::CSIComponent::CSIComponent() 
  throw(TIDThr::SystemException)
  : TIDorb::core::iop::TaggedComponent(TIDorb::core::iop::TAG_CSI_SEC_MECH_LIST)
{
  _str = NULL;
  _mec_list.stateful = false;
  _mec_list.mechanism_list.length(0);
}




TIDorb::core::security::CSIComponent::CSIComponent(CSIIOP::CompoundSecMechList mec_list) 
  throw(TIDThr::SystemException)
  : TIDorb::core::iop::TaggedComponent(TIDorb::core::iop::TAG_CSI_SEC_MECH_LIST),
    _mec_list(mec_list)
{
  _str = NULL;
}




TIDorb::core::security::CSIComponent::CSIComponent(const CSIComponent& v) 
  throw(TIDThr::SystemException)
  : TIDorb::core::iop::TaggedComponent(TIDorb::core::iop::TAG_CSI_SEC_MECH_LIST),
    _mec_list(v._mec_list)
{
  _str = NULL;
}




TIDorb::core::security::CSIComponent::~CSIComponent() 
  throw(TIDThr::SystemException)
{
  CORBA::string_free(_str);
}


const CSIIOP::CompoundSecMechList* 
TIDorb::core::security::CSIComponent::getCompoundSecMechList()
{
  return &_mec_list;
}



  
void TIDorb::core::security::CSIComponent::write(TIDorb::core::cdr::CDROutputStream& out) const
{
  TIDThr::Synchronized synchro(*((CSIComponent*)this));
  
  if(!_component_data) {

    TIDorb::core::cdr::CDROutputStream* encapsulation = out.create_encapsulation();
                       
    CSIIOP::_CompoundSecMechListHelper::write(*encapsulation, _mec_list);
   
    ((CSIComponent*) this)->_component_data = (TIDorb::core::cdr::CDRInputStream* )
                                              (encapsulation->create_input_stream());

    delete encapsulation;
  }
  
  TaggedComponent::write(out);
}



	
void TIDorb::core::security::CSIComponent::partial_read(TIDorb::core::cdr::CDRInputStream& input)
{
  CORBA::string_free(_str);
  _str = NULL;

  //TaggedComponent::partial_read(input);
  TIDorb::core::cdr::CDRInputStream* encapsulation = input.read_encapsulation();

  CSIIOP::_CompoundSecMechListHelper::read(*encapsulation, _mec_list);
  
  delete encapsulation;
}




const char* TIDorb::core::security::CSIComponent::toString() const
{
  if (!_str) {
    TIDorb::util::StringBuffer buffer;
    buffer << "CSIComponent { "; 
//     buffer << "port " << _ssl.port << " | ";
//     buffer << "target_supports " << _ssl.target_supports << " | ";
//     buffer << "target_requires " << _ssl.target_requires;
    buffer << " } ";
    ((CSIComponent*) this)->_str = CORBA::string_dup(buffer.str().data());
  }
	
  return _str;
}
