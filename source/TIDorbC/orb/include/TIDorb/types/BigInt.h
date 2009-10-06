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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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

