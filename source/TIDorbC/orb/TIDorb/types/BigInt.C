/////////////////////////////////////////////////////////////////////////
//
// File:        BigInt.C
// Description:
//              
// Rel:         01.00
// Created:     November, 2002
//
// Revised:    
//   05/29/2009 by Alvaro Vega <avega@tid.es>
//      * Some bugs fixed in operators < (infinite loops, memory leaks..)
//      * Performance improvements: 
//         - Memory management
//         - Both string representations
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

#include "CORBA.h"
#include "TIDorb/types/BigInt.h"
#include "TIDorb/util.h"

#include <string.h>
#include <stdlib.h>
#include <ctype.h>


#define LOWBYTE(x) ((unsigned char) ((x) & 0xff))
#define HIBYTE(x)  ((unsigned char) ((x) >> 8 & 0xff))

/*
 * Empty contructor.
 *  Initializes "mag" to zero.
 */
TIDorb::types::BigInt::BigInt()
{
  for (int i = 0; i < MAG_SIZE; i++)
    mag[i] = 0;

  sign = 0; // 0 value.
  
  _str = NULL;
  _hex_str = NULL;
  _modified = true;
}

/* 
 * Constructor from integer
 *  Initilizes "mag" to zero and assigns the integer to index 0 and 1 at "mag" 
 */
TIDorb::types::BigInt::BigInt(int val)
{
  if (val > 0) {
    sign = 1;
  } else if ( val < 0 ) {
    sign = -1;
    val  *= -1;
  }
  else 
    sign = 0;
	
  for(int i = 0; i < MAG_SIZE; i++)
    mag[i] = 0;

  // Init mag with integer value:
  mag[0] = LOWBYTE(val);
  mag[1] = HIBYTE(val);
	
  _str = NULL;
  _hex_str = NULL;
  _modified = true;
}

/*
 * Copy Constructor
 *
 */
TIDorb::types::BigInt::BigInt(const BigInt& val)
{
  for(int i=0; i < MAG_SIZE; i++)
    mag[i] = val[i];

  sign = val.sign;

  _str = NULL;
  _hex_str = NULL;
  _modified = true;
}

/*
 * Constructor from numeric string.
 *
 */
TIDorb::types::BigInt::BigInt(char* val) 
{
  // TODO: store 'val' in BigInt::_str
  char* number;
  int size = strlen(val);
	
  if(val != 0) {
    number = (char*)malloc(size+1);
    strcpy(number, val);
  } else { // for empty string return 0
    sign = 0;
    return;
  }

  char* is_negative = strchr(number, '-');
  bool negativize = false;

  if (is_negative) {
    // Store sign and remove it from string
    sign = (-1);
    negativize = true;
    char* renum = (char*)malloc(size-1+1);
    strcpy(renum, number + 1);
    char* old;
    old = number;
    number = renum;
    renum = old;
    size--;
    free(renum);
  }
	

  int rev_index = size;

  if (size%2 == 1) {
    // if string has edge length then add a 0 at the begin to be managed by the algoritm.
    char * renum = (char*) malloc (size+2);
    renum[0] = '0';
    strcpy(renum+1, number);
    char* old;
    old = number;
    number = renum;
    renum = old;
    size += 1;
    rev_index = size;
    free(renum);
  }
  
  BigInt aux, total;
  unsigned int value = 0;
  unsigned int pot10 = 0;
  char low = 0; 
  char high = 0;
  char current_char = 0;

  for ( ; rev_index >= 2; rev_index -= 2) {	

    current_char = number[rev_index-1];

    if (isdigit(current_char)) {
      low = current_char-'0';
    }
    else { 
      throw CORBA::DATA_CONVERSION("BigInt(char*): first character is not a digit");		
    }
		
    current_char = number[rev_index-2];
    if (isdigit(current_char)) {
      high = current_char-'0';
    }
    else { 
      throw CORBA::DATA_CONVERSION("BigInt(char*): second character is not a digit");
    }
		
    // value will storage the lower number of string
    value = high*10+low;
    aux = value;
    pot10 = size - rev_index;
    aux.x100(pot10/2);
    total += aux;
  }
	
  free(number);
  
  // Copy result in total to current BigInt object
  *this = total;
  if (negativize)
    sign = -1;


  _str = NULL;
  _hex_str = NULL;
  _modified = true;
}


TIDorb::types::BigInt::~BigInt()
{
  if (_str != NULL) 
    free(_str);

  if (_hex_str != NULL) 
    free(_hex_str);
}


