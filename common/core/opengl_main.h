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

/**
 * Header file for OpenGL code that doesn't belong
 * in any particular class.
 */

#ifndef __OPENGL_MAIN_H__
#define __OPENGL_MAIN_H__

#ifdef _M_IX86
#include <windows.h>
#else
//#include <stream.h>
#include <stdlib.h>
#endif

/**
 * Initializes any global variables that aren't architecture specific.
 * Note: This is called before OpenGL is initialized.  Takes as an
 * argument the name of the config file to load.  If NULL, loads
 * from the default location.
 */
extern void initFunc(int argc=0, char** argv=NULL, char* filename = NULL);

/**
 * Sets up the OpenGL viewport.  Takes as an argument the height and width
 * of the viewport.
 */
extern void setupViewport(int width, int height);

/**
 * Perform any animations
 */
extern void animate(void);

/**
 * Redraws the screen.
 */
extern void render(void);

/**
 * Contains any exit code that needs executed.  i.e. cleanup, etc.
 */
extern void exitFunc(void);

/**
 * Returns the number of milliseconds to set the timer to.
 */
extern long getTimerMillis();

/**
 * Fills the provided buffer with up to maxLen characters of
 * the null-terminated human-readable program name.  Returns
 * the number of characters actually placed in buff.
 */
extern int getProgName(char* buff, int maxLen);

#endif

