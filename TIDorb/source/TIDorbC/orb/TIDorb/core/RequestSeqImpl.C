/////////////////////////////////////////////////////////////////////////
//
// File:        RequestSeqImpl.C
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

#include "CORBA.h" 

CORBA::ORB::RequestSeq::RequestSeq(const CORBA::ORB::RequestSeq& other): m_release(true),m_length(other.m_length),m_max_length(other.m_max_length)
{
	allocbuf();
	for(size_t i = 0; i < m_length; i++)
		m_buffer[i] = other.m_buffer[i];
}
CORBA::ORB::RequestSeq& CORBA::ORB::RequestSeq::operator=(const CORBA::ORB::RequestSeq& other)
{
	if(this == &other)
		return *this;
	if(m_release)
		freebuf();
	m_release = true;
	m_length = other.m_length;
	m_max_length = other.m_max_length;
	allocbuf();
	for(size_t i = 0; i < m_length; i++)
		m_buffer[i] = other.m_buffer[i];
	return *this;
}
CORBA::ORB::RequestSeq::~RequestSeq()
{
	freebuf();
}
void CORBA::ORB::RequestSeq::length(CORBA::ULong v)
{
	if(!m_buffer) {
		m_release = true;
		m_length = v;
		m_max_length = (v>m_max_length)? v : m_max_length;
		allocbuf();
		for(size_t i = 0; i < m_length; i++)
			m_buffer[i] = 0;
		return;
	}
	if(v > m_length) {
		if(v > m_max_length) {
			CORBA::Request_ptr* aux = new CORBA::Request_ptr [v];
				for(size_t i = 0; i < m_length; i++)
				{
					aux[i] = CORBA::Request::_duplicate(m_buffer[i]);
				}
			freebuf();
			m_buffer = aux; 
			m_release = true;
			m_max_length = v;
		}
		for(size_t i = m_length; i < v; i++)
			m_buffer[i] = 0;
	} else if (v < m_length){
		for(size_t i = v; i < m_length; i++) 
			CORBA::release(m_buffer[i]);
		}
	m_length = v;
	return ;//*this;
}
const CORBA::Request_ptr* CORBA::ORB::RequestSeq::get_buffer() const
{
	if(!m_buffer)
		(const_cast< CORBA::ORB::RequestSeq* > (this))->allocbuf();
	return (const CORBA::Request_ptr*) m_buffer;
}
CORBA::Request_ptr* CORBA::ORB::RequestSeq::get_buffer(CORBA::Boolean orphan)
{
	if(!m_buffer)
		allocbuf();
	if(orphan) {
		if(m_release)
		return 0;
	else
		m_release = false;
	}
	return m_buffer;
}
	CORBA::Request_ptr* CORBA::ORB::RequestSeq::allocbuf(CORBA::ULong size)
	{
		return new CORBA::Request_ptr [size];
	}
	void CORBA::ORB::RequestSeq::freebuf(CORBA::Request_ptr* buf)
	{
		delete[] buf;
	}
void CORBA::ORB::RequestSeq::replace(CORBA::ULong max,CORBA::ULong length,CORBA::Request_ptr* data,CORBA::Boolean release)
{
	if(m_buffer && m_release) {
		freebuf();
	}
	m_buffer = data;
	m_length = length;
	m_max_length = max;
	m_release = release;
}
void CORBA::ORB::RequestSeq::allocbuf()
{
	if(m_max_length > 0) {
		m_buffer =  new CORBA::Request_ptr [m_max_length];
		m_release = true;
	}
	else
		m_buffer = 0;
}
void CORBA::ORB::RequestSeq::freebuf()
{
	if(m_release && !m_buffer) {
		for(size_t i = 0; i < m_length; i++)
			CORBA::release(m_buffer[i]);
		delete[] m_buffer;
	}
}
