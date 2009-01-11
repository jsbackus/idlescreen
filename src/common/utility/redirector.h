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
 * This code is used to execute a program in a different directory.
 * This is necessary because Windows requires screen savers and their
 * libraries to be installed in WINDOWS\SYSTEM32.  Unfortunately,
 * it isn't sufficient to modify the search path, because the OS doesn't
 * use it.
 *
 * Redirector figures out which program by looking for a key with the
 * same name as its call name in the registry.  In an effort to prevent
 * an infinite loop, redirector calculates a CRC value for itself and
 * the program specified in the registry.  If the values match, it will
 * not run.  There is the potential for false positives, but it is
 * low.
 *
 * Revision history:
 * 080308	jsbackus	Initial version.
 */

#ifndef __REDIRECTOR_H__
#define __REDIRECTOR_H__

#include <windows.h>
/**
 * Main entry point.  It takes the arguments that are
 * passed to int winmain() and returns the success/error code.
 * Also takes the keyname to look up as an argument.
 */
extern redirector(LPSTR lpCmdLine, const char* keyName);

#endif