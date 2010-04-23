/////////////////////////////////////////////////////////////////////////
//
// File:        DynComposite.C
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

#include "TIDorb/dynAny.h"
#include "TIDorb/core.h"

#include <typeinfo>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif


namespace TIDorb {
namespace dynAny {




DynComposite::DynComposite(DynamicAny::DynAnyFactory_ptr factory,
                           TIDorb::core::TIDORB* orb)
   throw(CORBA::SystemException)
   : DynAnyBase(factory, orb)
{
   _component_count = 0;
   _current         = -1;
   _complete_value  = NULL;
   _next_value      = NULL;
}




DynComposite::DynComposite(DynamicAny::DynAnyFactory_ptr factory,
                           TIDorb::core::TIDORB* orb,
                           CORBA::TypeCode_ptr type,
                           CORBA::TypeCode_ptr real_type)
   throw(CORBA::SystemException)
   : DynAnyBase(factory, orb, type, real_type)
{
   _component_count = 0;
   _current         = -1;
   _complete_value  = NULL;
   _next_value      = NULL;
}




DynComposite::DynComposite(DynamicAny::DynAnyFactory_ptr factory,
                           TIDorb::core::TIDORB* orb,
                           const CORBA::Any& any,
                           CORBA::TypeCode_ptr real_type)
   throw(CORBA::SystemException)
   : DynAnyBase(factory, orb, CORBA::TypeCode_var(any.type()), real_type)
{
   _component_count = 0;
   _current         = -1;
   _complete_value  = NULL;
   _next_value      = NULL;
   get_streams(any);
}




DynComposite::~DynComposite()
{
   delete _complete_value;
   delete _next_value;
}




void DynComposite::reset()
   throw(CORBA::SystemException)
{
   _current = (_component_count == 0) ? -1 : 0;

   long size = _components.size();
   for (long i=0; i<size; i++)
   {
      _components[i]->destroy();
   }

   _components.clear();

   delete _complete_value;
   _complete_value = NULL;

   delete _next_value;
   _next_value = NULL;
}




void DynComposite::last_component()
   throw(CORBA::SystemException)
{
   delete _complete_value;
   _complete_value = NULL;

   delete _next_value;
   _next_value = NULL;
}




void DynComposite::_read(TIDorb::portable::InputStream& in)
{
   reset();

   CORBA::Any_var any = _orb->create_any();
   // FIX bug [#393] Error processing a return of DII invocation
   // Call read_value with skip optimization   
   any->delegate().read_value(in, _base_type, true); ///

   get_streams(*any);
}




void DynComposite::_write(TIDorb::portable::OutputStream& out) const
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   try
   {
      for (CORBA::ULong i=0; i < _component_count; i++)
      {
         DynamicAny::DynAny_var dyn = ((DynComposite*) this)->get_component(i);
         DynAnyBase* base = dynamic_cast<DynAnyBase*>((DynamicAny::DynAny_ptr)dyn);
         if (base)
         {
            base->_write(out);
         }
         else
         {
            CORBA::Any_var any = dyn->to_any();
            any->delegate().write_value(out);
         }
      }
   }
   catch(DynamicAny::DynAny::TypeMismatch)
   {
      throw CORBA::INTERNAL();
   }
}




void DynComposite::destroy_now()
  throw(CORBA::SystemException)
{
   DynAnyBase::destroy_now();
   reset();
}




void DynComposite::get_streams(const CORBA::Any& any)
         throw(CORBA::SystemException)
{
   TIDorb::portable::InputStream* is;

  
    TIDorb::core::AnyImpl& impl =
         *(TIDorb::core::AnyImpl*)(&any.delegate());
    
    is = impl.create_input_stream();   

   _complete_value = (TIDorb::core::cdr::CDRInputStream*)(is);

   _complete_value->orb(_orb);

   _next_value = _complete_value->copy();
}




void DynComposite::pre_assign(DynamicAny::DynAny_ptr dyn_any)
   throw(DynamicAny::DynAny::TypeMismatch,CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

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

   // both are the same type
   reset();

   // _component_count = dyn_any->component_count();
}




void DynComposite::copy_to(DynComposite& dyn_any)
   throw(CORBA::SystemException)
{
   dyn_any._component_count = _component_count;

   if (_component_count == 0)
   {
      dyn_any._current = -1;
      return;
   }

   if (_complete_value != NULL)
   {
      dyn_any._complete_value = _complete_value->copy();
      _next_value = _complete_value->copy();
      return;
   }

   try
   {
      for (CORBA::ULong i=0; i < _component_count; i++)
      {
         dyn_any._components.push_back(get_component(i));
      }
   }
   catch(DynamicAny::DynAny::TypeMismatch)
   {
      throw CORBA::INTERNAL("Error with current_component");
   }
}




DynamicAny::DynAny_ptr DynComposite::get_component(CORBA::ULong position)
   throw(DynamicAny::DynAny::TypeMismatch, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynArray destroyed");
   }

