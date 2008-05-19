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

#include <math.h>

#ifndef PI
#define PI 3.14159265f
#endif

#include "IndexedPalette.h"

/**
 * Constructor
 */
IndexedPalette::IndexedPalette() {
	_palette = NULL;
	_lerpWidthBuff = NULL;
	_lerpHeightBuff = NULL;
	_width = 0;
	_height = 0;

	for(int i=0;i<3;i++)
		_defaultColor[i] = 0;
	_defaultColor[3] = 0;
}

/**
 * Constructor takes the number of colors in palette to
 * make room for.
 */
IndexedPalette::IndexedPalette(int width, int height) {
	_palette = NULL;
	_lerpWidthBuff = NULL;
	_lerpHeightBuff = NULL;
	_width = 0;
	_height = 0;

	for(int i=0;i<3;i++)
		_defaultColor[i] = 0;
	_defaultColor[3] = 0;

	empty(width, height);
}

IndexedPalette::~IndexedPalette() {
	_width = 0;
	_height = 0;
	if(_palette != NULL) {
		delete [] _palette;
		_palette = NULL;
	}
	if(_lerpWidthBuff != NULL) {
		delete [] _lerpWidthBuff;
		_lerpWidthBuff = NULL;
	}
	if(_lerpHeightBuff != NULL) {
		delete [] _lerpHeightBuff;
		_lerpHeightBuff = NULL;
	}
}

/**
 * Retrieves the width and height of this palette.
 */
int IndexedPalette::getWidth() {
	return _width;
}

int IndexedPalette::getHeight() {
	return _height;
}

/**
 * Empties the palette and makes space with the specified
 * dimensions
 */
void IndexedPalette::empty(int width, int height) {
	if(_palette != NULL) {
		delete [] _palette;
		_palette = NULL;
	}
	if(_lerpWidthBuff != NULL) {
		delete [] _lerpWidthBuff;
		_lerpWidthBuff = NULL;
	}
	if(_lerpHeightBuff != NULL) {
		delete [] _lerpHeightBuff;
		_lerpHeightBuff = NULL;
	}

	_width = width;
	_height = height;

	if(_width > 0 && _height > 0) {
		_palette = new GLubyte[_width*_height*4];
		_lerpWidthBuff = new GLubyte[_width*_height*4];
		_lerpHeightBuff = new GLubyte[_width*_height*4];

		if(_palette == NULL || _lerpWidthBuff == NULL || _lerpHeightBuff== NULL)
			return;

		for(int i=0; i< _width*_height; i++) {
			for(int j=0;j<4;j++) {
				_palette[i*4+j] = _defaultColor[j];
			}
		}
	}
}

/**
 * Sets the specified entry to the specified color.
 */
void IndexedPalette::setColor(int x, int y, GLubyte r, GLubyte g, GLubyte b, GLubyte a) {
	if(_palette != NULL && x >= 0 && x < _width && y >= 0 && y<_height) {
		_palette[(x+y*_width)*4] = r;
		_palette[(x+y*_width)*4+1] = g;
		_palette[(x+y*_width)*4+2] = b;
		_palette[(x+y*_width)*4+3] = a;
	}
}

/**
 * Sets the specified entry to the specified color.  Accepts a pointer to
 * an RGBA array.  Length can be either 3 or 4.  Both 1D and 2D versions.
 */
void IndexedPalette::setColor(int x, int y, GLubyte* buff, int length) {
	if(_palette != NULL && x >= 0 && x < _width && y >= 0 && y<_height) {
		_palette[(x+y*_width)*4+3] = 255;
		for(int i=0; i<length; i++) {
			_palette[(x+y*_width)*4+i] = buff[i];
		}
	}	
}

/**
 * Places the requested color into the array.  Must be large enough.
 * Data is in RGBA order.  Length can be either 3 or 4.  Both 1D
 * and 2D versions.
 */
void IndexedPalette::getColor(int x, int y, GLubyte* buff, int length) {
	if(_palette != NULL && x >= 0 && x < _width && y >= 0 && y<_height) {
		for(int i=0;i<length;i++) {
			buff[i] = _palette[(x+y*_width)*4+i];
		}
	}
}