unsigned char * TIDorb::types::BigInt::getMag() const
{
  return (unsigned char *)mag;
}


int TIDorb::types::BigInt::getSign() const
{
  return sign;
}


/*
 * Assign operator for integer values
 */
TIDorb::types::BigInt& TIDorb::types::BigInt::operator= (int val)
{
  if (val > 0) {
    sign =+ 1;
  } else if (val < 0) {	
    val *=-1; // Take care with this. If not it will be in 2 or 1 complement
    sign =-1;
  }
  else sign = 0;
	
  for (int i = 0; i < MAG_SIZE; i++)
    mag[i] = 0;

  // the operation is:
  mag[0] = LOWBYTE(val);
  mag[1] = HIBYTE(val);

  _modified = true;

  return *this;
}


TIDorb::types::BigInt TIDorb::types::BigInt::operator+ (const TIDorb::types::BigInt& bi) const 
{
  TIDorb::types::BigInt res;
  
  if (sign == 0) { // Direct assign
    res = bi;
    return res;
  }
  else if (bi.sign == 0) {
    res = (*this);
    return res;
  }
	
  if (sign == bi.sign) {

    unsigned int resInt = 0;

    for (int i=0; i<MAG_SIZE; i++) {
      resInt = mag[i] + bi[i] + HIBYTE(resInt);
      res[i] = LOWBYTE(resInt);
    }	
    if (HIBYTE(resInt) != 0) {
      throw CORBA::DATA_CONVERSION("BigInt::operator+: HIBYTE(restInt) is not 0");
    }
    res.sign = sign;
    return res;
  }

  else if (sign == -1) {
    // Add negative to positive: (+X) + (-Y)
    // is the same that: (+X) - (+Y)
    res = bi - (-(*this));
    return res;
  }
  else if(sign == 1) { 
    // Added a positive to negavie: (+X) + (-Y) 
    // is the same that: (+X) - (+Y)
    res = (*this) - bi;
    return *this;
  }

  return res;
}


TIDorb::types::BigInt TIDorb::types::BigInt::operator+(int v) const
{
  return (*this) + (BigInt) v;
}


TIDorb::types::BigInt TIDorb::types::BigInt::operator*(int v) const 
{
  TIDorb::types::BigInt res;
  
  if ( (sign==1) && (v!=0) ) {

    unsigned int resInt = 0;
		
    for (int i=0; i<MAG_SIZE; i++) {
      resInt = mag[i]*v + HIBYTE(resInt);
        
      res[i] = LOWBYTE(resInt);
    }	
		
    if (HIBYTE(resInt) != 0) {
      throw CORBA::DATA_CONVERSION("BigInt::operator*: HIBYTE(resInt) is not 0");
    }

    int new_sign = (sign * v);
    
    if (new_sign > 0) {
      res.sign = 1;
    }
    else if (new_sign < 0) {
      res.sign = -1;
    }
    else 
      res.sign = 0;
    }
  else if ( (sign==0) || (v==0) ) {
    res = 0;
  }
  else if (sign == -1) {
    res = ( (-(*this)) * (-v)); 
  }

  return res;
}


void TIDorb::types::BigInt::x100(int times)
{
  TIDorb::types::BigInt temp = (*this);
	
  for(int i=0; i < times; i++) {

    TIDorb::types::BigInt temp64 = (*this);
    TIDorb::types::BigInt temp32 = (*this);
    TIDorb::types::BigInt temp4  = (*this);
    
    temp64.shiftLeft(6);
    temp32.shiftLeft(5);
    temp4.shiftLeft(2);

    // X= res*64+res*32
    temp = temp64 + temp32;
    // res= X+res*4;
    temp += temp4;
    (*this) = temp;
  }
  
  _modified = true;
}


void TIDorb::types::BigInt::shiftLeft(int pos) 
{
  if (pos == 0) 
    return;

  unsigned int currentIntMag = 0;
  unsigned char previousMag  = 0;
	
  for (int i=0; i < MAG_SIZE; i++) {
    unsigned char value = mag[i];
    currentIntMag = value;
    currentIntMag = currentIntMag << pos;
    
    mag[i] = LOWBYTE(currentIntMag) + previousMag;
    
    previousMag = HIBYTE(currentIntMag);
  }
	
  if (previousMag > 0) {
    throw CORBA::DATA_CONVERSION("BigInt::shiftLeft: previousMag > 0");	
  }

  _modified = true;
}


