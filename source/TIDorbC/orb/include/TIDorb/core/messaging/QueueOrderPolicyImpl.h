/////////////////////////////////////////////////////////////////////////
//
// File:        QueueOrderPolicyImpl.h
// Description:
//              
// Rel:         01.00
// Created:     November, 2005
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

#include "TIDorb/core/messaging.h"

#ifndef _TIDORB_CORE_MESSAGING_QUEUE_ORDER_POLICY_IMPL_H_
#define _TIDORB_CORE_MESSAGING_QUEUE_ORDER_POLICY_IMPL_H_

namespace TIDorb {
namespace core {
namespace messaging {

class QueueOrderPolicyImpl: public virtual Messaging::QueueOrderPolicy,
                            public virtual CORBA::LocalObject,
                            virtual public TIDThr::RefCounter {

private:

  Messaging::Ordering m_allowedOrders;

  bool destroyed;

public:
  
  /**
   * @param allowedOrders
   */
  QueueOrderPolicyImpl(Messaging::Ordering allowedOrders);

  ~QueueOrderPolicyImpl() throw (TIDThr::SystemException);
  
  Messaging::Ordering allowed_orders();
  
  CORBA::PolicyType policy_type();
  
  CORBA::Policy_ptr copy() throw(CORBA::SystemException);

  void destroy();

  
};
}// messaging
}// core
}// TIDorb
#endif

