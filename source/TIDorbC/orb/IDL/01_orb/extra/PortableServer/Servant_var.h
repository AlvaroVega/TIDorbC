#include "PortableServer.h" // Parent Inclusion 


#ifndef _PORTABLE_SERVER_SERVANT_VAR_H_ 
#define _PORTABLE_SERVER_SERVANT_VAR_H_


template<typename Servant>
class Servant_var
{
  protected:
    void swap(Servant* lhs, Servant* rhs)
    {
      Servant *tmp = lhs;
      lhs = rhs;
      rhs = tmp;
    }

  public:
    Servant_var() : _ptr(0) {}
    Servant_var(Servant* p) : _ptr(p) {}
    Servant_var(const Servant_var& b)
    : _ptr(b._ptr)
    {
      if (_ptr != 0) _ptr->_add_ref();
    }
    
    ~Servant_var()
    {
      if (_ptr != 0) {
        try {
          _ptr->_remove_ref();
        } catch (...) {
          // swallow exceptions
        }
      }
    }
    
    Servant_var& operator=(Servant* p)
    {
      if (_ptr != p) {
        Servant_var<Servant> tmp = p;
        swap(_ptr, p);
      }
      return *this;
    }

    Servant_var& operator=(const Servant_var& b)
    {
      if (_ptr != b._ptr) {
        Servant_var<Servant> tmp = b;
        swap(_ptr, b._ptr);
      }
      return *this;
    }

    Servant* operator->() const { return _ptr; }

    Servant* in() const { return _ptr; }

    Servant*& inout() { return _ptr; }

    Servant*& out()
    {
      if (_ptr != 0) {
        Servant_var<Servant> tmp;
        swap(_ptr, tmp._ptr);
      }
      return _ptr;
    }

    Servant* _retn()
    {
      Servant* retval = _ptr;
      _ptr = 0;
      return retval;
    }

  private:
    Servant* _ptr;
};


#endif
