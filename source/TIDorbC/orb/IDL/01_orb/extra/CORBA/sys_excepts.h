#include "CORBA.h"


#ifndef _TIDORB_SYS_EXCEPTS_H_
#define _TIDORB_SYS_EXCEPTS_H_ 


// the unknown exception

extern const ::CORBA::TypeCode_ptr _tc_UNKNOWN;

class UNKNOWN
  : public virtual ::CORBA::SystemException
{
   public:
    UNKNOWN(const UNKNOWN& v): Exception(v), SystemException(v) {}
    UNKNOWN(const char* reason = NULL)
      : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_UNKNOWN){}
    UNKNOWN(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_UNKNOWN) {} 
    UNKNOWN(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_UNKNOWN) {} 
    
    //~UNKNOWN() {}  
    
    void _raise() const
    {
      throw UNKNOWN(*this);
    }
    
    const char* _name() const
    {
      return "UNKNOWN";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/UNKNOWN:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new UNKNOWN(*this);
    }
    
    UNKNOWN& operator= (const UNKNOWN& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static UNKNOWN* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<UNKNOWN* > (e);
    }
    
    static const UNKNOWN* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<UNKNOWN* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  UNKNOWN
  

extern const ::CORBA::TypeCode_ptr _tc_BAD_PARAM;

class BAD_PARAM
  : public virtual ::CORBA::SystemException
{
   public:
    BAD_PARAM(const BAD_PARAM& v): Exception(v), SystemException(v) {} 	
    BAD_PARAM(const char* reason = NULL): 
      ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_BAD_PARAM){}
    BAD_PARAM(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_BAD_PARAM) {} 
    BAD_PARAM(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_BAD_PARAM) {} 
    
    //~BAD_PARAM() {}  
    
    void _raise() const
    {
      throw BAD_PARAM(*this);
    }
    
    const char* _name() const
    {
      return "BAD_PARAM";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/BAD_PARAM:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new BAD_PARAM(*this);
    }
    
    BAD_PARAM& operator= (const BAD_PARAM& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static BAD_PARAM* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<BAD_PARAM* > (e);
    }
    
    static const BAD_PARAM* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<BAD_PARAM* > (const_cast< ::CORBA::Exception* > (e));
    }
}; // class BAD_PARAM


extern const ::CORBA::TypeCode_ptr _tc_NO_MEMORY;

// dynamic memory allocation failure
class NO_MEMORY
  : public virtual ::CORBA::SystemException
{
   public:
    NO_MEMORY(const NO_MEMORY& v): Exception(v), SystemException(v) {}
    NO_MEMORY(const char* reason = NULL)
      : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_NO_MEMORY){}
    NO_MEMORY(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_NO_MEMORY) {} 
    NO_MEMORY(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_NO_MEMORY) {} 
    
    //~NO_MEMORY() {}  
    
    void _raise() const
    {
      throw NO_MEMORY(*this);
    }
    
    const char* _name() const
    {
      return "NO_MEMORY";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/NO_MEMORY:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new NO_MEMORY(*this);
    }
    
    NO_MEMORY& operator= (const NO_MEMORY& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
		static NO_MEMORY* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<NO_MEMORY* > (e);
    }
    
    static const NO_MEMORY* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<NO_MEMORY* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  NO_MEMORY

extern const ::CORBA::TypeCode_ptr _tc_IMP_LIMIT;

// violated implementation limit
class IMP_LIMIT
  : public virtual ::CORBA::SystemException
{
   public:
    IMP_LIMIT(const IMP_LIMIT& v): Exception(v), SystemException(v) {}
    IMP_LIMIT(const char* reason = NULL)
      : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_IMP_LIMIT){}
    IMP_LIMIT(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_IMP_LIMIT) {} 
    IMP_LIMIT(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_IMP_LIMIT) {} 
    
    //~IMP_LIMIT() {}  
    
    void _raise() const
    {
      throw IMP_LIMIT(*this);
    }
    
    const char* _name() const
    {
      return "IMP_LIMIT";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/IMP_LIMIT:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new IMP_LIMIT(*this);
    }
    
    IMP_LIMIT& operator= (const IMP_LIMIT& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static IMP_LIMIT* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<IMP_LIMIT* > (e);
    }
    
    static const IMP_LIMIT* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<IMP_LIMIT* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  IMP_LIMIT

extern const ::CORBA::TypeCode_ptr _tc_COMM_FAILURE;

// communication failure
class COMM_FAILURE
  : public virtual ::CORBA::SystemException
{
   public:
    COMM_FAILURE(const COMM_FAILURE& v): Exception(v), SystemException(v) {}	
    COMM_FAILURE(const char* reason = NULL)
      : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_COMM_FAILURE){}
    COMM_FAILURE(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_COMM_FAILURE) {} 
    COMM_FAILURE(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_COMM_FAILURE) {} 
    
    //~COMM_FAILURE() {}  
    
    void _raise() const
    {
      throw COMM_FAILURE(*this);
    }
    
    const char* _name() const
    {
      return "COMM_FAILURE";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/COMM_FAILURE:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new COMM_FAILURE(*this);
    }
    
    COMM_FAILURE& operator= (const COMM_FAILURE& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static COMM_FAILURE* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<COMM_FAILURE* > (e);
    }
    
    static const COMM_FAILURE* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<COMM_FAILURE* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  COMM_FAILURE

