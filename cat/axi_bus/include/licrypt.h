/* licrypt.h 1.11 04/05/11
 *  Copyright 1992-2004 by Concept Engineering GmbH.
 *  All rights reserved.
 *  ===========================================================================
 *  This source code belongs to Concept Engineering.  It is considered 
 *  trade secret, and is not to be divulged or used or copied by parties
 *  who have not received written authorization from the owner, Concept
 *  Engineering.
 *  ===========================================================================
 *  Title:
 *	License cookie generator - This code is EXTREMLY CONFIDENTIAL!!!
 *  ===========================================================================
*/

#ifndef licrypt_h
#define licrypt_h

static void licrypt(char result[16],
	unsigned long t, const char* name, const char* key)
{
	unsigned long val;
	const char* p;
	int  i;

	const char* alphabet =
	    "qwertyuioplkjhgfdsazxcvbnm123456789QWERTYUIOPASDFGHJKLZXCVBNM%@?";

	static const unsigned short rtab[128] = {
	    82, 107, 92, 79, 113, 62, 48, 101, 44, 34, 19, 114, 98, 74, 
	    77, 105, 33, 122, 5, 36, 46, 119, 6, 60, 121, 3, 13, 86, 37, 
	    63, 9, 49, 59, 102, 12, 25, 57, 45, 72, 123, 24, 18, 15, 32, 
	    100, 118, 14, 10, 110, 127, 93, 39, 64, 68, 115, 95, 41, 35,
	    17, 26, 55, 112, 85, 97, 71, 52, 43, 106, 53, 116, 78, 30, 31, 
	    54, 21, 69, 99, 40, 124, 8, 126, 58, 11, 89, 2, 4, 117, 104, 
	    90, 84, 96, 7, 75, 65, 51, 56, 111, 20, 103, 73, 38, 61, 81, 
	    91, 80, 125, 0, 70, 16, 42, 29, 67, 27, 50, 23, 47, 109, 108, 
	    22, 94, 120, 66, 28, 88, 87, 76, 83, 1
	};

	val = 0;
	for(p=name; *p; p++) val = (val << 6) | (*p & 0x3f);
	t ^= val;

	val = 0;
	for(p=key; *p; p++) val = (val << 6) | (*p & 0x3f);
	t ^= val;

	for(i=0; i<32; i += 4)
	{
	    t &= 0xffffffffUL;	/* limit to 32 bit unsigned */
	    t ^= (unsigned long) (rtab[ (t>>i) & 0x7f ] << i);
	}

	t &= 0xffffffffUL;	/* limit to 32 bit unsigned */

	for(i=0; i<8; i++)
	{
	    unsigned short r = rtab[ t & 0x7f ];
	    result[i] = alphabet[ r & 0x3f ];	/* bits 5...0 for output */
	    t ^= (r & 0x70);			/* bits 7...4 for reuse */
	    t >>= 4;
	}

	/* Append first two characters from "key" to result[8..9] */
	for(p=key, i=8; *p && i < 10; p++, i++) result[i] = *p;
	result[10] = 0;
}
#endif