void TIDorb::types::BigInt::shiftBytesLeft(int pos) 
{
  int j;
  int i;

  for (i = MAG_SIZE-pos; i<MAG_SIZE; i++) {
    if (mag[i] != 0) {
      throw CORBA::DATA_CONVERSION("BigInt::shiftBytesLeft: mag[i] is not 0");	
    }
  }

  for (i=0; i < pos; i++) {

    for (j = MAG_SIZE-1; j >= 1; j--) {
      mag[j]   = mag[j-1];
      mag[j-1] = 0;
    }
  }

  _modified = true;
}

// Index for assign
unsigned char &TIDorb::types::BigInt::operator[] (int index)
{
  _modified = true;
  return mag[index];
}


// Index for use the value
unsigned char TIDorb::types::BigInt::operator[] (int index) const
{
  return mag[index];
}


TIDorb::types::BigInt& TIDorb::types::BigInt::operator= (const BigInt& val)
{
  if (&val != this) {

    for (int i = 0; i < MAG_SIZE; i++)
      mag[i] = val[i];

    sign = val.sign;
  }

  _modified = true;

  return *this;
}


const char* TIDorb::types::BigInt::toHexString() const
{
  if ( (!_modified) && (_hex_str != NULL) )
    return _hex_str;

  bool oneShowed = false;

  TIDorb::util::StringBuffer buffer;
  char char_buffer[5];

  if (sign == -1)
    buffer << "-";

  for (int i = MAG_SIZE; i > 0; i--) {

    if ( (mag[i-1] != 0) || oneShowed) {

      if (mag[i-1] <= 0xf)
        buffer << "0";

      sprintf(char_buffer, "%0x", mag[i-1]);
      buffer << char_buffer;
      oneShowed = true;
    }
  }
	
  if (!oneShowed)
    buffer << "0";


 // Free previous string representation
  if (_hex_str != NULL)
    free(((TIDorb::types::BigInt*) this)->_hex_str);

  if (_str != NULL) {
    free(((TIDorb::types::BigInt*) this)->_str);
    ((TIDorb::types::BigInt*) this)->_str = NULL;
  }

  ((TIDorb::types::BigInt*) this)->_hex_str = 
    CORBA::string_dup(buffer.str().data());

  // Mark current string representation as not modified
  ((TIDorb::types::BigInt*) this)->_modified = false;

  return _hex_str;
}


const char* TIDorb::types::BigInt::toString() const
{
  if ( (!_modified) && (_str != NULL) )
    return _str;
  
  char result[100+1];
  result[100] = '\0';
	
  int k = MAG_SIZE - 1;
	
  for ( ; k >= 0 ; k--) {
    // Calculamos la posicion de la primera cifra significativa para reducir 
    // el numero de ciclos.
    if(mag[k] != 0)
      break;
  }
  int max_mag_representative = k;
  if (max_mag_representative == -1) { // el numero es todo 0?s
    free(_str);
    ((TIDorb::types::BigInt*) this)->_str = (char*)malloc(2);
    _str[1] = '\0';
    _str[0] = '0';
    return _str;

  }
  int last_zero_values = 0;

  // La posicion 0 de cada una de las filas de decimal_converter contiene la 
  // posicion de la primera cifra significativa de la fila actual, para reducir el numero
  // de ciclos.
  int j = 0;
  unsigned long ireg = 0;
  for ( ; decimal_converter[j][0] <= max_mag_representative+1; j++) {
    for (int i = decimal_converter[j][0]; i <= max_mag_representative+1; i++) {       
      ireg += mag[i-1]*decimal_converter[j][i];
    }
    if (ireg == 0) {
      last_zero_values++;
    }
    else 
      last_zero_values = 0;

    // Nos quedamos con la cifra decimal correspondiente (la ultima)
    result[j] = ireg%10;
    //TIDORB_TYPES_BIG_INT_TRACE("[%d] Valores de IREG %d Comprobacion:[%d][%d][%d]\n",j,ireg,vm_1,vm,vm1);
    // y reducimos el accarreo para la siguiente vuelta.
    ireg = (long)ireg/10;
  }
  
  if (ireg == 0)
    j = j - last_zero_values;
		
  int last_zero = j;
  //TIDORB_TYPES_BIG_INT_TRACE("Quedaba acacarreo %d\n",ireg);
  for(; ireg != 0; last_zero++) {
    //incluimos el acarreo que pudiera quedar.
    result[last_zero] = ireg%10;
    ireg = (long)ireg/10;
  }

  // reservamos la memoria justa.
  char *returned;
  char *aux;

  if (sign == -1) {
    returned = (char*) malloc(last_zero+1+1);
    returned[last_zero+1] = '\0';
    returned[0] = '-';
    aux = returned + 1;
  }
  else {
    returned = (char*) malloc(last_zero+1);
    returned[last_zero] = '\0';
    aux = returned;
  }

  int index = 0;
  for (int i = 0; i < last_zero; i++) {
    // Le damos la vuelta al numero para representarlo de izq a derecha.
    index = last_zero-i-1;
    aux[i] = result[index] + '0';
  }
  
  
  // Free previous strings representation
  if (_str != NULL) 
    free(((TIDorb::types::BigInt*) this)->_str);

  if (_hex_str != NULL) {
    free(((TIDorb::types::BigInt*) this)->_hex_str);
    ((TIDorb::types::BigInt*) this)->_hex_str = NULL;
  }
  

  // Store current string representation
  ((TIDorb::types::BigInt*) this)->_str = returned;

  // Mark current string representation as not modified
  ((TIDorb::types::BigInt*) this)->_modified = false;

  return returned;
}


