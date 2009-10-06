/////////////////////////////////////////////////////////////////////////
//
// File:        Fixed.C
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



// explicit use of limit macros in linux for c++  
#define __STDC_LIMIT_MACROS 

#include "CORBA.h"

#ifdef TIDORB_HAVE_IOSTREAM
#include <iostream>
#else
#include <iostream.h>
#endif

#ifdef TIDORB_HAVE_NAMESPACE_STD
  using namespace std;
#endif

#include <ctype.h>
#include <stdlib.h> 
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <limits.h>




/*
 * Constructor Vacio.
 */
CORBA::Fixed::Fixed ()
  : _digits(0), _scale(0), _val(0)
{
}


/*
 * Constructor a partir de BigInt
 */
CORBA::Fixed::Fixed (TIDorb::types::BigInt val)
  : _digits(0), _scale(0), _val(val)
{
}


/*
 * Constructor a partir de un long 
 */
CORBA::Fixed::Fixed (CORBA::Long v)
{
  char buffer[100];
  sprintf(buffer,"%d", v);
  _val = extractFromString(buffer, _digits, _scale);
  adjust(); 
}


/*
 * Constructor a partir de un Ulong 
 */
CORBA::Fixed::Fixed (CORBA::ULong v)
{
  char buffer[100];
  sprintf(buffer,"%d", v);
  _val = extractFromString(buffer, _digits, _scale);
  adjust();
}


/*
 * Constructor a partir de un long long
 */
CORBA::Fixed::Fixed (CORBA::LongLong v)
{
  char buffer[100];
  sprintf(buffer,"%E", (double)v);
  _val = extractFromString(buffer, _digits, _scale);
  adjust();
}

/*
 * Constructor a partir de un Ulong long
 */
CORBA::Fixed::Fixed (CORBA::ULongLong v)
{
  char buffer[100];
  sprintf(buffer,"%d", (int)v);
  _val = extractFromString(buffer, _digits, _scale);
  adjust();
}


/*
 * Constructor a partir de un double
 */
CORBA::Fixed::Fixed (CORBA::Double v)
{
  char buffer[100];
  sprintf(buffer,"%E", v);
  _val = extractFromString(buffer, _digits, _scale);
  adjust();
}

/*
 * Constructor a partir de un long Double
 */
CORBA::Fixed::Fixed (CORBA::LongDouble v)
{
  char buffer[100];
  sprintf(buffer,"%LE", v);
  _val = extractFromString(buffer,_digits, _scale);
  adjust();
}

/*
 * Constructor a partir de un String 
 */
CORBA::Fixed::Fixed(const char *s)
{
  _val = extractFromString(s,_digits, _scale);
}



/*
 * Asignacion!!!  a partir de una cadena.
 * Dado que la asignacion se modifica por el hecho de amoldarse al asignado.
 * Esta es la forma de reasignarse modificando tambien la escala y los digitos (no amoldarse)
 */
CORBA::Fixed& CORBA::Fixed::from_string(const char* value)
{
  _val = extractFromString(value,_digits, _scale);
  return *this;
}

/* 
 * Multiplica un entero por la potencia de diez del parametro scale 
 * Con el objetivo de desplazar la cifra hacia la izquierda. 
 */
TIDorb::types::BigInt CORBA::Fixed::timesTenToThe(const TIDorb::types::BigInt &intPart,
                                                  unsigned short scale)
{
  TIDorb::types::BigInt value = (TIDorb::types::BigInt)intPart;
        
  int hundredTimes = scale / 2; // Veces cien lo moveremos.
  int tenTimes = scale % 2; // Veces 10 lo moveremos o 0 o 1.
  
  TIDorb::types::BigInt resul = value;
  if(hundredTimes) { 
    // Por que si no multiplicamos por 0
    value.x100(hundredTimes);
  }
  
  if(tenTimes) { 
    // Por que si no multiplicamos por 0
    resul = value * (10 * tenTimes);
  }
  else {
    resul = value;	
  }
  
  return resul;
}

/* 
 * Extrae un "Fixed" de una cadena de caracteres.
 * Aunque la cadena de caracteres contenga decimales el valor devuelto es entero.
 * El algoritmo indica en scale la escala a la que se ha transformado el numero 
 * contenido en la cadena para  evitar que contenga caracteres 'e'.
 * Ejemplo:
 *        str= 8323.3223e5 devuelve 832332230 en scala 0 y en digits 8. la escala nunca es negativa.
 *        str=0.003234 devuelve 3234 con escala 6 y en digits 7
 * La escala indica el numero de lugares que hay que desplazar desde el final para poner el "." 
 */  