   if (position == -1)
   {
      return NULL;
   }

   CORBA::ULong next_component_index = _components.size();

   if (position > next_component_index)
   {
      throw CORBA::INTERNAL("Component position out of sequence");
   }

   if (position == next_component_index)
   {
      // Initialize the component
      // When the DynAny has been introduced in a any (with to_any() operation)
      // prevents multihreaded writes (os.write_any() -> dyn_any._write() ->
      // dyn_any.get_element(i)._write()

      TIDThr::Synchronized sync(_mutex);

      if (position == _components.size())
      {
         DynAnyBase* dyn_component = NULL;
         DynamicAny::DynAny_ptr dynany = NULL;
         try
         {
            CORBA::TypeCode_var type = get_component_type(position);
            dynany = _factory->create_dyn_any_from_type_code(type);
            dyn_component = dynamic_cast<DynAnyBase*> (( DynamicAny::DynAny_ptr) dynany);
         }
         catch(DynamicAny::DynAnyFactory::InconsistentTypeCode)
         {
            throw DynamicAny::DynAny::TypeMismatch();
         }

         dyn_component->set_user_destroy(false); // component destruction
         _components.push_back(dyn_component);

         if (_next_value != NULL)
         {
            // demarshal the component
            dyn_component->_read(*_next_value);
         }

         if (position == (_component_count-1))
         {
            delete _complete_value;
            _complete_value = NULL;
            delete _next_value;
            _next_value = NULL;
         }
      }
   }

   return DynamicAny::DynAny::_duplicate(_components[position]);
}




void DynComposite::set_component(CORBA::ULong position, const CORBA::Any& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   try
   {
      CORBA::TypeCode_var value_type = value.type();
      CORBA::TypeCode_var comp_type  = get_component_type(position);

      if (! comp_type->equivalent(value_type))
      {
         throw DynamicAny::DynAny::TypeMismatch();
      }

      CORBA::ULong next_component_index = _components.size();

      if (position > next_component_index)
      {
         throw CORBA::INTERNAL("Component position out of sequence");
      }

      if (position == next_component_index)
      {
         // create the component
         DynamicAny::DynAny_var dyn_component = _factory->create_dyn_any(value);
         _components.push_back(dyn_component);
      }
      else
      {
         DynamicAny::DynAny_var dyn_component = _components[_current];
         dyn_component->from_any(value);
      }
   }
   catch(DynamicAny::DynAnyFactory::InconsistentTypeCode)
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }
}




void DynComposite::set_component(CORBA::ULong position, DynamicAny::DynAny_ptr value)
   throw(DynamicAny::DynAny::TypeMismatch, CORBA::SystemException)
{
   CORBA::TypeCode_var value_type = value->type();
   CORBA::TypeCode_var comp_type  = get_component_type(position);

   if (! comp_type->equivalent(value_type))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   CORBA::ULong next_component_index = _components.size();

   if (position > next_component_index)
   {
      throw CORBA::INTERNAL("Component position out of sequence");
   }

   if (position == next_component_index)
   {
      // create the component
      _components.push_back(DynamicAny::DynAny::_duplicate(value));
   }
   else
   {
      DynamicAny::DynAny_var dyn_component = _components[position];
      dyn_component->destroy();
      _components[_current] = DynamicAny::DynAny::_duplicate(value);
   }
}




