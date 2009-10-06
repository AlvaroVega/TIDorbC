#include "CORBA.h"


#ifndef _CORBA_TYPECODE_H_
#define _CORBA_TYPECODE_H_


class TypeCode
{

  public:

    class Bounds;
    typedef Bounds* Bounds_ptr;
    typedef TIDorb::templates::InterfaceT_var<Bounds> Bounds_var;
    typedef TIDorb::templates::InterfaceT_out<Bounds> Bounds_out;
    
    class BadKind;
    typedef BadKind* BadKind_ptr;
    typedef TIDorb::templates::InterfaceT_var<BadKind> BadKind_var;
    typedef TIDorb::templates::InterfaceT_out<BadKind> BadKind_out;
    
    #include "CORBA/TypeCode/Bounds.h"
    #include "CORBA/TypeCode/BadKind.h"   
    
    virtual ~TypeCode() {}
      
    virtual TCKind kind() const = 0;
    virtual Boolean equal(TypeCode_ptr) const = 0;
    virtual Boolean equivalent(TypeCode_ptr) const = 0;
    virtual TypeCode_ptr get_compact_typecode() const = 0;
    virtual const char* id() const = 0;
    virtual const char* name() const = 0;
    virtual ULong member_count() const = 0;
    virtual const char* member_name(ULong index) const = 0;
    virtual TypeCode_ptr member_type(ULong index) const = 0;
    virtual Any *member_label(ULong index) const = 0;
    virtual TypeCode_ptr discriminator_type() const = 0;
    virtual Long default_index() const = 0;
    virtual ULong length() const = 0;
    virtual TypeCode_ptr content_type() const = 0;
    virtual UShort fixed_digits() const = 0;
    virtual Short fixed_scale() const = 0;
    virtual Visibility member_visibility(ULong index) const = 0;
    virtual ValueModifier type_modifier() const = 0;
    virtual TypeCode_ptr concrete_base_type() const = 0;
    
    static TypeCode_ptr _duplicate(TypeCode_ptr tc);
    static TypeCode_ptr _nil();
  
  protected:
    TypeCode() {}
//MLG
  public:
    virtual void* _impl() = 0;
    virtual const char* _typeid() = 0;
//EMLG
};

extern const TypeCode_ptr _tc_TypeCode;
extern const TypeCode_ptr _tc_null;
extern const TypeCode_ptr _tc_void;
extern const TypeCode_ptr _tc_octet;
extern const TypeCode_ptr _tc_boolean;
extern const TypeCode_ptr _tc_char;
extern const TypeCode_ptr _tc_wchar;
extern const TypeCode_ptr _tc_string;
extern const TypeCode_ptr _tc_wstring;
extern const TypeCode_ptr _tc_short;
extern const TypeCode_ptr _tc_ushort;
extern const TypeCode_ptr _tc_long;
extern const TypeCode_ptr _tc_ulong;
extern const TypeCode_ptr _tc_longlong;
extern const TypeCode_ptr _tc_ulonglong;
extern const TypeCode_ptr _tc_float;
extern const TypeCode_ptr _tc_double;
extern const TypeCode_ptr _tc_longdouble;
extern const TypeCode_ptr _tc_any;
extern const TypeCode_ptr _tc_principal;
extern const TypeCode_ptr _tc_Object;
extern const TypeCode_ptr _tc_Value;


class _TypeCodeHelper{

		public:
		static ::CORBA::TypeCode_ptr type();

		static const char* id();

		static void insert(::CORBA::Any& any, CORBA::TypeCode_ptr _value);

		static void extract(const ::CORBA::Any& any, CORBA::TypeCode_ptr& _value);

		static void read(::TIDorb::portable::InputStream& is, CORBA::TypeCode_ptr& val);

		static void write(::TIDorb::portable::OutputStream& os, const CORBA::TypeCode_ptr val);
		
};// End of helper definition

class _TypeCodeHolder: public virtual ::TIDorb::portable::Streamable {

		public:
		CORBA::TypeCode_ptr value; 

		_TypeCodeHolder();
		//_TypeCodeHolder(CORBA::TypeCode_ptr initial);
		_TypeCodeHolder(const CORBA::TypeCode_ptr initial);

		::CORBA::TypeCode_ptr _type() const;
		void _write(::TIDorb::portable::OutputStream& outs) const;		
		void _read(::TIDorb::portable::InputStream& ins);
		
}; // end of holder class 


#endif