TIDorb::types::BigInt CORBA::Fixed::extractFromString (const char *str,
                                                       CORBA::UShort& digits,
                                                       CORBA::Short& scale)
{
  digits = 0; 
  scale = 0;
	
  while (*str && (isspace(*str))) {
    ++str; 
  }

  if(*str && (*str == '+') ) {
    ++str;  /* Discard leading '+' */
    if(*str && (*str == '-')) {
      // /* "+-123.456" illegal! */
      throw CORBA::DATA_CONVERSION("Fixed::extractFromString found characters: +-"); 
    }
  }
        
  // Eliminamos la D si existiera.
  char* decimalPart;
  decimalPart = strdup(str);
  if(*decimalPart ) {

    char* dExp = strchr(decimalPart,'d');
    if (dExp == NULL)
      dExp = strchr(decimalPart,'D');
    if(dExp!=NULL) {
      // Ha existido la D en alguna de sus posibles. [D|d]
      if(dExp == (decimalPart+strlen(decimalPart)-1)) { 
        // The D is at the end of STR
        *dExp = '\0';
      }
      else {
        // Unexpected D inside fixed number.
        throw CORBA::DATA_CONVERSION("Fixed::extractFromString() unexpected D inside fixed number");
      }
    }	
    
  }

  // If exponent is present, break into exponent and significand
  int exponent = 0;
  const char* eExpr = strchr(str,'e'); // eExpr = "e234";
        
  if (eExpr == NULL)
    eExpr = strchr(str,'E');
  if (eExpr != NULL) {
    if (strlen(eExpr) == 0) {              
      /* "1.2e" illegal! */
      throw CORBA::DATA_CONVERSION("Fixed::extractFromString() number finished in character e is ilegal");
    }
    if (*eExpr && (*eExpr == '+') ) {
      eExpr++;         /* Discard leading '+' */
      if (*eExpr && (*eExpr == '-')) {
        /* "123.456e+-7" illegal! */
        throw CORBA::DATA_CONVERSION("Fixed::extractFromString() number with e+- is ilegal");
      }
    }
    exponent = atoi(eExpr+1);
    if (eExpr == str) { 
        /* "e123" illegal! */
      throw CORBA::DATA_CONVERSION("Fixed::extractFromString() number with only e is ilegal"); 
      }
    int indxofE = strlen(str)-strlen(eExpr);
    decimalPart[indxofE] = '\0'; // Cortamos str antes de E. 
  }       
        
  // Parse significand
        
  char* pointPos = strchr(decimalPart,'.');
  //int intVal;
  TIDorb::types::BigInt intValue;
        
  if (pointPos == NULL) 
    {
      /* e.g. "123" */
      TIDorb::types::BigInt intPart = TIDorb::types::BigInt(decimalPart);
      intValue = timesTenToThe(intPart, scale);
      digits = intValue._digits(); // los digitos son todos.
    } 
  else if (pointPos == (decimalPart+(strlen(decimalPart)-1)))
    { /* e.g. "123." */
      *pointPos = '\0';
      TIDorb::types::BigInt intPart = TIDorb::types::BigInt(decimalPart);
      intValue = timesTenToThe(intPart, scale);
      digits = intValue._digits(); // los digitos son todos.
    } 
  else 
    { /* Fraction part exists */
                
      // El numero es del tipo 123.4523 entonces digits = (7) strlen decimaPart -1 por el punto
      if ( ((char)*(pointPos+1)) == '-')       
        {
          /* ".-123" illegal! */
          throw CORBA::DATA_CONVERSION("Fixed::extractFromString() number with .- is ilegal");
        }
      // Eliminamos los ceros a la derecha decimal.
      for(int i = strlen(pointPos)-1;i >= 0;i--)
        { // Comenzamos a mirar la cadena con el punto desde el final mientras sean 0 los cortamos
          // Cuando sea numero paramos 1.3000040000'\0' -->	1.300004'\0'
          if(*(pointPos+i) == '0')
            *(pointPos+i) = '\0';
          else break;
        }
      int zeroCounter = 0;
      for(int j = 1;j < strlen(pointPos);j++)
        { // Comenzamos a mirar la cadena con el punto desde el principio y mientras sean 0 
          // incrementamos el contador cuando sea numero paramos 
          // ->1.000040000'\0'   -->	1.000004 es distinto de 1.4 que es lo que genera sin esto.
          if(*(pointPos+j) == '0')
            zeroCounter++;
          else break;
        }
      TIDorb::types::BigInt fraction = TIDorb::types::BigInt(pointPos+1);
      scale = fraction._digits() + zeroCounter;
      digits = scale; // por ahora todos los digitos son  los de la parte decimal.     
      if (pointPos == decimalPart) 
        { /* e.g.  ".123" */
          intValue = fraction;
        }
      else if (*decimalPart && (*decimalPart == '-' && pointPos == (decimalPart+1))) 
        {// Si existe el signo negativo se contó como un digito.
          intValue = (-fraction);
        } 
      else  
        { /* e.g. "-123.456" or "123.456"*/ 
          char* intString;
          int indxOfPoint;
          bool is_negative = (*decimalPart == '-');

          if (is_negative) {
            intString = strdup(decimalPart+1);
            indxOfPoint = strlen(decimalPart)-strlen(pointPos)-1;
          }
          else {
            intString = strdup(decimalPart);
            indxOfPoint = strlen(decimalPart)-strlen(pointPos);
          }
          intString[indxOfPoint] = '\0'; // Nos quedamos con la parte entera.
                  	
          TIDorb::types::BigInt intPart = TIDorb::types::BigInt(intString);
          // ahora hay que desplazar intPart tantos como diga la Escala para poder sumarle la parte
          intValue = timesTenToThe(intPart, scale);

          if (is_negative) {
            intValue += (fraction);
            intValue *= (-1);
          }
          else {
            if(intValue.getSign() == -1)
              intValue -= fraction;
            else
              intValue += fraction;
          }

          free(intString);
          digits = intValue._digits();
        }
    }

  if((exponent > 31) || (digits > 32))
    {
      throw CORBA::DATA_CONVERSION("Fixed::extractFromString() exponent > 31 or digits > 32");
    }
  // Combine exponent into significand
  scale -= exponent;
  if (scale < 0) {
    digits = intValue._digits();
    intValue = timesTenToThe(intValue, -scale);         
    scale = 0;
  }
        
  free(decimalPart);

  return intValue;
        
}

