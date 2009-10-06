/////////////////////////////////////////////////////////////////////////
//
// File:        DynUnionImpl.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
//
// Revised:    
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Right
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////

#include "TIDorb/dynAny.h"
#include "TIDorb/core/typecode.h"


#define DYN_UNION_REP_ID        "IDL:omg.org/DynamicAny/DynUnion:1.0"


namespace TIDorb {
namespace dynAny {




DynUnionImpl::DynUnionImpl(DynamicAny::DynAnyFactory_ptr factory,
                           TIDorb::core::TIDORB* orb)
   throw(CORBA::SystemException)
   : DynAnyBase(factory, orb)
{
   _member            = NULL;
   _dyn_discriminator = NULL;
   _current           = -1;
   _component_count   =  1;
   _member_index      =  0;
   this->TIDThr::RefCounter::_add_ref();
}




DynUnionImpl::DynUnionImpl(DynamicAny::DynAnyFactory_ptr factory,
                           TIDorb::core::TIDORB* orb,
                           const CORBA::Any& any,
                           CORBA::TypeCode_ptr real_type)
   throw(CORBA::SystemException)
   : DynAnyBase(factory, orb, CORBA::TypeCode_var(any.type()), real_type)
{
   _member            = NULL;
   _dyn_discriminator = NULL;
   _current           = -1;
   _member_index      =  0;

   _component_count = 2;
   TIDorb::portable::InputStream* in = any.delegate().create_input_stream();
   _read(*in);
   delete in;
   this->TIDThr::RefCounter::_add_ref();
}




DynUnionImpl::DynUnionImpl(DynamicAny::DynAnyFactory_ptr factory,
                           TIDorb::core::TIDORB* orb,
                           CORBA::TypeCode_ptr type,
                           CORBA::TypeCode_ptr real_type)
   throw(CORBA::SystemException)
   : DynAnyBase(factory, orb, type, real_type)
{
   try
   {
      _member            = NULL;
      _dyn_discriminator = NULL;

      _member_index = _base_type->default_index();
      if (_member_index > -1)
      {
         _component_count   = 2;
      }
      else
        {
          //  For DynUnion, the operation sets the current position to zero. The discriminator
          //  value is set to a value consistent with the first named member of the union. That
          //  member is activated and (recursively) initialized to its default value.
          _member_index      = 0;
          _current           = 0;
          _component_count   = 2;
          try{
            CORBA::Any_var label = _base_type->member_label(_member_index);
            DynAnyBase* dyn_disc =
              dynamic_cast< DynAnyBase* > (_factory->create_dyn_any(*label));
            set_index(_member_index, dyn_disc);
            member();
            delete dyn_disc;
          }catch(...){
            throw CORBA::INTERNAL();
          }
        }
   }
   catch(CORBA::TypeCode::BadKind& bk)
   {
      throw CORBA::BAD_TYPECODE(bk._name());
   }
   this->TIDThr::RefCounter::_add_ref();
}




DynUnionImpl::~DynUnionImpl()
  throw (TIDThr::SystemException)
{
   delete _member;
   delete _dyn_discriminator;
}




DynamicAny::DynAny_ptr DynUnionImpl::copy()
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   DynUnionImpl* new_dyn = new DynUnionImpl(_factory, _orb);
   new_dyn->_dyn_type        = CORBA::TypeCode::_duplicate(_dyn_type);
   new_dyn->_base_type       = CORBA::TypeCode::_duplicate(_base_type);
   new_dyn->_member_index    = _member_index;
   new_dyn->_component_count = _component_count;

   if (_component_count == 2)
   {
      if (_member)
      {
         new_dyn->_member =
            dynamic_cast< DynAnyBase * > (_member->copy());
      }
      if (_dyn_discriminator)
      {
         new_dyn->_dyn_discriminator =
            dynamic_cast< DynAnyBase * > (_dyn_discriminator->copy());
      }
   }

