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
 */

/**
 * The methods in this file are used to generate a CRC-32 signature
 * of a file.  Uses standard libraries only.
 */

#ifndef __FILE_CRC_H__
#define __FILE_CRC_H__

/**
 * This method will read in the specified file in binary mode
 * and return a CRC signature.  Returns whether successful
 * or not.
 */
extern bool genCRC(const char* filename, unsigned long* crc);

#endif