//
// POA_CORBA.h (from module)
//


#ifndef POA_CORBA_H_
#define POA_CORBA_H_

#include "CORBA.h"
#include "PortableServer.h"

#ifdef TIDORB_HAVE_IOSTREAM
  #include <iostream>
#else
  #include <iostream.h>
#endif

#include <map>

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::map;
#endif

namespace POA_CORBA
{
	#include "POA_CORBA/Policy.h"
	#include "POA_CORBA/DomainManager.h"
	#include "POA_CORBA/ConstructionPolicy.h"
	#include "POA_CORBA/Current.h"
	#include "POA_CORBA/IRObject.h"
	#include "POA_CORBA/Contained.h"
	#include "POA_CORBA/Container.h"
	#include "POA_CORBA/IDLType.h"
	#include "POA_CORBA/Repository.h"
	#include "POA_CORBA/ModuleDef.h"
	#include "POA_CORBA/ConstantDef.h"
	#include "POA_CORBA/TypedefDef.h"
	#include "POA_CORBA/StructDef.h"
	#include "POA_CORBA/UnionDef.h"
	#include "POA_CORBA/EnumDef.h"
	#include "POA_CORBA/AliasDef.h"
	#include "POA_CORBA/NativeDef.h"
	#include "POA_CORBA/PrimitiveDef.h"
	#include "POA_CORBA/StringDef.h"
	#include "POA_CORBA/WstringDef.h"
	#include "POA_CORBA/FixedDef.h"
	#include "POA_CORBA/SequenceDef.h"
	#include "POA_CORBA/ArrayDef.h"
	#include "POA_CORBA/ExceptionDef.h"
	#include "POA_CORBA/AttributeDef.h"
	#include "POA_CORBA/OperationDef.h"
	#include "POA_CORBA/InterfaceDef.h"
	#include "POA_CORBA/ValueMemberDef.h"
	#include "POA_CORBA/ValueDef.h"
	#include "POA_CORBA/ValueBoxDef.h"
}

#endif //POA_CORBA_H_