/*
 * Transforma el Fixed en una cadena de caracteres 
 */
char* CORBA::Fixed::to_string()const
{
  if(_val.getSign() == 0) {
    char * numero = new char[2];
    numero[0] = '0';
    numero[1] = '\0';
    return numero;
  }
    
  const char* numero = _val.toString();
  int digitos = strlen(numero);
    
  bool existsDot = 0; /* Marcara la existencia del punto decimal */
  bool existsImportant = 0; /* Marcara la existencia de algun numero distinto de 0 en la parte izquierda del punto */
    
  char *result =new char [100]; 
    
  int indice=0; /* Marcara el indicce por el que avanza la cadena de carcteres */
       
  if (_val.getSign() == -1) { 
    /* Si el numero es negativo, lo mostramos como tal */
    result[indice++] = '-';
    numero++; // por que la cadena tambien tiene el signo negativo.
    digitos--;
    /* pero la representacion numerica tiene que ser positiva pra tratarlo*/
  }
    
  if (_scale >= digitos) { 
    /* si la escala es mayor o igual que los digitos habra que poner 0's hasta igualar */
    result[indice++] = '0';
    result[indice++] = '.';
    existsDot = 1;
    for (int i = _digits; i < _scale; ++i)
      {// Sumamos 1 a digits por que el 0. que ya se ha añadido tb cuenta como un digit. creo.
        result[indice++] = '0';
      }
  }
  int r = 0;
  for (int i = 0; i < digitos;i++) {
        
    if((i == (digitos-_scale))&&!existsDot)
      { /* si estamos justo en el punto a escalar marcamos el punto*/
        result[indice++] = '.';
        existsDot = 1;
      }
                
    r = numero[i];// El caracter correspondiente al digito.
    if(r != '0')
      { /* Cualquier numero != 0 se imprime */
        existsImportant = 1;
        result[indice++] = r;
      }
    else 
      { /* r==0 solo se imprimen a la derecha del punto si se imprimio ya alguno */
        if((i == 0))
          { /* el primero como minimo aunque sea "0" para hacer el "0." */
            result[indice++] = r;
          }
        else if(existsImportant || existsDot)
          { /* si ya se imprimio alguno es de la parte decimal
               o existe punto decimal y nforma parte de la parte flotante
               y son necesarios por que habra alguno representativo mas atras. */         
            result[indice++] = r;
          }
        else 
          { /* EN OTRO CASO NO SE IMPRIMEN. SON LOS QUE TRATA DE QUITAR TODA ESTA PARANOIA. */
            ;       
          }
      }
    /*  preparamos la siguiente vuelta */
        
  }
  if(!existsDot)
    { /* Si no hubo punto estos 0 importan en otro caso serian 0 a la derecha decimal que no cuentan. */
      for (int i1 = _scale; i1 < 0; ++i1)
        result[indice++] = '0';
    }
  else
    {// por el hecho de existir puntos, si habia ceros a la derecha, esos zeros no cuentan.
      int i1=indice-1;
      for (; i1 >= 0; i1--)
    	{
          if(result[i1] == '0')
            result[i1] = '\0';
          else break;
    	}
      if(result[i1] == '.')// Si desaparece toda la parte decimal, al menos dejamos un 0.
        result[i1+1] = '0';
    }

  result[indice] = '\0';      
  return result;
}

/*
 * Constructor para asignar la escala y los digitos del Objeto 
 */
CORBA::Fixed::Fixed(CORBA::UShort d, CORBA::Short s)
  : _digits(d), _scale(s), _val(0)
{
  if ((d > 31) || (s > 31)) 
    throw CORBA::DATA_CONVERSION("Fixed(d,s) ilegal (d > 31) || (s > 31) ");
}

