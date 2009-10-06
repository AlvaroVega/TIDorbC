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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