void DynComposite::from_any(const CORBA::Any& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);
   }

   CORBA::TypeCode_var type = value.type();
   if (! _base_type->equivalent(type))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   reset();
   get_streams(value);
}




CORBA::Any* DynComposite::to_any()
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




void DynComposite::assign(DynamicAny::DynAny_ptr dyn_any)
   throw(DynamicAny::DynAny::TypeMismatch, CORBA::SystemException)
{
   pre_assign(dyn_any);

   if (_component_count == 0)
   {
      return;
   }

   long other_aux_current = _component_count - 1;

   while (dyn_any->next())
   {
      other_aux_current--;
   }

   dyn_any->rewind();

   try
   {
      for (CORBA::ULong i=0; i < _component_count; i++)
      {
         DynamicAny::DynAny_var dyn_curr = dyn_any->current_component();
         set_component(i, dyn_curr);
      }
   }
   catch(DynamicAny::DynAny::TypeMismatch& tm)
   {
      reset();
      dyn_any->seek(other_aux_current);
      throw;
   }

   _current = 0;
   dyn_any->seek(other_aux_current);
}




CORBA::Boolean DynComposite::equal(DynamicAny::DynAny_ptr dyn_any)
  throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (!(dyn_any))
   {
      throw CORBA::BAD_PARAM("Null DynAny reference");
   }

   CORBA::TypeCode_var type = dyn_any->type();
   if (! _base_type->equivalent(type))
   {
      return false;
   }

   if (_component_count != dyn_any->component_count())
   {
      return false;
   }

   if (_component_count == 0)
   {
      return true;
   }

   long aux_current = _current;

   long other_aux_current = _component_count - 1;

   try
   {
      while (dyn_any->next())
      {
         other_aux_current--;
      }

      dyn_any->rewind();

      for (CORBA::ULong i=0; i < _component_count; i++)
      {
         DynamicAny::DynAny_var dyn_curr = dyn_any->current_component();
         DynamicAny::DynAny_var dyn_comp = get_component(i);
         if (! dyn_comp->equal(dyn_curr))
         {
            dyn_any->seek(other_aux_current);
            return false;
         }

         dyn_any->next();
      }
   }
   catch(DynamicAny::DynAny::TypeMismatch& tm)
   {
      dyn_any->seek(other_aux_current);
      throw CORBA::INTERNAL(tm._name());
   }

   // Finally
   dyn_any->seek(other_aux_current);

   return true;
}




DynamicAny::AnySeq* DynComposite::get_elements()
  throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   DynamicAny::AnySeq* any_array = new DynamicAny::AnySeq(_component_count);
   if (_component_count > 0)
   {
      try
      {
         any_array->length(_component_count);
         for (CORBA::ULong i=0; i < _component_count; i++)
         {
            DynamicAny::DynAny_var dyn_component = get_component(i);
            CORBA::Any_var any = dyn_component->to_any();
            (*any_array)[i] = *any;
         }
      }
      catch(DynamicAny::DynAny::TypeMismatch& tm)
      {
         // unreachable
         throw CORBA::INTERNAL(tm._name());
      }
   }

   return any_array;
}




void DynComposite::set_elements(const DynamicAny::AnySeq& value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   reset();

   if (value.length() == 0)
   {
      return;
   }

   if (_component_count != value.length())
   {
      throw DynamicAny::DynAny::InvalidValue();
   }

   for (CORBA::ULong i=0; i < _component_count; i++)
   {
      set_component(i, value[i]);
   }
}




DynamicAny::DynAnySeq* DynComposite::get_elements_as_dyn_any()
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   DynamicAny::DynAnySeq* array = new DynamicAny::DynAnySeq(_component_count);

   if (_component_count > 0)
   {
      try
        {
         array->length(_component_count);
         for (long i=0; i < _component_count; i++)
         {
            DynamicAny::DynAny_var dyn_component = get_component(i);
            (*array)[i] = DynamicAny::DynAny::_duplicate(dyn_component);
         }
      }
      catch(DynamicAny::DynAny::TypeMismatch& tm)
      {
         // unreachable
         throw CORBA::INTERNAL(tm._name());
      }
   }

   return array;
}