   return new_dyn;
}




DynamicAny::DynAny_ptr DynUnionImpl::get_discriminator()
   throw(DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_component_count == 1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "No active member"
   }

   if (_dyn_discriminator == NULL)
   {
      // default discriminator search
      CORBA::Any_var discriminator = _orb->create_any();
      TIDorb::core::typecode::UnionTypeCode* union_typecode =
          dynamic_cast <TIDorb::core::typecode::UnionTypeCode * > ((CORBA::TypeCode_ptr) _base_type);

      CORBA::Long i=0;
      CORBA::Long index;
      do
      {
         // search a value that will be not at the case labels
         set_discriminator_value(*discriminator, ++i);
         index = union_typecode->search_member_index(*discriminator);
      }
      while (index != _member_index);

      _dyn_discriminator =
         dynamic_cast<DynAnyBase*>(_factory->create_dyn_any(*discriminator));
   }

   return DynamicAny::DynAny::_duplicate(_dyn_discriminator);
}




void DynUnionImpl::set_discriminator(DynamicAny::DynAny_ptr d)
   throw(DynamicAny::DynAny::TypeMismatch, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   CORBA::Any_var any = d->to_any();
   TIDorb::core::typecode::UnionTypeCode* union_typecode =
      dynamic_cast<TIDorb::core::typecode::UnionTypeCode*> ((CORBA::TypeCode_ptr) _base_type);

   CORBA::Long index = union_typecode->search_member_index(*any);

   if (index < 0)
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   set_index(index, d);
}




void DynUnionImpl::set_index(CORBA::Long index,
                             DynamicAny::DynAny_ptr discriminator)
   throw(CORBA::SystemException)
{
   if (_member_index == index)
   {
      if ((_dyn_discriminator == NULL) && (discriminator != NULL))
      {
         _dyn_discriminator = dynamic_cast< DynAnyBase * > (discriminator->copy()) ;
      }

      return;
   }

   if (_dyn_discriminator)
   {
      _dyn_discriminator->destroy();
      delete _dyn_discriminator;
      _dyn_discriminator = NULL;
   }

   if (discriminator)
   {
      _dyn_discriminator = dynamic_cast< DynAnyBase* >(discriminator->copy());
   }

   _current         = 0;
   _component_count = 2;
   _member_index    = index;

   try
   {
      if (_member)
      {
         CORBA::TypeCode_var index_type =_base_type->member_type(_member_index);
         CORBA::TypeCode_var member_type=_member->type();
         if (! member_type->equivalent(index_type))
         {
            _member->destroy();
            delete _member;
            _member = NULL;
         }
      }
   }
   catch(CORBA::TypeCode::BadKind& bk)
   {
      throw CORBA::BAD_TYPECODE(bk._name());
   }
   catch(CORBA::TypeCode::Bounds& bd)
   {
      throw CORBA::INTERNAL(bd._name());
   }
}




void DynUnionImpl::set_to_default_member()
   throw(DynamicAny::DynAny::TypeMismatch, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   try
   {
      // set member idex to defalut index
      CORBA::Long index = _base_type->default_index();
      if (index < 0)
      {
         throw DynamicAny::DynAny::TypeMismatch();
      }

      set_index(index, NULL);
   }
   catch(CORBA::TypeCode::BadKind& bk)
   {
      throw CORBA::BAD_TYPECODE(bk._name());
   }
}




void DynUnionImpl::set_to_no_active_member()
   throw(DynamicAny::DynAny::TypeMismatch, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   try
   {
      if (_base_type->default_index() > -1)
      {
         throw DynamicAny::DynAny::TypeMismatch();
      }

      // are all the discriminator values uses
      if (all_cases_used())
      {
         throw DynamicAny::DynAny::TypeMismatch();
      }
   }
   catch(CORBA::TypeCode::BadKind& bk)
   {
      throw CORBA::BAD_TYPECODE(bk._name());
   }

   if (_member)
   {
      _member->destroy();
      delete _member;
      _member = NULL;
   }

   if (_dyn_discriminator)
   {
      _dyn_discriminator->destroy();
      delete _dyn_discriminator;
      _dyn_discriminator = NULL;
   }

   _member_index = -1;
   _component_count = 1;
}




