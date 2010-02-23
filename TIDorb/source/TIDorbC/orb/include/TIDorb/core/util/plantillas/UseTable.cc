/*//////////////////////////////////////////////////////////////////////////////////
//
// File:        UseTable.cc
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

/*
 * Implementation note: gcc 3.4 requires that template operations derived from
 * another template were referenced with Type paremeters. For example, if
 * f() is defined in the template Base<T>, in a derived template must be
 * referend as this->f() or Base<T>::f()
 */
namespace TIDorb {
namespace core {
namespace util {




template <class KeyType, class ContentType>
UseTable<KeyType,ContentType>::UseTable(int maximumSize, int recycleRatio): _maximumSize(maximumSize)
{
  _round = 0;
  _used_connections = 0;
  _removedObjects = NULL;
  _recycleRatio = recycleRatio;
}




template <class KeyType, class ContentType>
UseTable<KeyType,ContentType>::~UseTable()
{
  // we unknow type of ContenType for do: while(iterator<_use.end()) delete (*iterator++)
  _use.clear();
  if (_removedObjects) {
    delete _removedObjects;
  }
}




template <class KeyType, class ContentType>
void UseTable<KeyType,ContentType>::append(ContentType* item)
    throw (FullUseTableException)
{
    TIDThr::Synchronized synchro(_recursive_mutex);

    const KeyType& key = reinterpret_cast<KeyType>(*item);
    this->append(key, item);
    // append(*item, item);

}




template <class KeyType, class ContentType>
void UseTable<KeyType,ContentType>::append(const KeyType& key, ContentType* item)
    throw (FullUseTableException)
{
    TIDThr::Synchronized synchro(_recursive_mutex);

    _removedObjects = this->removeOldObjects();

    if (_removedObjects->empty()) {
      // No se hace nada con la _removedObjects 
      _removedObjects->clear();
      delete _removedObjects;
      _removedObjects=NULL;
    }


    if (_use.size() == this->getMaxSize()) {
        throw FullUseTableException("Not enough space to append a new object");
    }

    // Append new element
    _use[key]=PairT(item,_round-1);
      
}


template <class KeyType, class ContentType>
void UseTable<KeyType,ContentType>::incRound()
{

  TIDThr::Synchronized synchro(_recursive_mutex);

  _round++;

  // Should check if _round == maxInteger !!
  if (_round == INT_MAX) {
    _round=0;
    _used_connections = 0;
    for(UseMapIteratorT useIt = _use.begin(); useIt != _use.end(); useIt++)
      (*useIt).second.second = _round-1;
  }

}


template <class KeyType, class ContentType>
ContentType* UseTable<KeyType,ContentType>::remove(const KeyType& key)
{
  TIDThr::Synchronized synchro(_recursive_mutex);

  ContentType* content = NULL;

  UseMapIteratorT useIt = _use.find(key);
  if (useIt != _use.end()){
    content =(*useIt).second.first;
    _use.erase(useIt);
  }
  return content;
}

template <class KeyType, class ContentType>
ContentType* UseTable<KeyType,ContentType>::get(const KeyType& key)
{
  TIDThr::Synchronized synchro(_recursive_mutex);

  ContentType* content = NULL;

  UseMapIteratorT useIt = _use.find(key);
  if (useIt != _use.end()){
    content = (*useIt).second.first;
  }
  return content;

  //  return _use[key];
}


template <class KeyType, class ContentType>
void UseTable<KeyType,ContentType>::append(ContentType* key, ContentType* item, void* nil)
    throw (TIDorb::core::util::FullUseTableException)
{
    TIDThr::Synchronized synchro(_recursive_mutex);

    nil = NULL;
    const KeyType& key_aux = *key;
    this->append(key,item);
    //    this->append(*key,item);

}




template <class KeyType, class ContentType>
void UseTable<KeyType,ContentType>::use(const KeyType& key)
{
    TIDThr::Synchronized synchro(_recursive_mutex);

    // Update round
    UseMapIteratorT i = _use.find(key);
    if (i == _use.end())
        return;

    if ((*i).second.second != _round) {
      _used_connections++;
      if (_used_connections > (this->getMaxSize() / 2)) {
        this->incRound();
        _used_connections = 0;
      }
    }

    // Update the round of element
    (*i).second.second=_round;

}




template <class KeyType, class ContentType>
vector<ContentType*>* UseTable<KeyType,ContentType>::getRemovedObjects()
{
    TIDThr::Synchronized synchro(_recursive_mutex);
    return _removedObjects;
}




template <class KeyType, class ContentType>
vector<ContentType*>* UseTable<KeyType,ContentType>::removeOldObjects()
{
  TIDThr::Synchronized synchro(_recursive_mutex);

  typedef vector<ContentType*> ContentVectorT;
  
  ContentType* content = NULL;
  RemovableObject* removableKey = NULL;
  UseMapT reinserted;
  UseMapIteratorT useIt, rIt;

  ContentVectorT* removed = new  ContentVectorT;

  // Need to remove old objects: when 80% in _use
  if (_use.size() > this->getMaxSize() * 0.8) { 
    
    do {
      
      // Obtain candidates to remove
      useIt = _use.begin();
      
      while (useIt != _use.end()) {  
        if ((*useIt).second.second <= (_round - 4)) {
          content = (*useIt).second.first;
          if ((removableKey = reinterpret_cast<RemovableObject*>(content)) && 
              (!removableKey->canBeRemoved())) {
            reinserted[(*useIt).first]=PairT((*useIt).second.first, _round-1);
          } else {
            removed->push_back((*useIt).second.first);
          }
          _use.erase(useIt++); // Only the erased iterator is invalidated
          // Complexity O(n)
        } 
        else
          useIt++;
        
      } // while

      
      if ((_use.size() + reinserted.size()) >= (this->getMaxSize()*0.8)) {  
        this->incRound(); // To obtain more candidate
      }

    } while ( (_use.size() > 0) && 
              (_use.size() + reinserted.size()) >= (this->getMaxSize()*0.8)); 
  } // if (... * 0.8)      
  

  // Reinsert the elements of reinserted
  if (!reinserted.empty()){
    for (rIt = reinserted.begin(); rIt != reinserted.end(); rIt++) {
      _use[(*rIt).first]=PairT((*rIt).second.first,_round - 1);
    }
    reinserted.clear();
  }
  return removed;

}


template <class KeyType, class ContentType>
vector<ContentType*>* UseTable<KeyType,ContentType>::elements()
{
  TIDThr::Synchronized synchro(_recursive_mutex);

  typedef vector<ContentType*> ContentVectorT;
  
  UseMapIteratorT useIt;

  ContentVectorT* elements = new  ContentVectorT;

  useIt = _use.begin();
  
  while (useIt != _use.end()) {  
    elements->push_back((*useIt).second.first);
    useIt++;
  } 
      
  return elements;

}




template <class KeyType, class ContentType>
void UseTable<KeyType,ContentType>::dump(ostream& os)
{

  TIDThr::Synchronized synchro(_recursive_mutex);

  UseMapIteratorT useIt;
  
  for (useIt = _use.begin(); useIt != _use.end(); useIt++) {
    
    os << "Content: [";
    if (typeid((*useIt).second.first) != typeid(char*)) 
      os << *((*useIt).second.first);
    //   os << &(*useIt).second.first;
    else 
      os << (*useIt).second.first;

    os << "] Key: [";
    if (typeid((*useIt).second.first) != typeid(char*)) 
      os << *((*useIt).second.first);
    else 
      os << (*useIt).second.first;

    os << "] Use: [";
    os << (*useIt).second.second;
    os << "]" << endl;
  } // for

  os << flush;
}

template <class KeyType, class ContentType>
size_t UseTable<KeyType,ContentType>::getMaxSize()
{

  TIDThr::Synchronized synchro(_recursive_mutex);

  return  _maximumSize;
}

template <class KeyType, class ContentType>
size_t UseTable<KeyType,ContentType>::getSize()
{

  TIDThr::Synchronized synchro(_recursive_mutex);

  return  _use.size();
}

template <class KeyType, class ContentType>
void UseTable<KeyType,ContentType>::clear()
{

  TIDThr::Synchronized synchro(_recursive_mutex);

  return  _use.clear();
}

template <class KeyType,class ContentType>
bool UseTable<KeyType,ContentType>::object_exist(const KeyType& key)
{
    TIDThr::Synchronized synchro(_recursive_mutex);

    return (this->get(key) != NULL);
}



}
}
}




