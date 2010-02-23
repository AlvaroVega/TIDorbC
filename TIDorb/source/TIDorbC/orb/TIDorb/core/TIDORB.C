/////////////////////////////////////////////////////////////////////////
//
// File:        TIDORB.C
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
#include "TIDorb/core/typecode.h"
#include "TIDorb/dynAny.h"
#include "TIDorb/util.h"

#ifdef TIDORB_HAVE_IOSTREAM
#include <iostream>
#else
#include <iostream.h>
#endif

#ifdef TIDORB_HAVE_FSTREAM
#include <fstream>
#else
#include <fstream.h>
#endif

#include <string>
#include <exception>
#include <unistd.h>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::string;
#endif


#undef ERROR




TIDorb::core::TIDORB::ORBTable TIDorb::core::TIDORB::st_orb_instances;



TIDThr::RecursiveMutex* TIDorb::core::TIDORB::st_init_mutex =
  TIDorb::core::TIDORB::create_init_mutex();

const char* TIDorb::core::TIDORB::st_version = "6.3.1";


TIDThr::RecursiveMutex* TIDorb::core::TIDORB::create_init_mutex()
{
  try {
    TIDThr::init();
    return new TIDThr::RecursiveMutex();

  } catch (...) {
    cerr << "Critical ERROR: cannot initialize ORB Multithreading" << endl;
    exit(-1);
  }
}



//MLG
TIDorb::core::TIDORB::TIDORB()
  throw (TIDThr::SystemException)
  : trace(NULL), m_typecode_cache(NULL), m_services(this), m_state(this), 
    m_destroyed(false), commManager(NULL), rootPOA(NULL), current(NULL), 
    dyn_factory(NULL), codec_factory(NULL), policy_manager(NULL), 
    policy_current(NULL), policy_context_manager(NULL), 
    compression_manager(NULL), requestCounter(NULL)
{
  _add_ref();
}
//EMLG




TIDorb::core::TIDORB::~TIDORB()
  throw (TIDThr::SystemException)
{
//PRA
//MLG
//if (m_typecode_cache)
//  	delete m_typecode_cache;
//EMLG  	
//EPRA
  if(!m_destroyed)
    destroy();


}

//MLG
TIDorb::core::typecode::TypeCodeCache* TIDorb::core::TIDORB::getTypeCodeCache()
{
	return m_typecode_cache;
}
//EMLG

::CORBA::ORB_ptr CORBA::ORB_init(int& argc, char** argv, const char* orb_identifier)
{
  return TIDorb::core::TIDORB::init(argc, argv, orb_identifier);
}

::CORBA::ORB_ptr TIDorb::core::TIDORB::init(int& argc, char** argv, const char* orb_identifier)
{
  TIDThr::Synchronized synchro(*st_init_mutex);

  ORBTable::iterator p = st_orb_instances.find(orb_identifier);

  if(p != st_orb_instances.end())
    return CORBA::ORB::_duplicate((*p).second);

  TIDORB* orb = new TIDORB();
  try {
    orb->init_orb(argc, argv, orb_identifier);
  } catch (...) {
    delete orb;
    throw;
  }

  st_orb_instances[orb_identifier] = orb;
  return orb;
}

/*
 * Operation called by ServantDelegate::default_poa to obtain a valid POA to the _this() operation.
 * If there is only an ORB instance return it, otherwise return de -ORBid="" initialized one.
 */

 ::CORBA::ORB_ptr TIDorb::core::TIDORB::default_ORB()
{
  //jagd 2
  //CORBA::ORB_ptr default_orb = CORBA::ORB::_nil();
  CORBA::ORB_ptr default_orb = 0;

  {
    TIDThr::Synchronized synchro(*st_init_mutex);

    if(st_orb_instances.size() == 1)
    {
      ORBTable::iterator p = st_orb_instances.begin();
      default_orb = CORBA::ORB::_duplicate((*p).second);
    } else {
      ORBTable::iterator p = st_orb_instances.find(TIDorb::core::ConfORB::DEFAULT_ORB_ID);
      if(p != st_orb_instances.end())
      {
        default_orb = CORBA::ORB::_duplicate((*p).second);
      }
    }
  }

  //jagd 2
  //if(CORBA::is_nil(default_orb))
  if(!(default_orb))
     throw CORBA::BAD_INV_ORDER();

  return default_orb;
}