void DynComposite::set_elements_as_dyn_any(const DynamicAny::DynAnySeq& value)
   throw(DynamicAny::DynAny::TypeMismatch,
         DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   reset();

   if (value.length() == 0)
   {
      return;
   }

   if (_component_count != value.length())
   {
      throw DynamicAny::DynAny::InvalidValue();
   }

   for (long i=0; i < _component_count; i++)
   {
      set_component(i, value[i]);
   }
}




DynamicAny::DynAny_ptr DynComposite::current_component()
   throw(DynamicAny::DynAny::TypeMismatch)
{
  DynamicAny::DynAny_ptr current_comp_ptr;
  try {
    current_comp_ptr = get_component(_current);
  } catch (const CORBA::SystemException& ex) {
    throw DynamicAny::DynAny::TypeMismatch();
  }
  return current_comp_ptr;
}




CORBA::ULong DynComposite::component_count()
  throw(CORBA::SystemException)
{
   return _component_count;
}




CORBA::Boolean DynComposite::seek(CORBA::Long index)
  throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (index < 0)
   {
      _current = -1;
      return false;
   }

   if (_component_count == 0)
   {
      return false;
   }

   if (_component_count <= index)
   {
      return false;
   }

   _current = 0;

   for (long i=0; i < index ; i++)
   {
      next();
   }

   return true;
}




void DynComposite::rewind()
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_component_count > 0)
   {
      seek(0);
   }
}




CORBA::Boolean DynComposite::next()
   throw(CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_component_count <= (_current + 1))
   {
      return false;
   }

   // if the current object has not been initialized, do it
   if (_current == _components.size())
   {
      try
      {
         DynamicAny::DynAny_var dyn_curr = current_component();
      }
      catch(DynamicAny::DynAny::TypeMismatch)
      {
         return false;
      }
   }

   // leaves the current component ready
   _current++;
   return true;
}




void DynComposite::insert_boolean(CORBA::Boolean value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_boolean))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   dyn_curr->insert_boolean(value);
}




void DynComposite::insert_octet(CORBA::Octet value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_octet))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   dyn_curr->insert_octet(value);
}




void DynComposite::insert_char(char value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); //"Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_char))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   dyn_curr->insert_char(value);
}




void DynComposite::insert_short(CORBA::Short value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_short))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   dyn_curr->insert_short(value);
}




void DynComposite::insert_ushort(CORBA::UShort value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_ushort))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   dyn_curr->insert_ushort(value);
}




void DynComposite::insert_long(CORBA::Long value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_long))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   dyn_curr->insert_long(value);
}




void DynComposite::insert_ulong(CORBA::ULong value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_ulong))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   dyn_curr->insert_ulong(value);
}




void DynComposite::insert_float(CORBA::Float value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_float))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   dyn_curr->insert_float(value);
}




void DynComposite::insert_double(CORBA::Double value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_double))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   dyn_curr->insert_double(value);
}




void DynComposite::insert_string(const char* value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_string))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   dyn_curr->insert_string(value);
}




void DynComposite::insert_wstring(const CORBA::WChar* value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_wstring))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   dyn_curr->insert_wstring(value);
}




void DynComposite::insert_reference(CORBA::Object_ptr value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_Object))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   dyn_curr->insert_reference(value);
}




void DynComposite::insert_typecode(CORBA::TypeCode_ptr value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_TypeCode))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   dyn_curr->insert_typecode(value);
}




void DynComposite::insert_longlong(CORBA::LongLong value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_longlong))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   dyn_curr->insert_longlong(value);
}




void DynComposite::insert_ulonglong(CORBA::ULongLong value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_ulonglong))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   dyn_curr->insert_ulonglong(value);
}




void DynComposite::insert_longdouble(CORBA::LongDouble value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_longdouble))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   dyn_curr->insert_longdouble(value);
}




void DynComposite::insert_wchar(CORBA::WChar value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_wchar))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   dyn_curr->insert_wchar(value);
}




void DynComposite::insert_any(const CORBA::Any& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_any))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   dyn_curr->insert_any(value);
}




void DynComposite::insert_dyn_any(DynamicAny::DynAny_ptr value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_any))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   dyn_curr->insert_dyn_any(value);
}