int TIDorb::types::BigInt::_digits() const
{
  const char* str = toString();
  int value = strlen(str);
  if (sign == -1) {
    // descontamos el signo que tambien nos ha venido.
    value--; 
  }
  return value;	
}


TIDorb::types::BigInt TIDorb::types::BigInt::operator- (const TIDorb::types::BigInt &bi) const
{
  TIDorb::types::BigInt res;
  
  if ((*this) < bi) {
    res = bi - (*this); 
    res.sign *= -1;
    return res;
  }

  // apartir de Aqui (*this) siempre mayor que bi.
  if (sign == bi.sign) {

    if(sign == 0) {
      //ambos son 0.
      res = 0;
      return res;
    }
    
    int j;
    unsigned int ireg   = 0;
    unsigned int borrow = 0;
    bool isCero = true;
    
    for (j=0; j<MAG_SIZE; j++) {
			
      ireg=(256+mag[j]) - bi[j] - borrow;
      
      if(ireg>=256)
        borrow=0;
      else borrow=1;
      

      res[j]=LOWBYTE(ireg);
      if(res[j]!=0) 
        isCero=false;
    }
    
    if(isCero) {
      res.sign=0;
    }
    else res.sign=1;
		
  }
  else if (sign == -1) {
    res = -(*this) + bi;
    res.sign = -1;
  }
  else if (bi.sign == -1) {		
    res = (*this) + (-bi);
  }
  else if (sign == 0) {
    res = -bi;
  }
  else if (bi.sign == 0) {
    res = (*this);
  }
  
  return res;
}


TIDorb::types::BigInt 
TIDorb::types::BigInt::operator* (const TIDorb::types::BigInt &bi) const
{
  TIDorb::types::BigInt result;
  TIDorb::types::BigInt values;
  // hacemos la multiplicacion positiva y luego ya arreglamos el signo.
  TIDorb::types::BigInt positiveOp1 (*this);
  positiveOp1.sign = 1;

  result = 0;
  int i  = 0;

  if ( (sign==0) || (bi.sign==0) )
    result = 0;

  for (i=0; i<MAG_SIZE; i++) {	
    values = positiveOp1;
    
    if (bi[i] != 0) {
      values = values * bi[i];
      //TIDORB_TYPES_BIG_INT_TRACE("Si no era cero el resultultado es: [%s]\n",values.to_string());
      values.shiftBytesLeft(i);
      //TIDORB_TYPES_BIG_INT_TRACE("Desplazado: [%s]\n",values.to_string());
      result += values;
      //TIDORB_TYPES_BIG_INT_TRACE("Llevamos: [%s]\n",result.to_string());
    }
    
  }
  
  result.sign = sign*(bi.sign);
  
  return result;
}