extern const ::CORBA::TypeCode_ptr _tc_INV_OBJREF;

// invalid object reference
class INV_OBJREF
  : public virtual ::CORBA::SystemException
{
   public:
    INV_OBJREF(const INV_OBJREF& v): Exception(v), SystemException(v) {}
    INV_OBJREF(const char* reason = NULL)
      : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_INV_OBJREF){}
    INV_OBJREF(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_INV_OBJREF) {} 
    INV_OBJREF(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_INV_OBJREF) {} 
    
    //~INV_OBJREF() {}  
    
    void _raise() const
    {
      throw INV_OBJREF(*this);
    }
    
    const char* _name() const
    {
      return "INV_OBJREF";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/INV_OBJREF:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new INV_OBJREF(*this);
    }
    
    INV_OBJREF& operator= (const INV_OBJREF& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static INV_OBJREF* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<INV_OBJREF* > (e);
    }
    
    static const INV_OBJREF* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<INV_OBJREF* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  INV_OBJREF

extern const ::CORBA::TypeCode_ptr _tc_NO_PERMISSION;

// no permission for attempted op.
class NO_PERMISSION
  : public virtual ::CORBA::SystemException
{
   public:
    NO_PERMISSION(const NO_PERMISSION& v): Exception(v), SystemException(v) {}		
    NO_PERMISSION(const char* reason = NULL)
      : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_NO_PERMISSION){}
    NO_PERMISSION(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_NO_PERMISSION) {} 
    NO_PERMISSION(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_NO_PERMISSION) {} 
    
    //~NO_PERMISSION() {}  
    
    void _raise() const
    {
      throw NO_PERMISSION(*this);
    }
    
    const char* _name() const
    {
      return "NO_PERMISSION";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/NO_PERMISSION:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new NO_PERMISSION(*this);
    }
    
    NO_PERMISSION& operator= (const NO_PERMISSION& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static NO_PERMISSION* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<NO_PERMISSION* > (e);
    }
    
    static const NO_PERMISSION* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<NO_PERMISSION* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  NO_PERMISSION

extern const ::CORBA::TypeCode_ptr _tc_INTERNAL;

// ORB internal error
class INTERNAL
  : public virtual ::CORBA::SystemException
{
   public:
    INTERNAL(const INTERNAL& v): Exception(v), SystemException(v) {}		
    INTERNAL(const char* reason = NULL)
      : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_INTERNAL){}
    INTERNAL(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_INTERNAL) {} 
    INTERNAL(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_INTERNAL) {} 
    
    //~INTERNAL() {}  
    
    void _raise() const
    {
      throw INTERNAL(*this);
    }
    
    const char* _name() const
    {
      return "INTERNAL";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/INTERNAL:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new INTERNAL(*this);
    }
    
    INTERNAL& operator= (const INTERNAL& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static INTERNAL* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<INTERNAL* > (e);
    }
    
    static const INTERNAL* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<INTERNAL* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  INTERNAL

extern const ::CORBA::TypeCode_ptr _tc_MARSHAL;

// error marshaling param/result
class MARSHAL
  : public virtual ::CORBA::SystemException
{
   public:
    MARSHAL(const MARSHAL& v): Exception(v), SystemException(v) {}		
    MARSHAL(const char* reason = NULL)
      : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_MARSHAL){}
    MARSHAL(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_MARSHAL) {} 
    MARSHAL(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_MARSHAL) {} 
    
    //~MARSHAL() {}  
    
    void _raise() const
    {
      throw MARSHAL(*this);
    }
    
    const char* _name() const
    {
      return "MARSHAL";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/MARSHAL:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new MARSHAL(*this);
    }
    
    MARSHAL& operator= (const MARSHAL& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static MARSHAL* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<MARSHAL* > (e);
    }
    
    static const MARSHAL* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<MARSHAL* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  MARSHAL

extern const ::CORBA::TypeCode_ptr _tc_INITIALIZE;

// ORB initialization failure
class INITIALIZE
  : public virtual ::CORBA::SystemException
{
   public:
    INITIALIZE(const INITIALIZE& v): Exception(v), SystemException(v) {}		
    INITIALIZE(const char* reason = NULL)
      : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_INITIALIZE){}
    INITIALIZE(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_INITIALIZE) {} 
    INITIALIZE(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_INITIALIZE) {} 

    //~INITIALIZE() {}  
    
    void _raise() const
    {
      throw INITIALIZE(*this);
    }
    
    const char* _name() const
    {
      return "INITIALIZE";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/INITIALIZE:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new INITIALIZE(*this);
    }
    
    INITIALIZE& operator= (const INITIALIZE& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static INITIALIZE* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<INITIALIZE* > (e);
    }
    
    static const INITIALIZE* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<INITIALIZE* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  INITIALIZE

