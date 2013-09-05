//////////////////////////////////////////////////////////////////////////////////
//
// File:        TIDORB.h
// Description: TIDORB implementation.
//
// Rel:         01.00
// Created:     September, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
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
///////////////////////////////////////////////////////////////////////////////////

#include "CORBA.h"
#include "PortableServer.h"
#ifndef MINIMUN
#include "DynamicAny.h"
#endif

#include "TIDorb/core.h"
#include "TIDorb/core/typecode/TypeCodeCache.h"
#include "TIDorb/core/compression.h"

#ifndef _TIDorb_core_TIDORB_H_
#define _TIDorb_core_TIDORB_H_

#ifdef __SUNPRO_LIBRARY_IOSTREAM
#define __STD_ISTREAM__
#define __STD_OSTREAM__
#endif

#include <map>

#ifdef __SUNPRO_LIBRARY_IOSTREAM
#undef __STD_ISTREAM__
#undef __STD_OSTREAM__
#endif

#include <string>
#include <vector>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::map;
  using std::string;
  using std::vector;
#endif


namespace TIDorb {
namespace core {


void init();


class TIDORB : public TIDorb::portable::ORB,
               public TIDThr::Monitor
{
  friend class CORBA::ORB;
  friend void init ();

  public:

    TIDorb::core::comm::CommunicationManager* getCommunicationManager();

    typedef map<string, TIDORB*> ORBTable;

    TIDORB()throw (TIDThr::SystemException);

    ~TIDORB() throw (TIDThr::SystemException);

    static ::CORBA::ORB_ptr init(int& argc, char** argv, const char* orb_identifier);

    static ::CORBA::ORB_ptr default_ORB();

    ::TIDorb::portable::OutputStream* create_output_stream() const;
    ::CORBA::Any* create_any() const;

    const ConfORB& conf() const;

    const ORBServices& services();

    char* object_to_string(::CORBA::Object_ptr obj);

    ::CORBA::Object_ptr string_to_object(const char* str);

    void create_list(::CORBA::Long size, ::CORBA::NVList_out);
#ifndef MINIMUN
    void create_operation_list(::CORBA::OperationDef_ptr def, ::CORBA::NVList_out list);
#endif
    void create_named_value(::CORBA::NamedValue_out nv);

    void create_exception_list(::CORBA::ExceptionList_out list);

    void create_context_list(::CORBA::ContextList_out list);

    void get_default_context(::CORBA::Context_out list);

    void create_environment(::CORBA::Environment_out list);

    void send_multiple_requests_oneway( const ::CORBA::ORB::RequestSeq& seq);

    void send_multiple_requests_deferred(const ::CORBA::ORB::RequestSeq& seq);

    ::CORBA::Boolean poll_next_response();

    void get_next_response(::CORBA::Request_out req);

    ::CORBA::ORB::ObjectIdList_ptr list_initial_services();

    ::CORBA::Object_ptr resolve_initial_references(const char *identifier);

    ::CORBA::Boolean work_pending();

    void perform_work();

    void shutdown(::CORBA::Boolean wait_for_completion);

    void run();

    void destroy();

    TIDorb::core::PolicyManagerImpl* getPolicyManager();

    TIDorb::core::PolicyCurrentImpl* getPolicyCurrent();

    TIDorb::core::PolicyContextManagerImpl* getPolicyContextManager();

    TIDorb::core::compression::CompressionManagerImpl* getCompressionManager();

    TIDorb::core::DomainManagerImpl* getDefaultDomainManager();

    TIDorb::core::security::SecurityManagerImpl* getSecurityManager();

    TIDorb::core::RequestCounter* getRequestCounter();

    ::CORBA::Boolean get_service_information(::CORBA::ServiceType svc_type,
                                             ::CORBA::ServiceInformation_out svc_info);


    ::CORBA::Policy_ptr create_policy(::CORBA::PolicyType type, const ::CORBA::Any& val);

    // CORBA 2.3

    ::CORBA::ValueFactory register_value_factory(const char* id,
                                                 ::CORBA::ValueFactory factory);

    void unregister_value_factory(const char* id);

    ::CORBA::ValueFactory lookup_value_factory(const char* id);

    // CORBA 2.5

    ::CORBA::ORB::ORBId id();

    void register_initial_reference(const char* object_name,
                                    const ::CORBA::Object_ptr object);

    // CORBA 2.6

    ::CORBA::TypeCode_ptr create_struct_tc(const char* id,
                                           const char* name,
                                           const ::CORBA::StructMemberSeq& members);

    ::CORBA::TypeCode_ptr create_union_tc(const char* id,
                                          const char* name,
                                          ::CORBA::TypeCode_ptr discriminator_type,
                                          const ::CORBA::UnionMemberSeq& members);

    ::CORBA::TypeCode_ptr create_enum_tc(const char* id,
                                         const char* name,
                                         const ::CORBA::EnumMemberSeq& members);

    ::CORBA::TypeCode_ptr create_alias_tc(const char* id,
                                          const char* name,
                                          ::CORBA::TypeCode_ptr original_type);

    ::CORBA::TypeCode_ptr create_exception_tc(const char* id,
                                              const char* name,
                                              const ::CORBA::StructMemberSeq& members);

    ::CORBA::TypeCode_ptr create_interface_tc(const char* id,
                                              const char* name);

    ::CORBA::TypeCode_ptr create_string_tc(::CORBA::ULong bound);

    ::CORBA::TypeCode_ptr create_wstring_tc(::CORBA::ULong bound);

    ::CORBA::TypeCode_ptr create_fixed_tc(::CORBA::UShort digits, ::CORBA::Short scale);

    ::CORBA::TypeCode_ptr create_sequence_tc(::CORBA::ULong bound,
                                             ::CORBA::TypeCode_ptr element_type);
    // deprecated
    ::CORBA::TypeCode_ptr create_recursive_sequence_tc(::CORBA::ULong bound,
                                                       ::CORBA::ULong offset);

    ::CORBA::TypeCode_ptr create_array_tc(::CORBA::ULong length,
                                          ::CORBA::TypeCode_ptr element_type);

    ::CORBA::TypeCode_ptr create_value_tc(const char* id,
                                          const char* name,
                                          ::CORBA::ValueModifier type_modifier,
                                          ::CORBA::TypeCode_ptr concrete_base,
                                          const ::CORBA::ValueMemberSeq& members);

    ::CORBA::TypeCode_ptr create_value_box_tc(const char* id,
                                              const char* name,
                                              ::CORBA::TypeCode_ptr boxed_type);

    ::CORBA::TypeCode_ptr create_native_tc(const char* id,
                                           const char* name);

    ::CORBA::TypeCode_ptr create_recursive_tc(const char* id);

    ::CORBA::TypeCode_ptr create_abstract_interface_tc(const char* id,
                                                       const char* name);

    ::CORBA::TypeCode_ptr create_local_interface_tc(const char* id,
                                                    const char* name);

    // TIDORB

    bool destroyed() {return m_destroyed;}
    void do_complete_shutdown();

    CORBA::Object_ptr init_POA();
    CORBA::Object_ptr init_POACurrent();
    CORBA::Object_ptr init_DynAnyFactory();
    CORBA::Object_ptr init_CodecFactory();
    CORBA::Object_ptr init_PolicyManager();
    CORBA::Object_ptr init_PolicyCurrent();
    CORBA::Object_ptr init_CompressionManager();
    CORBA::Object_ptr init_DefaultDomainManager();
    CORBA::Object_ptr init_SecurityManager();

    void remove_POA();

    vector<PortableServer::POAManager_ptr> POAManagers;

    void print_trace(TIDorb::util::TraceLevel level, const char* message);
    void print_trace(TIDorb::util::TraceLevel level, const char** message, size_t elements);
    void print_trace(TIDorb::util::TraceLevel level, const char* message,
                     const CORBA::Exception& ex);
    void print_trace(TIDorb::util::TraceLevel level, const char* message,
                     const exception& ex);
    void print_trace(TIDorb::util::TraceLevel level, const char** message, size_t elements,
                     const exception& ex);

    void print_dump(TIDorb::util::TraceLevel level, const unsigned char* message, size_t length);
                     
    typecode::TypeCodeCache* getTypeCodeCache();
    
    /* Trace Service */
    TIDorb::util::Trace* trace;


  protected:

    void init_orb(int& argc, char** argv, const char* orb_identifier);
    void setup_initial_references();

    //URL resolution
    CORBA::Object_ptr file_url_to_object(const char* str) throw(CORBA::SystemException);
    CORBA::Object_ptr ior_to_object(const char* str) throw(CORBA::SystemException);
    CORBA::Object_ptr rir_url_to_object(const char* str) throw(CORBA::SystemException);
    CORBA::Object_ptr corbaname_url_to_object(const char* str) throw(CORBA::SystemException);
    CORBA::Object_ptr tidorb_url_to_object(const char* str) throw(CORBA::SystemException);
    CORBA::Object_ptr iiop_url_to_object(const char* str) throw(CORBA::SystemException);
    // MIOP extensions
    CORBA::Object_ptr miop_url_to_object(const char* str) throw(CORBA::SystemException);
    // end MIOP extensions
    CORBA::Object_ptr ssliop_url_to_object(const char* str) throw(CORBA::SystemException);

    //ORB instances tracking

    static TIDThr::RecursiveMutex* create_init_mutex();

    static ORBTable st_orb_instances;
    static TIDThr::RecursiveMutex* st_init_mutex;

    // Library correctly initializated
    static bool st_initialized;

    typecode::TypeCodeCache* m_typecode_cache;
    
    ConfORB m_conf;

    /* ORB Initial Services */

    ORBServices m_services;

    /* ORB Processing Status */

    ProcessingState m_state;

    /* ORB lifecycle */

    bool m_destroyed;

    /* CommunicationManager */
    TIDorb::core::comm::CommunicationManager* commManager;

    /* POA */

    TIDorb::core::poa::POAImpl* rootPOA;

    TIDorb::core::poa::CurrentImpl* current;
#ifndef MINIMUN
    DynamicAny::DynAnyFactory_ptr dyn_factory;
#endif
    IOP::CodecFactory_ptr codec_factory;

    TIDorb::core::PolicyManagerImpl* policy_manager;

    TIDorb::core::PolicyCurrentImpl* policy_current;

    TIDorb::core::PolicyContextManagerImpl* policy_context_manager;
    
    TIDorb::core::compression::CompressionManagerImpl* compression_manager;

    TIDorb::core::DomainManagerImpl* default_domain_manager;

    TIDorb::core::security::SecurityManagerImpl* security_manager;

    /* Maintains the order which the request was created */
    TIDorb::core::RequestCounter* requestCounter;

    /* TIDorbC++ Version number */
    static const char* st_version;

}; // class TIDORB

} //core
} //TIDorb

#endif