void IndexedPalette::getColor(int x, int y, int* buff, int length) {
	if(_palette != NULL && x >= 0 && x < _width && y >= 0 && y<_height) {
		for(int i=0;i<length;i++) {
			buff[i] = (int)_palette[(x+y*_width)*4+i];
		}
	}
}

/**
 * Traverses the palette linearily interpolating between the
 * colors that are specified.  Will wrap if requested.
 */
void IndexedPalette::interpolate(bool bWrap, InterpType interp) {
	int last = -1;

	if(bWrap) {
		//if we are wrapping the palette start by
		//lerping from the last row
		int y=_height-1;
		while(_palette[y*_width*4+3] == 0 && y>0) {
			y--;
		}
		if(y>0) {
			for(int x=0;x<_width;x++) {
				if(_palette[(x+y*_width)*4 + 3] > 0) {
					interpColors(y, _height, x, false, interp, &_palette[x*4]);
				}
			}
		}

		//for each row that has a color at 0, search for
		//the last color, then lerp from last color to color 0.
		for(y = 0; y<_height; y++) {
			if(_palette[y*_width*4+3] > 0) {
				int x = _width-1;
				while(_palette[(x+y*_width)*4 + 3] == 0 && x > 0) {
					x--;
				}
				if(x>0) {
					interpColors(x, _width, y, true, interp, &_palette[y*_width*4]);
				}
			}
		}
	}

	//lerp width but only
	//if the first color of row
	//has been specified.
	for(int y = 0; y<_height; y++) {
		if(_palette[y*_width*4+3] > 0) {
			last = 0;
			for(int x=1; x<_width; x++) {
				if(_palette[(x+y*_width)*4 + 3] > 0) {
					interpColors(last, x, y, true, interp);
					last = x;
				}
			}
		}
	}

	//lerp height
	for(int x=0; x<_width; x++) {
		last = -1;
		for(int y = 0; y<_height; y++) {
			if(_palette[(x+y*_width)*4 + 3] > 0) {
				if(last >= 0) {
					interpColors(last, y, x, false, interp);
				}
				last = y;
			}
		}
	}
	
}

/**
 * Interpolates between the two endpoints on the specified row or column.
 * The results are put into the appropriate buffer.  If bInterpRow is
 * true, then rowcolumn = column, and the results go into
 * _interpWidthBuff.
 */
void IndexedPalette::interpColors(int start, int stop, int rowcolumn, bool bInterpRow, InterpType interpMethod, GLubyte* wrapColor) {
	// look at the value of interpMethod and call the correct helper method
	switch(interpMethod) {
		case sine:
			sineInterpColors(start, stop, rowcolumn, bInterpRow, wrapColor);
			break;
		case linear:
			lerpColors(start, stop, rowcolumn, bInterpRow, wrapColor);
			break;
		case none:
		default:
			break;
	}
}

// use sinusoidal interpolation
void IndexedPalette::sineInterpColors(int start, int stop, int rowcolumn, bool bInterpRow, GLubyte* wrapColor) {
	//*** NOTE: bInterpRow means iterate over X!!!

	if(stop - start < 2)
		return;

	float stepVals[4];

	//calculates the stepVals used in lerping
	for(int j = 0; j<4;j++) {
		if( bInterpRow ) {
			if(wrapColor != NULL) {
				stepVals[j] = float(wrapColor[j] - _palette[(start + rowcolumn*_width)*4 + j]);
			} else {
				stepVals[j] = float(_palette[(stop + rowcolumn*_width)*4 + j] - _palette[(start + rowcolumn*_width)*4 + j]);
			}
		} else {
			if(wrapColor != NULL) {
				stepVals[j] = float(wrapColor[j] - _palette[(rowcolumn + start *_width)*4 + j]);
			} else {
				stepVals[j] = float(_palette[(rowcolumn + stop*_width)*4 + j] - _palette[(rowcolumn + start *_width)*4 + j]);
			}
		}
	}

	for(int i=start; i<stop; i++) {
		for(int j = 0; j<4; j++) {
			if( bInterpRow ) {				
				_palette[(i + rowcolumn*_width)*4+j] = GLubyte( int(sin(float(i - start)*PI/(2.0*float(stop-start)))*stepVals[j]) + _palette[(start+rowcolumn*_width)*4+j]);
			} else {
				_palette[(rowcolumn + i*_width)*4+j] = GLubyte( int(sin(float(i - start)*PI/(2.0*float(stop-start)))*stepVals[j]) + _palette[(rowcolumn+start*_width)*4+j]);
			}
		}
	}
}

