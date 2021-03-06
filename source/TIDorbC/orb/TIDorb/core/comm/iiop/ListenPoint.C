//////////////////////////////////////////////////////////////////////////////////
//
// File:        ListenPoint.C
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

#include "TIDorb/core/comm/iiop/ListenPoint.h"




namespace TIDorb {
namespace core {
namespace comm {
namespace iiop {




ListenPoint::ListenPoint()
{
        _str = NULL;
        //_host = CORBA::string_dup("");
        _host = NULL;
        _port = 0;
        _ssl_port = 0;
        _incoming_interface = NULL;
        _outgoing_interface = NULL;

        // FT extensions
        _heartbeat_enabled = false;
        // end FT extensions
}




ListenPoint::ListenPoint(const char* host, CORBA::UShort port)
{
        _str = NULL;
        _host = CORBA::string_dup(host);
        _port = port;
        _ssl_port = 0;
        _incoming_interface = NULL;
        _outgoing_interface = NULL;

        // FT extensions
        _heartbeat_enabled = false;
        // end FT extensions
}


ListenPoint::ListenPoint(const char* host, CORBA::UShort port, 
                         const char* incoming_interface, const char* outgoing_interface)
{
        _str = NULL;
        _host = CORBA::string_dup(host);
        _port = port;
        _ssl_port = 0;
        _incoming_interface = CORBA::string_dup(incoming_interface);
        _outgoing_interface = CORBA::string_dup(outgoing_interface);

        // FT extensions
        _heartbeat_enabled = false;
        // end FT extensions
}


ListenPoint::ListenPoint(const ListenPoint& other)
{
        _str = CORBA::string_dup(other._str);
        _host = CORBA::string_dup(other._host);
        _port = other._port;
        _ssl_port = other._ssl_port;
        _incoming_interface = CORBA::string_dup(other._incoming_interface);
        _outgoing_interface = CORBA::string_dup(other._outgoing_interface);

        // FT extensions
        _heartbeat_enabled = other._heartbeat_enabled;
        // end FT extensions
}




ListenPoint::~ListenPoint()
{
        CORBA::string_free(_str);
        CORBA::string_free(_host);
        CORBA::string_free(_incoming_interface);
        CORBA::string_free(_outgoing_interface);
}




ListenPoint& ListenPoint::operator= (const ListenPoint& other)
{
  CORBA::string_free(_str);
  CORBA::string_free(_host);
  CORBA::string_free(_incoming_interface);
  CORBA::string_free(_outgoing_interface);

  _str  = CORBA::string_dup(other._str);
  _host = CORBA::string_dup(other._host);
  _incoming_interface = CORBA::string_dup(other._incoming_interface);
  _outgoing_interface = CORBA::string_dup(other._outgoing_interface);
  _port = other._port;
  _ssl_port = other._ssl_port;
  // FT extensions
  _heartbeat_enabled = other._heartbeat_enabled;
  // end FT extensions

  return *this;
}




bool ListenPoint::operator== (const ListenPoint& other) const
{
  return ( (_port == other._port) &&
           (strcmp(_host, other._host) == 0) );
}




bool ListenPoint::operator< (const ListenPoint& other) const
{
  return ( (_port < other._port) ||
           ((_port == other._port) && (strcmp(_host, other._host) < 0)) );
}




/*
CORBA::ULong ListenPoint::hashCode()
{
  return atol(_host) + _port;
}
*/




void ListenPoint::read(TIDorb::core::cdr::CDRInputStream& input)
{
  CORBA::string_free(_str);
  if(_host){
    CORBA::string_free(_host);
  }
  input.read_string(_host);
  input.read_ushort(_port);
}




void ListenPoint::write(TIDorb::core::cdr::CDROutputStream& output) const
{
  if(_host)
    output.write_string(_host);
  else
    output.write_string("");
  output.write_ushort(_port);
}




const char* ListenPoint::toString() const
{
  if(!_str) {
    TIDorb::util::StringBuffer buffer;
    if(_host)
      buffer << "ListenPoint(" << _port << '@'<< _host << ')';
    else 
      buffer << "ListenPoint(" << _port << '@'<< "" << ')';
    ((ListenPoint*) this)->_str = CORBA::string_dup(buffer.str().data());
  }

  return _str;
}




bool ListenPoint::fromString(const char* s)
{
  string point = s;
  string::size_type colon_position = point.find(':');
  if ((colon_position == string::npos) || (point.rfind(':') != colon_position)) {
    return false;
  }

  CORBA::string_free(_str);
  CORBA::string_free(_host);
  _host = CORBA::string_dup(point.substr(0, colon_position).c_str());
  _port = atoi(point.substr(colon_position + 1).c_str());

  return true;
}




}
}
}
}