void TIDorb::types::BigInt::div(TIDorb::types::BigInt &quotient, 
                                const TIDorb::types::BigInt &divider, 
                                const TIDorb::types::BigInt &divisor,
                                TIDorb::types::BigInt &remainder)
{
  if (divisor.sign == 0)
    return; // Overflow o NAN

  if (divider.sign == 0) {
    // 0/XXX = 0 resto 0
    quotient  = 0;
    remainder = 0;
    return; 
  }

  if (divider.abs() < divisor.abs() ) {
    // Por ser la division entera, al ser mas grande el divisor que el dividendo
    // El cociente es 0 y el resto es el divisor.
    quotient  = 0;
    remainder = divider;
    return; 
  }

  TIDorb::types::BigInt dividendoParcial,restoParcial,cocienteParcial,divisorTemporal;
	
  // Para dividir un numero entre uno negativo hacemos la division por uno positivo y
  // al final lo arreglamos.
  divisorTemporal=divisor;
  if(divisor.sign==-1)
    divisorTemporal.sign=1;

  TIDorb::types::BigInt sustraendoParcial,aditivo;
	
  int index=MAG_SIZE-1;

  dividendoParcial=(unsigned int)divider[index--];
  // dividendo parcial contintiene la primera cifra del dividendo.
  while(1)
    {	
      if(dividendoParcial < divisorTemporal)
        { // Si es menor el dividendoParcial que el divisor, 
          // aumentamos el divisor mientras podamos
		
          cocienteParcial.shiftBytesLeft(1); // Pongo cero a la derecha.
          if(index==-1)
            {
              break; // Salimos del While.
            }
          dividendoParcial.shiftBytesLeft(1);//Pongo cero a la derecha.
          // y le sumo la siguiente cifra. que va en la poscion 0
          dividendoParcial += (unsigned int)divider[index--];
        }
      else
        {	// En el momento que el dividendo parcial es mayor que el divisor, 
          // la division ya es entera y comenzamos a buscar un cociente valido.
          int indicador=1;
          while (1)
            {// Calculo del nuevo caciente Parcial
				
              sustraendoParcial = divisorTemporal * indicador;
              // Calculamos el sustraendo parcial de multilicar el divisor por el indicador
              // que estamos probando.
				
              restoParcial = dividendoParcial - sustraendoParcial;
              // realizamos la resta del dividendo parcial y el sustraendo parcial.
              // el resultado es el resto parcial, si este es menor que el divisor hemos encontrado
              // el cociente mas proximo.
				

              if(restoParcial < divisorTemporal)
                {// hemos encontrado cociente.
                  break;
                }
              else 
                { // Si el resto es mas grande (estricto) que el divisor 
                  // hay que seguir buscando.
                  indicador++; 
                  // if(indicador>255)
                  // "Errororororo en la division 
                  // Suponemos que el indicador ira hasta 255 como mucho. JiJi. ;-p
                }

            }// End While (1)
          // Desplazamos el cociente parcial que llevabamos para sumarle el indicador.
          cocienteParcial.shiftBytesLeft(1);
          cocienteParcial += indicador;
          // Ahora al resto parcial hay que a?adirle un numero del dividendo.
          // Si no quedan mas numeros que a?adir, este es el resto.
          if(index==-1)
            {
              // Ya no hay mas cifras que a?adir, este es el resto.
              dividendoParcial=restoParcial;
              break; // salimos del While.
            }
          else 
            {
              // Desplazamos el resto parcial, y le a?adimos la nueva cifra.
              restoParcial.shiftBytesLeft(1);
              dividendoParcial  = restoParcial;
              dividendoParcial += (unsigned int)divider[index--];
            }
        }// End Else
      // en la siquiente vuelta el dividendo parcial es el resto obtenido mas una cifra del dividendo.
    } // End While (1).

  if((divider.sign==divisor.sign)&&(divisor.sign==1))
    {// ambos  positivos.
      quotient=cocienteParcial;
      remainder=dividendoParcial;
      return;
    }
  else if((divider.sign==divisor.sign)&&(divisor.sign==-1))
    {// ambos negativos 
      quotient=cocienteParcial;
      remainder=-dividendoParcial;
      return;
    }
  else if((divider.sign==-1))
    {// negativo dividido entre positivo.
      quotient=-cocienteParcial;
      remainder=-dividendoParcial;
      return;
    }
  else if(divisor.sign==-1)
    {// positivo dividido entre negativo.
      quotient=-cocienteParcial;
      remainder=dividendoParcial;
      return;
    }
}


void TIDorb::types::BigInt::move(TIDorb::types::BigInt &res)
{
  int j;
  for (j=0; j < MAG_SIZE; j++) 
    res[j] = mag[j];

  res._modified = true;
}

TIDorb::types::BigInt  TIDorb::types::BigInt::abs () const
{
  TIDorb::types::BigInt result = *this;
  if (sign != 0)
    result.sign = 1;
  return result;
}

bool  TIDorb::types::BigInt::operator == (const TIDorb::types::BigInt &bi) const
{
  if (sign != bi.sign)
    return false;
		
  for(int i = MAG_SIZE-1; i >= 0; i--) {
    if (mag[i] != bi[i]) {
      return false; // (*this) is different than bi
    }
    
  }
  return true; // is equal
}


bool  TIDorb::types::BigInt::operator != (const TIDorb::types::BigInt &bi) const
{ 
  return ! ((*this) == bi);
}


