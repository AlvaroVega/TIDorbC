// //////////////////////////////////////////////////////////////////////////////////
// //
// // File:        ProfileZIOP.C
// // Description: 
// //
// // Rel:         01.00
// // Created:     March, 2008
// // Author:      Alvaro Vega Garcia      ( avega@tid.es   )
// //
// // Revised:
// //
// // Copyright 2008 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// // The copyright to the software program(s) is property of Telefonica I+D.
// // The program(s) may be used and or copied only with the express written
// // consent of Telefonica I+D or in acordance with the terms and conditions
// // stipulated in the agreement/contract under which the program(s) have
// // been supplied.
// //
// ///////////////////////////////////////////////////////////////////////////////////

// #include "TIDorb/core/comm/ziop/ProfileZIOP.h"




// TIDorb::core::comm::ziop::ProfileZIOP::ProfileZIOP() throw (TIDThr::SystemException)
//   : TIDorb::core::iop::TaggedProfile(TIDorb::core::iop::TAG_ZIOP_POLICIES)
// {
//   cerr << "ProfileZIOP()" << endl;
//   _profile_data = NULL;
//   _components_extracted = false;
// }




// TIDorb::core::comm::ziop::ProfileZIOP::ProfileZIOP(
//                        const TIDorb::core::comm::ziop::Version& version)
//   throw (TIDThr::SystemException)
//   : TIDorb::core::iop::TaggedProfile(TIDorb::core::iop::TAG_ZIOP_POLICIES)
// {
//   cerr << "ProfileZIOP(version)" << endl;
//   _version = version;
 
//   _profile_data = NULL;
//   _components_extracted = true;
// }




// TIDorb::core::comm::ziop::ProfileZIOP::ProfileZIOP(
//                         const TIDorb::core::comm::ziop::Version& version,
//                         const TIDorb::core::iop::VectorTaggedComponent& tagged_components)
//   throw (TIDThr::SystemException)
//   : TIDorb::core::iop::TaggedProfile(TIDorb::core::iop::TAG_ZIOP_POLICIES)
// {
//   cerr << "ProfileZIOP(version, tagged_components)" << endl;
//   _version = version;
//   _components = tagged_components;
//   _profile_data = NULL;
//   _components_extracted = true;
// }




// TIDorb::core::comm::ziop::ProfileZIOP::ProfileZIOP(
//                          const TIDorb::core::comm::ziop::ProfileZIOP& other)
//   throw (TIDThr::SystemException)
//   : TIDorb::core::iop::TaggedProfile(TIDorb::core::iop::TAG_ZIOP_POLICIES)
// {
//   cerr << "ProfileZIOP(other)" << endl;
//   if (! other._components_extracted)
//     ((ProfileZIOP*) &other)->extract_members();
  
//   _version = other._version;
//   _components = other._components;
//   _profile_data = NULL;
//   _components_extracted = true;
// }




// TIDorb::core::comm::ziop::ProfileZIOP::~ProfileZIOP() throw(TIDThr::SystemException)
// {
//   delete _profile_data;
// }




// TIDorb::core::comm::ziop::ProfileZIOP&
// TIDorb::core::comm::ziop::ProfileZIOP::operator= (const TIDorb::core::comm::ziop::ProfileZIOP& other)
// {
//   if (! other._components_extracted)
//     ((ProfileZIOP*) &other)->extract_members();
  
//   _version = other._version;
//   _components = other._components;
//   _profile_data = NULL;
//   _components_extracted = true;
//   return *this;
// }




// void TIDorb::core::comm::ziop::ProfileZIOP::extract_members()
// {
//   cerr << "ProfileZIOP::extract_members" << endl;
//   if (!_components_extracted) {
    
//     TIDThr::Synchronized synchro(*this);
    
//     if (_components_extracted) // May be changed after return of synchro
//       return;
    
    
//     if (_profile_data == NULL) {
//       throw CORBA::INTERNAL("Empty Profile");
//     }
    
//     _version.read(*_profile_data);
    
//     //_listen_point.read(*_profile_data);
    
//     CORBA::ULong size;
//     _profile_data->read_ulong(size);
    
//     if (size < 0) {
//       throw CORBA::MARSHAL("Invalid component size");
//     } else {
//       _components.resize(size);
      
//       for(size_t i = 0; i < size; i++) {
//         _components[i] = TIDorb::core::iop::TaggedComponentReader::read(*_profile_data);
//       }
//     }

//     _components_extracted = true;
//   }
// }




// const TIDorb::core::comm::ziop::Version&
// TIDorb::core::comm::ziop::ProfileZIOP::getVersion() const
// {
//   cerr << "ProfileZIOP::getVersion()" << endl;
//   if (! _components_extracted) {
//     ((ProfileZIOP*) this)->extract_members();
//   }
//   return _version;
// }




// const TIDorb::core::iop::VectorTaggedComponent&
// TIDorb::core::comm::ziop::ProfileZIOP::getTaggedComponents() const
// {
//   cerr << "ProfileZIOP::getTaggedComponents()" << endl;
//   if (! _components_extracted) {
//     ((ProfileZIOP*) this)->extract_members();
//   }
//   return _components;
// }