CORBA::Boolean DynUnionImpl::has_no_active_member()
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   return (_component_count < 2);
}




CORBA::TCKind DynUnionImpl::discriminator_kind()
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   try
   {
      CORBA::TypeCode_var type = _base_type->discriminator_type();
      return type->kind();
   }
   catch(CORBA::TypeCode::BadKind& bk)
   {
      // unreachable
      throw CORBA::BAD_TYPECODE(bk._name());
   }
}




DynamicAny::DynAny_ptr DynUnionImpl::member()
   throw(DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
//    if (_destroyed)
//    {
//       throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
//    }
   if (has_no_active_member())
   {
      throw DynamicAny::DynAny::InvalidValue(); // "No active member"
   }
//   if (_component_count == 1)
//    {
//       throw DynamicAny::DynAny::InvalidValue(); // "No active member"
//    }

   if (_member == NULL)
   {
      try
      {
         CORBA::TypeCode_var type = _base_type->member_type(_member_index);
         _member = dynamic_cast< DynAnyBase* >
                               (_factory->create_dyn_any_from_type_code(type));
      }
      catch(CORBA::TypeCode::BadKind& bk)
      {
         throw CORBA::BAD_TYPECODE(bk._name());
      }
      catch(CORBA::TypeCode::Bounds& bd)
      {
         throw CORBA::INTERNAL(bd._name());
      }
      catch(DynamicAny::DynAnyFactory::InconsistentTypeCode)
      {
         // unreachable
      }
   }

   return DynamicAny::DynAny::_duplicate(_member);
}




DynamicAny::FieldName DynUnionImpl::member_name()
   throw(DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_component_count == 1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "No active member"
   }

   try
   {
      return CORBA::string_dup(_base_type->member_name(_member_index));
   }
   catch(CORBA::TypeCode::BadKind& bk)
   {
      throw CORBA::BAD_TYPECODE(bk._name());
   }
   catch(CORBA::TypeCode::Bounds& bd)
   {
      throw CORBA::INTERNAL(bd._name());
   }
}




void DynUnionImpl::member_name(DynamicAny::FieldName name)
   throw(DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_component_count == 1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "No active member"
   }

   try
   {
      CORBA::Long member_count = _base_type->member_count();
      for (CORBA::Long i=0; i < member_count; i++)
      {
         if(!strcmp(name, _base_type->member_name(i)))
         {
            CORBA::Any_var label = _base_type->member_label(i);
            DynAnyBase* dyn_disc =
               dynamic_cast< DynAnyBase* > (_factory->create_dyn_any(*label));
            set_index(i, dyn_disc);
            delete dyn_disc;
            return;
         }
      }
   }
   catch(CORBA::TypeCode::BadKind& bk)
   {
      throw CORBA::BAD_TYPECODE(bk._name());
   }
   catch(CORBA::TypeCode::Bounds& bd)
   {
      throw CORBA::INTERNAL(bd._name());
   }
   catch(DynamicAny::DynAnyFactory::InconsistentTypeCode)
   {
      // unreachable
      return;
   }

   throw DynamicAny::DynAny::InvalidValue();
}




CORBA::TCKind DynUnionImpl::member_kind()
   throw(DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_member_index < 0)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "No member selected"
   }

   CORBA::TypeCode_var type = _base_type->member_type(_member_index);
   return type->kind();
}



CORBA::Boolean DynUnionImpl::is_set_to_default_member()
{
  if (_destroyed)
  {
    throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
  }
	
  //_base_type should be a tc_union
  // index is the position in the UnionMember sequence
  // of the default member
  CORBA::Long index = _base_type->default_index();
  
  if (index == -1) // The type has no default member
  {
    return false;
  }
  
  return _member_index == index; 
  
}


