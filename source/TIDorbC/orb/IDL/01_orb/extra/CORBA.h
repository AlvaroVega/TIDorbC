//
// CORBA.h (module)
//

#ifndef _CORBA_H_
#define _CORBA_H_

#include "TIDorb/config.h"
#include <unistd.h>

#include "CORBA/basic_types.h"

namespace CORBA {
    // Pseudo-objects forward declarations for template instantiation
    
    class ORB;
    typedef ORB* ORB_ptr;
    void release(ORB_ptr orb);
    Boolean is_nil(ORB_ptr orb);
    
    class Object;
    typedef Object* Object_ptr;    
    void release(Object_ptr obj);
    Boolean is_nil(Object_ptr obj);
    
    class TypeCode;
    typedef TypeCode* TypeCode_ptr;    
    void release(TypeCode_ptr obj);
    Boolean is_nil(TypeCode_ptr obj);    
    
    class Context;
    typedef Context* Context_ptr;    
    void release(Context_ptr obj);
    Boolean is_nil(Context_ptr obj);
    
    class ContextList;
    typedef ContextList* ContextList_ptr;    
    void release(ContextList_ptr cl);
    Boolean is_nil(ContextList_ptr cl);
    
    class NVList;
    typedef NVList* NVList_ptr;
    void release(NVList_ptr nv);
    Boolean is_nil(NVList_ptr nv);
    
    class NamedValue;
    typedef NamedValue* NamedValue_ptr; 
    void release(NamedValue_ptr nv);
    Boolean is_nil(NamedValue_ptr nv);
    
    class ExceptionList;
    typedef ExceptionList* ExceptionList_ptr;   
    void release(ExceptionList_ptr el);
    Boolean is_nil(ExceptionList_ptr el);

    class Request;
    typedef Request* Request_ptr;    
    void release(Request_ptr req);
    Boolean is_nil(Request_ptr req);   
    
    class Environment;
    typedef Environment* Environment_ptr;    
    void release(Environment_ptr ev);
    Boolean is_nil(Environment_ptr ev);

    class AbstractBase;
    typedef AbstractBase* AbstractBase_ptr; 
    Boolean is_nil(AbstractBase_ptr ab);
    void release(AbstractBase_ptr ab);

    class ValueBase;
    typedef ValueBase* ValueBase_ptr;    
    void add_ref(ValueBase_ptr vb);
    void remove_ref(ValueBase_ptr vb);

    class ValueFactoryBase;
    typedef ValueFactoryBase* ValueFactory;    
    void add_ref(ValueFactory vb);
    void remove_ref(ValueFactory vb);
   
    class ServerRequest;
    typedef ServerRequest* ServerRequest_ptr;    
    void release(ServerRequest_ptr obj);
    Boolean is_nil(ServerRequest_ptr obj);

}

#include "TIDorb/templates.h"
#include "CORBA/String.h" 
#include "CORBA/WString.h"
#include "CORBA/Fixed.h"

namespace CORBA
{
	// Begin of forward declarations

	class ServiceDetail;
	typedef ServiceDetail* ServiceDetail_ptr;    
	typedef ::TIDorb::templates::VariableSizeT_var<ServiceDetail> ServiceDetail_var;
	typedef ::TIDorb::templates::VariableSizeT_out<ServiceDetail> ServiceDetail_out;

	class ServiceInformation;
	typedef ServiceInformation* ServiceInformation_ptr;    
	typedef ::TIDorb::templates::VariableSizeT_var<ServiceInformation> ServiceInformation_var;
	typedef ::TIDorb::templates::VariableSizeT_out<ServiceInformation> ServiceInformation_out;

	class Policy;
	typedef Policy* Policy_ptr;    
	typedef ::TIDorb::templates::InterfaceT_var<Policy> Policy_var;
	typedef ::TIDorb::templates::InterfaceT_out<Policy> Policy_out;

	class PolicyError;
	typedef PolicyError* PolicyError_ptr;
    typedef ::TIDorb::templates::FixedSizeT_var<PolicyError> PolicyError_var;
	typedef PolicyError &PolicyError_out;

	class InvalidPolicies;
	typedef InvalidPolicies* InvalidPolicies_ptr;    
	typedef ::TIDorb::templates::VariableSizeT_var<InvalidPolicies> InvalidPolicies_var;
	typedef ::TIDorb::templates::VariableSizeT_out<InvalidPolicies> InvalidPolicies_out;