CORBA::Boolean DynComposite::get_boolean()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_boolean))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   return dyn_curr->get_boolean();
}




CORBA::Octet DynComposite::get_octet()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_octet))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   return dyn_curr->get_octet();
}




char DynComposite::get_char()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_char))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   return dyn_curr->get_char();
}




CORBA::Short DynComposite::get_short()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_short))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   return dyn_curr->get_short();
}




CORBA::UShort DynComposite::get_ushort()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_ushort))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   return dyn_curr->get_ushort();
}




CORBA::Long DynComposite::get_long()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_long))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   return dyn_curr->get_long();
}




CORBA::ULong DynComposite::get_ulong()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_ulong))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   return dyn_curr->get_ulong();
}




CORBA::Float DynComposite::get_float()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_float))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   return dyn_curr->get_float();
}




CORBA::Double DynComposite::get_double()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_double))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   return dyn_curr->get_double();
}




char* DynComposite::get_string()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_string))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   return dyn_curr->get_string();
}




CORBA::WChar* DynComposite::get_wstring()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_wstring))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   return dyn_curr->get_wstring();
}




CORBA::Object_ptr DynComposite::get_reference()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_Object))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   return dyn_curr->get_reference();
}




CORBA::TypeCode_ptr DynComposite::get_typecode()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_TypeCode))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   return dyn_curr->get_typecode();
}




CORBA::LongLong DynComposite::get_longlong()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_longlong))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   return dyn_curr->get_longlong();
}




CORBA::ULongLong DynComposite::get_ulonglong()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_ulonglong))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   return dyn_curr->get_ulonglong();
}




CORBA::LongDouble DynComposite::get_longdouble()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_longdouble))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   return dyn_curr->get_longdouble();
}





CORBA::WChar DynComposite::get_wchar()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_wchar))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   return dyn_curr->get_wchar();
}




CORBA::Any* DynComposite::get_any()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_any))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   return dyn_curr->get_any();
}




DynamicAny::DynAny_ptr DynComposite::get_dyn_any()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   CORBA::TypeCode_var type = get_component_type(_current);
   if (! type->equivalent(CORBA::_tc_any))
   {
      throw DynamicAny::DynAny::TypeMismatch();
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   return dyn_curr->get_dyn_any();
}




void DynComposite::insert_boolean_seq(const CORBA::BooleanSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   dyn_curr->insert_boolean_seq(value);
}




void DynComposite::insert_octet_seq(const CORBA::OctetSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   dyn_curr->insert_octet_seq(value);
}




void DynComposite::insert_char_seq(const CORBA::CharSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   dyn_curr->insert_char_seq(value);
}




void DynComposite::insert_short_seq(const CORBA::ShortSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   dyn_curr->insert_short_seq(value);
}




void DynComposite::insert_ushort_seq(const CORBA::UShortSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   dyn_curr->insert_ushort_seq(value);
}




void DynComposite::insert_long_seq(const CORBA::LongSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   dyn_curr->insert_long_seq(value);
}




void DynComposite::insert_ulong_seq(const CORBA::ULongSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   dyn_curr->insert_ulong_seq(value);
}




void DynComposite::insert_float_seq(const CORBA::FloatSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   dyn_curr->insert_float_seq(value);
}




void DynComposite::insert_double_seq(const CORBA::DoubleSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   dyn_curr->insert_double_seq(value);
}




void DynComposite::insert_longlong_seq(const CORBA::LongLongSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   dyn_curr->insert_longlong_seq(value);
}




void DynComposite::insert_ulonglong_seq(const CORBA::ULongLongSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   dyn_curr->insert_ulonglong_seq(value);
}




void DynComposite::insert_longdouble_seq(const CORBA::LongDoubleSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   dyn_curr->insert_longdouble_seq(value);
}



void DynComposite::insert_wchar_seq(const CORBA::WCharSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   dyn_curr->insert_wchar_seq(value);
}




CORBA::BooleanSeq* DynComposite::get_boolean_seq()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   return dyn_curr->get_boolean_seq();
}