DynamicAny::DynAny_ptr DynUnionImpl::current_component()
   throw(DynamicAny::DynAny::TypeMismatch, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      //jagd
      //return DynamicAny::DynAny::_nil();
      return 0;
   }

   try
   {
      switch (_current)
      {
         case 0:
            return get_discriminator();

         case 1:
            return member();
      }
   }
   catch(DynamicAny::DynAny::InvalidValue& iv)
   {
      throw CORBA::INTERNAL(iv._name());
   }

   throw CORBA::INTERNAL("Current out of range");
}




void DynUnionImpl::_read(TIDorb::portable::InputStream& in)
{
   CORBA::Any_var discriminator_any = _orb->create_any();

   try
   {
      CORBA::TypeCode_var type = _base_type->discriminator_type();
      discriminator_any->delegate().read_value(in, type);

      TIDorb::core::typecode::UnionTypeCode* union_typecode =
         dynamic_cast< TIDorb::core::typecode::UnionTypeCode* > ((CORBA::TypeCode_ptr) _base_type);

      _member_index = union_typecode->search_member_index(*discriminator_any);

      if (_member_index < 0)
      {
        // "Invalid Union label: %ld", _member_index);
         throw CORBA::BAD_PARAM();
      }

      _dyn_discriminator = dynamic_cast< DynAnyBase* >
                           (_factory->create_dyn_any(*discriminator_any));

      _dyn_discriminator->set_user_destroy(false);

      type = _base_type->member_type(_member_index);
      _member = dynamic_cast< DynAnyBase* >
                (_factory->create_dyn_any_from_type_code(type));

      _member->set_user_destroy(false);

      _member->_read(in);
   }
   catch(CORBA::TypeCode::BadKind& bk)
   {
      throw CORBA::BAD_TYPECODE(bk._name());
   }
   catch(CORBA::TypeCode::Bounds& bd)
   {
      throw CORBA::INTERNAL(bd._name());
   }
   catch(DynamicAny::DynAnyFactory::InconsistentTypeCode)
   {
      // unreachable
   }
}



void DynUnionImpl::_write(TIDorb::portable::OutputStream& out) const
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

//    if (_component_count == 1)
   if (_component_count < 2)
   {
      throw CORBA::BAD_OPERATION("Uncompleted union");
   }

   try
   {
      DynUnionImpl* this_union = (DynUnionImpl*) this;
      DynamicAny::DynAny_var dyn_any = this_union->get_discriminator();

      DynAnyBase* dyn_base =
         dynamic_cast< DynAnyBase* > ((DynamicAny::DynAny_ptr) dyn_any);
      dyn_base->_write(out);

      dyn_any = this_union->member();
      dyn_base = dynamic_cast< DynAnyBase* > ((DynamicAny::DynAny_ptr) dyn_any);
      dyn_base->_write(out);
   }
   catch(DynamicAny::DynAny::InvalidValue& iv)
   {
      throw CORBA::INTERNAL(iv._name());
   }
}




void DynUnionImpl::assign(DynamicAny::DynAny_ptr dyn_any)
   throw(DynamicAny::DynAny::TypeMismatch, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   //jagd
   //if (CORBA::is_nil(dyn_any))
   if (!(dyn_any))
   {
      throw CORBA::BAD_PARAM("Null DynAny reference", 0, CORBA::COMPLETED_NO);
   }

   CORBA::TypeCode_var type = dyn_any->type();
   if (! _base_type->equivalent(type))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynUnion_ptr dyn_union;
   dyn_union = DynamicAny::DynUnion::_narrow(dyn_any);
   if (dyn_union)
   {
      try
      {
         if (dyn_union->has_no_active_member())
         {
            set_to_no_active_member();
         }
         else
         {
            DynamicAny::DynAny_var value = dyn_union->get_discriminator();
            set_discriminator(value);

            value = member();
            value->assign(DynamicAny::DynAny_var(dyn_union->member()));
         }
      }
      catch(DynamicAny::DynAny::InvalidValue)
      {
      }
   }
   else
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }
}




