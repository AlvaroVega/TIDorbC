/////////////////////////////////////////////////////////////////////////
//
// File:        Fixed.h
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
//
// Revised:    
//
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
//
/////////////////////////////////////////////////////////////////////////


#include "CORBA.h"


#ifndef _CORBA_FIXED_H_
#define _CORBA_FIXED_H_

  
#include "TIDorb/types/BigInt.h"


namespace CORBA { 

class Fixed{

  friend class FixedHandler;
  
private:

  // Private members
  CORBA::UShort         _digits; 
  CORBA::Short          _scale;
  TIDorb::types::BigInt _val;


public:

  Fixed ();
  Fixed (CORBA::Long v);
  Fixed (CORBA::ULong v);
  Fixed (CORBA::LongLong v);
  Fixed (CORBA::ULongLong v);
  Fixed (CORBA::Double v);
  Fixed (CORBA::LongDouble v);
  Fixed (const char* s);
  Fixed (CORBA::UShort d, CORBA::Short s);
  Fixed (const Fixed& f);
  Fixed (TIDorb::types::BigInt v, CORBA::Short s); // utilizada por Fixed Holder
  
  ~Fixed ();


private:

  Fixed (TIDorb::types::BigInt v);

  static TIDorb::types::BigInt power10 (int x);
  static CORBA::LongDouble epsilonl (CORBA::LongDouble v);
  static CORBA::Double epsilon (CORBA::Double v);
  static CORBA::UShort digits (const TIDorb::types::BigInt& val);
  static TIDorb::types::BigInt rescale (CORBA::Short nscale, CORBA::Short oscale,
                                        const TIDorb::types::BigInt& value, 
                                        bool round = true);
  
  static void  add (Fixed &res, const Fixed &a1, const Fixed a2);
  static void  sub (Fixed &res, const Fixed &a1, const Fixed a2);
  static void  mul (Fixed &res, const Fixed &a1, const Fixed a2);
  static void  div (Fixed &res, const Fixed &a1, const Fixed a2);
    
  TIDorb::types::BigInt transform (CORBA::Short nscale) const;
  TIDorb::types::BigInt extractFromString (const char *str, 
                                           CORBA::UShort &_digits, 
                                           CORBA::Short &_scale);
  TIDorb::types::BigInt timesTenToThe (const TIDorb::types::BigInt& intPart, 
                                       unsigned short scale);
  TIDorb::types::BigInt& adjust(); 
  

	
  
public:

  Fixed  round    (CORBA::UShort nscale) const;
  Fixed  truncate (CORBA::UShort nscale) const;
  
  char*  to_string () const;
  Fixed& from_string (const char* value);
  
  CORBA::UShort fixed_digits () const { return _digits; }
  CORBA::Short  fixed_scale ()  const { return _scale;  }
  TIDorb::types::BigInt fixed_value () const   { return _val; }

  operator CORBA::LongLong   () const;
  operator CORBA::LongDouble () const;
  
  Fixed&  operator=  (const Fixed &f);
  Fixed&  operator+= (const Fixed &f);
  Fixed&  operator-= (const Fixed &f);
  Fixed&  operator*= (const Fixed &f);
  Fixed&  operator/= (const Fixed &f);
  
  Fixed& operator++ ();
  Fixed  operator++ (int);
  Fixed& operator-- ();
  Fixed  operator-- (int);
  
  Fixed  operator+ () const;
  Fixed  operator- () const;
  
  CORBA::Boolean  operator! () const;

  Fixed operator+ (const Fixed &f) const;
  Fixed operator- (const Fixed &f) const;
  Fixed operator* (const Fixed &f) const;
  Fixed operator/ (const Fixed &f) const;

  CORBA::Boolean operator>  (const Fixed &f) const;
  CORBA::Boolean operator<  (const Fixed &f) const;
  CORBA::Boolean operator>= (const Fixed &f) const;
  CORBA::Boolean operator<= (const Fixed &f) const;
  CORBA::Boolean operator== (const Fixed &f) const;
  CORBA::Boolean operator!= (const Fixed &f) const;

};

} // end of namespace

#endif //_CORBA_FIXED_H