CORBA::OctetSeq* DynComposite::get_octet_seq()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   return dyn_curr->get_octet_seq();
}




CORBA::CharSeq* DynComposite::get_char_seq()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   return dyn_curr->get_char_seq();
}




CORBA::ShortSeq* DynComposite::get_short_seq()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   return dyn_curr->get_short_seq();
}




CORBA::UShortSeq* DynComposite::get_ushort_seq()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   return dyn_curr->get_ushort_seq();
}




CORBA::LongSeq* DynComposite::get_long_seq()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   return dyn_curr->get_long_seq();
}




CORBA::ULongSeq* DynComposite::get_ulong_seq()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   return dyn_curr->get_ulong_seq();
}




CORBA::FloatSeq* DynComposite::get_float_seq()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   return dyn_curr->get_float_seq();
}




CORBA::DoubleSeq* DynComposite::get_double_seq()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   return dyn_curr->get_double_seq();
}




CORBA::LongLongSeq* DynComposite::get_longlong_seq()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   return dyn_curr->get_longlong_seq();
}




CORBA::ULongLongSeq* DynComposite::get_ulonglong_seq()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   return dyn_curr->get_ulonglong_seq();
}




CORBA::LongDoubleSeq* DynComposite::get_longdouble_seq()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   return dyn_curr->get_longdouble_seq();
}




CORBA::WCharSeq* DynComposite::get_wchar_seq()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (_destroyed)
   {
      throw CORBA::OBJECT_NOT_EXIST("DynAny destroyed", 0, CORBA::COMPLETED_NO);
   }

   if (_current == -1)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Current position is -1"
   }

   DynamicAny::DynAny_var dyn_curr = current_component();
   return dyn_curr->get_wchar_seq();
}




void DynComposite::insert_boolean_members(const CORBA::BooleanSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (value.length() != _component_count)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Invalid length"
   }

   reset();

   for (CORBA::ULong i=0; i < value.length(); i++)
   {
      DynamicAny::DynAny_var dyn_curr = current_component();
      dyn_curr->insert_boolean(value[i]);
      next();
   }

   rewind();
}




void DynComposite::insert_octet_members(const CORBA::OctetSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (value.length() != _component_count)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Invalid length"
   }

   reset();

   for (CORBA::ULong i=0; i < value.length(); i++)
   {
      DynamicAny::DynAny_var dyn_curr = current_component();
      dyn_curr->insert_octet(value[i]);
      next();
   }

   rewind();
}




void DynComposite::insert_char_members(const CORBA::CharSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (value.length() != _component_count)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Invalid length"
   }

   reset();

   for (CORBA::ULong i=0; i < value.length(); i++)
   {
      DynamicAny::DynAny_var dyn_curr = current_component();
      dyn_curr->insert_char(value[i]);
      next();
   }

   rewind();
}




void DynComposite::insert_short_members(const CORBA::ShortSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (value.length() != _component_count)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Invalid length"
   }

   reset();

   for (CORBA::ULong i=0; i < value.length(); i++)
   {
      DynamicAny::DynAny_var dyn_curr = current_component();
      dyn_curr->insert_short(value[i]);
      next();
   }

   rewind();
}




void DynComposite::insert_ushort_members(const CORBA::UShortSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (value.length() != _component_count)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Invalid length"
   }

   reset();

   for (CORBA::ULong i=0; i < value.length(); i++)
   {
      DynamicAny::DynAny_var dyn_curr = current_component();
      dyn_curr->insert_ushort(value[i]);
      next();
   }

   rewind();
}




void DynComposite::insert_long_members(const CORBA::LongSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (value.length() != _component_count)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Invalid length"
   }

   reset();

   for (CORBA::ULong i=0; i < value.length(); i++)
   {
      DynamicAny::DynAny_var dyn_curr = current_component();
      dyn_curr->insert_long(value[i]);
      next();
   }

   rewind();
}




void DynComposite::insert_ulong_members(const CORBA::ULongSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (value.length() != _component_count)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Invalid length"
   }

   reset();

   for (CORBA::ULong i=0; i < value.length(); i++)
   {
      DynamicAny::DynAny_var dyn_curr = current_component();
      dyn_curr->insert_ulong(value[i]);
      next();
   }

   rewind();
}