extern const ::CORBA::TypeCode_ptr _tc_NO_IMPLEMENT;

// operation implementation unavailable
class NO_IMPLEMENT
  : public virtual ::CORBA::SystemException
{
   public:
    NO_IMPLEMENT(const NO_IMPLEMENT& v): Exception(v), SystemException(v) {}		
    NO_IMPLEMENT(const char* reason = NULL)
      : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_NO_IMPLEMENT){}
    NO_IMPLEMENT(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_NO_IMPLEMENT) {} 
    NO_IMPLEMENT(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_NO_IMPLEMENT) {} 

    //~NO_IMPLEMENT() {}  
    
    void _raise() const
    {
      throw NO_IMPLEMENT(*this);
    }
    
    const char* _name() const
    {
      return "NO_IMPLEMENT";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/NO_IMPLEMENT:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new NO_IMPLEMENT(*this);
    }
    
    NO_IMPLEMENT& operator= (const NO_IMPLEMENT& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static NO_IMPLEMENT* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<NO_IMPLEMENT* > (e);
    }
    
    static const NO_IMPLEMENT* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<NO_IMPLEMENT* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  NO_IMPLEMENT

extern const ::CORBA::TypeCode_ptr _tc_BAD_TYPECODE;

// bad typecode
class BAD_TYPECODE
  : public virtual ::CORBA::SystemException
{
   public:
    BAD_TYPECODE(const BAD_TYPECODE& v): Exception(v), SystemException(v) {}		
    BAD_TYPECODE(const char* reason = NULL)
      : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_BAD_TYPECODE){}
    BAD_TYPECODE(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_BAD_TYPECODE) {} 
    BAD_TYPECODE(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_BAD_TYPECODE) {} 
    
    //~BAD_TYPECODE() {}  
    
    void _raise() const
    {
      throw BAD_TYPECODE(*this);
    }
    
    const char* _name() const
    {
      return "BAD_TYPECODE";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/BAD_TYPECODE:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new BAD_TYPECODE(*this);
    }
    
    BAD_TYPECODE& operator= (const BAD_TYPECODE& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static BAD_TYPECODE* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<BAD_TYPECODE* > (e);
    }
    
    static const BAD_TYPECODE* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<BAD_TYPECODE* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  BAD_TYPECODE

extern const ::CORBA::TypeCode_ptr _tc_BAD_OPERATION;

// invalid operation
class BAD_OPERATION
  : public virtual ::CORBA::SystemException
{
   public:
    BAD_OPERATION(const BAD_OPERATION& v): Exception(v), SystemException(v) {}		
    BAD_OPERATION(const char* reason = NULL)
      : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_BAD_OPERATION){}
    BAD_OPERATION(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_BAD_OPERATION) {} 
    BAD_OPERATION(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_BAD_OPERATION) {} 
    
    //~BAD_OPERATION() {}  
    
    void _raise() const
    {
      throw BAD_OPERATION(*this);
    }
    
    const char* _name() const
    {
      return "BAD_OPERATION";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/BAD_OPERATION:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new BAD_OPERATION(*this);
    }
    
    BAD_OPERATION& operator= (const BAD_OPERATION& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static BAD_OPERATION* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<BAD_OPERATION* > (e);
    }
    
    static const BAD_OPERATION* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<BAD_OPERATION* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  BAD_OPERATION

extern const ::CORBA::TypeCode_ptr _tc_NO_RESOURCES;

// insufficient resources for req.
class NO_RESOURCES
  : public virtual ::CORBA::SystemException
{
   public:
    NO_RESOURCES(const NO_RESOURCES& v): Exception(v), SystemException(v) {}		
    NO_RESOURCES(const char* reason = NULL)
      : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_NO_RESOURCES){}
    NO_RESOURCES(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_NO_RESOURCES) {} 
    NO_RESOURCES(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_NO_RESOURCES) {} 
    
    //~NO_RESOURCES() {}  
    
    void _raise() const
    {
      throw NO_RESOURCES(*this);
    }
    
    const char* _name() const
    {
      return "NO_RESOURCES";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/NO_RESOURCES:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new NO_RESOURCES(*this);
    }
    
    NO_RESOURCES& operator= (const NO_RESOURCES& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static NO_RESOURCES* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<NO_RESOURCES* > (e);
    }
    
    static const NO_RESOURCES* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<NO_RESOURCES* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  NO_RESOURCES

extern const ::CORBA::TypeCode_ptr _tc_NO_RESPONSE;

