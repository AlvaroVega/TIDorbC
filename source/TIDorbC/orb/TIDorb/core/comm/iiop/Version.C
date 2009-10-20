//////////////////////////////////////////////////////////////////////////////////
//
// File:        Version.C
// Description: 
//
// Rel:         01.00
// Created:     September, 2002
// Author:      Juan A. Caceres Exposito      ( caceres@tid.es   )
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
///////////////////////////////////////////////////////////////////////////////////

#include "TIDorb/core.h"

const TIDorb::core::comm::iiop::Version TIDorb::core::comm::iiop::Version::VERSION_1_0 (1,0);
const char* TIDorb::core::comm::iiop::Version::version_1_0_name = "Version(1.0)";
	
const TIDorb::core::comm::iiop::Version TIDorb::core::comm::iiop::Version::VERSION_1_1 (1,1);
const char* TIDorb::core::comm::iiop::Version::version_1_1_name = "Version(1.1)";

const TIDorb::core::comm::iiop::Version TIDorb::core::comm::iiop::Version::VERSION_1_2 (1,2);
const char* TIDorb::core::comm::iiop::Version::version_1_2_name = "Version(1.2)";


TIDorb::core::comm::iiop::Version::Version()
:	_major(1), _minor(2) {};


TIDorb::core::comm::iiop::Version::Version(const TIDorb::core::comm::iiop::Version& v)
: _major(v._major), _minor (v._minor) {};

TIDorb::core::comm::iiop::Version::Version(CORBA::ULong maj, CORBA::ULong min)
:	_major(maj) ,	_minor(min) {};

TIDorb::core::comm::iiop::Version& 
  TIDorb::core::comm::iiop::Version::operator=(const TIDorb::core::comm::iiop::Version& v)
{
   _major = v._major;
   _minor = v._minor;
   
   return *this;
};
	
CORBA::ULong TIDorb::core::comm::iiop::Version::getMajor() const
{
	return _major;
};
	
CORBA::ULong TIDorb::core::comm::iiop::Version::getMinor() const
{
	return _minor;
};
	
bool TIDorb::core::comm::iiop::Version::operator==(const TIDorb::core::comm::iiop::Version& ver) const
{
	return (_major == ver._major) && (_minor == ver._minor);
};

bool TIDorb::core::comm::iiop::Version::operator!=(const TIDorb::core::comm::iiop::Version& ver) const
{
	return (_major != ver._major) && (_minor != ver._minor);
};

bool TIDorb::core::comm::iiop::Version::operator>(const TIDorb::core::comm::iiop::Version& ver) const
{
  if(_major < ver._major)
    return false;
  else if(_major == ver._major)
    return 	(_minor > ver._minor);
  else
    return true;
};

bool TIDorb::core::comm::iiop::Version::operator>=(const TIDorb::core::comm::iiop::Version& ver) const
{
  if(_major < ver._major)
    return false;
  else if(_major == ver._major)
    return 	(_minor >= ver._minor);
  else
    return true;
};

bool TIDorb::core::comm::iiop::Version::operator<(const TIDorb::core::comm::iiop::Version& ver) const
{
  if(_major > ver._major)
    return false;
  else if(_major == ver._major)
    return 	(_minor < ver._minor);
  else
    return true;
};

bool TIDorb::core::comm::iiop::Version::operator<=(const TIDorb::core::comm::iiop::Version& ver) const
{
  if(_major > ver._major)
    return false;
  else if(_major == ver._major)
    return 	(_minor <= ver._minor);
  else
    return true;
};


void TIDorb::core::comm::iiop::Version::read(TIDorb::portable::InputStream& input)
{
	
	CORBA::Octet major;	
	input.read_octet(major);
	CORBA::Octet minor;
	input.read_octet(minor);

        _major = major;
        _minor = minor;	
  	
	if ((_major != 1)  || (_minor > 2))
  	  throw CORBA::MARSHAL();

  
};
	
void TIDorb::core::comm::iiop::Version::write(TIDorb::portable::OutputStream& output) const
{
	output.write_octet((CORBA::Octet)_major);
	output.write_octet((CORBA::Octet)_minor);
};

const char* TIDorb::core::comm::iiop::Version::toString() const
{
	if (_minor == 0)
		return TIDorb::core::comm::iiop::Version::version_1_0_name;
	if (_minor == 1)
		return TIDorb::core::comm::iiop::Version::version_1_1_name;
	else
		return TIDorb::core::comm::iiop::Version::version_1_2_name;

};
	