void TIDorb::core::TIDORB::init_orb(int& argc, char** argv, const char* orb_identifier)
{

  m_conf.get_parameters(argc, argv, orb_identifier);

  commManager = new TIDorb::core::comm::CommunicationManager(this);

  setup_initial_references();

//MLG  
  if (m_conf.typecode_cache_size > 0)
  	m_typecode_cache = new TIDorb::core::typecode::TypeCodeCache(m_conf.typecode_cache_size);
//EMLG  


  // Initialize random number generator
  srand48((long) TIDorb::core::util::Time::currentTimeMillis());

  // Trace
  if(m_conf.trace_level != TIDorb::util::TR_NONE)
  {
         TIDorb::util::TraceLevel trace_level =
                (TIDorb::util::TraceLevel) m_conf.trace_level;

     if(m_conf.trace_file != NULL)
     { /*
       if (m_conf.trace_num_files>1)
       {
        CircularTraceFile ctf = new CircularTraceFile (m_conf.trace_num_files,
                                                       m_conf.trace_file_size,
                                                       trace_file);

        trace = Trace.create_trace(ctf, m_conf.orb_id, trace_level);
       }
       else */
       trace = TIDorb::util::Trace::create_trace(m_conf.trace_file,
                                                 m_conf.orb_id,
                                                 trace_level);
     }
     else
     {
       trace = TIDorb::util::Trace::create_trace(m_conf.orb_id,
                                                 trace_level);
     }
  }

  if (trace != NULL) {
    TIDorb::util::StringBuffer buffer;
    buffer << "ORB initialization OK!";
    if (m_conf.trace_level >= TIDorb::util::TR_DEEP_DEBUG) m_conf.dump(buffer);
    buffer << "\tTIDorbC++ version " << st_version;
    print_trace(TIDorb::util::TR_ERROR, buffer.str().data());
  }
}

TIDorb::core::comm::CommunicationManager* 
    TIDorb::core::TIDORB::getCommunicationManager()
{
  return commManager;
}

void TIDorb::core::TIDORB::setup_initial_references()
{
  size_t references = conf().initial_references.size();

  for(size_t i = 0; i < references; i++)
  {
    const TIDorb::core::util::InitialReference& ref = conf().initial_references[i];
    m_services.set_service(ref.get_name(), string_to_object(ref.get_url()));
  }
}

//PRA
//::TIDorb::portable::OutputStream* TIDorb::core::TIDORB::create_output_stream()
::TIDorb::portable::OutputStream* TIDorb::core::TIDORB::create_output_stream() const
//EPRA
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  return new TIDorb::core::cdr::CDROutputStream((TIDORB*) this);
}

//PRA
//CORBA::Any* TIDorb::core::TIDORB::create_any()
CORBA::Any* TIDorb::core::TIDORB::create_any() const
//EPRA
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  CORBA::Any* any =  new CORBA::Any();
  //jagd 
  //(dynamic_cast<AnyImpl&> (any->delegate())).orb((TIDORB*) this);
  ((AnyImpl*)&(any->delegate()))->orb((TIDORB*)this);
  return any;
}

const TIDorb::core::ConfORB& TIDorb::core::TIDORB::conf() const
{
  return m_conf;
}

const TIDorb::core::ORBServices& TIDorb::core::TIDORB::services() 
{
  return m_services;
}

char* TIDorb::core::TIDORB::object_to_string(::CORBA::Object_ptr obj)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);
 
  //jagd 0
  //if(CORBA::is_nil(obj)) {
  if(!(obj)) {
    //PRA
    //return (char*)TIDorb::core::iop::IOR::null_ior()->toString();
    return CORBA::string_dup(TIDorb::core::iop::IOR::null_ior()->toString());
    //EPRA
  }

  CORBA::LocalObject_var local = CORBA::LocalObject::_narrow(obj);

  //jagd 2
  //if(!CORBA::is_nil(local))
  if((local))
    throw CORBA::MARSHAL("Impossible to marshal a local object.",
                         4 , CORBA::COMPLETED_NO);

  TIDorb::portable::Stub* stub =
    dynamic_cast < TIDorb::portable::Stub* > (obj);

  if (!stub)
    throw CORBA::BAD_PARAM("Unexpected Object reference");

  TIDorb::core::ObjectDelegateImpl *delegate =
    dynamic_cast < ObjectDelegateImpl* > (stub->_get_delegate());

  //PRA
  //return (char*) delegate->getReference()->toString();
  return CORBA::string_dup(delegate->getReference()->toString());
  //EPRA
}