// response to req. not yet available
class NO_RESPONSE
  : public virtual ::CORBA::SystemException
{
   public:
    NO_RESPONSE(const NO_RESPONSE& v): Exception(v), SystemException(v) {}		
    NO_RESPONSE(const char* reason = NULL)
      : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_NO_RESPONSE){}
    NO_RESPONSE(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_NO_RESPONSE) {} 
    NO_RESPONSE(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_NO_RESPONSE) {} 
    
    //~NO_RESPONSE() {}  
    
    void _raise() const
    {
      throw NO_RESPONSE(*this);
    }
    
    const char* _name() const
    {
      return "NO_RESPONSE";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/NO_RESPONSE:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new NO_RESPONSE(*this);
    }
    
    NO_RESPONSE& operator= (const NO_RESPONSE& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static NO_RESPONSE* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<NO_RESPONSE* > (e);
    }
    
    static const NO_RESPONSE* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<NO_RESPONSE* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  NO_RESPONSE

extern const ::CORBA::TypeCode_ptr _tc_PERSIST_STORE;

// persistent storage failure
class PERSIST_STORE
  : public virtual ::CORBA::SystemException
{
   public:
    PERSIST_STORE(const PERSIST_STORE& v): Exception(v), SystemException(v) {}		
    PERSIST_STORE(const char* reason = NULL)
      : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_PERSIST_STORE){}
    PERSIST_STORE(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_PERSIST_STORE) {} 
    PERSIST_STORE(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_PERSIST_STORE) {} 
    
    //~PERSIST_STORE() {}  
    
    void _raise() const
    {
      throw PERSIST_STORE(*this);
    }
    
    const char* _name() const
    {
      return "PERSIST_STORE";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/PERSIST_STORE:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new PERSIST_STORE(*this);
    }
    
    PERSIST_STORE& operator= (const PERSIST_STORE& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static PERSIST_STORE* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<PERSIST_STORE* > (e);
    }
    
    static const PERSIST_STORE* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<PERSIST_STORE* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  PERSIST_STORE

extern const ::CORBA::TypeCode_ptr _tc_BAD_INV_ORDER;

// routine invocations out of order
class BAD_INV_ORDER
  : public virtual ::CORBA::SystemException
{
   public:
    BAD_INV_ORDER(const BAD_INV_ORDER& v): Exception(v), SystemException(v) {}		
    BAD_INV_ORDER(const char* reason = NULL)
      : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_BAD_INV_ORDER){}
    BAD_INV_ORDER(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_BAD_INV_ORDER) {} 
    BAD_INV_ORDER(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_BAD_INV_ORDER) {} 
    
    //~BAD_INV_ORDER() {}  
    
    void _raise() const
    {
      throw BAD_INV_ORDER(*this);
    }
    
    const char* _name() const
    {
      return "BAD_INV_ORDER";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/BAD_INV_ORDER:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new BAD_INV_ORDER(*this);
    }
    
    BAD_INV_ORDER& operator= (const BAD_INV_ORDER& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static BAD_INV_ORDER* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<BAD_INV_ORDER* > (e);
    }
    
    static const BAD_INV_ORDER* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<BAD_INV_ORDER* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  BAD_INV_ORDER

extern const ::CORBA::TypeCode_ptr _tc_TRANSIENT;

// transient failure - reissue request
class TRANSIENT
  : public virtual ::CORBA::SystemException
{
   public:
    TRANSIENT(const TRANSIENT& v): Exception(v), SystemException(v) {}		
    TRANSIENT(const char* reason = NULL)
      : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_TRANSIENT){}
    TRANSIENT(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_TRANSIENT) {} 
    TRANSIENT(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_TRANSIENT) {} 
    
    //~TRANSIENT() {}  
    
    void _raise() const
    {
      throw TRANSIENT(*this);
    }
    
    const char* _name() const
    {
      return "TRANSIENT";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/TRANSIENT:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new TRANSIENT(*this);
    }
    
    TRANSIENT& operator= (const TRANSIENT& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static TRANSIENT* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<TRANSIENT* > (e);
    }
    
    static const TRANSIENT* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<TRANSIENT* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  TRANSIENT

extern const ::CORBA::TypeCode_ptr _tc_FREE_MEM;

// cannot free memory
class FREE_MEM
  : public virtual ::CORBA::SystemException
{
   public:
    FREE_MEM(const FREE_MEM& v): Exception(v), SystemException(v) {}		
    FREE_MEM(const char* reason = NULL)
      : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_FREE_MEM){}
    FREE_MEM(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_FREE_MEM) {} 
    FREE_MEM(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_FREE_MEM) {}  
    
    //~FREE_MEM() {}  
    
    void _raise() const
    {
      throw FREE_MEM(*this);
    }
    
    const char* _name() const
    {
      return "FREE_MEM";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/FREE_MEM:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new FREE_MEM(*this);
    }
    
    FREE_MEM& operator= (const FREE_MEM& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static FREE_MEM* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<FREE_MEM* > (e);
    }
    
    static const FREE_MEM* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<FREE_MEM* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  FREE_MEM

extern const ::CORBA::TypeCode_ptr _tc_INV_IDENT;