	class DomainManager;
	typedef DomainManager* DomainManager_ptr;    
	typedef ::TIDorb::templates::InterfaceT_var<DomainManager> DomainManager_var;
	typedef ::TIDorb::templates::InterfaceT_out<DomainManager> DomainManager_out;

	class ConstructionPolicy;
	typedef ConstructionPolicy* ConstructionPolicy_ptr;
    typedef ::TIDorb::templates::InterfaceT_var<ConstructionPolicy> ConstructionPolicy_var;
	typedef ::TIDorb::templates::InterfaceT_out<ConstructionPolicy> ConstructionPolicy_out;

	class Current;
	typedef Current* Current_ptr;    
	typedef ::TIDorb::templates::InterfaceT_var<Current> Current_var;
	typedef ::TIDorb::templates::InterfaceT_out<Current> Current_out;

	// TODO: uncomment StringValue forward declarations
	//class StringValue;
	//typedef StringValue* StringValue_ptr;
    //void release(StringValue_ptr obj);
    //Boolean is_nil(StringValue_ptr obj);
	//typedef ::TIDorb::templates::RefCount_var<StringValue> StringValue_var;
	//typedef ::TIDorb::templates::RefCount_out<StringValue> StringValue_out;

	// TODO: uncomment WStringValue forward declarations
	//class WStringValue;
	//typedef WStringValue* WStringValue_ptr;
    //void release(WStringValue_ptr obj);
    //Boolean is_nil(WStringValue_ptr obj);
	//typedef ::TIDorb::templates::RefCount_var<WStringValue> WStringValue_var;
	//typedef ::TIDorb::templates::RefCount_out<WStringValue> WStringValue_out;

	class IRObject;
	typedef IRObject* IRObject_ptr;    
	typedef ::TIDorb::templates::InterfaceT_var<IRObject> IRObject_var;
	typedef ::TIDorb::templates::InterfaceT_out<IRObject> IRObject_out;

	class Contained;
	typedef Contained* Contained_ptr;    
	typedef ::TIDorb::templates::InterfaceT_var<Contained> Contained_var;
	typedef ::TIDorb::templates::InterfaceT_out<Contained> Contained_out;

	class StructMember;
	typedef StructMember* StructMember_ptr;    
	typedef ::TIDorb::templates::VariableSizeT_var<StructMember> StructMember_var;
	typedef ::TIDorb::templates::VariableSizeT_out<StructMember> StructMember_out;

	class Initializer;
	typedef Initializer* Initializer_ptr;    
	typedef ::TIDorb::templates::VariableSizeT_var<Initializer> Initializer_var;
	typedef ::TIDorb::templates::VariableSizeT_out<Initializer> Initializer_out;

	class UnionMember;
	typedef UnionMember* UnionMember_ptr;    
	typedef ::TIDorb::templates::VariableSizeT_var<UnionMember> UnionMember_var;
	typedef ::TIDorb::templates::VariableSizeT_out<UnionMember> UnionMember_out;

	class Container;
	typedef Container* Container_ptr;    
	typedef ::TIDorb::templates::InterfaceT_var<Container> Container_var;
	typedef ::TIDorb::templates::InterfaceT_out<Container> Container_out;

	class IDLType;
	typedef IDLType* IDLType_ptr;    
	typedef ::TIDorb::templates::InterfaceT_var<IDLType> IDLType_var;
	typedef ::TIDorb::templates::InterfaceT_out<IDLType> IDLType_out;

	class Repository;
	typedef Repository* Repository_ptr;    
	typedef ::TIDorb::templates::InterfaceT_var<Repository> Repository_var;
	typedef ::TIDorb::templates::InterfaceT_out<Repository> Repository_out;

	class ModuleDef;
	typedef ModuleDef* ModuleDef_ptr;    
	typedef ::TIDorb::templates::InterfaceT_var<ModuleDef> ModuleDef_var;
	typedef ::TIDorb::templates::InterfaceT_out<ModuleDef> ModuleDef_out;

	class ModuleDescription;
	typedef ModuleDescription* ModuleDescription_ptr;    
	typedef ::TIDorb::templates::VariableSizeT_var<ModuleDescription> ModuleDescription_var;
	typedef ::TIDorb::templates::VariableSizeT_out<ModuleDescription> ModuleDescription_out;