/* 
 * Constructor de copia 
 */
CORBA::Fixed::Fixed(const CORBA::Fixed &f)
  : _digits(f._digits), _scale(f._scale), _val(f._val)
{
}


CORBA::Fixed::Fixed (TIDorb::types::BigInt val, CORBA::Short s)
  : _digits(0), _scale(s), _val(val)
{
}


/* 
 *  Destructor 
 */
CORBA::Fixed::~Fixed()
{
}


/* 
 * Operador de Casting a CORBA::LongLong
 * Lanza excepcion en caso de overflow o underflow al hacer el casting.
 */
CORBA::Fixed::operator CORBA::LongLong () const
{   

#if defined (__sun) && defined (__GNUC__)
  static const CORBA::Fixed maxLongLong((int64_t)INT64_MAX);
  static const CORBA::Fixed minLongLong((int64_t)INT64_MIN);
#else    
  static const CORBA::Fixed maxLongLong(INT64_MAX);
  static const CORBA::Fixed minLongLong(INT64_MIN);
#endif
  if ( ((*this) > maxLongLong) || ((*this) < minLongLong) )
    throw CORBA::DATA_CONVERSION("Fixed::operator LongLong(): overflow");
  else
    {
      CORBA::LongLong result;
      
      CORBA::Fixed unscaled;
      
      unscaled = *this;
 	
      CORBA::String_var str = unscaled.fixed_value().toString();
    	
      sscanf(str, "%d" , &result);
    	
      return result;
    }
}

/* 
 * Operador de Casting a CORBA::LongDouble
 */
CORBA::Fixed::operator CORBA::LongDouble () const
{
  CORBA::String_var str = to_string();
  CORBA::LongDouble result;
  sscanf(str, "%Lf", &result);
  return result;
}

/* 
 * Operacion de Redondeo.
 */
CORBA::Fixed CORBA::Fixed::round (CORBA::UShort nscale) const
{
  Fixed f(_digits, nscale);
  f._val = rescale(nscale,_scale,_val);
  return f;
}

/* 
 * Operacion de Truncado.
 */
CORBA::Fixed CORBA::Fixed::truncate (CORBA::UShort nscale) const
{
  Fixed f (_digits, nscale);		 // El valor de este parametro por defecto es true, osea redondear.
  f._val = rescale(nscale,_scale,_val,false);
  return f;				// por ser falso, trunca.
}




/*
 * Operacion Transform
 * Reduce o aumenta el numero de decimales
 */
TIDorb::types::BigInt CORBA::Fixed::transform (CORBA::Short nscale) const
{
  // Supuestamente nscale siempre va a ser positivo, o cero.
  // o  Cero
  TIDorb::types::BigInt value;
  CORBA::Short fscale = nscale-_scale; /* Final scale */
  // final escale puede ser >0, 0 o <0
  if (fscale == 0)/* no hay nada que transformar son las mismas escalas */
    {	
      value = _val;
      return value;
    }
  else 
    {
      /* si la escala a la que pasamos es mayor, hay que añadir 0's */ 
      value = _val * power10 (fscale);
    }
  return value;
    
}


/*
 *
 */
TIDorb::types::BigInt& CORBA::Fixed::adjust()
{
  if ( (_val.getSign() != 0) && (_scale != 0) ) {
    TIDorb::types::BigInt previo = 0;
    TIDorb::types::BigInt coc    = 0;
    TIDorb::types::BigInt div    = 0;
    TIDorb::types::BigInt zero   = 0;
    coc = _val;
    int nscale = 0;


    while (previo == zero) {
      //El objetivo es reducir la representación interna al maximo.
      div = coc;
      TIDorb::types::BigInt::div(coc,div,10,previo);
      // Al dividir, si el resto es 0. el valor es reduccible.
      // Por ser !=0 el numero, esto parara.
      nscale++; //Indica la escala que hemos reducido.
    }
    nscale--; // la ultima división no cuenta.
    _scale -= nscale;
    _val = div;
  }
  return _val;
}


/* 
 * Operador de asignacion con transformacion de parametros.
 */
CORBA::Fixed & CORBA::Fixed::operator= (const CORBA::Fixed &f)
{
  _val = f.rescale(_scale,f._scale,f._val);
  //adjust ();
  _digits = CORBA::Fixed::digits(_val);
  if (_digits>31) {
    throw CORBA::DATA_CONVERSION("Fixed::operator= digits > 31");
  }
  return *this;
}

/*
 * Operador de suma y asignacion
 */
CORBA::Fixed & CORBA::Fixed::operator+= (const CORBA::Fixed &f)
{
  CORBA::Fixed res (_digits, _scale);
  add (res, *this, f);
  *this = res;
  return *this;
}

/*
 * Operador de resta y asignacion
 */