// invalid identifier syntax
class INV_IDENT
  : public virtual ::CORBA::SystemException
{
   public:
    INV_IDENT(const INV_IDENT& v): Exception(v), SystemException(v) {}		
    INV_IDENT(const char* reason = NULL)
      : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_INV_IDENT){}
    INV_IDENT(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_INV_IDENT) {} 
    INV_IDENT(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_INV_IDENT) {} 
    
    //~INV_IDENT() {}  
    
    void _raise() const
    {
      throw INV_IDENT(*this);
    }
    
    const char* _name() const
    {
      return "INV_IDENT";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/INV_IDENT:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new INV_IDENT(*this);
    }
    
    INV_IDENT& operator= (const INV_IDENT& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static INV_IDENT* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<INV_IDENT* > (e);
    }
    
    static const INV_IDENT* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<INV_IDENT* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  INV_IDENT

extern const ::CORBA::TypeCode_ptr _tc_INV_FLAG;

// invalid flag was specified
class INV_FLAG
  : public virtual ::CORBA::SystemException
{
   public:
    INV_FLAG(const INV_FLAG& v): Exception(v), SystemException(v) {}		
    INV_FLAG(const char* reason = NULL)
      : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_INV_FLAG){}
    INV_FLAG(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_INV_FLAG) {} 
    INV_FLAG(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_INV_FLAG) {} 
    
    //~INV_FLAG() {}  
    
    void _raise() const
    {
      throw INV_FLAG(*this);
    }
    
    const char* _name() const
    {
      return "INV_FLAG";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/INV_FLAG:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new INV_FLAG(*this);
    }
    
    INV_FLAG& operator= (const INV_FLAG& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static INV_FLAG* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<INV_FLAG* > (e);
    }
    
    static const INV_FLAG* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<INV_FLAG* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  INV_FLAG

extern const ::CORBA::TypeCode_ptr _tc_INTF_REPOS;

// error accessing interface repository
class INTF_REPOS
  : public virtual ::CORBA::SystemException
{
   public:
    INTF_REPOS(const INTF_REPOS& v): Exception(v), SystemException(v) {}		
    INTF_REPOS(const char* reason = NULL)
      : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_INTF_REPOS){}
    INTF_REPOS(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_INTF_REPOS) {} 
    INTF_REPOS(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_INTF_REPOS) {} 
    
    //~INTF_REPOS() {}  
    
    void _raise() const
    {
      throw INTF_REPOS(*this);
    }
    
    const char* _name() const
    {
      return "INTF_REPOS";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/INTF_REPOS:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new INTF_REPOS(*this);
    }
    
    INTF_REPOS& operator= (const INTF_REPOS& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static INTF_REPOS* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<INTF_REPOS* > (e);
    }
    
    static const INTF_REPOS* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<INTF_REPOS* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  INTF_REPOS

extern const ::CORBA::TypeCode_ptr _tc_BAD_CONTEXT;

// error processing context object
class BAD_CONTEXT
  : public virtual ::CORBA::SystemException
{
   public:
    BAD_CONTEXT(const BAD_CONTEXT& v): Exception(v), SystemException(v) {}		
    BAD_CONTEXT(const char* reason = NULL)
      : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_BAD_CONTEXT){}
    BAD_CONTEXT(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_BAD_CONTEXT) {} 
    BAD_CONTEXT(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_BAD_CONTEXT) {} 
    
    //~BAD_CONTEXT() {}  
    
    void _raise() const
    {
      throw BAD_CONTEXT(*this);
    }
    
    const char* _name() const
    {
      return "BAD_CONTEXT";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/BAD_CONTEXT:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new BAD_CONTEXT(*this);
    }
    
    BAD_CONTEXT& operator= (const BAD_CONTEXT& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static BAD_CONTEXT* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<BAD_CONTEXT* > (e);
    }
    
    static const BAD_CONTEXT* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<BAD_CONTEXT* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  BAD_CONTEXT

extern const ::CORBA::TypeCode_ptr _tc_OBJ_ADAPTER;

// failure detected by object adapter
class OBJ_ADAPTER
  : public virtual ::CORBA::SystemException
{
   public:
    OBJ_ADAPTER(const OBJ_ADAPTER& v): Exception(v), SystemException(v) {}		
    OBJ_ADAPTER(const char* reason = NULL)
      : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_OBJ_ADAPTER){}
    OBJ_ADAPTER(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_OBJ_ADAPTER) {} 
    OBJ_ADAPTER(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_OBJ_ADAPTER) {} 
    
    //~OBJ_ADAPTER() {}  
    
    void _raise() const
    {
      throw OBJ_ADAPTER(*this);
    }
    
    const char* _name() const
    {
      return "OBJ_ADAPTER";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/OBJ_ADAPTER:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new OBJ_ADAPTER(*this);
    }
    
    OBJ_ADAPTER& operator= (const OBJ_ADAPTER& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static OBJ_ADAPTER* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<OBJ_ADAPTER* > (e);
    }
    
    static const OBJ_ADAPTER* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<OBJ_ADAPTER* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  OBJ_ADAPTER

