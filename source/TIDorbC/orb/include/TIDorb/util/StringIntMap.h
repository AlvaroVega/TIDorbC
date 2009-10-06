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
// Copyright 2002 Telefonica, I+D. Printed in Spain (Europe). All Rights
// Reserved.
// The copyright to the software program(s) is property of Telefonica I+D.
// The program(s) may be used and or copied only with the express written
// consent of Telefonica I+D or in acordance with the terms and conditions
// stipulated in the agreement/contract under which the program(s) have
// been supplied.
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