	class ConstantDef;
	typedef ConstantDef* ConstantDef_ptr;    
	typedef ::TIDorb::templates::InterfaceT_var<ConstantDef> ConstantDef_var;
	typedef ::TIDorb::templates::InterfaceT_out<ConstantDef> ConstantDef_out;

	class ConstantDescription;
	typedef ConstantDescription* ConstantDescription_ptr;    
	typedef ::TIDorb::templates::VariableSizeT_var<ConstantDescription> ConstantDescription_var;
	typedef ::TIDorb::templates::VariableSizeT_out<ConstantDescription> ConstantDescription_out;

	class TypedefDef;
	typedef TypedefDef* TypedefDef_ptr;    
	typedef ::TIDorb::templates::InterfaceT_var<TypedefDef> TypedefDef_var;
	typedef ::TIDorb::templates::InterfaceT_out<TypedefDef> TypedefDef_out;

	class TypeDescription;
	typedef TypeDescription* TypeDescription_ptr;    
	typedef ::TIDorb::templates::VariableSizeT_var<TypeDescription> TypeDescription_var;
	typedef ::TIDorb::templates::VariableSizeT_out<TypeDescription> TypeDescription_out;

	class StructDef;
	typedef StructDef* StructDef_ptr;
	typedef ::TIDorb::templates::InterfaceT_var<StructDef> StructDef_var;
	typedef ::TIDorb::templates::InterfaceT_out<StructDef> StructDef_out;

	class UnionDef;
	typedef UnionDef* UnionDef_ptr;
	typedef ::TIDorb::templates::InterfaceT_var<UnionDef> UnionDef_var;
	typedef ::TIDorb::templates::InterfaceT_out<UnionDef> UnionDef_out;

	class EnumDef;
	typedef EnumDef* EnumDef_ptr;
	typedef ::TIDorb::templates::InterfaceT_var<EnumDef> EnumDef_var;
	typedef ::TIDorb::templates::InterfaceT_out<EnumDef> EnumDef_out;

	class AliasDef;
	typedef AliasDef* AliasDef_ptr;
	typedef ::TIDorb::templates::InterfaceT_var<AliasDef> AliasDef_var;
	typedef ::TIDorb::templates::InterfaceT_out<AliasDef> AliasDef_out;

	class NativeDef;
	typedef NativeDef* NativeDef_ptr;
	typedef ::TIDorb::templates::InterfaceT_var<NativeDef> NativeDef_var;
	typedef ::TIDorb::templates::InterfaceT_out<NativeDef> NativeDef_out;

	class PrimitiveDef;
	typedef PrimitiveDef* PrimitiveDef_ptr;
	typedef ::TIDorb::templates::InterfaceT_var<PrimitiveDef> PrimitiveDef_var;
	typedef ::TIDorb::templates::InterfaceT_out<PrimitiveDef> PrimitiveDef_out;

	class StringDef;
	typedef StringDef* StringDef_ptr;
	typedef ::TIDorb::templates::InterfaceT_var<StringDef> StringDef_var;
	typedef ::TIDorb::templates::InterfaceT_out<StringDef> StringDef_out;

	class WstringDef;
	typedef WstringDef* WstringDef_ptr;
	typedef ::TIDorb::templates::InterfaceT_var<WstringDef> WstringDef_var;
	typedef ::TIDorb::templates::InterfaceT_out<WstringDef> WstringDef_out;

	class FixedDef;
	typedef FixedDef* FixedDef_ptr;
	typedef ::TIDorb::templates::InterfaceT_var<FixedDef> FixedDef_var;
	typedef ::TIDorb::templates::InterfaceT_out<FixedDef> FixedDef_out;

	class SequenceDef;
	typedef SequenceDef* SequenceDef_ptr;
	typedef ::TIDorb::templates::InterfaceT_var<SequenceDef> SequenceDef_var;
	typedef ::TIDorb::templates::InterfaceT_out<SequenceDef> SequenceDef_out;

	class ArrayDef;
	typedef ArrayDef* ArrayDef_ptr;
	typedef ::TIDorb::templates::InterfaceT_var<ArrayDef> ArrayDef_var;
	typedef ::TIDorb::templates::InterfaceT_out<ArrayDef> ArrayDef_out;

