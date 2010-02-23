/////////////////////////////////////////////////////////////////////////
//
// File:        Trace.C
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

#include "TIDorb/util.h"
#include <stdlib.h>

#include <iomanip>
#include <fstream>
#include <stdexcept>
#include <typeinfo>

#include <sys/time.h>
#include <unistd.h>
#include <time.h>

#ifdef TIDORB_HAVE_NAMESPACE_STD
using namespace std;
#endif


static const char* level_name [] = 
  {"      NONE", "     ERROR", "      USER", "     DEBUG", "DEEP_DEBUG", "      DUMP"};

TIDorb::util::Trace::Trace(ostream* log, 
		           const char*  applicationName, 
		           TIDorb::util::TraceLevel level) 
  throw(TIDThr::SystemException)
  :  _log(log),
    _applicationName(applicationName),
    _level(level),
    _include_date(true),
    _do_flush_at(1),
    _message_count(0)
{
}


TIDorb::util::Trace* 
  TIDorb::util::Trace::create_trace(const char* applicationName, 
                                    TIDorb::util::TraceLevel level)
  throw ()
{
  ostream* log = cerr.tie();
  return new Trace(log, applicationName, level);
}

/**
 * Creates the trace using the given output stream.
 * @param log the output stream where the messages will be written.
 * @param applicationName may be null
 * @param level trace level addopted
 * @return a new Trace instance
 */

TIDorb::util::Trace*   
  TIDorb::util::Trace::create_trace(ostream* log, 
                                    const char* applicationName, 
		                    TIDorb::util::TraceLevel level) 
       throw ()
{
	return new Trace(log, applicationName, level);
}

/**
 * Creates the trace using the given output stream.
 * @param file_name the file to be open
 * @param applicationName may be null
 * @param level trace level addopted
 * @return a new Trace instance
 */

TIDorb::util::Trace*
  TIDorb::util::Trace::create_trace(const char* file_name,
		                    const char* applicationName,
                                    TIDorb::util::TraceLevel level)
	  throw ()
{
  ofstream* log = new ofstream(file_name);
  if(!log->is_open())
    throw runtime_error("Cannot open file");
  return new Trace(log, applicationName, level);
}

TIDorb::util::Trace::~Trace()
  throw(TIDThr::SystemException)
{
  delete _log;
}


void TIDorb::util::Trace::printHeader(TIDorb::util::TraceLevel level)
{
  (*_log) << '[' << _message_count++ << "] ";
  (*_log) << level_name[level] << ' ';

  if(_include_date)
  {
    time_t         abs_time;
    struct timeval abs_tval;
    struct tm      the_time;

    gettimeofday(&abs_tval, NULL);
    abs_time = abs_tval.tv_sec;
    localtime_r(&abs_time, &the_time);
    strftime(date_buffer, DATE_BUFFER_SIZE, "%d/%m/%y %H:%M:%S", &the_time);
    (*_log) << date_buffer  << ' ';
  }

  (*_log) << _applicationName << ' ';
}

void TIDorb::util::Trace::print(TIDorb::util::TraceLevel level, 
		                const char* message)
throw()
{
  if((_log) && (level <= _level))
  {
    TIDThr::Synchronized synchro(*this);

    printHeader(level); 

    (*_log) << message << endl;

    if((_message_count % _do_flush_at) == 0)
      _log->flush();
  }
}
   
void TIDorb::util::Trace::print(TIDorb::util::TraceLevel level, 
                                const char** message, 
	                        size_t elements) 
throw()
{
  if((_log) && (level <= _level))
  {
    TIDThr::Synchronized synchro(*this);
    printHeader(level);

    for(size_t i = 0; i < elements; i++)
      (*_log) << message[i];

    (*_log) << endl;

    if((_message_count % _do_flush_at) == 0)
      _log->flush();
  }
}

void TIDorb::util::Trace::print(TIDorb::util::TraceLevel level, 
                                const char* message,
	                        const exception& ex) 
throw()
{
  if((_log) && (level <= _level))
  {
    TIDThr::Synchronized synchro(*this);
    printHeader(level);

    (*_log) << message << endl;
    printException(ex);
    (*_log) << endl;

    if((_message_count % _do_flush_at) == 0)
      _log->flush();
  }
}


void TIDorb::util::Trace::print(TIDorb::util::TraceLevel level, 
                                const char** message, 
                                size_t elements,
                                const exception& ex) 
throw()
{
  if((_log) && (level <= _level))
  {
    TIDThr::Synchronized synchro(*this);
    printHeader(level);

    for(size_t i = 0; i < elements; i++)
      (*_log) << message[i];

    (*_log) << endl;
    printException(ex);
    (*_log) << endl;

    if((_message_count % _do_flush_at) == 0)
      _log->flush();
  }
}


void TIDorb::util::Trace::dump(TIDorb::util::TraceLevel level, 
                               const unsigned char* message, 
                               size_t length) 
throw()
{
  if((_log) && (level <= _level))
  {
    TIDThr::Synchronized synchro(*this);

    char line[17];
    int position = 0;

    for(size_t i = 0; i < length; i++) {

      if ( ((i %  8) == 0) && (i > 0) )
        (*_log) << " ";

      if ( ((i % 16) == 0) && (i > 0) ){
        line[16] = '\0';
        (*_log) << " " << line << endl;
        position = 0;
      }

      (*_log) << setfill('0') << setw(2) << hex << (unsigned short)message[i] << " "; 
      
      if ( (message[i] >= 32) && (message[i] <= 126) ) // Is in ascii char set?
        line[position] = message[i];
      else
        line[position] = '.';

      position++;
    }

    line[position] = '\0';

    (*_log) << setfill(' ') << setw( 16 + ((16-position)*2) + (position > 8 ? 2 : 3) ) << line << dec << endl;

    if((_message_count % _do_flush_at) == 0)
      _log->flush();
  }
}

void TIDorb::util::Trace::close() throw()
{
   TIDThr::Synchronized synchro(*this);

   if(_log != NULL) {
     (*_log) << "LOG CLOSED";
     delete _log;
     _log = NULL;
   }
}

void  TIDorb::util::Trace::flush() throw()
{
  if(_log != NULL)
    _log->flush();
}

void TIDorb::util::Trace::printException(const exception& ex)
{
	(*_log) << " Exception " << typeid(ex).name() << ": " 
		<< ex.what();

}
