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

#ifdef _M_IX86
#include <windows.h>
#else
//#include <stream.h>
#include <stdlib.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

//Constants
#define VIEWPORT_WIDTH 6.5
#define VIEWPORT_HEIGHT 6.5
#define PI 3.14159265f

//data structures
struct color_struct
{
	float r,g,b;
};

struct screen_struct
{
	//this is the "display" buffer.
	GLubyte* _pixels;
	//these are the dimensions of the "display"
	int _sizeX, _sizeY;
	//These are the offset values to pass to glTexSubImage2D.
	int _tXOff, _tYOff;
	//these are the dimensions of the texture.  Must be powers of 2.
	int _tSizeX, _tSizeY;
	//these are the texel coordinates to attach to the corners. 0.0 .. 1.0
	float _left, _right, _top, _bottom;
	//the texture index returned by OpenGL
	GLuint _textureIndex;
};

/**
 * Holds coordinates for each iteration of the fractal
 * generation.  Since we are dealing with rectangles,
 * it is only necessary to hold on to the top left
 * and bottom right corners.
 */
struct coord_node
{
	long x1,y1, x2, y2;
	coord_node* next;
};

//enumeration of sides
enum side_type {
	NoSide,
	Left,
	Right,
	Top,
	Bottom
};

#endif