// // TODO: Borrar este programa de prueba


// int main(int argc, char** argv)
// {
//     using namespace TIDorb::core::util;

//     int** array1=(int**) new int*[15],i=0;
//     char** cadena=(char**) new char*[15];
//     PRUEBA** prueba1=(PRUEBA**) new PRUEBA*[15];
//     UseTable<int,PRUEBA> jl(10);

//     try {

//     for (i=0;i<15;i++)
//     {
//       array1[i]=new int(i);
//       cadena[i]=new char[10];
//       sprintf(cadena[i],"upcad: %d",i);
//       prueba1[i]=new PRUEBA;
//       prueba1[i]->miembro1=*array1[i];
//       sprintf(prueba1[i]->miembro2,"%s",cadena[i]);
//         if (i<10)
//         jl.append(array1[i],prueba1[i]);
//     }

//     cout<<endl; jl.dump(cout);
//     cout<<endl<<"Objeto: "<<*jl.get(array1[5]);
//     jl.use(array1[4]);
//     cout<<endl; jl.dump(cout);
//     jl.use(array1[5]);
//     cout<<endl; jl.dump(cout);
//     jl.use(array1[1]);
//     cout<<endl; jl.dump(cout);
//     jl.use(array1[7]);
//     cout<<endl; jl.dump(cout);
//     jl.use(array1[2]);
//     cout<<endl; jl.dump(cout);
//     jl.use(array1[3]);
//     cout<<endl; jl.dump(cout);

