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
 * This class is the base class for all lens sprites.
 * It maintains a field of lookup values for nonlinear
 * transformations of the background.
 * It maintains an outline of the object in order
 * to properly tint and compliment colors.  It is
 * up to the subclass to generate the lookup
 * map and the outline.
 */

#ifndef __LENSOBJECT_H__
#define __LENSOBJECT_H__

#ifdef _M_IX86
#include <windows.h>
#else
#include <stream.h>
#endif

//#include <gl/gl.h>
//#include <gl/glu.h>
//#include <glut.h>
//#include <gl/glaux.h>

#include <math.h>

#include "../globaldefs.h"

class LensObject {
public:

	//constructor and destructor
	LensObject(screen_struct* screenObj, int sizeX, int sizeY, int outlineWidth=1);
	LensObject();
	~LensObject();

	//Renders the lens
	virtual void drawLens();

	//called to animate the lens
	virtual void animate();

	//sets the tint.  If NULL then tinting
	//is disabled.  tint must be 4 GLubytes
	//long!
	void setTint(GLubyte* tint);

	//sets the lens to complement colors
	void setComplement(bool bComplement);

	//returns whether this lens is still visible
	bool isVisible();

	//sets the visibility of the lens if it is on screen.
	void setVisible(bool bVisible);

	//Gives the lens a new position
	void setPosition(int posX, int posY);

	//gives the lens a new direction
	void setDirection(float dirX, float dirY);

	//sets the speed
	void setSpeed(float speed);

	//sets the outline tint.  If NULL, then
	//outline is disabled.
	//color must be 4 GLubytes long!
	void setOutlineTint(GLubyte* color);

	//whether the outline should use complementary
	//colors.
	void setOutlineComplement(bool bComplement);

protected:
	/**
	 * Routine that actually draws.  In order to make it more generic for
	 * any subclasses, it accepts position, size, field, outline, and bitmap data
	 * as arguments instead of using the data located inside the class.
	 */
	void renderLens(int posX, int posY, int sizeX, int sizeY, int* field, bool* bitmap, bool* outline);

	/**
	 * Moves the lens.
	 */
	void moveLens();

	//sets up all datastructures.  Must be called before subclasses set field and outline values.
	void setupLens(screen_struct* screenObj, int sizeX, int sizeY, int outlineWidth);

	//cleans up
	void destroyLens();

	/**
	 * Calculates the outline from the bitmap.
	 * Looks for any false values next to a true
	 * value.  To make it more generic, it accepts
	 * size data and pointers to ouline and
	 * bitmap buffers.
	 */
	void calcOutline(int sizeX, int sizeY, bool* bitmap, bool* outline);
	//convenience function that uses what data contained within base class
	void calcOutline();

	screen_struct* _screenObj;
	int _sizeX;
	int _sizeY;
	int* _field;
	GLubyte* _screenBuf;  //holds screen contents while we draw
	bool* _outline;
	bool* _bitmap;

	bool _bComplementColors;
	bool _bTint;
	float _tint[4];

	float _outlineTint[4];
	bool _bOutlineTint;
	bool _bOutlineComplementColors;
	int _outlineWidth;

	int _posX, _posY;
	float _dirX, _dirY;
	float _speed;

	bool _bVisible;
};

#endif