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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
///////////////////////////////////////////////////////////////////////////////////

#include "CORBA.h"
#include "PortableServer.h"
#include "DynamicAny.h"

#include "TIDorb/core.h"
//MLG
#include "TIDorb/core/typecode/TypeCodeCache.h"
//EMLG
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


class TIDORB : public TIDorb::portable::ORB,
               public TIDThr::Monitor
{
  friend class CORBA::ORB;

  public:

    TIDorb::core::comm::CommunicationManager* getCommunicationManager();

    typedef map<string, TIDORB*> ORBTable;

    TIDORB()throw (TIDThr::SystemException);

    ~TIDORB() throw (TIDThr::SystemException);

    static ::CORBA::ORB_ptr init(int& argc, char** argv, const char* orb_identifier);

    static ::CORBA::ORB_ptr default_ORB();

    //PRA
    //::TIDorb::portable::OutputStream* create_output_stream();
    //::CORBA::Any* create_any();
    ::TIDorb::portable::OutputStream* create_output_stream() const;
    ::CORBA::Any* create_any() const;
    //EPRA

    const ConfORB& conf() const;

    const ORBServices& services();

    char* object_to_string(::CORBA::Object_ptr obj);

    ::CORBA::Object_ptr string_to_object(const char* str);

    void create_list(::CORBA::Long size, ::CORBA::NVList_out);

    void create_operation_list(::CORBA::OperationDef_ptr def, ::CORBA::NVList_out list);

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
                     
//MLG
    typecode::TypeCodeCache* getTypeCodeCache();
//EMLG
    
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
    // pra@tid.es - MIOP extensions
    CORBA::Object_ptr miop_url_to_object(const char* str) throw(CORBA::SystemException);
    // end MIOP extensions


    //ORB instances tracking

    static TIDThr::RecursiveMutex* create_init_mutex();

    static ORBTable st_orb_instances;
    static TIDThr::RecursiveMutex* st_init_mutex;

//MLG
    typecode::TypeCodeCache* m_typecode_cache;
//EMLG
    
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

    DynamicAny::DynAnyFactory_ptr dyn_factory;

    IOP::CodecFactory_ptr codec_factory;

    TIDorb::core::PolicyManagerImpl* policy_manager;

    TIDorb::core::PolicyCurrentImpl* policy_current;

    TIDorb::core::PolicyContextManagerImpl* policy_context_manager;
    
    TIDorb::core::compression::CompressionManagerImpl* compression_manager;

    /* Maintains the order which the request was created */
    TIDorb::core::RequestCounter* requestCounter;

    /* TIDorbC++ Version number */
    static const char* st_version;

}; // class TIDORB

} //core
} //TIDorb

#endif