/*
  public String object_to_URL(org.omg.CORBA.Object obj)
  {
    if (destroyed)
      throw new OBJECT_NOT_EXIST(0,CompletionStatus.COMPLETED_NO);

    if(obj == null)
      return Corbaloc.to_URL(IOR.null_ior());

    org.omg.CORBA.portable.Delegate delegate =
      ((org.omg.CORBA.portable.ObjectImpl)obj)._get_delegate();

    if (delegate instanceof DelegateImpl)
    {
      return Corbaloc.to_URL(((DelegateImpl)delegate).getReference());
    }
    else
      throw new BAD_PARAM("This Object has not been create by TIDorb",
        0,CompletionStatus.COMPLETED_NO);
  }
*/

::CORBA::Object_ptr TIDorb::core::TIDORB::string_to_object(const char* str)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  if(!str)
    throw CORBA::BAD_PARAM("Null String reference", 0, CORBA::COMPLETED_NO);

  if (strncmp(str, TIDorb::core::util::URL::CORBANAME_PROTOCOL,
             strlen(TIDorb::core::util::URL::CORBANAME_PROTOCOL)) == 0)
  {
    return corbaname_url_to_object(str);
  }

  if(strncmp(str, TIDorb::core::util::URL::FILE_PROTOCOL,
             strlen(TIDorb::core::util::URL::FILE_PROTOCOL)) == 0)
  {
    return file_url_to_object(str);
  }

  if(strncmp(str, TIDorb::core::util::URL::RIR_PROTOCOL,
             strlen(TIDorb::core::util::URL::RIR_PROTOCOL)) == 0)
  {
    return rir_url_to_object(str);
  }

  if (strncmp(str, TIDorb::core::util::URL::TIDORB_PROTOCOL,
             strlen(TIDorb::core::util::URL::TIDORB_PROTOCOL)) == 0)
  {
    return tidorb_url_to_object(str);
  }

  if ((strncmp(str, TIDorb::core::util::URL::IIOP_PROTOCOL_1,
               strlen(TIDorb::core::util::URL::IIOP_PROTOCOL_1)) == 0)  ||
      (strncmp(str, TIDorb::core::util::URL::IIOP_PROTOCOL_2,
               strlen(TIDorb::core::util::URL::IIOP_PROTOCOL_2)) == 0))
  {
    return iiop_url_to_object(str);
  }

  // pra@tid.es - MIOP extensions
  if (strncmp(str, TIDorb::core::util::URL::MIOP_PROTOCOL,
             strlen(TIDorb::core::util::URL::MIOP_PROTOCOL)) == 0)
  {
    return miop_url_to_object(str);
  }
  // end MIOP extensions

  return ior_to_object(str);
}


CORBA::Object_ptr TIDorb::core::TIDORB::ior_to_object(const char* str) throw(CORBA::SystemException)
{
  TIDorb::core::iop::IOR* ior =
    TIDorb::core::iop::IOR::fromString(this, str);

  return TIDorb::core::ObjectImpl::fromIOR(this, ior);
}




//PRE: str starts with corbaloc:rir:
CORBA::Object_ptr TIDorb::core::TIDORB::rir_url_to_object(const char* str) throw(CORBA::SystemException)
{
  string url = str;
  const char* rir_protocol = "corbaloc:rir:/";

  if(url.find(rir_protocol) == string::npos)
    throw CORBA::BAD_PARAM(str);

  try
  {
    return resolve_initial_references(url.substr(strlen(rir_protocol)).c_str());
  }
  catch(const CORBA::ORB::InvalidName& e)
  {
    throw CORBA::BAD_PARAM("InvalidName", 0, CORBA::COMPLETED_NO);
  }
}




//PRE: str starts with corbaname:
CORBA::Object_ptr TIDorb::core::TIDORB::corbaname_url_to_object(const char* str) throw(CORBA::SystemException)
{
  try
  {
    return TIDorb::core::util::Corbaname::to_object(this,str);
  }
  catch (const CORBA::Exception& e)
  {
    throw CORBA::BAD_PARAM("Invalid corbaname URL", 0,CORBA::COMPLETED_NO);
  }
}


//PRE: str starts with corbaloc:tidorb:
CORBA::Object_ptr TIDorb::core::TIDORB::tidorb_url_to_object(const char* str) throw(CORBA::SystemException)
{
  TIDorb::core::iop::IOR* ior = NULL;

  try
  {
    ior = TIDorb::core::comm::iiop::Corbaloc::get_IOR(str);
  }
  catch(const CORBA::Exception& e)
  {
    throw CORBA::BAD_PARAM("Invalid corbaloc:tidorb URL", 0, CORBA::COMPLETED_NO);
  }

  return TIDorb::core::ObjectImpl::fromIOR(this, ior);
}


