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
 * Additionally, this code makes heavy use of the Qt libraries from
 * Trolltech.  These libraries are released under the GNU
 * General Public License, Version 2.0, as well.  Please contact
 * Trolltech for more information.
 *
 *     http://trolltech.com/
 * 
 */

#ifndef __GLOBALDEFS_H__
#define __GLOBALDEFS_H__

/**
 * This file contains globally defined parameters and functions
 * prototypes.  The functions are defined in globaldefs.cpp.
 */

#include <QString>

/**
 * This function returns the string used to create the path for
 * the settings file.  It is also used to define the XML
 * document root.
 */
extern QString getAppConfigName();

/**
 * Returns the full name of the screen saver to be used in
 * places such as title bars of dialog boxes and the About
 * dialog box.
 */
extern QString getAppFullName();

/**
 * Returns the version string for the screen saver.
 */
extern QString getAppVersion();

#endif
