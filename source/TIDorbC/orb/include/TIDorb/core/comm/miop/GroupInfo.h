///////////////////////////////////////////////////////////////////////////
//
// File:        GroupInfo.h
// Description: 
// Rel:         01.00
// Created:     May, 2001
//
// Revised:
//
// Copyright 2001 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
///////////////////////////////////////////////////////////////////////////

#include "TIDorb/core/comm/miop.h"


#ifndef _TIDORB_CORE_COMM_MIOP_GROUPINFO_H_
#define _TIDORB_CORE_COMM_MIOP_GROUPINFO_H_


namespace TIDorb {
namespace core {
namespace comm {
namespace miop {

class GroupInfo : public virtual TIDorb::core::iop::TaggedComponent
{
private:
        char* _str;

public:
        Version          _version;
        char*            _group_domain_id;
        CORBA::ULongLong _object_group_id;
        CORBA::ULong     _object_group_ref_version;

        GroupInfo() throw (TIDThr::SystemException);
        GroupInfo(const Version& version,
                  const char* group_domain_id,
                  CORBA::ULongLong object_group_id,
                  CORBA::ULong object_group_ref_version) throw (TIDThr::SystemException);
        GroupInfo(const GroupInfo& other) throw (TIDThr::SystemException);

        ~GroupInfo() throw (TIDThr::SystemException);

	bool operator== (const GroupInfo& other) const;

        void write(TIDorb::core::cdr::CDROutputStream& out) const;

        void partial_read(TIDorb::core::cdr::CDRInputStream& input);

        const char* toString() const;

};
}// miop
}// comm
}// core
}// TIDorb

#endif
