/*/////////////////////////////////////////////////////////////////////////
//
// File:        StringIntMap.h
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

/* 
 * Fast hash table to map strings to integers. Implements an
 * open-addressing scheme, with linear probing
 *
 * On my machine this class is twice as fast as the STL map class
 *                            BUT
 *              Performance comes with a price: 
 *
 * For optimization reasons *NO CHECKS ARE PERFORMED* on insertion 
 * and key search. 
 *
 * Pedro Diaz (pdiaz@laurel.datsi.fi.upm.es)
 */
#ifndef TIDORB_STRINGINTMAP_H
#define TIDORB_STRINGINTMAP_H

#define HASH_SIZE 256

#include <string.h>
class StringIntMap {
	
	public:
		// Quite popular hash function
		static int hash( const char *s ) {
			unsigned char h = (unsigned char)5381;
			int i=0;
			while (*s) {
				h = ((h << 5) + h) + *s;
				s++;
			}
			return (h );
		};

		// Constructor
		StringIntMap( void ) {
			for (int i=0; i < HASH_SIZE; i++) {
				map[i].key = NULL;
			}
		};

		int& operator[]( const char *s)  {
			int pos = StringIntMap::hash( s );
			int c = 0;
			while (map[pos].key != NULL && c++ != HASH_SIZE)  {
				if (strcmp(map[pos].key, s ) == 0) {
					return map[pos].val;
				}
				pos++;
				if (pos == HASH_SIZE)
					pos = 0;
			}
			
			// If the table is full an arbitrary 
			// entry is OVERWRITEN. 
			map[pos].key = strdup( s );
			map[pos].val = 0;
			return map[pos].val;
		};



	private:
		typedef struct {
			char *key;
			int val;
		} node_t;
		node_t map[HASH_SIZE];
	
		

};

#endif
