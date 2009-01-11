/**
 * Copyright (c) 2008 Jeff Backus.
 *
 * Licensed under the GNU General Public License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * The algorithms used in this file are based on the algorithms written
 * by Ross Williams(ross@guest.adelaide.edu.au) and released to the
 * public domain as of 3 June 1993.  For more information, please refer to:
 *
 * http://www.repairfaq.org/filipg/LINK/F_crc_v3.html
 *
 */

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

// divisor is one used for CRC-32 IEEE 802.3
const unsigned long CRC_POLY = 0x04C11DB7;

/**
 * Function to calculate the table.  Only does so
 * the first time for efficiency sake.  Please see:
 * http://www.repairfaq.org/filipg/LINK/F_crc_v3.html
 * for more information.
 */
const unsigned long* getCRCTable() {
	static unsigned long table[256];
	static bool bSetup = false;

	// generate the table if this is the first time.
	if(!bSetup) {
		for(int idx=0; idx<256; idx++) {
			unsigned long topbit = 1<<31;
			unsigned long inbyte = idx;
			unsigned long r = inbyte << 24;

			for(int i=0; i<8; i++) {
				if(r & topbit)
					r = (r << 1) ^ CRC_POLY;
				else
					r <<= 1;
			}
			table[idx] = r & 0xFFFFFFFF;
		}
		bSetup = true;
	}

	return &table[0];
}

/**
 * This method will read in the specified file in binary mode
 * and return a CRC signature.  Returns whether successful
 * or not.
 */
bool genCRC(const char* filename, unsigned long* crc) {
	const unsigned long* crc_table = getCRCTable();

	// first, try to open the file
	ifstream in(filename, ios::in | ios::binary);
	if(!in)
		return false;

	*crc = 0;
	char inByte;
	// try to grab the first four bytes of data
	int i=0;
	while(i<4 && in) {
		in.get(inByte);
		*crc = (*crc << 8) | inByte;
		i++;
	}

	// begin CRC calculation
	while(in) {
		in.get(inByte);
		*crc = (*crc << 8) ^ crc_table[(*crc >> 24) ^ inByte];
	}

	in.close();

	return true;
}