extern const ::CORBA::TypeCode_ptr _tc_DATA_CONVERSION;

// data conversion error
class DATA_CONVERSION
  : public virtual ::CORBA::SystemException
{
   public:
    DATA_CONVERSION(const DATA_CONVERSION& v): Exception(v), SystemException(v) {}		
    DATA_CONVERSION(const char* reason = NULL)
      : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_DATA_CONVERSION){}
    DATA_CONVERSION(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_DATA_CONVERSION) {} 
    DATA_CONVERSION(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_DATA_CONVERSION) {} 
    
    //~DATA_CONVERSION() {}  
    
    void _raise() const
    {
      throw DATA_CONVERSION(*this);
    }
    
    const char* _name() const
    {
      return "DATA_CONVERSION";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/DATA_CONVERSION:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new DATA_CONVERSION(*this);
    }
    
    DATA_CONVERSION& operator= (const DATA_CONVERSION& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static DATA_CONVERSION* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<DATA_CONVERSION* > (e);
    }
    
    static const DATA_CONVERSION* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<DATA_CONVERSION* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  DATA_CONVERSION

extern const ::CORBA::TypeCode_ptr _tc_OBJECT_NOT_EXIST;

// non-existent object, delete reference
class OBJECT_NOT_EXIST
  : public virtual ::CORBA::SystemException
{
   public:
    OBJECT_NOT_EXIST(const OBJECT_NOT_EXIST& v): Exception(v), SystemException(v) {}		
    OBJECT_NOT_EXIST(const char* reason = NULL)
      : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_OBJECT_NOT_EXIST){}
    OBJECT_NOT_EXIST(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_OBJECT_NOT_EXIST) {} 
    OBJECT_NOT_EXIST(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_OBJECT_NOT_EXIST) {} 
    
    //~OBJECT_NOT_EXIST() {}  
    
    void _raise() const
    {
      throw OBJECT_NOT_EXIST(*this);
    }
    
    const char* _name() const
    {
      return "OBJECT_NOT_EXIST";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/OBJECT_NOT_EXIST:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new OBJECT_NOT_EXIST(*this);
    }
    
    OBJECT_NOT_EXIST& operator= (const OBJECT_NOT_EXIST& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static OBJECT_NOT_EXIST* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<OBJECT_NOT_EXIST* > (e);
    }
    
    static const OBJECT_NOT_EXIST* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<OBJECT_NOT_EXIST* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  OBJECT_NOT_EXIST

extern const ::CORBA::TypeCode_ptr _tc_TRANSACTION_REQUIRED;

// transaction required
class TRANSACTION_REQUIRED
  : public virtual ::CORBA::SystemException
{
   public:
    TRANSACTION_REQUIRED(const TRANSACTION_REQUIRED& v): Exception(v), SystemException(v) {}		
    TRANSACTION_REQUIRED(const char* reason = NULL)
      : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_TRANSACTION_REQUIRED){}
    TRANSACTION_REQUIRED(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_TRANSACTION_REQUIRED) {} 
    TRANSACTION_REQUIRED(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_TRANSACTION_REQUIRED) {} 
    
    //~TRANSACTION_REQUIRED() {}  
    
    void _raise() const
    {
      throw TRANSACTION_REQUIRED(*this);
    }
    
    const char* _name() const
    {
      return "TRANSACTION_REQUIRED";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/TRANSACTION_REQUIRED:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new TRANSACTION_REQUIRED(*this);
    }
    
    TRANSACTION_REQUIRED& operator= (const TRANSACTION_REQUIRED& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static TRANSACTION_REQUIRED* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<TRANSACTION_REQUIRED* > (e);
    }
    
    static const TRANSACTION_REQUIRED* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<TRANSACTION_REQUIRED* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  TRANSACTION_REQUIRED

extern const ::CORBA::TypeCode_ptr _tc_TRANSACTION_ROLLEDBACK;

// transaction rolled back
class TRANSACTION_ROLLEDBACK
  : public virtual ::CORBA::SystemException
{
   public:
    TRANSACTION_ROLLEDBACK(const TRANSACTION_ROLLEDBACK& v): Exception(v), SystemException(v) {}		
    TRANSACTION_ROLLEDBACK(const char* reason = NULL)
    : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_TRANSACTION_ROLLEDBACK){}
    TRANSACTION_ROLLEDBACK(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_TRANSACTION_ROLLEDBACK) {} 
    TRANSACTION_ROLLEDBACK(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_TRANSACTION_ROLLEDBACK) {} 
    
    //~TRANSACTION_ROLLEDBACK() {}  
    
    void _raise() const
    {
      throw TRANSACTION_ROLLEDBACK(*this);
    }
    
    const char* _name() const
    {
      return "TRANSACTION_ROLLEDBACK";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/TRANSACTION_ROLLEDBACK:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new TRANSACTION_ROLLEDBACK(*this);
    }
    
    TRANSACTION_ROLLEDBACK& operator= (const TRANSACTION_ROLLEDBACK& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static TRANSACTION_ROLLEDBACK* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<TRANSACTION_ROLLEDBACK* > (e);
    }
    
    static const TRANSACTION_ROLLEDBACK* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<TRANSACTION_ROLLEDBACK* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  TRANSACTION_ROLLEDBACK