// use linear interpolation
void IndexedPalette::lerpColors(int start, int stop, int rowcolumn, bool bInterpRow, GLubyte* wrapColor) {
	//*** NOTE: bInterpRow means iterate over X!!!

	if(stop - start < 2)
		return;

	float stepVals[4];

	//calculates the stepVals used in lerping
	for(int j = 0; j<4;j++) {
		if( bInterpRow ) {
			if(wrapColor != NULL) {
				stepVals[j] = float(wrapColor[j] - _palette[(start + rowcolumn*_width)*4 + j]) / float(stop-start);
			} else {
				stepVals[j] = float(_palette[(stop + rowcolumn*_width)*4 + j] - _palette[(start + rowcolumn*_width)*4 + j]) / float(stop-start);
			}
		} else {
			if(wrapColor != NULL) {
				stepVals[j] = float(wrapColor[j] - _palette[(rowcolumn + start *_width)*4 + j]) / float(stop-start);
			} else {
				stepVals[j] = float(_palette[(rowcolumn + stop*_width)*4 + j] - _palette[(rowcolumn + start *_width)*4 + j]) / float(stop-start);
			}
		}
	}

	for(int i=start; i<stop; i++) {
		for(int j = 0; j<4; j++) {
			if( bInterpRow ) {				
				_palette[(i + rowcolumn*_width)*4+j] = GLubyte( int(float(i - start)*stepVals[j]) + _palette[(start+rowcolumn*_width)*4+j]);
			} else {
				_palette[(rowcolumn + i*_width)*4+j] = GLubyte( int(float(i - start)*stepVals[j]) + _palette[(rowcolumn+start*_width)*4+j]);
			}
		}
	}
}

/**
 * Sets the specified entry to the specified color.
 */
void IndexedPalette::setDefaultColor(GLubyte r, GLubyte g, GLubyte b) {
	_defaultColor[0] = r;
	_defaultColor[1] = g;
	_defaultColor[2] = b;
	_defaultColor[3] = 0;
}

/**
 * Overloaded =
 */
IndexedPalette& IndexedPalette::operator=(IndexedPalette& other) {

	_width = other._width;
	_height = other._height;

	for(int i=0; i<4; i++) {
		_defaultColor[i] = other._defaultColor[i];
	}

	//make space
	empty(_width, _height);

	//copy buffers
	for(int i=0;i<_height*_width*4;i++) {
		_palette[i] = other._palette[i];
		_lerpWidthBuff[i] = other._lerpWidthBuff[i];
		_lerpHeightBuff[i] = other._lerpHeightBuff[i];
	}

	return *this;
}

/**
 * Converts InterpType to/from QString.
 */
string IndexedPalette::InterpTypeToString(InterpType interpType) {
	string retVal;
	switch(interpType) {
		case sine:
			retVal = string("Sinusoidal");
			break;
		case linear:
			retVal = string("Linear");
			break;
		case none:
		default:
			retVal = string("None");
			break;
	}
	return retVal;
}

InterpType IndexedPalette::stringToInterpType(string str) {
	InterpType retVal;

	if(str == string("Linear") ) {
		retVal = linear;
	} else if(str == string("Sinusoidal") ) {
		retVal = sine;
	} else if(str == string("true") ) {
		retVal = linear;
	} else {
		retVal = none;
	}

	return retVal;
}