	class ExceptionDef;
	typedef ExceptionDef* ExceptionDef_ptr;
	typedef ::TIDorb::templates::InterfaceT_var<ExceptionDef> ExceptionDef_var;
	typedef ::TIDorb::templates::InterfaceT_out<ExceptionDef> ExceptionDef_out;

	class ExceptionDescription;
	typedef ExceptionDescription* ExceptionDescription_ptr;    
	typedef ::TIDorb::templates::VariableSizeT_var<ExceptionDescription> ExceptionDescription_var;
	typedef ::TIDorb::templates::VariableSizeT_out<ExceptionDescription> ExceptionDescription_out;

	class AttributeDef;
	typedef AttributeDef* AttributeDef_ptr;
	typedef ::TIDorb::templates::InterfaceT_var<AttributeDef> AttributeDef_var;
	typedef ::TIDorb::templates::InterfaceT_out<AttributeDef> AttributeDef_out;

	class AttributeDescription;
	typedef AttributeDescription* AttributeDescription_ptr;    
	typedef ::TIDorb::templates::VariableSizeT_var<AttributeDescription> AttributeDescription_var;
	typedef ::TIDorb::templates::VariableSizeT_out<AttributeDescription> AttributeDescription_out;

	class ParameterDescription;
	typedef ParameterDescription* ParameterDescription_ptr;    
	typedef ::TIDorb::templates::VariableSizeT_var<ParameterDescription> ParameterDescription_var;
	typedef ::TIDorb::templates::VariableSizeT_out<ParameterDescription> ParameterDescription_out;

	class OperationDef;
	typedef OperationDef* OperationDef_ptr;
	typedef ::TIDorb::templates::InterfaceT_var<OperationDef> OperationDef_var;
	typedef ::TIDorb::templates::InterfaceT_out<OperationDef> OperationDef_out;

	class OperationDescription;
	typedef OperationDescription* OperationDescription_ptr;    
	typedef ::TIDorb::templates::VariableSizeT_var<OperationDescription> OperationDescription_var;
	typedef ::TIDorb::templates::VariableSizeT_out<OperationDescription> OperationDescription_out;

	class InterfaceDef;
	typedef InterfaceDef* InterfaceDef_ptr;
	typedef ::TIDorb::templates::InterfaceT_var<InterfaceDef> InterfaceDef_var;
	typedef ::TIDorb::templates::InterfaceT_out<InterfaceDef> InterfaceDef_out;

	class InterfaceDescription;
	typedef InterfaceDescription* InterfaceDescription_ptr;
    typedef ::TIDorb::templates::VariableSizeT_var<InterfaceDescription> InterfaceDescription_var;
	typedef ::TIDorb::templates::VariableSizeT_out<InterfaceDescription> InterfaceDescription_out;

	class ValueMember;
	typedef ValueMember* ValueMember_ptr;
    typedef ::TIDorb::templates::VariableSizeT_var<ValueMember> ValueMember_var;
	typedef ::TIDorb::templates::VariableSizeT_out<ValueMember> ValueMember_out;

	class ValueMemberDef;
	typedef ValueMemberDef* ValueMemberDef_ptr;
	typedef ::TIDorb::templates::InterfaceT_var<ValueMemberDef> ValueMemberDef_var;
	typedef ::TIDorb::templates::InterfaceT_out<ValueMemberDef> ValueMemberDef_out;

	class ValueDef;
	typedef ValueDef* ValueDef_ptr;
	typedef ::TIDorb::templates::InterfaceT_var<ValueDef> ValueDef_var;
	typedef ::TIDorb::templates::InterfaceT_out<ValueDef> ValueDef_out;

	class ValueDescription;
	typedef ValueDescription* ValueDescription_ptr;
    typedef ::TIDorb::templates::VariableSizeT_var<ValueDescription> ValueDescription_var;
	typedef ::TIDorb::templates::VariableSizeT_out<ValueDescription> ValueDescription_out;

	class ValueBoxDef;
	typedef ValueBoxDef* ValueBoxDef_ptr;
	typedef ::TIDorb::templates::InterfaceT_var<ValueBoxDef> ValueBoxDef_var;
	typedef ::TIDorb::templates::InterfaceT_out<ValueBoxDef> ValueBoxDef_out;
	 