extern const ::CORBA::TypeCode_ptr _tc_INVALID_TRANSACTION;

// invalid transaction
class INVALID_TRANSACTION
  : public virtual ::CORBA::SystemException
{
   public:
    INVALID_TRANSACTION(const INVALID_TRANSACTION& v): Exception(v), SystemException(v) {}		
    INVALID_TRANSACTION(const char* reason = NULL)
      : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_INVALID_TRANSACTION){}
    INVALID_TRANSACTION(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_INVALID_TRANSACTION) {} 
    INVALID_TRANSACTION(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_INVALID_TRANSACTION) {} 
    
    //~INVALID_TRANSACTION() {}  
    
    void _raise() const
    {
      throw INVALID_TRANSACTION(*this);
    }
    
    const char* _name() const
    {
      return "INVALID_TRANSACTION";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/INVALID_TRANSACTION:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new INVALID_TRANSACTION(*this);
    }
    
    INVALID_TRANSACTION& operator= (const INVALID_TRANSACTION& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static INVALID_TRANSACTION* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<INVALID_TRANSACTION* > (e);
    }
    
    static const INVALID_TRANSACTION* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<INVALID_TRANSACTION* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  INVALID_TRANSACTION

extern const ::CORBA::TypeCode_ptr _tc_INV_POLICY;

// invalid policy
class INV_POLICY
  : public virtual ::CORBA::SystemException
{
   public:
     INV_POLICY(const INV_POLICY& v): Exception(v), SystemException(v) {}		
     INV_POLICY(const char* reason = NULL)
       : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_INV_POLICY){}
     INV_POLICY(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_INV_POLICY) {} 
     INV_POLICY(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_INV_POLICY) {} 
    
    //~INV_POLICY() {}  
    
    void _raise() const
    {
      throw INV_POLICY(*this);
    }
    
    const char* _name() const
    {
      return "INV_POLICY";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/INV_POLICY:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new INV_POLICY(*this);
    }
    
    INV_POLICY& operator= (const INV_POLICY& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static INV_POLICY* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<INV_POLICY* > (e);
    }
    
    static const INV_POLICY* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<INV_POLICY* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  INV_POLICY

extern const ::CORBA::TypeCode_ptr _tc_CODESET_INCOMPATIBLE;

// incompatible code set
class CODESET_INCOMPATIBLE
  : public virtual ::CORBA::SystemException
{
   public:
    CODESET_INCOMPATIBLE(const CODESET_INCOMPATIBLE& v): Exception(v), SystemException(v) {}		
    CODESET_INCOMPATIBLE(const char* reason = NULL)
      : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_CODESET_INCOMPATIBLE){}
    CODESET_INCOMPATIBLE(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_CODESET_INCOMPATIBLE) {} 
    CODESET_INCOMPATIBLE(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_CODESET_INCOMPATIBLE) {} 
    
    //~CODESET_INCOMPATIBLE() {}  
    
    void _raise() const
    {
      throw CODESET_INCOMPATIBLE(*this);
    }
    
    const char* _name() const
    {
      return "CODESET_INCOMPATIBLE";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/CODESET_INCOMPATIBLE:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new CODESET_INCOMPATIBLE(*this);
    }
    
    CODESET_INCOMPATIBLE& operator= (const CODESET_INCOMPATIBLE& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static CODESET_INCOMPATIBLE* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<CODESET_INCOMPATIBLE* > (e);
    }
    
    static const CODESET_INCOMPATIBLE* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<CODESET_INCOMPATIBLE* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  CODESET_INCOMPATIBLE

extern const ::CORBA::TypeCode_ptr _tc_REBIND;

// rebind needed
class REBIND
  : public virtual ::CORBA::SystemException
{
   public:
    REBIND(const REBIND& v): Exception(v), SystemException(v) {}		
    REBIND(const char* reason = NULL)
      : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_REBIND){}
    REBIND(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_REBIND) {} 
    REBIND(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_REBIND) {} 
    
    //~REBIND() {}  
    
    void _raise() const
    {
      throw REBIND(*this);
    }
    
    const char* _name() const
    {
      return "REBIND";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/REBIND:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new REBIND(*this);
    }
    
    REBIND& operator= (const REBIND& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static REBIND* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<REBIND* > (e);
    }
    
    static const REBIND* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<REBIND* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  REBIND

extern const ::CORBA::TypeCode_ptr _tc_TIMEOUT;

