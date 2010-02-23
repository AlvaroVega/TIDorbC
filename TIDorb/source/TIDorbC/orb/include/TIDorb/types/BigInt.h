/////////////////////////////////////////////////////////////////////////
//
// File:        BigInt.h
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
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

#ifndef TIDORB_TYPES_BIG_INT
#define TIDORB_TYPES_BIG_INT

namespace TIDorb{
namespace types{

class BigInt
{
private: 
  // this value cannot be greather due to the base 10 transformation
  static const int MAG_SIZE=32; 

  static unsigned char decimal_converter [80][35];

  unsigned char mag[MAG_SIZE];

  int sign; // 0 then 0, // 1 then positive // -1 then negative.

  char* _str;      // Internal string representation 
  char* _hex_str;  // Internal string representation in hexadecimal
  bool _modified;  // Flag to say if internal representations needs to be recalculated

public:

  BigInt();
  BigInt(int val);
  BigInt(char * val);
  BigInt(const BigInt& val);

  ~BigInt();

  
public:	// Static methods;
  
  static void  div (BigInt &quotient, const BigInt &divider, 
                    const BigInt &divisor, BigInt &remainder);
  
public:

  void  x100 (int times);	
  void  move (BigInt& bi);

  void shiftLeft (int pos);
  void shiftBytesLeft (int pos);
	
  const char*    toString()    const;
  const char*    toHexString() const;
	
  unsigned char* getMag()  const;	
  int 		 getSign() const;
  int 		 _digits() const;
  BigInt 	 abs()     const;

  
  // Common operators

  BigInt& operator= (int val);
  BigInt& operator= (const BigInt& val);
  
  operator CORBA::LongLong   () const;
  operator CORBA::LongDouble () const;

  unsigned char& operator[] (int index);
  unsigned char  operator[] (int index) const;
  
  bool operator== (const BigInt &f) const;
  bool operator!= (const BigInt &f) const;
  bool operator<  (const BigInt &f) const;
  bool operator<= (const BigInt &f) const;
  bool operator>  (const BigInt &f) const;
  bool operator>= (const BigInt &f) const;
  
  BigInt operator+ (const BigInt& bi) const;
  BigInt operator- (const BigInt& bi) const;
  BigInt operator+ (int v) const;
  BigInt operator* (int v) const;
  BigInt operator* (const BigInt& bi) const;
  BigInt operator/ (const BigInt& bi) const;

  BigInt& operator+= (const BigInt &f);
  BigInt& operator-= (const BigInt &f);
  BigInt& operator*= (const BigInt &f);
  BigInt& operator/= (const BigInt &f);
  
  BigInt& operator++ (); /* Pre-increment */
  BigInt  operator++ (int);
  BigInt& operator-- (); /* Pre-decrement */
  BigInt  operator-- (int);
  
  BigInt  operator+ () const;
  BigInt  operator- () const;
  
  bool    operator! () const;	
  
};
} // end of namespace types
} // end of namespace TIDorb

#endif

