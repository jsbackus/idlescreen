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
 * This class represents all background objects that use
 * an indexed palette.  This palette can then be animated
 * by rotating the color indices.
 */

#ifndef __INDEXEDPALETTEBACKGROUND_H__
#define __INDEXEDPALETTEBACKGROUND_H__

#include "globaldefs.h"
#include "Background.h"
#include "../IndexedPalette.h"

class IndexedPaletteBackground : public Background {

public:
	/**
	 * Sets the color palette
	 */
	void setPalette(IndexedPalette* pal);

	/**
	 * Sets the palette animation speeds.
	 */
	void speed(float xSpeed, float ySpeed = 1.0);

	/**
	 * Default constructor.
	 */
	IndexedPaletteBackground();

	/**
	 * Constructor takes the dimensions and a pointer to the palette.
	 */
	IndexedPaletteBackground(int sizeX, int sizeY, IndexedPalette* pal);

	~IndexedPaletteBackground();

	// *** Begin Extended Background Methods ***

	/**
	 * Translates the buffer of indices to 32bit colors and places in screenObj
	 */
	virtual void drawBackground(screen_struct* screenObj);

	/**
	 * Rotates the palette.
	 */
	virtual void clocktick();

	// *** End Extended Background Methods ***

protected:
	/**
	 * Called from the constructor and does all of the work,
	 * so that derived classes can call from their constructors.
	 */
	void initIndexedPaletteBackground(int sizeX, int sizeY);

	/**
	 * Called from the destructor and does all of the work,
	 * so that derived classes can call from their destructors.
	 */
	void destroyIndexedPaletteBackground();

	/**
	 * Given a color index, returns the shifted color index.
	 * If a number less than 0 is passed in, a number that
	 * corresponds to black is returned.
	 */
	int getShiftedIndex(int idx);

	/**
	 * Rotates the color index.
	 */
	void rotateColorIndex();

	int* _palette;
	int _paletteWidth, _paletteHeight;
	float _xIndexOffset;
	float _yIndexOffset;
	int _yAdjustedOffset;
	float _xSpeed;
	float _ySpeed;

	int* _field; //this is a _sizeX x _sizeY field of pixel color indices

};

#endif