// operation timed out
class TIMEOUT
  : public virtual ::CORBA::SystemException
{
   public:
    TIMEOUT(const TIMEOUT& v): Exception(v), SystemException(v) {}		
    TIMEOUT(const char* reason = NULL)
      : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_TIMEOUT) {}
    TIMEOUT(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_TIMEOUT) {} 
    TIMEOUT(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_TIMEOUT) {} 
    
    //~TIMEOUT() {}  
    
    void _raise() const
    {
      throw TIMEOUT(*this);
    }
    
    const char* _name() const
    {
      return "TIMEOUT";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/TIMEOUT:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new TIMEOUT(*this);
    }
    
    TIMEOUT& operator= (const TIMEOUT& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static TIMEOUT* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<TIMEOUT* > (e);
    }
    
    static const TIMEOUT* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<TIMEOUT* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  TIMEOUT

extern const ::CORBA::TypeCode_ptr _tc_TRANSACTION_UNAVAILABLE;

// no transaction
class TRANSACTION_UNAVAILABLE
  : public virtual ::CORBA::SystemException
{
   public:
    TRANSACTION_UNAVAILABLE(const TRANSACTION_UNAVAILABLE& v): Exception(v), SystemException(v) {}		
    TRANSACTION_UNAVAILABLE(const char* reason = NULL)
      : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_TRANSACTION_UNAVAILABLE){}
    TRANSACTION_UNAVAILABLE(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_TRANSACTION_UNAVAILABLE) {} 
    TRANSACTION_UNAVAILABLE(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_TRANSACTION_UNAVAILABLE) {} 
    
    //~TRANSACTION_UNAVAILABLE() {}  
    
    void _raise() const
    {
      throw TRANSACTION_UNAVAILABLE(*this);
    }
    
    const char* _name() const
    {
      return "TRANSACTION_UNAVAILABLE";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/TRANSACTION_UNAVAILABLE:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new TRANSACTION_UNAVAILABLE(*this);
    }
    
    TRANSACTION_UNAVAILABLE& operator= (const TRANSACTION_UNAVAILABLE& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static TRANSACTION_UNAVAILABLE* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<TRANSACTION_UNAVAILABLE* > (e);
    }
    
    static const TRANSACTION_UNAVAILABLE* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<TRANSACTION_UNAVAILABLE* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  TRANSACTION_UNAVAILABLE

extern const ::CORBA::TypeCode_ptr _tc_TRANSACTION_MODE;

// invalid transaction mode
class TRANSACTION_MODE
  : public virtual ::CORBA::SystemException
{
   public:
    TRANSACTION_MODE(const TRANSACTION_MODE& v): Exception(v), SystemException(v) {}		
     TRANSACTION_MODE(const char* reason = NULL)
     : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_TRANSACTION_MODE){}
    TRANSACTION_MODE(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_TRANSACTION_MODE) {} 
    TRANSACTION_MODE(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_TRANSACTION_MODE) {} 
    
    //~TRANSACTION_MODE() {}  
    
    void _raise() const
    {
      throw TRANSACTION_MODE(*this);
    }
    
    const char* _name() const
    {
      return "TRANSACTION_MODE";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/TRANSACTION_MODE:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new TRANSACTION_MODE(*this);
    }
    
    TRANSACTION_MODE& operator= (const TRANSACTION_MODE& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static TRANSACTION_MODE* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<TRANSACTION_MODE* > (e);
    }
    
    static const TRANSACTION_MODE* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<TRANSACTION_MODE* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  TRANSACTION_MODE

extern const ::CORBA::TypeCode_ptr _tc_BAD_QOS;

// bad quality of service
class BAD_QOS
  : public virtual ::CORBA::SystemException
{
   public:
    BAD_QOS(const BAD_QOS& v): Exception(v), SystemException(v) {}		
    BAD_QOS(const char* reason = NULL)
      : ::CORBA::SystemException (reason, 0, CORBA::COMPLETED_NO, _tc_BAD_QOS){}
    BAD_QOS(::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(NULL, minor, completed, _tc_BAD_QOS) {} 
    BAD_QOS(const char* reason, ::CORBA::ULong minor, CompletionStatus completed)
      : ::CORBA::SystemException(reason, minor, completed, _tc_BAD_QOS) {} 
    
    //~BAD_QOS() {}  
    
    void _raise() const
    {
      throw BAD_QOS(*this);
    }
    
    const char* _name() const
    {
      return "BAD_QOS";
    }  
     
    const char* _rep_id() const
    {
      return "IDL:omg.org/CORBA/BAD_QOS:1.0";
    }
  
    ::CORBA::Exception* _duplicate()
    {
      return new BAD_QOS(*this);
    }
    
    BAD_QOS& operator= (const BAD_QOS& e)
    {
      SystemException::operator=(e);
      return *this;
    }
		
    static BAD_QOS* _downcast(::CORBA::Exception* e)
    {      
      return dynamic_cast<BAD_QOS* > (e);
    }
    
    static const BAD_QOS* _downcast(const ::CORBA::Exception* e)
    {      
      return dynamic_cast<BAD_QOS* > (const_cast< ::CORBA::Exception* > (e));
    }
}; //  BAD_QOS




#endif