void DynUnionImpl::from_any(const CORBA::Any& value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   CORBA::TypeCode_var type = value.type();
   if (! _base_type->equivalent(type))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   TIDorb::portable::InputStream* in = NULL;
   try
   {
      in = value.delegate().create_input_stream();
      _read(*in);
      delete in;
   }
   catch(CORBA::MARSHAL)
   {
      delete in;
      throw DynamicAny::DynAny::InvalidValue();
   }
}




CORBA::Any* DynUnionImpl::to_any()
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   CORBA::Any* new_any = new CORBA::Any();

   // Fix bug [#392] Any::type(tc) reset any value
   //new_any->type(_dyn_type);
   new_any->delegate().set_type(_dyn_type);

   TIDorb::portable::OutputStream* output =
    new_any->delegate().create_output_stream();

   _write(*output);

   delete output;

   return new_any;
}




CORBA::Boolean DynUnionImpl::equal(DynamicAny::DynAny_ptr dyn_any)
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   //jagd 
   //if (CORBA::is_nil(dyn_any))
   if (!(dyn_any))
   {
      throw CORBA::BAD_PARAM("Null DynAny reference", 0, CORBA::COMPLETED_NO);
   }

   CORBA::TypeCode_var type = dyn_any->type();
   if (! _base_type->equivalent(type))
   {
      return false;
   }

   DynamicAny::DynUnion_ptr dyn_union;
   dyn_union = DynamicAny::DynUnion::_narrow(dyn_any);
   if (dyn_union)
   {
      try
      {
         DynamicAny::DynAny_var value = get_discriminator();
         DynamicAny::DynAny_var dyuni = dyn_union->get_discriminator();
         if (! value->equal(dyuni))
         {
            return false;
         }

         value = member();
         dyuni = dyn_union->member();
         return value->equal(dyuni);
      }
      catch(DynamicAny::DynAny::InvalidValue& iv)
      {
         throw CORBA::INTERNAL(iv._name());
      }
   }
   else
   {
      throw CORBA::BAD_PARAM(
            "DynAny value does not instantiate DynUnion but its type is union");
   }
}




void DynUnionImpl::destroy_now()
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_member)
   {
      _member->destroy();
      delete _member;
      _member = NULL;
   }

   if (_dyn_discriminator)
   {
      _dyn_discriminator->destroy();
      delete _dyn_discriminator;
      _dyn_discriminator = NULL;
   }
}




CORBA::Boolean DynUnionImpl::all_cases_used()
   throw(DynamicAny::DynAny::TypeMismatch, CORBA::SystemException)
{
   CORBA::TypeCode_var disc_type;
   CORBA::Long member_count;

   try
   {
      disc_type = _base_type->discriminator_type();

      while (disc_type->kind() == CORBA::tk_alias)
      {
         disc_type = disc_type->content_type();
      }

      member_count = _base_type->member_count();

      switch (disc_type->kind())
      {
         case CORBA::tk_short:
         case CORBA::tk_long:
         case CORBA::tk_longlong:
         case CORBA::tk_ushort:
         case CORBA::tk_ulong:
         case CORBA::tk_ulonglong:
         case CORBA::tk_wchar:
         case CORBA::tk_char:
            return false;

         case CORBA::tk_boolean:
            return (member_count <= 2);

         case CORBA::tk_enum:
            return (disc_type->member_count() <= member_count);
         default:
            break;
      }
   }
   catch(CORBA::TypeCode::BadKind& bk)
   {
      throw CORBA::INTERNAL(bk._name());
   }

   throw DynamicAny::DynAny::TypeMismatch(); // "Invalid discriminator value"
}