// bool TIDorb::core::comm::ziop::ProfileZIOP::operator== (const TIDorb::core::iop::TaggedProfile& other) const
// {
//   const TIDorb::core::comm::ziop::ProfileZIOP* profile =
//     dynamic_cast<const TIDorb::core::comm::ziop::ProfileZIOP*>(&other);
  
//   if((profile == NULL) || (_tag != profile->_tag))
//     return false;
  
//   if (! _components_extracted) {
//     ((ProfileZIOP*) this)->extract_members();
//   }
  
// //   TIDorb::core::comm::ziop::GroupInfo*        group_info       = NULL;
// //   TIDorb::core::comm::ziop::GroupIIOPProfile* group_iiop       = NULL;
  
// //   TIDorb::core::comm::ziop::GroupInfo*        other_group_info = NULL;
// //   TIDorb::core::comm::ziop::GroupIIOPProfile* other_group_iiop = NULL;

// //   TIDorb::core::iop::VectorTaggedComponent::const_iterator iter;
// //   TIDorb::core::iop::VectorTaggedComponent::const_iterator end = _components.end();

// //   for (iter = _components.begin(); iter != end; iter++) {
// //     TIDorb::core::iop::TaggedComponent* comp = (TIDorb::core::iop::TaggedComponent*) *iter;
// //     if (comp->_tag == TIDorb::core::iop::TAG_GROUP) {
// //       group_info = dynamic_cast<TIDorb::core::comm::ziop::GroupInfo*>(comp);
// //     } else if (comp->_tag == TIDorb::core::iop::TAG_GROUP_IIOP) {
// //       group_iiop = dynamic_cast<TIDorb::core::comm::ziop::GroupIIOPProfile*>(comp);
// //     }
// //   }
// //   end = profile->_components.end();
// //   for (iter = profile->_components.begin(); iter != end; iter++) {
// //     TIDorb::core::iop::TaggedComponent* comp = (TIDorb::core::iop::TaggedComponent*) *iter;
// //     if (comp->_tag == TIDorb::core::iop::TAG_GROUP) {
// //       other_group_info = dynamic_cast<TIDorb::core::comm::ziop::GroupInfo*>(comp);
// //     } else if (comp->_tag == TIDorb::core::iop::TAG_GROUP_IIOP) {
// //       other_group_iiop = dynamic_cast<TIDorb::core::comm::ziop::GroupIIOPProfile*>(comp);
// //     }
// //   }


//   // Compare profiles

//   if (getVersion() != profile->getVersion()) {
//     return false;
//   }

// //   if (getListenPoint() != profile->getListenPoint()) {
// //     return false;
// //   }

// //   if (((group_info == NULL) && (other_group_info != NULL))
// //       || ((group_info != NULL) && (other_group_info == NULL))) {
// //     return false;
// //   }

// //   if (group_info && other_group_info && !(*group_info == *other_group_info)) {
// //     return false;
// //   }

// //   if (((group_iiop == NULL) && (other_group_iiop != NULL))
// //       || ((group_iiop != NULL) && (other_group_iiop == NULL))) {
// //     return false;
// //   }

// //   if (group_iiop && other_group_iiop && !(*group_iiop == *other_group_iiop)) {
// //     return false;
// //   }

//   return true;
// }




// void TIDorb::core::comm::ziop::ProfileZIOP::partial_read(TIDorb::core::cdr::CDRInputStream& input)
// {
//   cerr << "ProfileZIOP::partial_read(input)" << endl;
//   _profile_data = input.read_encapsulation();
// }




// void TIDorb::core::comm::ziop::ProfileZIOP::partial_write(TIDorb::core::cdr::CDROutputStream& out) const
// {
//   TIDThr::Synchronized synchro (*((ProfileZIOP*) this));
//   cerr << "ProfileZIOP::partial_write(out)" << endl;
//   // write profile data
//   if (_profile_data == NULL) {

//     TIDorb::core::cdr::CDROutputStream encapsulation((TIDorb::core::TIDORB*) (out.orb()));
    
//     encapsulation.write_boolean(encapsulation.get_byte_order());
    
//     _version.write(encapsulation);
//     //    _listen_point.write(encapsulation);
    
//     size_t size = _components.size();
//     encapsulation.write_ulong((CORBA::ULong) size);
//     cerr << "ProfileZIOP::partial_write writing " << size << " components" << endl;
//     for(size_t i = 0; i < size; i++) {
//       _components[i]->write(encapsulation);
//     }
    
//     ((ProfileZIOP*) this)->_profile_data = (TIDorb::core::cdr::CDRInputStream*)
//       (encapsulation.create_input_stream());
//   }
  
//   out.write_buffer(*(_profile_data->get_buffer()));
// }




// char* TIDorb::core::comm::ziop::ProfileZIOP::toString() const
// {
//   cerr << "ProfileZIOP::toString()" << endl;
//   ((ProfileZIOP*) this)->extract_members();

//   TIDorb::util::StringBuffer buffer;

//   buffer << ("Profile ZIOP: {")
//          << getVersion().toString(); 
// //          << ", "
// //          << getListenPoint().toString();
          
//   buffer << " Components {";
//   for (size_t i = 0; i <_components.size(); i++) {
//     buffer << " " << _components[i]->toString();
//   }
//   buffer << " }}";

//   return CORBA::string_dup(buffer.str().data());
// }