void DynComposite::insert_float_members(const CORBA::FloatSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (value.length() != _component_count)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Invalid length"
   }

   reset();

   for (CORBA::ULong i=0; i < value.length(); i++)
   {
      DynamicAny::DynAny_var dyn_curr = current_component();
      dyn_curr->insert_float(value[i]);
      next();
   }

   rewind();
}




void DynComposite::insert_double_members(const CORBA::DoubleSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (value.length() != _component_count)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Invalid length"
   }

   reset();

   for (CORBA::ULong i=0; i < value.length(); i++)
   {
      DynamicAny::DynAny_var dyn_curr = current_component();
      dyn_curr->insert_double(value[i]);
      next();
   }

   rewind();
}




void DynComposite::insert_longlong_members(const CORBA::LongLongSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (value.length() != _component_count)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Invalid length"
   }

   reset();

   for (CORBA::ULong i=0; i < value.length(); i++)
   {
      DynamicAny::DynAny_var dyn_curr = current_component();
      dyn_curr->insert_longlong(value[i]);
      next();
   }

   rewind();
}




void DynComposite::insert_ulonglong_members(const CORBA::ULongLongSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (value.length() != _component_count)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Invalid length"
   }

   reset();

   for (CORBA::ULong i=0; i < value.length(); i++)
   {
      DynamicAny::DynAny_var dyn_curr = current_component();
      dyn_curr->insert_ulonglong(value[i]);
      next();
   }

   rewind();
}




void DynComposite::insert_longdouble_members(const CORBA::LongDoubleSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (value.length() != _component_count)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Invalid length"
   }

   reset();

   for (CORBA::ULong i=0; i < value.length(); i++)
   {
      DynamicAny::DynAny_var dyn_curr = current_component();
      dyn_curr->insert_longdouble(value[i]);
      next();
   }

   rewind();
}




void DynComposite::insert_wchar_members(const CORBA::WCharSeq& value)
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   if (value.length() != _component_count)
   {
      throw DynamicAny::DynAny::InvalidValue(); // "Invalid length"
   }

   reset();

   for (CORBA::ULong i=0; i < value.length(); i++)
   {
      DynamicAny::DynAny_var dyn_curr = current_component();
      dyn_curr->insert_wchar(value[i]);
      next();
   }

   rewind();
}




CORBA::BooleanSeq* DynComposite::get_boolean_members()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   CORBA::BooleanSeq* value = new CORBA::BooleanSeq(_component_count);

   if (_component_count > 0)
   {
      value->length(_component_count);
      for (CORBA::ULong i=0; i < _component_count; i++)
      {
         DynamicAny::DynAny_var dyn_component = get_component(i);
         value[i] = dyn_component->get_boolean();
      }
   }

   return value;
}




CORBA::OctetSeq* DynComposite::get_octet_members()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   CORBA::OctetSeq* value = new CORBA::OctetSeq(_component_count);

   if (_component_count > 0)
   {
      value->length(_component_count);
      for (CORBA::ULong i=0; i < _component_count; i++)
      {
         DynamicAny::DynAny_var dyn_component = get_component(i);
         value[i] = dyn_component->get_octet();
      }
   }

   return value;
}




CORBA::CharSeq* DynComposite::get_char_members()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   CORBA::CharSeq* value = new CORBA::CharSeq(_component_count);

   if (_component_count > 0)
   {
      value->length(_component_count);
      for (CORBA::ULong i=0; i < _component_count; i++)
      {
         DynamicAny::DynAny_var dyn_component = get_component(i);
         value[i] = dyn_component->get_char();
      }
   }

   return value;
}




CORBA::ShortSeq* DynComposite::get_short_members()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   CORBA::ShortSeq* value = new CORBA::ShortSeq(_component_count);

   if (_component_count > 0)
   {
      value->length(_component_count);
      for (CORBA::ULong i=0; i < _component_count; i++)
      {
         DynamicAny::DynAny_var dyn_component = get_component(i);
         value[i] = dyn_component->get_short();
      }
   }

   return value;
}