void DynUnionImpl::set_discriminator_value(CORBA::Any& discriminator,
                                           CORBA::LongLong value)
   throw(CORBA::SystemException)
{
   CORBA::TypeCode_var type = discriminator.type();
   switch (type->kind())
   {
      case CORBA::tk_short:
         discriminator <<= (CORBA::Short) value;
         break;

      case CORBA::tk_long:
         discriminator <<= (CORBA::Long) value;
         break;

      case CORBA::tk_longlong:
         discriminator <<= value;
         break;

      case CORBA::tk_ushort:
         discriminator <<= (CORBA::UShort) value;
         break;

      case CORBA::tk_ulong:
         discriminator <<= (CORBA::ULong) value;
         break;

      case CORBA::tk_ulonglong:
         discriminator <<= (CORBA::ULongLong) value;
         break;

      case CORBA::tk_boolean:
         discriminator <<= CORBA::Any::from_boolean((value) ? true:false);
         break;

      case CORBA::tk_char:
         discriminator <<= CORBA::Any::from_char((char) value);
         break;

      case CORBA::tk_wchar:
         discriminator <<= (CORBA::WChar) value;
         break;

      case CORBA::tk_enum:
         try
         {
/*
            discriminator.insert_Streamable(
            new EnumHolder((EnumTypeCode)base_type.discriminator_type(),value));
*/
         }
         catch(CORBA::TypeCode::BadKind)
         {
         }
         break;
      default:
         break;
   }
}




CORBA::Boolean DynUnionImpl::_is_a(const char* repositoryIdentifier)
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (repositoryIdentifier == NULL)
   {
      throw CORBA::BAD_PARAM("Null string reference");
   }

   if (!strcmp(repositoryIdentifier, DYN_UNION_REP_ID))
   {
      return true;
   }
   return ::CORBA::LocalObject::_is_a(repositoryIdentifier);
   //   return DynAnyBase::_is_a(repositoryIdentifier);
}




void DynUnionImpl::insert_boolean(CORBA::Boolean value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_boolean))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   if (_current == 0)
   {
      try
      {
         DynamicAny::DynAny_var new_disc =
            _factory->create_dyn_any_from_type_code(type);
         new_disc->insert_boolean(value);
         set_discriminator(new_disc);
      }
      catch(DynamicAny::DynAnyFactory::InconsistentTypeCode& itc)
      {
         throw CORBA::INTERNAL(itc._name());
      }
   }
   else
   {
      dyn_curr->insert_boolean(value);
   }
}




void DynUnionImpl::insert_char(char value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_char))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   if (_current == 0)
   {
      try
      {
         DynamicAny::DynAny_var new_disc =
            _factory->create_dyn_any_from_type_code(type);
         new_disc->insert_char(value);
         set_discriminator(new_disc);
      }
      catch(DynamicAny::DynAnyFactory::InconsistentTypeCode& itc)
      {
         throw CORBA::INTERNAL(itc._name());
      }
   }
   else
   {
      dyn_curr->insert_char(value);
   }
}




void DynUnionImpl::insert_wchar(CORBA::WChar value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_wchar))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   if (_current == 0)
   {
      try
      {
         DynamicAny::DynAny_var new_disc =
            _factory->create_dyn_any_from_type_code(type);
         new_disc->insert_wchar(value);
         set_discriminator(new_disc);
      }
      catch(DynamicAny::DynAnyFactory::InconsistentTypeCode& itc)
      {
         throw CORBA::INTERNAL(itc._name());
      }
   }
   else
   {
      dyn_curr->insert_wchar(value);
   }
}




void DynUnionImpl::insert_octet(CORBA::Octet value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_octet))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   if (_current == 0)
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }
   else
   {
      dyn_curr->insert_octet(value);
   }
}




void DynUnionImpl::insert_short(CORBA::Short value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_short))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   if (_current == 0)
   {
      try
      {
         DynamicAny::DynAny_var new_disc =
            _factory->create_dyn_any_from_type_code(type);
         new_disc->insert_short(value);
         set_discriminator(new_disc);
      }
      catch(DynamicAny::DynAnyFactory::InconsistentTypeCode& itc)
      {
         throw CORBA::INTERNAL(itc._name());
      }
   }
   else
   {
      dyn_curr->insert_short(value);
   }
}