	typedef ::TIDorb::templates::InterfaceT_var<TypeCode> TypeCode_var;
	typedef ::TIDorb::templates::InterfaceT_out<TypeCode> TypeCode_out;

	class BadFixedValue;
	typedef BadFixedValue* BadFixedValue_ptr;
    typedef ::TIDorb::templates::FixedSizeT_var<BadFixedValue> BadFixedValue_var;
	typedef BadFixedValue &BadFixedValue_out;

	class DataInputStream;
	typedef DataInputStream* DataInputStream_ptr;       
    typedef ::TIDorb::templates::RefCount_var<DataInputStream> DataInputStream_var;
	typedef ::TIDorb::templates::RefCount_out<DataInputStream> DataInputStream_out;

	class DataOutputStream;
	typedef DataOutputStream* DataOutputStream_ptr;    
	typedef ::TIDorb::templates::RefCount_var<DataOutputStream> DataOutputStream_var;
	typedef ::TIDorb::templates::RefCount_out<DataOutputStream> DataOutputStream_out;

	class CustomMarshal;
	typedef CustomMarshal* CustomMarshal_ptr;    
    typedef ::TIDorb::templates::RefCount_var<CustomMarshal> CustomMarshal_var;
	typedef ::TIDorb::templates::RefCount_out<CustomMarshal> CustomMarshal_out;

	class PolicyManager;
    typedef PolicyManager* PolicyManager_ptr;         
	typedef TIDorb::templates::InterfaceT_var<PolicyManager> PolicyManager_var;
	typedef TIDorb::templates::InterfaceT_out<PolicyManager> PolicyManager_out;

	class PolicyCurrent;
	typedef PolicyCurrent* PolicyCurrent_ptr;    
	typedef TIDorb::templates::InterfaceT_var<PolicyCurrent> PolicyCurrent_var;
	typedef TIDorb::templates::InterfaceT_out<PolicyCurrent> PolicyCurrent_out; 

	// End of forward declarations


	// PSEUDO OBJECTS memory handlers
        
    typedef ::TIDorb::templates::InterfaceT_var<ORB> ORB_var;
    typedef ::TIDorb::templates::InterfaceT_out<ORB> ORB_out;
    
	typedef TIDorb::templates::InterfaceT_var<Context> Context_var;
	typedef TIDorb::templates::InterfaceT_out<Context> Context_out;
	
	typedef TIDorb::templates::InterfaceT_var<ContextList> ContextList_var;
	typedef TIDorb::templates::InterfaceT_out<ContextList> ContextList_out;
    
    //Object  
	typedef TIDorb::templates::InterfaceT_var<Object> Object_var;
	typedef TIDorb::templates::InterfaceT_out<Object> Object_out;

	class LocalObject;
	typedef LocalObject* LocalObject_ptr;    
	typedef TIDorb::templates::InterfaceT_var<LocalObject> LocalObject_var;
	typedef TIDorb::templates::InterfaceT_out<LocalObject> LocalObject_out;

  	class Any;
	typedef Any* Any_ptr;
	typedef TIDorb::templates::VariableSizeT_var<Any> Any_var;
	typedef TIDorb::templates::VariableSizeT_out<Any> Any_out;
	    
	typedef TIDorb::templates::InterfaceT_var<NVList> NVList_var;
	typedef TIDorb::templates::InterfaceT_out<NVList> NVList_out;
		
    typedef TIDorb::templates::InterfaceT_var<NamedValue> NamedValue_var;
	typedef TIDorb::templates::InterfaceT_out<NamedValue> NamedValue_out;
	
    typedef TIDorb::templates::InterfaceT_var<ExceptionList> ExceptionList_var;
	typedef TIDorb::templates::InterfaceT_out<ExceptionList> ExceptionList_out;
		
    typedef TIDorb::templates::InterfaceT_var<Request> Request_var;
	typedef TIDorb::templates::InterfaceT_out<Request> Request_out;
    
    typedef TIDorb::templates::InterfaceT_var<ServerRequest> ServerRequest_var;
    typedef TIDorb::templates::InterfaceT_out<ServerRequest> ServerRequest_out;
   