// PRE str starts with corbaloc:iiop: or corbaloc::
CORBA::Object_ptr TIDorb::core::TIDORB::iiop_url_to_object(const char* str) throw(CORBA::SystemException)
{
  TIDorb::core::iop::IOR* ior = NULL;

  try
  {
    ior = TIDorb::core::comm::iiop::IIOPCorbaloc::get_IOR(str);
  }
  catch(const CORBA::Exception& e)
  {
    throw CORBA::BAD_PARAM("Invalid corbaloc URL", 0, CORBA::COMPLETED_NO);
  }

  return TIDorb::core::ObjectImpl::fromIOR(this, ior);
}


/*
 * pra@tid.es - MIOP extensions
 */

// PRE str starts with corbaloc:miop:
CORBA::Object_ptr TIDorb::core::TIDORB::miop_url_to_object(const char* str) throw(CORBA::SystemException)
{
  TIDorb::core::iop::IOR* ior = NULL;

  try
  {
    ior = TIDorb::core::comm::miop::MIOPCorbaloc::get_IOR(str);
  }
  catch(const CORBA::Exception& e)
  {
    throw CORBA::BAD_PARAM("Invalid corbaloc:miop URL", 0, CORBA::COMPLETED_NO);
  }

  return TIDorb::core::ObjectImpl::fromIOR(this, ior);
}

/*
 * end MIOP extensions
 */


// PRE str starts with file:
CORBA::Object_ptr TIDorb::core::TIDORB::file_url_to_object(const char* str) throw(CORBA::SystemException)
{
  string url = str;
  string file_path = url.substr(strlen(TIDorb::core::util::URL::FILE_PROTOCOL));

  if (file_path.length() == 0)
    throw CORBA::BAD_PARAM("Invalid file name");

  // TODO: improve this
  
  char file_url[1024];
  try
  {
    ifstream fl(file_path.c_str());
    if (!fl) {
      throw CORBA::BAD_PARAM("Invalid file URL");
    }

    fl.getline(file_url, 1024);
    fl.close();

  }
  catch (const exception& fnf)
  {
    throw CORBA::BAD_PARAM("Invalid file url");
  }
  
  return string_to_object(file_url);    
}




void TIDorb::core::TIDORB::create_list(::CORBA::Long size, ::CORBA::NVList_out list)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  list = new NVListImpl(this);
}

void TIDorb::core::TIDORB::create_operation_list(::CORBA::OperationDef_ptr def, ::CORBA::NVList_out list)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  throw CORBA::NO_IMPLEMENT(0,CORBA::COMPLETED_NO);
}

void TIDorb::core::TIDORB::create_named_value(::CORBA::NamedValue_out nv)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  nv = new NamedValueImpl(CORBA::string_dup(""), create_any(), CORBA::ARG_IN);
}

void TIDorb::core::TIDORB::create_exception_list(::CORBA::ExceptionList_out list)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  list = new ExceptionListImpl();
}

void TIDorb::core::TIDORB::create_context_list(::CORBA::ContextList_out list)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  list = new ContextListImpl();
}

void TIDorb::core::TIDORB::get_default_context(::CORBA::Context_out ctx)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  throw CORBA::NO_IMPLEMENT(0,CORBA::COMPLETED_NO);
}

void TIDorb::core::TIDORB::create_environment(::CORBA::Environment_out env)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  env = new EnvironmentImpl();
}


void TIDorb::core::TIDORB::send_multiple_requests_oneway(const ::CORBA::ORB::RequestSeq& seq)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  throw CORBA::NO_IMPLEMENT(0,CORBA::COMPLETED_NO);
}

void TIDorb::core::TIDORB::send_multiple_requests_deferred(const ::CORBA::ORB::RequestSeq& seq)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  throw CORBA::NO_IMPLEMENT(0,CORBA::COMPLETED_NO);
}

::CORBA::Boolean TIDorb::core::TIDORB::poll_next_response()
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  throw CORBA::NO_IMPLEMENT(0,CORBA::COMPLETED_NO);
}

void TIDorb::core::TIDORB::get_next_response(::CORBA::Request_out req)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  throw CORBA::NO_IMPLEMENT(0,CORBA::COMPLETED_NO);
}


::CORBA::ORB::ObjectIdList_ptr TIDorb::core::TIDORB::list_initial_services()
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  return m_services.list_initial_services();
}

::CORBA::Object_ptr TIDorb::core::TIDORB::resolve_initial_references(const char *identifier)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  return m_services.resolve_service(identifier);
}