//     jl.append(array1[10], prueba1[10]);
//     jl.append(array1[11], prueba1[11]);
//     cout<<endl; jl.dump(cout);
//     jl.append(array1[12], prueba1[12]);
//     cout<<endl; jl.dump(cout);
//     jl.append(array1[13], prueba1[13]);
//     cout<<endl; jl.dump(cout);
//     jl.append(array1[14], prueba1[14]);

//     } catch (const exception& t) {
//       cerr<<t.what()<<endl<<flush;
//     }


//     jl.remove(array1[5]);
//     cout<<endl; jl.dump(cout);
//     jl.use(array1[6]);
//     cout<<endl; jl.dump(cout);
//     jl.use(array1[11]);
//     cout<<endl; jl.dump(cout);
//     cout<<endl<<"Tam: "<<jl.getSize()<<endl<<flush;

//     for (i=0;i<15;i++)
//     {
//       delete[] cadena[i];
//       delete   array1[i];
//       delete   prueba1[i];
//     }

//     delete[] cadena;
//     delete[] array1;
//     delete[] prueba1;

//     int** array3=(int**) new int*[15];
//     REMOVIBLE** cadena3=(REMOVIBLE**) new REMOVIBLE*[15];
//     UseTable<int,REMOVIBLE> jl3(10);

//     try {

//     for (i=0;i<15;i++)
//     {
//       array3[i]=new int(i);
//       cadena3[i]=new REMOVIBLE(i);

//         if (i<10)
//         jl3.append(array3[i],cadena3[i]);
//     }

//     cout<<endl; jl3.dump(cout);
//     cout<<endl<<"Objeto: "<<*jl3.get(array3[5]);
//     jl3.use(array3[4]);
//     cout<<endl; jl3.dump(cout);
//     jl3.use(array3[5]);
//     cout<<endl; jl3.dump(cout);
//     jl3.use(array3[1]);
//     cout<<endl; jl3.dump(cout);
//     jl3.use(array3[7]);
//     cout<<endl; jl3.dump(cout);
//     jl3.use(array3[2]);
//     cout<<endl; jl3.dump(cout);
//     jl3.use(array3[3]);
//     cout<<endl; jl3.dump(cout);

//     jl3.append(array3[10], cadena3[10]);
//     jl3.append(array3[11], cadena3[11]);
//     cout<<endl; jl3.dump(cout);
//     jl3.append(array3[12], cadena3[12]);
//     cout<<endl; jl3.dump(cout);
//     jl3.append(array3[13], cadena3[13]);
//     cout<<endl; jl3.dump(cout);
//     jl3.append(array3[14], cadena3[14]);

//     } catch (const exception& t) {
//       cerr<<t.what()<<endl<<flush;
//     }


//     jl3.remove(array3[5]);
//     cout<<endl; jl3.dump(cout);
//     jl3.use(array3[6]);
//     cout<<endl; jl3.dump(cout);
//     jl3.use(array3[11]);
//     cout<<endl; jl3.dump(cout);
//     cout<<endl<<"Tam: "<<jl3.getSize()<<endl<<flush;

//     for (i=0;i<15;i++)
//     {
//       delete cadena3[i];
//       delete array3[i];
//     }

//     delete[] cadena3;
//     delete[] array3;


//     return 0;
//   }