	typedef TIDorb::templates::InterfaceT_var<Environment> Environment_var;
	typedef TIDorb::templates::InterfaceT_out<Environment> Environment_out;

	class Exception;
	typedef Exception* Exception_ptr;
	typedef TIDorb::templates::VariableSizeT_var<Exception> Exception_var;
	typedef TIDorb::templates::VariableSizeT_out<Exception> Exception_out;

	class UserException;
	typedef UserException* UserException_ptr;
	typedef TIDorb::templates::VariableSizeT_var<UserException> UserException_var;
	typedef TIDorb::templates::VariableSizeT_out<UserException> UserException_out;

	class SystemException;
	typedef SystemException* SystemException_ptr;
	typedef TIDorb::templates::VariableSizeT_var<SystemException> SystemException_var;
	typedef TIDorb::templates::VariableSizeT_out<SystemException> SystemException_out;
	
    typedef TIDorb::templates::InterfaceT_var<AbstractBase> AbstractBase_var;
	typedef TIDorb::templates::InterfaceT_out<AbstractBase> AbstractBase_out;
   
	typedef TIDorb::templates::RefCount_var<ValueBase> ValueBase_var;
	typedef TIDorb::templates::RefCount_out<ValueBase> ValueBase_out;
   
	typedef TIDorb::templates::RefCount_var<ValueFactoryBase> ValueFactory_var;
	typedef TIDorb::templates::RefCount_out<ValueFactoryBase> ValueFactory_out;

	class DefaultValueRefCountBase;
    typedef TIDorb::templates::RefCount_var<DefaultValueRefCountBase> DefaultValueRefCountBase_var;
    typedef TIDorb::templates::RefCount_out<DefaultValueRefCountBase> DefaultValueRefCountBase_out;
 

	// End of forward declarations

} //namespace CORBA


#include "TIDorb/portable.h"
#include "TIDorb/types.h"


namespace CORBA
{
	#include "CORBA/InterfaceDefSeq.h"
	#include "CORBA/ValueDefSeq.h"
	#include "CORBA/ContainedSeq.h"
	#include "CORBA/BooleanSeq.h"
	#include "CORBA/CharSeq.h"
	#include "CORBA/WCharSeq.h"
	#include "CORBA/OctetSeq.h"
	#include "CORBA/ShortSeq.h"
	#include "CORBA/UShortSeq.h"
	#include "CORBA/LongSeq.h"
	#include "CORBA/ULongSeq.h"
	#include "CORBA/LongLongSeq.h"
	#include "CORBA/ULongLongSeq.h"
	#include "CORBA/LongDoubleSeq.h"
	#include "CORBA/FloatSeq.h"
	#include "CORBA/DoubleSeq.h"
	#include "CORBA/StringSeq.h"
	#include "CORBA/WStringSeq.h"
	#include "CORBA/RepositoryId.h"
	#include "CORBA/RepositoryIdSeq.h"
	#include "CORBA/ScopedName.h"
	#include "CORBA/Flags.h"
	#include "CORBA/ARG_IN.h"
	#include "CORBA/ARG_OUT.h"
	#include "CORBA/ARG_INOUT.h"
	#include "CORBA/CTX_RESTRICT_SCOPE.h"
	#include "CORBA/Identifier.h"
	#include "CORBA/OMGVMCID.h"
	#include "CORBA/SetOverrideType.h"
	#include "CORBA/Visibility.h"
	#include "CORBA/PRIVATE_MEMBER.h"
	#include "CORBA/PUBLIC_MEMBER.h"
	#include "CORBA/CompletionStatus.h"
	#include "CORBA/Exception.h"
	#include "CORBA/UserException.h"
	#include "CORBA/SystemException.h"
	#include "CORBA/sys_excepts.h"
	#include "CORBA/ValueBase.h"
	#include "CORBA/DefaultValueRefCountBase.h"
	#include "CORBA/AbstractBase.h"
	#include "CORBA/ValueFactory.h"
	#include "CORBA/Any.h"
	#include "CORBA/AnySeq.h"
	#include "CORBA/UnknownUserException.h"

	#include "CORBA/ServiceType.h"
	#include "CORBA/Security.h"
	#include "CORBA/ServiceOption.h"
	#include "CORBA/ServiceDetailType.h"
	#include "CORBA/ServiceDetail.h"
	#include "CORBA/ServiceInformation.h"
	#include "CORBA/PolicyType.h"

