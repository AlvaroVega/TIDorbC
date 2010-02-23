/*//////////////////////////////////////////////////////////////////////////////////
//
// File:        UseTable.h
// Description: 
//
// Rel:         01.00
// Created:     
// Author:      
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
///////////////////////////////////////////////////////////////////////////////////*/

#include "TIDorb/core/util.h"

#ifndef _TIDORB_core_util_UseTable_H_
#define _TIDORB_core_util_UseTable_H_ 1

#ifdef TIDORB_HAVE_IOSTREAM
#include <iostream>
#else
#include <iostream.h>
#endif

#ifdef __sun
#include <inet/common.h>
#else
#include <limits.h>
#endif

#include <exception>
#include <string>
#include <vector>
#include <utility>
#include <map>
#include <typeinfo>

#ifdef TIDORB_HAVE_NAMESPACE_STD
using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
using std::string;
using std::vector;
using std::map;
using std::pair;
using std::exception;
#endif


namespace TIDorb {
namespace core {
namespace util {
      

  class FullUseTableException : public virtual exception {
  public:
    FullUseTableException(){}
    FullUseTableException(const char* reason): m_reason(reason) {}
    ~FullUseTableException() throw() {}
    
    const char* what() const throw () { return m_reason.data(); }
    
  protected:
    string m_reason;
  };


  template <class KeyType, class ContentType> class UseTable {
  public:

    typedef pair<ContentType*, int>                 PairT;
    typedef map< KeyType, PairT >                   UseMapT;
    typedef typename UseMapT::iterator              UseMapIteratorT;
    typedef vector<ContentType*>                    ObjectList;
    UseTable(int maximumSize, int recycleRatio = 80);
    ~UseTable();

    void                  append(ContentType* item) throw (FullUseTableException);
    void                  append(const KeyType& key, ContentType* item) throw (FullUseTableException);
    ContentType*          remove(const KeyType& key);
    ContentType*          get(const KeyType& key);
    void                  use(const KeyType& key);
    vector<ContentType*>* elements();
    vector<ContentType*>* getRemovedObjects();
    void                  dump(ostream& os);
    size_t                getSize();
    size_t                getMaxSize();
    void                  incRound();
    void                  clear();
    bool                  object_exist(const KeyType& key);

  protected:
    vector<ContentType*>* removeOldObjects();
    void                  append(ContentType* key, ContentType* item, void* nil) throw (FullUseTableException);
    
  private:
    UseMapT               _use;
    size_t                _maximumSize;
    int                   _round;
    int                   _used_connections;
    int                   _recycleRatio;
    vector<ContentType*>* _removedObjects;
    TIDThr::RecursiveMutex _recursive_mutex;
  };


}
}
}


#include "TIDorb/core/util/UseTable.cc"

#endif
