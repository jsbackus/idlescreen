/**
 * Copyright (c) 2009 Jeff Backus.
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

/**
 * This file contains the definitions for externs that setup
 * project-specific quantities.  It is a hack that allows
 * certain objects, like ConfigManager to be used without
 * overloading, since that would involve overloading several
 * other objects.
 *
 * Revision History:
 * 090307	jsbackus	Initial revision.
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