	#include "CORBA/DomainManagerList.h"
	#include "CORBA/PolicyList.h"
	#include "CORBA/PolicyTypeSeq.h"
	#include "CORBA/InvalidPolicies.h"

	#include "CORBA/Object.h"
	#include "CORBA/LocalObject.h"

} //namespace CORBA


#include "TIDorb/portable/Stub.h"


namespace CORBA
{
	#include "CORBA/Policy.h"
	// Interface Stub Inclusion (Client Side).
	#include "CORBA/_PolicyStub.h"

	#include "CORBA/PolicyErrorCode.h"
	#include "CORBA/BAD_POLICY.h"
	#include "CORBA/UNSUPPORTED_POLICY.h"
	#include "CORBA/BAD_POLICY_TYPE.h"
	#include "CORBA/BAD_POLICY_VALUE.h"
	#include "CORBA/UNSUPPORTED_POLICY_VALUE.h"

	#include "CORBA/PolicyError.h"

	#include "CORBA/DomainManager.h"
	#include "CORBA/SecConstruction.h"
	// Interface Stub Inclusion (Client Side).
	#include "CORBA/_DomainManagerStub.h"

	#include "CORBA/ConstructionPolicy.h"
	// Interface Stub Inclusion (Client Side).
	#include "CORBA/_ConstructionPolicyStub.h"

	#include "CORBA/Current.h"
	// Interface Stub Inclusion (Client Side).
	#include "CORBA/_CurrentStub.h"

	// TODO: uncomment StringValue and WStringValue header inclusion
	//#include "CORBA/StringValue.h"
	//#include "CORBA/WStringValue.h"

	#include "CORBA/DefinitionKind.h"
	#include "CORBA/TCKind.h"
	#include "CORBA/ValueModifier.h"
	#include "CORBA/VM_NONE.h"
	#include "CORBA/VM_CUSTOM.h"
	#include "CORBA/VM_ABSTRACT.h"
	#include "CORBA/VM_TRUNCATABLE.h"
	#include "CORBA/TypeCode.h"
	#include "CORBA/IRObject.h"
	#include "CORBA/IDLType.h"
	// Interface Stub Inclusion (Client Side).
	#include "CORBA/_IRObjectStub.h"

	#include "CORBA/VersionSpec.h"
	#include "CORBA/Contained.h"
	// Interface Stub Inclusion (Client Side).
	#include "CORBA/_ContainedStub.h"

	#include "CORBA/StructMember.h"
	#include "CORBA/StructMemberSeq.h"
	#include "CORBA/Initializer.h"
	#include "CORBA/InitializerSeq.h"
	#include "CORBA/UnionMember.h"
	#include "CORBA/UnionMemberSeq.h"
	#include "CORBA/EnumMemberSeq.h"
	#include "CORBA/Container.h"
	// Interface Stub Inclusion (Client Side).
	#include "CORBA/_ContainerStub.h"

	// Interface Stub Inclusion (Client Side).
	#include "CORBA/_IDLTypeStub.h"

	#include "CORBA/PrimitiveKind.h"
	#include "CORBA/Repository.h"
	// Interface Stub Inclusion (Client Side).
	#include "CORBA/_RepositoryStub.h"

	#include "CORBA/ModuleDef.h"
	// Interface Stub Inclusion (Client Side).
	#include "CORBA/_ModuleDefStub.h"

	#include "CORBA/ModuleDescription.h"
	#include "CORBA/ConstantDef.h"
	// Interface Stub Inclusion (Client Side).
	#include "CORBA/_ConstantDefStub.h"

	#include "CORBA/ConstantDescription.h"
	#include "CORBA/TypedefDef.h"
	// Interface Stub Inclusion (Client Side).
	#include "CORBA/_TypedefDefStub.h"

	#include "CORBA/TypeDescription.h"
	#include "CORBA/StructDef.h"
	// Interface Stub Inclusion (Client Side).
	#include "CORBA/_StructDefStub.h"

	#include "CORBA/UnionDef.h"
	// Interface Stub Inclusion (Client Side).
	#include "CORBA/_UnionDefStub.h"

	#include "CORBA/EnumDef.h"
	// Interface Stub Inclusion (Client Side).
	#include "CORBA/_EnumDefStub.h"

