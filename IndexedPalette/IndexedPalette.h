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
 * This class represents an indexed 2D palette.  It has the ability
 * to linearily interpolate between specified colors if all colors
 * are not specified.  Rotates both X and Y directions
 */

#ifndef __INDEXEDPALETTE_H__
#define __INDEXEDPALETTE_H__

#ifdef _M_IX86
#include <windows.h>
#else
#include <stream.h>
#endif

#include <gl/gl.h>
#include <gl/glu.h>

class IndexedPalette {

public:
	/**
	 * Constructor
	 */
	IndexedPalette();

	/**
	 * Constructor takes the number of colors in palette to
	 * make room for.
	 */
	IndexedPalette(int width, int height = 1);

	~IndexedPalette();

	/**
	 * Retrieves the width and height of this palette.
	 */
	int getWidth();
	int getHeight();

	/**
	 * Empties the palette and makes space with the specified
	 * dimensions
	 */
	void empty(int width = 0, int height = 0);

	/**
	 * Sets the specified entry to the specified color.
	 */
	void setColor(int x, int y, GLubyte r, GLubyte g, GLubyte b, GLubyte a = 255);

	/**
	 * Sets the specified entry to the specified color.  Accepts a pointer to
	 * an RGBA array.  Length can be either 3 or 4.
	 */
	void setColor(int x, int y, GLubyte* buff, int length = 3);

	/**
	 * Places the requested color into the array.  Must be large enough.
	 * Data is in RGBA order.  Length can be either 3 or 4.
	 */
	void getColor(int x, int y, GLubyte* buff, int length = 3);
	void getColor(int x, int y, int* buff, int length = 3);

	/**
	 * Traverses the palette linearily interpolating between the
	 * colors that are specified.  Will wrap if requested.
	 */
	void interpolate(bool bWrap);

	/**
	 * Sets the specified entry to the specified color.
	 */
	void setDefaultColor(GLubyte r, GLubyte g, GLubyte b);

	/**
	 * Overloaded =
	 */
	IndexedPalette& operator=(IndexedPalette& other);

private:
	/**
	 * Lerps between the two endpoints on the specified row or column.
	 * The results are put into the appropriate buffer.  If bLerpRow is
	 * true, then rowcolumn = column, and the results go into
	 * _lerpWidthBuff.
	 */
	void lerpColors(int start, int stop, int rowcolumn, bool bLerpRow, GLubyte* wrapColor = NULL);

	int _width, _height;
	GLubyte* _palette;
	GLubyte* _lerpWidthBuff;
	GLubyte* _lerpHeightBuff;
	GLubyte _defaultColor[4];

};

#endif