CORBA::UShortSeq* DynComposite::get_ushort_members()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   CORBA::UShortSeq* value = new CORBA::UShortSeq(_component_count);

   if (_component_count > 0)
   {
      value->length(_component_count);
      for (CORBA::ULong i=0; i < _component_count; i++)
      {
         DynamicAny::DynAny_var dyn_component = get_component(i);
         value[i] = dyn_component->get_ushort();
      }
   }

   return value;
}




CORBA::LongSeq* DynComposite::get_long_members()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   CORBA::LongSeq* value = new CORBA::LongSeq(_component_count);

   if (_component_count > 0)
   {
      value->length(_component_count);
      for (CORBA::ULong i=0; i < _component_count; i++)
      {
         DynamicAny::DynAny_var dyn_component = get_component(i);
         value[i] = dyn_component->get_long();
      }
   }

   return value;
}




CORBA::ULongSeq* DynComposite::get_ulong_members()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   CORBA::ULongSeq* value = new CORBA::ULongSeq(_component_count);

   if (_component_count > 0)
   {
      value->length(_component_count);
      for (CORBA::ULong i=0; i < _component_count; i++)
      {
         DynamicAny::DynAny_var dyn_component = get_component(i);
         value[i] = dyn_component->get_ulong();
      }
   }

   return value;
}




CORBA::FloatSeq* DynComposite::get_float_members()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   CORBA::FloatSeq* value = new CORBA::FloatSeq(_component_count);

   if (_component_count > 0)
   {
      value->length(_component_count);
      for (CORBA::ULong i=0; i < _component_count; i++)
      {
         DynamicAny::DynAny_var dyn_component = get_component(i);
         (*value)[i] = dyn_component->get_float();
      }
   }

   return value;
}




CORBA::DoubleSeq* DynComposite::get_double_members()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   CORBA::DoubleSeq* value = new CORBA::DoubleSeq(_component_count);

   if (_component_count > 0)
   {
      value->length(_component_count);
      for (CORBA::ULong i=0; i < _component_count; i++)
      {
         DynamicAny::DynAny_var dyn_component = get_component(i);
         (*value)[i] = dyn_component->get_double();
      }
   }

   return value;
}




CORBA::LongLongSeq* DynComposite::get_longlong_members()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   CORBA::LongLongSeq* value = new CORBA::LongLongSeq(_component_count);

   if (_component_count > 0)
   {
      value->length(_component_count);
      for (CORBA::ULong i=0; i < _component_count; i++)
      {
         DynamicAny::DynAny_var dyn_component = get_component(i);
         value[i] = dyn_component->get_longlong();
      }
   }

   return value;
}




CORBA::ULongLongSeq* DynComposite::get_ulonglong_members()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   CORBA::ULongLongSeq* value = new CORBA::ULongLongSeq(_component_count);

   if (_component_count > 0)
   {
      value->length(_component_count); 
      for (CORBA::ULong i=0; i < _component_count; i++)
      {
         DynamicAny::DynAny_var dyn_component = get_component(i);
         value[i] = dyn_component->get_ulonglong();
      }
   }

   return value;
}




CORBA::LongDoubleSeq* DynComposite::get_longdouble_members()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   CORBA::LongDoubleSeq* value = new CORBA::LongDoubleSeq(_component_count);

   if (_component_count > 0)
   {
      value->length(_component_count); 
      for (CORBA::ULong i=0; i < _component_count; i++)
      {
         DynamicAny::DynAny_var dyn_component = get_component(i);
         value[i] = dyn_component->get_longdouble();
      }
   }

   return value;
}




CORBA::WCharSeq* DynComposite::get_wchar_members()
   throw(DynamicAny::DynAny::TypeMismatch, DynamicAny::DynAny::InvalidValue, CORBA::SystemException)
{
   CORBA::WCharSeq* value = new CORBA::WCharSeq(_component_count);

   if (_component_count > 0)
   {
      value->length(_component_count);
      for (CORBA::ULong i=0; i < _component_count; i++)
      {
         DynamicAny::DynAny_var dyn_component = get_component(i);
         value[i] = dyn_component->get_wchar();
      }
   }

   return value;
}




} //namespace dynAny
} //namespace TIDorb