CORBA::Fixed & CORBA::Fixed::operator-= (const CORBA::Fixed &f)
{
  CORBA::Fixed res (_digits, _scale);
  sub (res, *this, f);
  *this = res;
  return *this;
}

/*
 * Operador de producto y asignacion
 */
CORBA::Fixed & CORBA::Fixed::operator*= (const CORBA::Fixed &f)
{
  CORBA::Fixed res (_digits, _scale);
  mul (res, *this, f);
  *this = res;
  return *this;
}

/*
 * Operador de division y asignacion
 */
CORBA::Fixed &CORBA::Fixed::operator/= (const CORBA::Fixed &f)
{
  CORBA::Fixed res (_digits, _scale);
  div (res, *this, f);
  *this = res;
  return *this;
}

/*
 * Operador de suma de una unidad y asignacion 
 * PREINCREMENTO
 */
CORBA::Fixed &CORBA::Fixed::operator++ ()
{
  CORBA::Fixed res (_digits, _scale);
  add (res, *this, CORBA::Fixed ((CORBA::LongDouble)1.0));
  *this = res;
  return *this;
}

/*
 * Operador de suma de una unidad y asignacion
 * POSTINCREMENTO
 */
CORBA::Fixed CORBA::Fixed::operator++ (int)
{
  CORBA::Fixed res (*this);
  add (*this, res, CORBA::Fixed ((CORBA::LongDouble)1.0));
  return res;
}

/*
 * Operador de resta de una unidad y asignacion
 * PREDECREMENTO
 */
CORBA::Fixed& CORBA::Fixed::operator-- ()
{
  CORBA::Fixed res (_digits, _scale);
  sub (res, *this, CORBA::Fixed ((CORBA::LongDouble)1.0));
  *this = res;
  return *this;
}

/*
 * Operador de resta de una unidad y asignacion
 * POSTDECREMENTO
 */
CORBA::Fixed CORBA::Fixed::operator-- (int)
{
  CORBA::Fixed res (*this);
  sub (*this, res, CORBA::Fixed ((CORBA::LongDouble)1.0));
  return res;
}

/*
 * Operador unario positivo 
 */
CORBA::Fixed CORBA::Fixed::operator+ () const
{
  return *this;
}

/*
 * Operador unario negativo
 */
CORBA::Fixed CORBA::Fixed::operator- () const
{
  CORBA::Fixed res (_digits, _scale);
  res._val = CORBA::Fixed::rescale(res._scale,_scale, - _val);
  res._digits = CORBA::Fixed::digits(res._val);    
  return res;
}

/*
 * Operador unario de negacion
 */
CORBA::Boolean CORBA::Fixed::operator!() const
{
  return !_val;
}



#define FMAX(a,b) ((a)>(b)?(a):(b))

/*
 * Operador binario de suma
 */
CORBA::Fixed CORBA::Fixed::operator+ (const CORBA::Fixed& f) const
{
  CORBA::Short scale = FMAX (fixed_scale(), f.fixed_scale());
  CORBA::UShort digits = 1+scale + FMAX (fixed_digits()-fixed_scale(),
                                         f.fixed_digits()-f.fixed_scale());

  if (digits > 31) {
    	
    scale = 31 - digits + scale;
    digits = 31;
    //        if(scale<0)
    //        {// El numero no se va a poder representar correctamente con escala negativa, es decir
    //         // faltarian digitos.
    //        	throw DATA_CONVERSION();
    //        }        
 
  }
  CORBA::Fixed res (digits, scale);
  add (res, *this, f);
  return res;
}

/*
 * Operador binario de resta
 */
CORBA::Fixed CORBA::Fixed::operator- (const CORBA::Fixed& f) const
{
  CORBA::Short scale = FMAX (fixed_scale(), f.fixed_scale());
  CORBA::UShort digits = 1+scale + FMAX (fixed_digits()-fixed_scale(),
                                         f.fixed_digits()-f.fixed_scale());
  if (digits > 31) {
    scale = 31 - digits + scale;
    digits = 31;
    //        if(scale<0)
    //        {// El numero no se va a poder representar correctamente con escala negativa, es decir
    //         // faltarian digitos.
    //        	throw DATA_CONVERSION();
    //        }
  }
  CORBA::Fixed res (digits, scale);
  sub(res, (*this), f);
  return res;
}

/*
 * Operador binario de multiplicacion
 */
CORBA::Fixed CORBA::Fixed::operator* (const CORBA::Fixed& f) const
{
  CORBA::Short scale = fixed_scale() + f.fixed_scale();
  CORBA::UShort digits = fixed_digits() + f.fixed_digits();
  if (digits > 31) {
    scale = 31 - digits + scale;
    digits = 31;
    //        if(scale<0)
    //        {// El numero no se va a poder representar correctamente con escala negativa, es decir
    //         // faltarian digitos.
    //        	throw DATA_CONVERSION();
    //        }
  }
  CORBA::Fixed res (digits, scale);
  mul (res, (*this), f);
  return res;
}