void DynUnionImpl::insert_ushort(CORBA::UShort value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_ushort))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   if (_current == 0)
   {
      try
      {
         DynamicAny::DynAny_var new_disc =
            _factory->create_dyn_any_from_type_code(type);
         new_disc->insert_ushort(value);
         set_discriminator(new_disc);
      }
      catch(DynamicAny::DynAnyFactory::InconsistentTypeCode& itc)
      {
         throw CORBA::INTERNAL(itc._name());
      }
   }
   else
   {
      dyn_curr->insert_ushort(value);
   }
}




void DynUnionImpl::insert_long(CORBA::Long value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_long))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   if (_current == 0)
   {
      try
      {
         DynamicAny::DynAny_var new_disc =
            _factory->create_dyn_any_from_type_code(type);
         new_disc->insert_long(value);
         set_discriminator(new_disc);
      }
      catch(DynamicAny::DynAnyFactory::InconsistentTypeCode& itc)
      {
         throw CORBA::INTERNAL(itc._name());
      }
   }
   else
   {
      dyn_curr->insert_long(value);
   }
}




void DynUnionImpl::insert_ulong(CORBA::ULong value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_ulong))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   if (_current == 0)
   {
      try
      {
         DynamicAny::DynAny_var new_disc =
            _factory->create_dyn_any_from_type_code(type);
         new_disc->insert_ulong(value);
         set_discriminator(new_disc);
      }
      catch(DynamicAny::DynAnyFactory::InconsistentTypeCode& itc)
      {
         throw CORBA::INTERNAL(itc._name());
      }
   }
   else
   {
      dyn_curr->insert_ulong(value);
   }
}




void DynUnionImpl::insert_longlong(CORBA::LongLong value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_longlong))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   if (_current == 0)
   {
      try
      {
         DynamicAny::DynAny_var new_disc =
            _factory->create_dyn_any_from_type_code(type);
         new_disc->insert_longlong(value);
         set_discriminator(new_disc);
      }
      catch(DynamicAny::DynAnyFactory::InconsistentTypeCode& itc)
      {
         throw CORBA::INTERNAL(itc._name());
      }
   }
   else
   {
      dyn_curr->insert_longlong(value);
   }
}




void DynUnionImpl::insert_ulonglong(CORBA::ULongLong value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_ulonglong))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   if (_current == 0)
   {
      try
      {
         DynamicAny::DynAny_var new_disc =
            _factory->create_dyn_any_from_type_code(type);
         new_disc->insert_ulonglong(value);
         set_discriminator(new_disc);
      }
      catch(DynamicAny::DynAnyFactory::InconsistentTypeCode& itc)
      {
         throw CORBA::INTERNAL(itc._name());
      }
   }
   else
   {
      dyn_curr->insert_ulonglong(value);
   }
}




void DynUnionImpl::insert_longdouble(CORBA::LongDouble value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_longdouble))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   if (_current == 0)
   {
      try
      {
         DynamicAny::DynAny_var new_disc =
            _factory->create_dyn_any_from_type_code(type);
         new_disc->insert_longdouble(value);
         set_discriminator(new_disc);
      }
      catch(DynamicAny::DynAnyFactory::InconsistentTypeCode& itc)
      {
         throw CORBA::INTERNAL(itc._name());
      }
   }
   else
   {
      dyn_curr->insert_longdouble(value);
   }
}




void DynUnionImpl::insert_float(CORBA::Float value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_float))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   if (_current == 0)
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }
   else
   {
      dyn_curr->insert_float(value);
   }
}




void DynUnionImpl::insert_double(CORBA::Double value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_double))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   if (_current == 0)
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }
   else
   {
      dyn_curr->insert_double(value);
   }
}




void DynUnionImpl::insert_string(const char* value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_string))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   if (_current == 0)
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }
   else
   {
      dyn_curr->insert_string(value);
   }
}