::CORBA::Boolean TIDorb::core::TIDORB::work_pending()
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  throw CORBA::NO_IMPLEMENT(0,CORBA::COMPLETED_NO);
}

void TIDorb::core::TIDORB::perform_work()
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  throw CORBA::NO_IMPLEMENT(0,CORBA::COMPLETED_NO);
}

void TIDorb::core::TIDORB::shutdown(::CORBA::Boolean wait_for_completion)
{

  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  if (m_state.is_shutdowned())
    throw CORBA::BAD_INV_ORDER(4, CORBA::COMPLETED_NO);

  m_state.shutdown();

  if (wait_for_completion) {
    // Is the thread serving a request?

    CORBA::Object_var current_ref = init_POACurrent();

    if(current->in_context())
    { // yes, it is in the serving request context

      throw CORBA::BAD_INV_ORDER(3, CORBA::COMPLETED_NO);
    }

    try{
      m_state.wait_for_shutdown();
    } catch (const TIDThr::Exception& ex) {
      if (trace != NULL) {
        TIDorb::util::StringBuffer buffer;
        buffer << "Unexpected " << ex << " at ORB shutdown";
        print_trace(TIDorb::util::TR_ERROR, buffer.str().data());
      }
    }

  }
}

void TIDorb::core::TIDORB::do_complete_shutdown()
{
  vector<PortableServer::POAManager*>::iterator iter = POAManagers.begin();
  while (iter != POAManagers.end()) {
    PortableServer::POAManager_ptr poamgr = *iter;
    iter = POAManagers.erase(iter);
    try {
      poamgr->deactivate(true, true); // etherealize objects, wait_for_completion
    } catch (...) {
    }
  }
  	
  //PRA
  try {
    if (rootPOA != NULL)
      rootPOA->destroy(false, true);
  } catch (...) { /*NO_CONTEXT*/
  }

  try {
    if (commManager != NULL)
      commManager->shutdown();
  } catch (...) { /*NO_CONTEXT*/
  }
  //EPRA

  m_state.shutdowned();
}


void TIDorb::core::TIDORB::run()
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  if (m_state.is_shutdowned())
    throw CORBA::BAD_INV_ORDER(4,CORBA::COMPLETED_NO);

  if (!rootPOA) {
    throw CORBA::INTERNAL("RootPOA has not been created.");
  }

  m_state.wait_for_shutdown();

}



void TIDorb::core::TIDORB::destroy()
{

  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);


  // Is thread serving a request?

  CORBA::Object_var current_ref = init_POACurrent();

  if(current->in_context())
  { // yes, it is in the serving request context
    throw CORBA::BAD_INV_ORDER(3, CORBA::COMPLETED_NO);
  }

  m_state.shutdown();

  m_state.wait_for_shutdown();


  try {

    TIDThr::Synchronized synchro(*this);

    /*
      pra@tid.es
      if (!commManager)
      return;
      
      if(trace != NULL) {
      print_trace(TIDorb::util::TR_USER, "Closing ORB Session.");
      delete trace; // trace = NULL;
      }
    */
    
    /*
      value_factories.clear();
      value_factories = null;
    */
    
    if (commManager) {
      commManager->destroy();
      delete commManager;
      commManager = 0;
    }
    
    m_services.destroy();
    
    if(m_typecode_cache) {
      delete m_typecode_cache;
      m_typecode_cache = NULL;
    }

    if(compression_manager) {
      delete compression_manager;
      compression_manager = NULL;
    }
    
    if(policy_context_manager) {
      delete policy_context_manager;
      policy_context_manager = NULL;
    }
    
    if(policy_manager) {
      delete policy_manager;
      policy_manager = NULL;
    }

    if(policy_current) {
      delete policy_current;
      policy_current = NULL;
    }
    
    if(rootPOA) {
      CORBA::release(rootPOA);
      rootPOA = NULL;
    }
    
    if(current) {
      CORBA::release(current);
      current = NULL;
    }
    
    if(dyn_factory) {
      //jagd
      //TIDorb::dynAny::DynAnyFactoryImpl* dyn_impl =
      //  dynamic_cast< TIDorb::dynAny::DynAnyFactoryImpl* > (dyn_factory);
      TIDorb::dynAny::DynAnyFactoryImpl* dyn_impl =
        ( TIDorb::dynAny::DynAnyFactoryImpl* ) (dyn_factory);
      dyn_impl->destroy();
      CORBA::release(dyn_factory);
      dyn_factory = NULL;
    }

    if(codec_factory) {
      CORBA::release(codec_factory);
      codec_factory = NULL;
    }
    
    if(trace != NULL) {
      print_trace(TIDorb::util::TR_USER, "Closing ORB Session.");
      delete trace;

    trace = NULL;
    }
    
    m_destroyed = true;
  }
  catch(...){
    m_destroyed = true;
  }

  {
    TIDThr::Synchronized synchro(*st_init_mutex);

    st_orb_instances.erase((const char*) m_conf.orb_id);
  }

}