bool  TIDorb::types::BigInt::operator< (const TIDorb::types::BigInt &bi) const
{
  if (sign == bi.sign) { 
    // both have the same sign

    if (sign == 0)
      return false; // both are 0
    
    for (int i = MAG_SIZE-1; i >= 0; i--) {

      if (mag[i] < bi[i]) {
        if (sign == 1)
          return true; // op1 is minor than bi
        else 
          return false;

      }
      else if (mag[i] == bi[i]) {
        continue; // bi is equal than op1
      }
      else { // msg[i] > bi[i]
        if (sign == 1)
          return false;
        else 
          return true;
      }
    } // for
    return false; // it is equal
  }
  if (sign == 1) {
    return false; 
  }
  else if (sign == 0) {
    if (bi.sign == -1)
      return false;
    else 
      return true; //bi.sign==1;
  }
  else if (bi.sign == 1) { //op1=(-1)   
    return true;
  }
  else { 
    // compiler requirement.
    return true;
  }
	
}

bool  TIDorb::types::BigInt::operator<= (const TIDorb::types::BigInt &bi) const

{
  if (sign == bi.sign) {
    // both have the same sign

    if (sign == 0)
      return true; // both are 0

	
    for (int i = MAG_SIZE-1; i >= 0; i--) {

      if (mag[i] < bi[i]) {
        if (sign == 1)
          return true; // op1 is minor than bi
        else 
          return false;
        
      }
      else if (mag[i] == bi[i]) {
        continue; // bi is equal than op1
      }
      else { // msg[i] > bi[i]	
        if (sign == 1)
          return false; // bi es mayor que 
        else 
          return true;
      }
		

    } // for
    return false; // it is equal
  }
  else if (sign == 1) {
    return false;
  }
  else if (bi.sign == 1) {
    return true;
  }
  else { 
    // compiler requirement.
    return true;
  }
}


bool  TIDorb::types::BigInt::operator> (const TIDorb::types::BigInt &bi) const
{
  return ! ((*this) <= bi);
}


bool  TIDorb::types::BigInt::operator>= (const TIDorb::types::BigInt &bi) const

{
  return ! ((*this) < bi);
}


TIDorb::types::BigInt&  TIDorb::types::BigInt::operator+= (const TIDorb::types::BigInt &bi)
{
  (*this) = (*this) + bi;	
  _modified = true;
  return *this;
}


TIDorb::types::BigInt &  TIDorb::types::BigInt::operator-= (const TIDorb::types::BigInt &bi)
{
  (*this) = (*this) - bi;	
  _modified = true;
  return *this;	
}


bool TIDorb::types::BigInt::operator! () const    
{
  return (sign == 0);
}


TIDorb::types::BigInt &  TIDorb::types::BigInt::operator*= (const TIDorb::types::BigInt &bi)
{
  (*this) = (*this) * bi; 
  _modified = true;
  return *this;
}


TIDorb::types::BigInt &  TIDorb::types::BigInt::operator/= (const TIDorb::types::BigInt &bi)
{
  (*this) = (*this) / bi; 
  _modified = true;
  return *this;
}


/* 
 *  PRE increment operator
 */
TIDorb::types::BigInt&  TIDorb::types::BigInt::operator++ ()
{		
  (*this) = (*this) + 1;
  _modified = true;
  return *this;
}


/*
 * POST increment operator
 */
TIDorb::types::BigInt  TIDorb::types::BigInt::operator++ (int)
{
  TIDorb::types::BigInt result = (*this);
  (*this) = (*this)  + 1;		
  _modified = true;
  return result;
}


/* 
 * PRE decrement operator
 */
TIDorb::types::BigInt&   TIDorb::types::BigInt::operator-- ()
{
  (*this) = (*this) - BigInt(1);
  _modified = true;
  return *this;
}


/* 
 * POST decrement operator
 */
TIDorb::types::BigInt   TIDorb::types::BigInt::operator-- (int)
{
  TIDorb::types::BigInt result = (*this);
  (*this) = (*this) - BigInt(1);
  _modified = true;
  return result;
}


TIDorb::types::BigInt TIDorb::types::BigInt::operator+ () const
{
  return *this;
}


TIDorb::types::BigInt TIDorb::types::BigInt::operator- () const
{
  TIDorb::types::BigInt res((TIDorb::types::BigInt&)*this);
  res.sign = sign*(-1);
  return res;
}