/*
 * Operador binario de division
 */
CORBA::Fixed CORBA::Fixed::operator/ (const CORBA::Fixed& f) const
{
  CORBA::Short scale = fixed_scale() + f.fixed_scale();
  CORBA::UShort digits = fixed_digits() -fixed_scale() + f.fixed_scale();
  // XXX what are the correct digits and scale for result???
  if (digits > 31) {
    scale = 31 - digits + scale;
    digits = 31;
    //        if(scale<0)
    //        {// El numero no se va a poder representar correctamente con escala negativa, es decir
    //         // faltarian digitos.
    //        	throw DATA_CONVERSION();
    //        }
  }
  CORBA::Fixed res (fixed_digits(), fixed_scale());
  div (res, (*this), f);
  return res;
}

#undef FMAX

/*
 * OPERADOR de igualdad
 */

bool CORBA::Fixed::operator == (const CORBA::Fixed& f) const
{
  if (_scale == f._scale)
    return (_val == f._val);
  if (_scale > f._scale)
    return _val == f.transform (_scale);
        
  return transform (f._scale) ==f._val;
}

/*
 * OPERADOR de desigualdad
 */
bool CORBA::Fixed::operator != (const CORBA::Fixed& f) const
{
  return ! ((*this) == f);    
}

/*
 * Operador menor que 
 */
bool CORBA::Fixed::operator < (const CORBA::Fixed& f) const
{
  if (_scale == f._scale)
    return _val < f._val;
  if (_scale > f._scale)
    return _val < f.transform (_scale);
        
  return transform (f._scale) < f._val;
}

/*
 * Operador mayor que 
 */
bool CORBA::Fixed::operator > (const CORBA::Fixed& f) const
{
  return ! ((*this) <= f);
}

/*
 * Operador menor o igual 
 */
bool CORBA::Fixed::operator <= (const CORBA::Fixed& f) const
{
  if (_scale == f._scale)
    return (_val <= f._val);
  if (_scale > f._scale)
    return (_val <= f.transform (_scale));
        
  return (transform (f._scale)  <= f._val);
}

/*
 * Operador mayor igual que 
 */
bool CORBA::Fixed::operator >= (const CORBA::Fixed& f) const
{
  return ! ((*this) < f);
}


TIDorb::types::BigInt CORBA::Fixed::power10 (int x)
{
  static const TIDorb::types::BigInt unidad = 1;
  TIDorb::types::BigInt resul = unidad; // Ya que en caso de ser x=0 10^0 =1.
    
  int hundredTimes = x/2; // Veces cien lo moveremos.
    
  if(hundredTimes)
    {// Por que si no multiplica por 0 metemos la zarpa.
      resul.x100(hundredTimes);
    }
    
  int tenTimes = x%2; // Veces 10 lo moveremos o 0 o 1.
  if(tenTimes)
    { // Por que si no multiplica por 0 metemos la zarpa.
      resul *= 10;
    }
 
  return resul;
}


CORBA::LongDouble CORBA::Fixed::epsilonl (CORBA::LongDouble v)
{
  static CORBA::LongDouble d = 0.0;
  if (d == 0.0) {
    /*
     * calculate the error in representing 0.6 in a LongDouble.
     */
    d = 6;
    d /= 10;
    /*
     * subtract 0.5 before multiply instead of subtracting 5
     * afterwards to make sure multiplying by 10 doesn't overflow
     * mantissa.
     */
    d -= 0.5;
    d *= 10;
    d -= 1;

    int exp;
    frexp (d, &exp); 

    d = ldexp (1.0, exp); 
    /*
     * now d is the smallest power of 2 that is greater than
     * the error in representing 0.6 as a LongDouble
     */
  }

  int exp;
  frexp (v, &exp); 
  return ldexp (d, exp); 
}


CORBA::Double CORBA::Fixed::epsilon (CORBA::Double v)
{
  static CORBA::Double d = 0.0;
  if (d == 0.0) {
    /*
     * calculate the error in representing 0.6 in a Double.
     */
    d = 6;
    d /= 10;
    /*
     * subtract 0.5 before multiply instead of subtracting 5
     * afterwards to make sure multiplying by 10 doesn't overflow
     * mantissa.
     */
    d -= 0.5;
    d *= 10;
    d -= 1;

    int exp;
    frexp (d, &exp); 

    d = ldexp (1.0, exp);
    /*
     * now d is the smallest power of 2 that is greater than
     * the error in representing 0.6 as a Double
     */
  }

  int exp;
  frexp (v, &exp);
  return ldexp (d, exp);
}


/* 
 * Devuelve el numero de digitos dentro de un BigInt (entero).
 */
CORBA::UShort CORBA::Fixed::digits (const TIDorb::types::BigInt& val)
{
  const char * cad = val.toString();
  CORBA::UShort digits = strlen(cad);
  if(val.getSign() == -1) // Si el numero es negativo descontamos el signo.
    digits--;
  return digits;
}