::CORBA::Boolean TIDorb::core::TIDORB::get_service_information(::CORBA::ServiceType svc_type,
                                                               ::CORBA::ServiceInformation_out svc_info)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  throw CORBA::NO_IMPLEMENT(0, CORBA::COMPLETED_NO);
}

::CORBA::Policy_ptr TIDorb::core::TIDORB::create_policy(::CORBA::PolicyType type,
                                                        const ::CORBA::Any& val)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  return PolicyFactory::create_policy(type, val);
}

// CORBA 2.3

::CORBA::ValueFactory TIDorb::core::TIDORB::register_value_factory
  (const char* id,
   ::CORBA::ValueFactory factory)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  return m_services.register_value_factory(id, factory);
}

void TIDorb::core::TIDORB::unregister_value_factory
  (const char* id)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  m_services.unregister_value_factory(id);
}

::CORBA::ValueFactory TIDorb::core::TIDORB::lookup_value_factory
  (const char* id)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  return m_services.lookup_value_factory(id);
}

// CORBA 2.5

::CORBA::ORB::ORBId TIDorb::core::TIDORB::id()
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  if (m_state.is_shutdowned())
    throw CORBA::BAD_INV_ORDER(4,CORBA::COMPLETED_NO);


  return CORBA::string_dup(m_conf.orb_id);
}


void TIDorb::core::TIDORB::register_initial_reference(const char* object_name,
                                                      const ::CORBA::Object_ptr object)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  m_services.register_initial_reference(object_name, object);
}

// CORBA 2.6

::CORBA::TypeCode_ptr
TIDorb::core::TIDORB::create_struct_tc(const char* id,
                                       const char* name,
                                       const ::CORBA::StructMemberSeq& members)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  return TIDorb::portable::TypeCodeFactory::create_struct_tc(id, name, new ::CORBA::StructMemberSeq(members));
}


::CORBA::TypeCode_ptr
TIDorb::core::TIDORB::create_union_tc(const char* id,
                                      const char* name,
                                      ::CORBA::TypeCode_ptr discriminator_type,
                                      const ::CORBA::UnionMemberSeq& members)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  return TIDorb::portable::TypeCodeFactory::create_union_tc(id, name, discriminator_type, new ::CORBA::UnionMemberSeq(members));
}


::CORBA::TypeCode_ptr
TIDorb::core::TIDORB::create_enum_tc(const char* id,
                                     const char* name,
                                     const ::CORBA::EnumMemberSeq& members)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  return TIDorb::portable::TypeCodeFactory::create_enum_tc(id, name, new ::CORBA::EnumMemberSeq(members));
}


::CORBA::TypeCode_ptr
TIDorb::core::TIDORB::create_alias_tc(const char* id,
                                      const char* name,
                                      ::CORBA::TypeCode_ptr original_type)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  return TIDorb::portable::TypeCodeFactory::create_alias_tc(id, name, original_type);
}


::CORBA::TypeCode_ptr
TIDorb::core::TIDORB::create_exception_tc(const char* id,
                                          const char* name,
                                          const ::CORBA::StructMemberSeq& members)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  return TIDorb::portable::TypeCodeFactory::create_exception_tc(id, name, new ::CORBA::StructMemberSeq(members));
}


::CORBA::TypeCode_ptr
TIDorb::core::TIDORB::create_interface_tc(const char* id,
                                          const char* name)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  return TIDorb::portable::TypeCodeFactory::create_interface_tc(id, name);
}


::CORBA::TypeCode_ptr
TIDorb::core::TIDORB::create_string_tc(::CORBA::ULong bound)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  return TIDorb::portable::TypeCodeFactory::create_string_tc(bound);
}


::CORBA::TypeCode_ptr
TIDorb::core::TIDORB::create_wstring_tc(::CORBA::ULong bound)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

   return TIDorb::portable::TypeCodeFactory::create_wstring_tc(bound);
}


::CORBA::TypeCode_ptr
TIDorb::core::TIDORB::create_fixed_tc(::CORBA::UShort digits, ::CORBA::Short scale)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  return TIDorb::portable::TypeCodeFactory::create_fixed_tc(digits, scale);
}