TIDorb::types::BigInt TIDorb::types::BigInt::operator/ (const TIDorb::types::BigInt &v) const
{
  TIDorb::types::BigInt quotient = 0;
  TIDorb::types::BigInt remainder = 0;
  TIDorb::types::BigInt::div(quotient, *this, v, remainder);
  return quotient;
}


TIDorb::types::BigInt::operator CORBA::LongLong () const
{
  return atol(toString());
}


TIDorb::types::BigInt::operator CORBA::LongDouble () const
{
  return atol(toString());
  // ¿¿atold(to_string()); ??
}


unsigned char TIDorb::types::BigInt::decimal_converter [80] [35] =

{
{ 1,1,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6},
{ 2,0,5,3,1,9,7,5,3,1,9,7,5,3,1,9,7,5,3,1,9,7,5,3,1,9,7,5,3,1,9,7,5,3,1},
{ 2,0,2,5,2,2,7,6,9,6,6,1,0,3,0,0,5,4,7,4,4,9,8,1,8,8,3,2,5,2,2,7,6,9,6},
{ 3,0,0,5,7,7,7,0,7,1,3,6,1,0,6,0,4,1,2,0,6,2,1,5,4,2,1,2,7,9,9,9,2,9,3},
{ 3,0,0,6,7,6,2,1,2,5,1,0,8,5,8,2,4,1,3,8,8,4,0,7,3,1,0,5,7,4,9,1,6,3,2},
{ 4,0,0,0,7,9,6,7,9,5,2,7,7,9,2,2,3,2,1,9,9,5,0,4,6,5,3,1,9,2,7,6,6,6,8},
{ 4,0,0,0,6,4,1,6,7,9,5,4,4,3,1,9,0,8,2,5,0,2,1,6,7,4,5,7,0,0,3,2,0,9,7},
{ 4,0,0,0,1,9,1,7,3,0,4,7,2,4,5,2,8,6,6,0,3,3,3,3,3,3,3,3,1,1,2,9,1,2,8},
{ 5,0,0,0,0,2,5,9,0,7,6,1,7,5,2,3,2,7,6,5,5,9,7,1,9,0,8,8,3,6,2,2,9,1,1},
{ 5,0,0,0,0,4,9,4,4,3,9,9,8,3,7,6,0,1,2,1,5,5,8,5,4,4,2,5,4,3,4,1,0,3,1},
{ 6,0,0,0,0,0,9,7,9,7,6,2,6,9,4,9,6,3,3,6,4,5,1,9,5,6,9,6,5,0,2,0,3,1,6},
{ 6,0,0,0,0,0,0,4,5,0,8,6,0,5,9,4,0,4,5,3,5,6,9,1,9,4,7,9,2,1,5,2,5,9,7},
{ 6,0,0,0,0,0,1,1,7,4,2,4,5,7,3,0,7,7,2,8,0,9,1,1,5,4,2,4,1,1,2,6,7,7,5},
{ 7,0,0,0,0,0,0,8,5,4,8,1,4,3,2,3,0,0,0,4,8,1,3,7,2,4,8,9,1,8,6,0,8,0,2},
{ 7,0,0,0,0,0,0,2,0,7,4,6,3,3,4,5,8,6,5,6,9,0,0,1,8,4,7,3,1,4,1,6,1,0,0},
{ 8,0,0,0,0,0,0,0,2,6,6,9,1,4,0,8,3,4,9,2,7,3,3,6,9,5,3,8,9,2,5,1,1,4,6},
{ 8,0,0,0,0,0,0,0,7,4,6,1,2,6,7,2,0,7,9,7,9,8,5,7,9,5,9,0,4,7,5,2,3,8,0},
{ 9,0,0,0,0,0,0,0,0,4,3,8,8,2,6,6,9,3,8,2,7,2,4,9,8,3,9,4,5,5,5,1,1,5,5},
{ 9,0,0,0,0,0,0,0,0,8,2,5,9,4,1,7,2,3,3,8,7,6,8,3,7,2,2,6,0,0,6,8,9,7,1},
{ 9,0,0,0,0,0,0,0,0,1,2,2,0,1,5,2,7,6,2,1,7,1,6,2,3,0,0,6,0,4,8,5,7,5,4},
{10,0,0,0,0,0,0,0,0,0,7,9,0,5,6,8,8,4,6,7,8,7,3,3,9,1,2,9,4,5,3,9,2,4,1},
{10,0,0,0,0,0,0,0,0,0,4,8,5,2,3,4,5,3,6,5,3,2,5,4,6,6,2,3,5,2,0,8,3,9,1},
{10,0,0,0,0,0,0,0,0,0,0,0,8,6,0,3,1,6,4,3,4,3,7,9,4,1,2,8,9,2,7,1,5,3,0},
{11,0,0,0,0,0,0,0,0,0,0,2,4,1,6,5,9,4,6,5,1,8,1,8,9,4,5,6,0,4,1,6,4,0,1},
{11,0,0,0,0,0,0,0,0,0,0,1,9,8,9,8,4,8,6,1,3,4,7,6,4,6,2,5,5,4,2,5,8,4,4},
{12,0,0,0,0,0,0,0,0,0,0,0,0,2,0,5,8,3,4,4,3,8,1,9,0,6,0,4,9,4,7,7,5,6,9},
{12,0,0,0,0,0,0,0,0,0,0,0,3,2,4,8,7,9,2,1,2,6,3,1,4,6,6,2,8,1,9,8,1,5,3},
{13,0,0,0,0,0,0,0,0,0,0,0,0,9,2,6,5,0,0,3,4,3,3,9,4,7,2,6,8,7,8,6,0,0,4},
{13,0,0,0,0,0,0,0,0,0,0,0,0,7,8,9,9,2,6,2,2,0,4,8,3,0,6,2,1,3,0,9,0,4,8},
{14,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,9,9,7,6,5,2,1,6,4,2,1,6,3,6,1,8,6,6,9},
{14,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,7,6,1,0,9,8,0,6,2,3,1,7,2,3,1,3,7,5,3},
{14,0,0,0,0,0,0,0,0,0,0,0,0,0,2,9,2,6,3,3,3,1,6,9,5,2,4,3,3,7,5,8,7,6,0},
{15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,9,5,8,9,0,3,5,4,3,3,4,6,4,4,8,6,4},
{15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,9,2,5,8,3,2,7,7,3,9,2,9,0,0,2,7,5,4,4},
{16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,8,8,9,2,0,4,4,3,8,9,3,7,3,5,2,3,8,7},
{16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,2,1,8,9,1,6,7,7,0,6,6,6,4,8,8,9,0},
{16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,2,5,0,0,1,3,3,5,2,5,3,9,5,5,1,9,6},
{17,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,1,4,7,3,1,5,3,3,6,9,8,1,2,1,5,6,1},
{17,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,1,7,7,3,7,0,7,8,9,2,6,0,0,5,0,2,1},
{18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,7,6,8,1,3,1,2,7,7,7,8,0,3,7},
{18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,0,9,3,5,1,2,6,4,4,2,8,7,1,4,5,3},
{19,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,9,6,1,1,2,7,5,7,0,0,2,9,1,8,4},
{19,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,8,1,9,4,4,0,5,8,8,5,2,2,7,7,0},
{19,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,1,0,5,8,7,3,1,1,6,4,8,0,1},
{20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,5,4,3,8,6,2,5,9,0,8,7,1,9,4},
{20,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,1,4,1,3,6,0,0,7,7,5,0,0,7,4},
{21,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,4,7,5,8,9,1,9,6,5,0,9,8,0},
{21,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,1,9,6,3,9,5,6,6,7,5,1,6,1},
{21,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,4,0,8,5,8,1,6,4,4,7,0,8,4},
{22,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,8,2,3,5,0,8,9,3,9,6,0,2},
{22,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,7,9,7,2,3,1,7,4,2,1,0,2},
{23,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,9,1,4,0,7,9,7,3,4,5,2},
{23,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,1,0,4,3,5,3,8,8,2,8,6},
{24,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,1,0,3,5,6,7,5,3,9,1},
{24,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,7,8,9,8,0,3,9,3,0,2},
{24,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,7,3,3,6,5,6,2,7,7,7},
{25,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,9,1,6,1,5,3,3,5,1},
{25,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,1,7,0,4,8,3,4},
{26,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,9,6,9,8,8,2,3},
{26,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,3,2,6,7,3,3,4,4},
{26,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,2,6,6,8,6,5,8},
{27,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,4,4,7,6,9,2},
{27,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,3,9,3,7,2,1,0},
{28,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,9,6,4,3,6,6},
{28,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,4,6,8,1,4},
{28,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,9,7,0,5,3,9},
{29,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,1,7,8,7,2},
{29,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,4,4,3,5},
{30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,8,8,2,7},
{30,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,2,9,4},
{31,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,1,8,4},
{31,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,3,0,8},
{31,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,2,4},
{32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,9,7},
{32,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,7,7},
{33,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,2},
{33,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,4},
{33,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,6},
{34,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9},
{34,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2}
};