	#include "CORBA/AliasDef.h"
	// Interface Stub Inclusion (Client Side).
	#include "CORBA/_AliasDefStub.h"

	#include "CORBA/NativeDef.h"
	// Interface Stub Inclusion (Client Side).
	#include "CORBA/_NativeDefStub.h"

	#include "CORBA/PrimitiveDef.h"
	// Interface Stub Inclusion (Client Side).
	#include "CORBA/_PrimitiveDefStub.h"

	#include "CORBA/StringDef.h"
	// Interface Stub Inclusion (Client Side).
	#include "CORBA/_StringDefStub.h"
	#include "CORBA/WstringDef.h"
	// Interface Stub Inclusion (Client Side).
	#include "CORBA/_WstringDefStub.h"

	#include "CORBA/FixedDef.h"
	// Interface Stub Inclusion (Client Side).
	#include "CORBA/_FixedDefStub.h"

	#include "CORBA/SequenceDef.h"
	// Interface Stub Inclusion (Client Side).
	#include "CORBA/_SequenceDefStub.h"

	#include "CORBA/ArrayDef.h"
	// Interface Stub Inclusion (Client Side).
	#include "CORBA/_ArrayDefStub.h"

	#include "CORBA/ExceptionDef.h"
	// Interface Stub Inclusion (Client Side).
	#include "CORBA/_ExceptionDefStub.h"

	#include "CORBA/ExceptionDescription.h"
	#include "CORBA/AttributeMode.h"
	#include "CORBA/AttributeDef.h"
	// Interface Stub Inclusion (Client Side).
	#include "CORBA/_AttributeDefStub.h"

	#include "CORBA/AttributeDescription.h"
	#include "CORBA/OperationMode.h"
	#include "CORBA/ParameterMode.h"
	#include "CORBA/ParameterDescription.h"
	#include "CORBA/ParDescriptionSeq.h"
	#include "CORBA/ContextIdentifier.h"
	#include "CORBA/ContextIdSeq.h"
	#include "CORBA/ExceptionDefSeq.h"
	#include "CORBA/ExcDescriptionSeq.h"
	#include "CORBA/OperationDef.h"
	// Interface Stub Inclusion (Client Side).
	#include "CORBA/_OperationDefStub.h"

	#include "CORBA/OperationDescription.h"
	#include "CORBA/OpDescriptionSeq.h"
	#include "CORBA/AttrDescriptionSeq.h"
	#include "CORBA/InterfaceDef.h"
	// Interface Stub Inclusion (Client Side).
	#include "CORBA/_InterfaceDefStub.h"

	#include "CORBA/InterfaceDescription.h"

	#include "CORBA/ValueMember.h"
	#include "CORBA/ValueMemberSeq.h"
	#include "CORBA/ValueMemberDef.h"
	// Interface Stub Inclusion (Client Side).
	#include "CORBA/_ValueMemberDefStub.h"

	#include "CORBA/ValueDef.h"
	// Interface Stub Inclusion (Client Side).
	#include "CORBA/_ValueDefStub.h"

	#include "CORBA/ValueDescription.h"
	#include "CORBA/ValueBoxDef.h"
	// Interface Stub Inclusion (Client Side).
	#include "CORBA/_ValueBoxDefStub.h"

	#include "CORBA/BadFixedValue.h"

	// Custom valuetypes marshalling
	#include "CORBA/DataInputStream.h"
	#include "CORBA/DataOutputStream.h"
	#include "CORBA/CustomMarshal.h"

	#include "CORBA/PolicyManager.h"
	#include "CORBA/PolicyCurrent.h"

	// PSEUDO OBJECTS
	#include "CORBA/ORB.h"
	#include "CORBA/Context.h"
	#include "CORBA/NVList.h"
	#include "CORBA/NamedValue.h"
	#include "CORBA/ExceptionList.h"
	#include "CORBA/ContextList.h"
	#include "CORBA/Environment.h"
	#include "CORBA/NVList.h"
	#include "CORBA/Request.h"
	#include "CORBA/ServerRequest.h"

} //namespace CORBA

#include "TIDorb/portable/ORB.h"
#include "TIDorb/portable/TypeCodeFactory.h"
#include "CORBA_ext.h"

#endif // _CORBA_H_
