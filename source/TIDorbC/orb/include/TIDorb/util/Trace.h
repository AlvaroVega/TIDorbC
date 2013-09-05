/*/////////////////////////////////////////////////////////////////////////
//
// File:        Trace.h
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
/////////////////////////////////////////////////////////////////////////*/

#include "TIDorb/util.h"


#ifndef _TIDORB_UTIL_Trace_H_
#define _TIDORB_UTIL_Trace_H_

#include <sys/types.h>
#include <exception>
#include <string>

#include "TIDThr.h"


#ifdef TIDORB_HAVE_NAMESPACE_STD
using namespace std;
#endif

#ifdef __SUNPRO_LIBRARY_IOSTREAM
#include <iostream.h>
using std::exception;
#else
#include <ostream>
#endif

namespace TIDorb {
namespace util {

enum TraceLevel {TR_NONE = 0, TR_ERROR, TR_USER, TR_DEBUG, TR_DEEP_DEBUG, TR_DUMP};

class Trace : public virtual TIDThr::RecursiveMutex
{

  private:
  /**
  * Trace Log Stream.
  */
  ostream* _log; 

  /**
   * Aplication Name printed in log messages.
   */
  string _applicationName;

  /**
   * Current trace level.
   */
   TraceLevel _level;

   /**
    * Include the date in log messages.
   */
    bool _include_date;

   /**
    * Invokes the stream flush each x times. 
    * It improves the file output performance.
    */
    size_t _do_flush_at;
     

   /**
    * Printed messages counter
    */
    size_t _message_count;

   /**
     * Buffer size to print the date
     */

    static const size_t DATE_BUFFER_SIZE = 120;

    /**
     * Internal buffer to print the date.
     */

    char date_buffer[DATE_BUFFER_SIZE];

  protected:

    Trace(ostream* log, const char*  applicationName, TraceLevel level)
      throw(TIDThr::SystemException);

  public:
    /**
     *Creates the trace using the standard error output.
     * @param applicationName may be null
     * @param level trace level addopted
     * @return a new Trace instance
     */
     static Trace* create_trace(const char* applicationName, 
                               TraceLevel level)
        throw (); 

    /**
     * Creates the trace using the given output stream.
     * @param log the output stream where the messages will be written.
     * @param applicationName may be null
     * @param level trace level addopted
     * @return a new Trace instance
     */

     static Trace* create_trace(ostream* log, 
                                const char* applicationName, 
		                TraceLevel level) 
       throw ();

   /**
    * Creates the trace using the given output stream.
    * @param file_name the file to be open
    * @param applicationName may be null
    * @param level trace level addopted
    * @return a new Trace instance
    */

    static Trace* create_trace(const char* file_name, 
                               const char* applicationName,
			       TraceLevel level)
      throw ();


   ~Trace() throw(TIDThr::SystemException);
  
   void print(TraceLevel level, 
              const char* message) throw();
   
   void print(TraceLevel level, 
              const char** message, 
	      size_t elements) throw();

   void print(TraceLevel level, 
              const char* message,
	      const exception& ex) throw();

   void print(TraceLevel level, 
              const char** message, 
              size_t elements,
              const exception& ex) throw();

   void dump(TraceLevel level, 
             const unsigned char* message, 
             size_t length) throw();

  /**
   * Sets the trace level. If level is 0 traces will be deactivated.
   * @param level new trace level
   */
    void setLevel(TraceLevel level)
    {
       _level = level;
    }

 /**
   * Activates/deactivates the date inclussion in trace messages.
   * @param value if <code>true</code> include it, otherwise do not.
   */
    void setIncludeDate(bool value)
    {
      _include_date = value;
    }

   /**
    * Sets each times the stream flush operation is invoked. 
    * It improves the file output performance.
    * @param value buffer flush round.
    */
    void setDoFlushAt(size_t value)
    {
       if(value > 0)
         _do_flush_at = value;
    }

   /**
    * @return the buffer flush round value.
    */
    size_t getDoFlushAt()
    {
      return _do_flush_at;
    }

   /**
    * @return the log stream used by the trace instance.
    */
    ostream* getLog()
    {
       return _log;
    }

  /**
    * Forces the buffered stream flushing.
    */

   void flush() throw(); 

 /**
   * Closes the trace log stream.
   */
   void close() throw();
  
  protected:
    void printHeader(TraceLevel level);
    void printException(const exception& ex);

  }; //Trace

} // namespace util 
} // namespace TIDorb 

#endif