/*
 * Operacion de Transformacion de una scala a otra 
 * Si el exponente actual es menor que el que pasan como parametro se multiplica 
 *   por la potencia de 10 resultado de la resta  el valor 
 *   	Si es al contrario se divide por la potencia de diez 
 *   	El numero es el mismo
 */

TIDorb::types::BigInt 
CORBA::Fixed::rescale(CORBA::Short nscale, CORBA::Short oscale,
                      const TIDorb::types::BigInt& value, bool round) 
{
  // Si el ultimo parametro que esta predefinido a true es cierto, entonces redondea, en otro caso trunca.
  
  CORBA::Short fscale=nscale-oscale; /* Final scale */
  static const TIDorb::types::BigInt cinco=5;
  
  if (fscale == 0)
    {// Si las escalas son las mismas devolvemos el valor sin modificarlo.
      TIDorb::types::BigInt val;
      val=value;
      return val;
    }
  
  if(fscale < 0)
    { /* En este caso se van a perder digitos decimales asi que habra que redondear 
         Ejemplo:
         1001345 con escala = 5 (10.01345) si lo pasamos a escala 3 10013 --> 10.013
      */
      TIDorb::types::BigInt coc = 0;
      TIDorb::types::BigInt rest = 0;			
      TIDorb::types::BigInt cincoPotScale = 0;
      if(round)			// en este caso fscale es negativo.
        cincoPotScale = cinco*power10((-fscale)-1);
      if(value.getSign() == -1)
        cincoPotScale *= -1;
 	
      TIDorb::types::BigInt::div(coc,value+cincoPotScale,power10(-fscale),rest);
      // El cociente es el resultado y el resto es lo que perdemos.
  	    	
      return coc;//*power10(fscale);

    }
  /* si la escala a la que pasamos es mayor, hay que añadir 0's */ 
  // fscale >0.
  TIDorb::types::BigInt val = 0;
  TIDorb::types::BigInt pot;
  pot=power10 (fscale);
  val = value * pot;
  return val;
       
}

/*
 * Operacion de Suma de Fixed. 
 * @param res El resultado de la operacion
 * @param a1 El primero operando
 * @param a2 El segundo operando
 * @return res=a1+a2
 */

void CORBA::Fixed::add (CORBA::Fixed &res, 
                        const CORBA::Fixed& a1, 
                        const CORBA::Fixed a2)
{
  CORBA::Short res_scale;
  TIDorb::types::BigInt val = 0;
  TIDorb::types::BigInt op1 = 0;
  TIDorb::types::BigInt op2 = 0;
    
  if (a1.fixed_scale() == a2.fixed_scale()) 
    {
      res_scale = a1.fixed_scale();
      op1 = a1.fixed_value();
      op2 = a2.fixed_value();
      val = a1.fixed_value() + a2.fixed_value();
    } else if (a1.fixed_scale() > a2.fixed_scale()) 
    {// Si la escala del op1 es mayor que la del operador 2
      // dejamos el resultado con la escala del operador 1 y transformamos la escala del op2.
      res_scale = a1.fixed_scale();
      op1 = a1.fixed_value();
      op2 = a2.transform (a1.fixed_scale());
      val = a1.fixed_value() + a2.transform (a1.fixed_scale());
    } else 
    {
      res_scale = a2.fixed_scale();
      op1 = a1.transform (a2.fixed_scale());
      op2 = a2.fixed_value();
      val = a1.transform (a2.fixed_scale()) + a2.fixed_value();
    }
   
  // si me pasan un fixed con una escala determinada se lo tengo que devolver con la misma.
  // por eso tengo que reajustar.
  res._val = Fixed::rescale(res.fixed_scale(),res_scale,val);
  res._digits =Fixed::digits(res.fixed_value());

}

/*
 * Operacion de Resta de Fixed. 
 * @param res El resultado de la operacion
 * @param a1 El primero operando
 * @param a2 El segundo operando
 * @return res=a1-a2
 */
void CORBA::Fixed::sub (CORBA::Fixed &res, 
                        const CORBA::Fixed& a1, 
                        const CORBA::Fixed a2)
{
  CORBA::Short res_scale;
  TIDorb::types::BigInt val = 0;
  TIDorb::types::BigInt op1 = 0;
  TIDorb::types::BigInt op2 = 0;
    
  if (a1.fixed_scale() == a2.fixed_scale()) 
    {
      res_scale = a1.fixed_scale();
      op1 = a1.fixed_value();
      op2 = a2.fixed_value();
      val = a1.fixed_value() - a2.fixed_value();
        
    } else if (a1.fixed_scale() > a2.fixed_scale()) 
    {
    	
      res_scale = a1.fixed_scale();
      op1 = a1.fixed_value();
      op2 = a2.transform (a1.fixed_scale());
      val = a1.fixed_value() - a2.transform (a1.fixed_scale());
        
    } else 
    {
      res_scale = a2.fixed_scale();
      op1 = a1.transform (a2.fixed_scale());
      op2 = a2.fixed_value();
      val = a1.transform (a2.fixed_scale()) - a2.fixed_value();
    }
  // si me pasan un fixed con una escala determinada se lo tengo que devolver con la misma.
  // por eso tengo que reajustar.
  res._val = Fixed::rescale(res.fixed_scale(),res_scale,val);
  res._digits = Fixed::digits(res.fixed_value());
}

