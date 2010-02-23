#include "CORBA.h"


#ifndef _CORBA_ORB_H_
#define _CORBA_ORB_H_


ORB_ptr ORB_init(int& argc, char** argv, const char* orb_identifier = "");

class ORB
{
  public:

    virtual ~ORB() {}

    virtual char* object_to_string(Object_ptr) = 0;

    virtual Object_ptr string_to_object(const char* str) = 0;

    virtual void create_list(Long size, NVList_out) = 0;

    virtual void create_operation_list(OperationDef_ptr def, NVList_out list) = 0;

    virtual void create_named_value(NamedValue_out nv) = 0;

    virtual void create_exception_list(ExceptionList_out list) = 0;

    virtual void create_context_list(ContextList_out list) = 0;

    virtual void get_default_context(Context_out ctx) = 0;

    virtual void create_environment(Environment_out env) = 0;

    #include "CORBA/ORB/RequestSeq.h"

    virtual void send_multiple_requests_oneway(const RequestSeq& seq) = 0;

    virtual void send_multiple_requests_deferred(const RequestSeq& seq) = 0;

    virtual Boolean poll_next_response() = 0;

    virtual void get_next_response(Request_out) = 0;

    // Obtaining initial object references
    class InvalidName;
    typedef InvalidName* InvalidName_ptr;
    typedef TIDorb::templates::InterfaceT_var<InvalidName> InvalidName_var;
    typedef TIDorb::templates::InterfaceT_out<InvalidName> InvalidName_out;

    #include "CORBA/ORB/ObjectId.h"
    #include "CORBA/ORB/ObjectIdList.h"
    #include "CORBA/ORB/InvalidName.h"

    virtual ObjectIdList_ptr list_initial_services() = 0;

    virtual Object_ptr resolve_initial_references(const char *identifier) = 0;

    virtual Boolean work_pending() = 0;

    virtual void perform_work() = 0;

    virtual void shutdown(Boolean wait_for_completion) = 0;

    virtual void run() = 0;

    virtual void destroy() = 0;

    virtual Boolean get_service_information(ServiceType svc_type,
                                            ServiceInformation_out svc_info) = 0;

    virtual Policy_ptr create_policy(PolicyType type, const Any& val) = 0;

    // CORBA 2.3

    virtual ValueFactory register_value_factory(const char* id,
                                                ValueFactory factory) = 0;

    virtual void unregister_value_factory(const char* id) = 0;

    virtual ValueFactory lookup_value_factory(const char* id) = 0;

    // CORBA 2.5

    typedef char* ORBId;
    virtual ORBId id() = 0;

    virtual void register_initial_reference(const char* object_name,
                                            const Object_ptr object) = 0;

    // CORBA 2.6

    virtual TypeCode_ptr create_struct_tc(const char* id,
                                          const char* name,
                                          const StructMemberSeq& members) = 0;

    virtual TypeCode_ptr create_union_tc(const char* id,
                                         const char* name,
                                         TypeCode_ptr discriminator_type,
                                         const UnionMemberSeq& members) = 0;

    virtual TypeCode_ptr create_enum_tc(const char* id,
                                        const char* name,
                                        const EnumMemberSeq& members) = 0;

    virtual TypeCode_ptr create_alias_tc(const char* id,
                                         const char* name,
                                         TypeCode_ptr original_type) = 0;

    virtual TypeCode_ptr create_exception_tc(const char* id,
                                             const char* name,
                                             const StructMemberSeq& members) = 0;

    virtual TypeCode_ptr create_interface_tc(const char* id,
                                             const char* name) = 0;

    virtual TypeCode_ptr create_string_tc(ULong bound) = 0;

    virtual TypeCode_ptr create_wstring_tc(ULong bound) = 0;

    virtual TypeCode_ptr create_fixed_tc(UShort digits, Short scale) = 0;

    virtual TypeCode_ptr create_sequence_tc(ULong bound, TypeCode_ptr element_type) = 0;

    // deprecated
    virtual TypeCode_ptr create_recursive_sequence_tc(ULong bound, ULong offset) = 0;

    virtual TypeCode_ptr create_array_tc(ULong length, TypeCode_ptr element_type) = 0;

    virtual TypeCode_ptr create_value_tc(const char* id,
                                         const char* name,
                                         ValueModifier type_modifier,
                                         TypeCode_ptr concrete_base,
                                         const ValueMemberSeq& members) = 0;

    virtual TypeCode_ptr create_value_box_tc(const char* id,
                                             const char* name,
                                             TypeCode_ptr boxed_type) = 0;

    virtual TypeCode_ptr create_native_tc(const char* id,
                                          const char* name) = 0;

    virtual TypeCode_ptr create_recursive_tc(const char* id) = 0;

    virtual TypeCode_ptr create_abstract_interface_tc(const char* id,
                                                      const char* name) = 0;

    virtual TypeCode_ptr create_local_interface_tc(const char* id,
                                                   const char* name) = 0;

    // Memory Management
    static ORB_ptr _duplicate(ORB_ptr orb);
    static ORB_ptr _nil();

};


#endif
