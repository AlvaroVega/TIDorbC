//////////////////////////////////////////////////////////////////////////////////
//
// File:        TargetAddress.C
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

#include "TIDorb/core/comm/iiop/TargetAddress.h"

TIDorb::core::comm::iiop::TargetAddress::TargetAddress
    (const TIDorb::core::comm::iiop::TargetAddress& other)
:  _discriminator(other._discriminator),
   _object_key(other._object_key),
   _profile(other._profile)
{
  if(other._ior != NULL) { 
    _ior = new TIDorb::core::comm::iiop::IORAddressingInfo(*(other._ior));
  } else {
    _ior = NULL;
  }
}

void TIDorb::core::comm::iiop::TargetAddress::reset()
{
        _discriminator = -1;
        _object_key    = NULL;
        _profile       = NULL;
        _ior           = NULL;
}




TIDorb::core::comm::iiop::TargetAddress::TargetAddress()
{
        reset();
}




TIDorb::core::comm::iiop::TargetAddress::~TargetAddress()
{
        delete _ior;
}




TIDorb::core::comm::iiop::ObjectKey* TIDorb::core::comm::iiop::TargetAddress::object_key() const
{
        return _object_key;
}




CORBA::Short TIDorb::core::comm::iiop::TargetAddress::object_key(TIDorb::core::comm::iiop::ObjectKey* value)
{
        reset();
        _discriminator = (CORBA::Short) TIDorb::core::comm::iiop::KeyAddr;
        _object_key    = value;

        return _discriminator;
}




//
// pra@tid.es - MIOP extensions
//

/*
TIDorb::core::comm::iiop::ProfileIIOP* TIDorb::core::comm::iiop::TargetAddress::profile() const
{
        return _profile;
}

CORBA::Short TIDorb::core::comm::iiop::TargetAddress::profile(TIDorb::core::comm::iiop::ProfileIIOP* value)
{
        reset();
        _discriminator = (CORBA::Short) TIDorb::core::comm::iiop::ProfileAddr;
        _profile       = value;

        return _discriminator;
}
*/


TIDorb::core::iop::TaggedProfile* TIDorb::core::comm::iiop::TargetAddress::profile() const
{
        return _profile;
}




CORBA::Short TIDorb::core::comm::iiop::TargetAddress::profile(TIDorb::core::iop::TaggedProfile* value)
{
        reset();
        _discriminator = (CORBA::Short) TIDorb::core::comm::iiop::ProfileAddr;
        _profile       = value;

        return _discriminator;
}

//
// end MIOP extensions
//




TIDorb::core::comm::iiop::IORAddressingInfo* TIDorb::core::comm::iiop::TargetAddress::ior() const
{
        return _ior;
}




CORBA::Short TIDorb::core::comm::iiop::TargetAddress::ior(TIDorb::core::comm::iiop::IORAddressingInfo* value)
{
        reset();
        _discriminator = (CORBA::Short) TIDorb::core::comm::iiop::ReferenceAddr;
        _ior           = value;

        return _discriminator;
}




CORBA::Short TIDorb::core::comm::iiop::TargetAddress::discriminator() const
{
        return _discriminator;
}




TIDorb::core::comm::iiop::ObjectKey* TIDorb::core::comm::iiop::TargetAddress::getObjectKey() const
{
        if (_discriminator < 0)
                return NULL;

        switch (_discriminator) {
                case TIDorb::core::comm::iiop::KeyAddr:
                {
                        return _object_key;
                }

                case TIDorb::core::comm::iiop::ProfileAddr:
                {
                        // pra@tid.es - MIOP extensions
                        TIDorb::core::iop::TaggedProfile* tag_profile;
                        tag_profile = (TIDorb::core::iop::TaggedProfile*) _profile;

                        TIDorb::core::comm::iiop::ProfileIIOP* iiop_profile;
                        iiop_profile = dynamic_cast<TIDorb::core::comm::iiop::ProfileIIOP*>(tag_profile);
                        if (iiop_profile == NULL) {
                                return NULL; // Not a IIOP profile to get ObjectKey from
                        }
                        return iiop_profile->getObjectKey();
                        // end MIOP extensions
                }

                case TIDorb::core::comm::iiop::ReferenceAddr:
                {
                        TIDorb::core::iop::TaggedProfile* tag_profile;
                        tag_profile = _ior->_ior.get_profile(_ior->_selected_profile_index);

                        TIDorb::core::comm::iiop::ProfileIIOP* iiop_profile;
                        iiop_profile = dynamic_cast<TIDorb::core::comm::iiop::ProfileIIOP*>(tag_profile);
                        if (iiop_profile == NULL) {
                                return NULL; // Not a IIOP profile to get ObjectKey from
                        }
                        return iiop_profile->getObjectKey();
                }

                default:
                        return NULL;
        }
}

void TIDorb::core::comm::iiop::TargetAddress::write_object_key_address
            (TIDorb::core::cdr::CDROutputStream& out,
             const TIDorb::core::iop::IOR& ior)
{
    out.write_short(TIDorb::core::comm::iiop::KeyAddr);
    
    if (ior.is_group_reference()) {
        // TODO: implement IORWrapper::to_object_key_address() for MIOP
        throw CORBA::MARSHAL();
    }
    
    ior.object_key()->write(out);
}

void TIDorb::core::comm::iiop::TargetAddress::write_object_key_address
            (TIDorb::core::cdr::CDROutputStream& out,
             const TIDorb::core::comm::iiop::ObjectKey& key)
{
    out.write_short(TIDorb::core::comm::iiop::KeyAddr);   
    key.write(out);
}


void TIDorb::core::comm::iiop::TargetAddress::write_profile_address
            (TIDorb::core::cdr::CDROutputStream& out,
             const TIDorb::core::iop::IOR& ior)
{
    out.write_short(TIDorb::core::comm::iiop::ProfileAddr);
    
    TIDorb::core::iop::TaggedProfile* profile;
    
    profile = (ior.is_group_reference()) ?
        (TIDorb::core::iop::TaggedProfile*) ior.profile_MIOP() :
        (TIDorb::core::iop::TaggedProfile*) ior.profile_IIOP() ;
  
    profile->write(out);
}

void TIDorb::core::comm::iiop::TargetAddress::write_ior_address
            (TIDorb::core::cdr::CDROutputStream& out,
             const TIDorb::core::iop::IOR& ior)
{
  
    
  // pra@tid.es - MIOP extensions
  // Pre-GIOP 1.2 support: UIPMC profile should be placed in the profile field
  if (ior.is_group_reference()) {
    throw CORBA::MARSHAL("UIPMC profile should be placed in the profile field");
  }
  
  out.write_short(TIDorb::core::comm::iiop::ReferenceAddr);
    
  CORBA::ULong position = ior.profile_IIOP_position();
  
  IORAddressingInfo::write(out, position, ior);
}



void TIDorb::core::comm::iiop::TargetAddress::read(TIDorb::core::cdr::CDRInputStream& in)
{
        reset();

        AddressingDisposition disposition = AddressingDispositionHelper::read(in);
        _discriminator = (CORBA::Short) disposition;

        switch (_discriminator) {
                case TIDorb::core::comm::iiop::KeyAddr:
                        _object_key = new ObjectKey();
                        _object_key->read(in);
                        break;
                case TIDorb::core::comm::iiop::ProfileAddr:
                        _profile = TIDorb::core::iop::TaggedProfileReader::read(in);
                        break;
                case TIDorb::core::comm::iiop::ReferenceAddr:
                        _ior = TIDorb::core::comm::iiop::IORAddressingInfo::read(in);
                        break;
        }
}
