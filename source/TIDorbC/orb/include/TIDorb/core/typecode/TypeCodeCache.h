#include "TIDorb/core/typecode.h"

#ifndef _TypeCodeCache_H_
#define _TypeCodeCache_H_

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#include "CORBA.h"
#include "TIDThr.h"

namespace TIDorb {
namespace core {
namespace typecode {

struct m_cache
{
  char* m_repid;
  CORBA::TypeCode_ptr m_typecode;
};
	
class TypeCodeCache
{
private:
  TIDThr::RecursiveMutex recursive_mutex;
  int m_max, m_index;
  
protected:
  m_cache** m_table;
  
public:
  TypeCodeCache(int max);
  ~TypeCodeCache();
  CORBA::TypeCode_ptr find(CORBA::RepositoryId rep_id);
  /**
   * Tries to insert one TypeCode in the TypeCodeCache.
   * 
   * @param type
   * @return if there is also a TypeCode with the same id, returns the
   *         TypeCode cached otherwise, the same Typecode put. *
   */    
  CORBA::TypeCode_ptr put(CORBA::TypeCode_ptr type) throw (CORBA::INTERNAL);
  void destroy();
};

}
}
}
#endif