/*
 * Operacion de Multiplicacion de Fixed. 
 * @param res El resultado de la operacion
 * @param a1 El primero operando
 * @param a2 El segundo operando
 * @return res=a1*a2
 */
void CORBA::Fixed::mul (CORBA::Fixed &res, 
                        const CORBA::Fixed& a1, 
                        const CORBA::Fixed a2)
{
  CORBA::Short res_scale;
  TIDorb::types::BigInt val = 0;
  TIDorb::types::BigInt op1 = 0;
  TIDorb::types::BigInt op2 = 0;
    
  if (a1.fixed_scale() == a2.fixed_scale()) {
    res_scale = a1.fixed_scale();
    op1 = a1.fixed_value();
    op2 = a2.fixed_value();
    val = a1.fixed_value() * a2.fixed_value();
  } else if (a1.fixed_scale() > a2.fixed_scale()) {
    res_scale = a1.fixed_scale();
    op1 = a1.fixed_value();
    op2 = a2.transform (a1.fixed_scale());
    val = a1.fixed_value() * a2.transform (a1.fixed_scale());
  } else {
    res_scale = a2.fixed_scale();
    op1 = a1.transform (a2.fixed_scale());
    op2 = a2.fixed_value();
    val = a1.transform(a2.fixed_scale()) * a2.fixed_value();
  }
    
  res_scale=res_scale*2;
    
  // si me pasan un fixed con una escala determinada se lo tengo que devolver con la misma.
  // por eso tengo que reajustar.
  res._val = Fixed::rescale(res.fixed_scale(),res_scale,val);
  res._digits = Fixed::digits(res.fixed_value());
    
}

/*
 * Operacion de Multiplicacion de Fixed. 
 * @paAram res El resultado de la operacion
 * @param a1 El primero operando
 * @param a2 El segundo operando
 * @return res=a1/a2
 */

void CORBA::Fixed::div (CORBA::Fixed &res, 
                        const Fixed &a1, 
                        const Fixed a2)
{
  CORBA::Short res_scale;
  TIDorb::types::BigInt coc = 0;
  TIDorb::types::BigInt resto=0;
  TIDorb::types::BigInt dividendo=0;
  TIDorb::types::BigInt divisor=0;
    
  // BigInt produce una division entera, asi que con el objetivo de producir decimales.
  // El valor que Se dividirá los despazaremos tantos digitos como precisión requiera el resultado.
  // y luego lo arreglamos para que quede bien.
  if (a1.fixed_scale() == a2.fixed_scale()) 
    {
      res_scale = a1.fixed_scale();
      dividendo=a1.fixed_value();
      divisor=a2.fixed_value();
    } 
  else if (a1.fixed_scale() > a2.fixed_scale()) 
    {
      res_scale = a1.fixed_scale();
      dividendo=a1.fixed_value();
      divisor=a2.transform (a1.fixed_scale());
    } 
  else 
    {
      res_scale = a2.fixed_scale();
      dividendo=a1.transform (a2.fixed_scale());
      divisor=a2.fixed_value();
    }
  // Con el objetivo de no perder mas precision que la fijada en el fixed.
  // Al multiplicar el dividendo por 10^(res.fixed_scale()+1) desplazamos la "," a la derecha (res.fixed_scale()+1) ceros.
  // EN LA DIVISION ENTERA el cociente tambien estara desplazado a la la derecha (res.fixed_scale()+1).
  dividendo*=power10(res.fixed_scale()+1);
  TIDorb::types::BigInt::div(coc,dividendo,divisor,resto);

  // Atencion a diferencia de los demas aqui se construye un nuevo fixed.
  Fixed fix=coc;
  // Es ahora al incrementar la escala (res.fixed_scale()+1) cuando volveremos a tener un cociente REAL
  // con tantos decimales como el fixed requiere.
  fix._scale += (res.fixed_scale()+1);
  // al construir un fixed a partir de un BigInt la escala quedara 0. y al sumarle res.fixed_scale()+1 
  // quedara con escala  res.fixed_scale()+1, al reajustar se redondeara esa +1 cifra a la que corresponda.	    
  res._val = Fixed::rescale(res.fixed_scale(),fix.fixed_scale(),fix.fixed_value());
  res._digits = Fixed::digits(res.fixed_value());

}