::CORBA::TypeCode_ptr
TIDorb::core::TIDORB::create_sequence_tc(::CORBA::ULong bound,
                                         ::CORBA::TypeCode_ptr element_type)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  return TIDorb::portable::TypeCodeFactory::create_sequence_tc(bound, element_type);
}


// deprecated
::CORBA::TypeCode_ptr
TIDorb::core::TIDORB::create_recursive_sequence_tc(::CORBA::ULong bound, ::CORBA:: ULong offset)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  throw CORBA::NO_IMPLEMENT(0, CORBA::COMPLETED_NO);
}


::CORBA::TypeCode_ptr
TIDorb::core::TIDORB::create_array_tc(::CORBA::ULong length,
                                      ::CORBA::TypeCode_ptr element_type)
{
  return TIDorb::portable::TypeCodeFactory::create_array_tc(length, element_type);
}


::CORBA::TypeCode_ptr
TIDorb::core::TIDORB::create_value_tc(const char* id,
                                      const char* name,
                                      ::CORBA::ValueModifier type_modifier,
                                      ::CORBA::TypeCode_ptr concrete_base,
                                      const ::CORBA::ValueMemberSeq& members)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  return TIDorb::portable::TypeCodeFactory::create_value_tc(id, name, type_modifier, concrete_base,
                                                            new ::CORBA::ValueMemberSeq(members));
}


::CORBA::TypeCode_ptr
TIDorb::core::TIDORB::create_value_box_tc(const char* id,
                                          const char* name,
                                          ::CORBA::TypeCode_ptr boxed_type)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  return TIDorb::portable::TypeCodeFactory::create_value_box_tc(id, name, boxed_type);
}


::CORBA::TypeCode_ptr
TIDorb::core::TIDORB::create_native_tc(const char* id,
                                       const char* name)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  return TIDorb::portable::TypeCodeFactory::create_native_tc(id, name);
}


::CORBA::TypeCode_ptr
TIDorb::core::TIDORB::create_recursive_tc(const char* id)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

   return TIDorb::portable::TypeCodeFactory::create_recursive_tc(id);
}


::CORBA::TypeCode_ptr
TIDorb::core::TIDORB::create_abstract_interface_tc(const char* id,
                                                   const char* name)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  return TIDorb::portable::TypeCodeFactory::create_abstract_interface_tc(id, name);
}


::CORBA::TypeCode_ptr
TIDorb::core::TIDORB::create_local_interface_tc(const char* id,
                                                const char* name)
{
  if (m_destroyed)
    throw CORBA::OBJECT_NOT_EXIST(0, CORBA::COMPLETED_NO);

  return TIDorb::portable::TypeCodeFactory::create_local_interface_tc(id, name);
}


CORBA::Object_ptr TIDorb::core::TIDORB::init_POA()
{
  TIDThr::Synchronized synchro (*this);

  if(rootPOA == NULL)
  {
    // pra@tid.es - MIOP extensions
    // rootPOA = TIDorb::core::poa::POAImpl::createRootPOA(this);
    rootPOA = TIDorb::core::goa::GOAImpl::createRootPOA(this);
    // end MIOP extensions

    // init the orb listening point
    commManager->getExternalLayer()->init_server_listener();

    m_state.running();
  }

  return PortableServer::POA::_duplicate(rootPOA);
}

CORBA::Object_ptr TIDorb::core::TIDORB::init_POACurrent()
{
  TIDThr::Synchronized synchro (*this);

  if (current == NULL)
  {
    current = new TIDorb::core::poa::CurrentImpl(this);
  }
  return PortableServer::Current::_duplicate(current);
}

CORBA::Object_ptr TIDorb::core::TIDORB::init_DynAnyFactory()
{
  TIDThr::Synchronized synchro (*this);

  if (dyn_factory == NULL) {
    TIDorb::dynAny::DynAnyFactoryImpl* dyn_impl =
      new TIDorb::dynAny::DynAnyFactoryImpl();
    dyn_impl->set_orb(this);
    dyn_factory = dyn_impl;
  }

  return DynamicAny::DynAnyFactory::_duplicate(dyn_factory);
}

CORBA::Object_ptr TIDorb::core::TIDORB::init_CodecFactory()
{
  TIDThr::Synchronized synchro (*this);
  
  if (codec_factory == NULL) {
    codec_factory = new TIDorb::core::CodecFactoryImpl(this);
  }
  return IOP::CodecFactory::_duplicate(codec_factory);
}

CORBA::Object_ptr TIDorb::core::TIDORB::init_PolicyManager()
{
  return getPolicyManager();
}