void DynUnionImpl::insert_wstring(const CORBA::WChar* value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_wstring))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   if (_current == 0)
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }
   else
   {
      dyn_curr->insert_wstring(value);
   }
}




void DynUnionImpl::insert_any(const CORBA::Any& value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_any))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   if (_current == 0)
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }
   else
   {
      dyn_curr->insert_any(value);
   }
}




void DynUnionImpl::insert_dyn_any(DynamicAny::DynAny_ptr value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_any))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   if (_current == 0)
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }
   else
   {
      dyn_curr->insert_dyn_any(value);
   }
}




void DynUnionImpl::insert_typecode(CORBA::TypeCode_ptr value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_TypeCode))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   if (_current == 0)
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }
   else
   {
      dyn_curr->insert_typecode(value);
   }
}




void DynUnionImpl::insert_reference(CORBA::Object_ptr value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_Object))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   if (_current == 0)
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }
   else
   {
      dyn_curr->insert_reference(value);
   }
}




CORBA::Boolean DynUnionImpl::get_boolean()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_boolean))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   return dyn_curr->get_boolean();
}




char DynUnionImpl::get_char()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_char))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   return dyn_curr->get_char();
}




CORBA::WChar DynUnionImpl::get_wchar()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_wchar))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   return dyn_curr->get_wchar();
}




CORBA::Octet DynUnionImpl::get_octet()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_octet))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   return dyn_curr->get_octet();
}




CORBA::Short DynUnionImpl::get_short()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_short))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   return dyn_curr->get_short();
}




CORBA::UShort DynUnionImpl::get_ushort()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_ushort))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   return dyn_curr->get_ushort();
}




CORBA::Long DynUnionImpl::get_long()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_long))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   return dyn_curr->get_long();
}




CORBA::ULong DynUnionImpl::get_ulong()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_ulong))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   return dyn_curr->get_ulong();
}




CORBA::LongLong DynUnionImpl::get_longlong()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_longlong))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   return dyn_curr->get_longlong();
}




CORBA::ULongLong DynUnionImpl::get_ulonglong()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_ulonglong))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   return dyn_curr->get_ulonglong();
}




CORBA::LongDouble DynUnionImpl::get_longdouble()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_ulonglong))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   return dyn_curr->get_longdouble();
}




CORBA::Float DynUnionImpl::get_float()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_float))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   return dyn_curr->get_float();
}




CORBA::Double DynUnionImpl::get_double()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_double))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   return dyn_curr->get_double();
}




char* DynUnionImpl::get_string()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_string))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   return dyn_curr->get_string();
}




CORBA::WChar* DynUnionImpl::get_wstring()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_wstring))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   return dyn_curr->get_wstring();
}




CORBA::Any* DynUnionImpl::get_any()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_any))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   return dyn_curr->get_any();
}




DynamicAny::DynAny_ptr DynUnionImpl::get_dyn_any()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_any))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   return dyn_curr->get_dyn_any();
}




CORBA::TypeCode_ptr DynUnionImpl::get_typecode()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_TypeCode))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   return dyn_curr->get_typecode();
}




CORBA::Object_ptr DynUnionImpl::get_reference()
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed");
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   CORBA::TypeCode_var type = dyn_curr->type();
   if (! type->equivalent(CORBA::_tc_Object))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   return dyn_curr->get_reference();
}




CORBA::ULong DynUnionImpl::component_count()
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   //return 0;
   return _component_count;
}




CORBA::Boolean DynUnionImpl::seek(CORBA::Long index)
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      return false;
   }

   if (index < 0)
   {
      _current = -1;
      return false;
   }

   if (index < component_count())
   {
      _current = index;
      return true;
   }
   else
   {
      return false;
   }
}




void DynUnionImpl::rewind()
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current != -1)
   {
      _current = 0;
   }
}




CORBA::Boolean DynUnionImpl::next()
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      return false;
   }

   if (_current < (component_count() - 1))
   {
      _current++;
      return true;
   }
   else
   {
      return false;
   }
}




} //namespace TIDorb
} //namespace dynAny
