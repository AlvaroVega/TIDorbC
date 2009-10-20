//////////////////////////////////////////////////////////////////////////////////
//
// File:        PolicyContext.h
// Description: PolicyContext implementation.
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

#ifndef _TIDORB_CORE_POLICY_CONTEXT_H
#define _TIDORB_CORE_POLICY_CONTEXT_H

#include "CORBA.h"
#include "TIDThr.h"

#ifdef __SUNPRO_LIBRARY_IOSTREAM
#define __STD_ISTREAM__
#define __STD_OSTREAM__
#endif

#include <map>

#ifdef __SUNPRO_LIBRARY_IOSTREAM
#undef __STD_ISTREAM__
#undef __STD_OSTREAM__
#endif

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
  using std::map;
#endif


namespace TIDorb {
namespace core {

class PolicyContext: public virtual TIDThr::RecursiveMutex
{
    public:

    PolicyContext() throw (TIDThr::SystemException);

    ~PolicyContext() throw (TIDThr::SystemException);

    PolicyContext(TIDorb::core::PolicyContext* father_context); 

    void setFatherContext(TIDorb::core::PolicyContext* father_context);

    void setPolicies(const CORBA::PolicyList& policies,
                     CORBA::SetOverrideType set_add)
        throw (CORBA::InvalidPolicies);

    CORBA::Policy_ptr getPolicy(CORBA::PolicyType type) const;

    CORBA::PolicyList* getPolicies(const CORBA::PolicyTypeSeq& ts) const;

    void setPolicy(CORBA::Policy_ptr policy);
   
    void removePolicy(CORBA::PolicyType type);

    void reset();

    PolicyContext* duplicate();

    void partialRead(TIDorb::core::cdr::CDRInputStream& input);

    void write(TIDorb::core::cdr::CDROutputStream& output) const;

    void dump(ostream& out);

    size_t getSize();

    TIDorb::core::comm::iiop::InvocationPoliciesContext* getInvocationPolicyServiceContext();


    protected:

      typedef map<CORBA::PolicyType, CORBA::Policy_var> PolicyTableT;

      PolicyTableT m_policy_table;

      PolicyContext* m_father_context;
};


} //core
} //TIDorb

#endif