CORBA::Object_ptr TIDorb::core::TIDORB::init_PolicyCurrent()
{
  return getPolicyCurrent();
}

CORBA::Object_ptr TIDorb::core::TIDORB::init_CompressionManager()
{
  return getCompressionManager();
}

void TIDorb::core::TIDORB::remove_POA()
{
  TIDThr::Synchronized synchro (*this);

  rootPOA = NULL;
}

TIDorb::core::PolicyManagerImpl* TIDorb::core::TIDORB::getPolicyManager()
{
  TIDThr::Synchronized synchro(*this);

  if (policy_manager == NULL) {
    policy_manager = new PolicyManagerImpl(this);
  }

  return policy_manager;
}

TIDorb::core::PolicyCurrentImpl* TIDorb::core::TIDORB::getPolicyCurrent()
{
  TIDThr::Synchronized synchro(*this);

  if (policy_current == NULL) {
    policy_current = new PolicyCurrentImpl(this->getPolicyContextManager());
  }

  return policy_current;
}

TIDorb::core::PolicyContextManagerImpl* TIDorb::core::TIDORB::getPolicyContextManager()
{
  TIDThr::Synchronized synchro(*this);

  if (policy_context_manager == NULL) {
    policy_context_manager = new PolicyContextManagerImpl(this);
  }

  return policy_context_manager;
}

TIDorb::core::compression::CompressionManagerImpl* 
TIDorb::core::TIDORB::getCompressionManager()
{
  TIDThr::Synchronized synchro(*this);

  if (compression_manager == NULL) {
    compression_manager = new TIDorb::core::compression::CompressionManagerImpl(this);
  }

  return compression_manager;
}



TIDorb::core::RequestCounter* TIDorb::core::TIDORB::getRequestCounter()
{
  if (requestCounter == NULL) {
    requestCounter = new RequestCounter();
  }
  return requestCounter;
}


CORBA::ORB_ptr CORBA::ORB::_duplicate(CORBA::ORB_ptr orb)
{
  if(!orb)
    return orb;

  try {
  
    //jagd
    //TIDorb::core::TIDORB* tidorb = dynamic_cast<TIDorb::core::TIDORB*> (orb);
    TIDorb::core::TIDORB* tidorb = (TIDorb::core::TIDORB*) (orb);

    if(tidorb)
      tidorb->_add_ref();

    return orb;

  } catch (const TIDThr::Exception& ex) {
    throw CORBA::INTERNAL(0, CORBA::COMPLETED_NO);
  }
}

CORBA::ORB_ptr CORBA::ORB::_nil()
{
  return 0;
}

void CORBA::release(CORBA::ORB_ptr orb)
{
  try {
 
    //jagd
    //TIDorb::core::TIDORB* tidorb = dynamic_cast< TIDorb::core::TIDORB* > (orb);
    TIDorb::core::TIDORB* tidorb = ( TIDorb::core::TIDORB* ) (orb);

    if(tidorb)
      tidorb->_remove_ref();

  } catch (const TIDThr::Exception& ex) {
    throw CORBA::INTERNAL(0, CORBA::COMPLETED_NO);
  }
}

CORBA::Boolean CORBA::is_nil(CORBA::ORB_ptr orb)
{
  return !orb;
}

void TIDorb::core::TIDORB::print_trace(TIDorb::util::TraceLevel level, const char* message){
  if (trace != NULL)
    trace->print(level, message);

}

void TIDorb::core::TIDORB::print_trace(TIDorb::util::TraceLevel level, const char** message,
                                       size_t elements){
  if (trace != NULL)
    trace->print(level, message, elements);
}

void TIDorb::core::TIDORB::print_trace(TIDorb::util::TraceLevel level, const char* message,
                                       const CORBA::Exception& ex){
  if (trace != NULL)
    trace->print(level, message, TIDorb::core::util::CORBAException(ex));
}

void TIDorb::core::TIDORB::print_trace(TIDorb::util::TraceLevel level, const char* message,
                                       const exception& ex){
  if (trace != NULL)
    trace->print(level, message, ex);
}

void TIDorb::core::TIDORB::print_trace(TIDorb::util::TraceLevel level, const char** message,
                                       size_t elements, const exception& ex){
  if (trace != NULL)
    trace->print(level, message, elements, ex);
}


void TIDorb::core::TIDORB::print_dump(TIDorb::util::TraceLevel level, const unsigned char* message,
                                      size_t length){
  if (trace != NULL)
    trace->dump(level, message, length);
}
