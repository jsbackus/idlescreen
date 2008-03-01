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

#include "IndexedPaletteBackground.h"

/**
 * Sets the color palette
 */
void IndexedPaletteBackground::setPalette(IndexedPalette* pal) {
	if(pal == NULL)
		return;

	if(_palette != NULL) {
		delete [] _palette;
		_palette = NULL;
	}

	_paletteWidth = pal->getWidth();
	_paletteHeight = pal->getHeight();
	_yAdjustedOffset = 0;

	if(_paletteWidth <= 0 || _paletteHeight <= 0)
		return;
	_palette = new int[(_paletteHeight*_paletteWidth+1)*4];
	if(_palette == NULL)
		return;

	GLubyte buff[4];
	for(int x=0;x<_paletteWidth; x++) {
		for(int y=0;y<_paletteHeight; y++) {
			pal->getColor(x,y,&buff[0], 4);
			for(int j=0;j<4;j++) {
				_palette[(x+y*_paletteWidth)*4+j] = buff[j];
			}
		}
	}

	//make "last" color black
	for(int i = 0; i< 3; i++) {
		_palette[_paletteHeight*_paletteWidth*4 + i] = 0;
	}
	_palette[_paletteHeight*_paletteWidth*4 + 3] = 255;
}

/**
 * Sets the palette animation speeds.
 */
void IndexedPaletteBackground::speed(float xSpeed, float ySpeed) {
	_xSpeed = xSpeed;
	_ySpeed = ySpeed;
}

/**
 * Default constructor.
 */
IndexedPaletteBackground::IndexedPaletteBackground() {
	_xSpeed = 1.0;
	_ySpeed = 1.0;
	_paletteWidth = 0;
	_paletteHeight = 0;
}

/**
 * Constructor takes number of colors and pointer to palette
 */
IndexedPaletteBackground::IndexedPaletteBackground(int sizeX, int sizeY, IndexedPalette* pal) {
		initIndexedPaletteBackground(sizeX, sizeY);
		setPalette(pal);
}

IndexedPaletteBackground::~IndexedPaletteBackground() {
	destroyIndexedPaletteBackground();
}

/**
 * Translates the buffer of indices to 32bit colors and places in screenObj
 */
void IndexedPaletteBackground::drawBackground(screen_struct* screenObj) {
	if(_field == NULL)
		return;
	if(_palette == NULL)
		return;

	for(int y = 0; y < _sizeY; y++)
	{
		int fieldY = y*_sizeX;
		int imageY = fieldY*4;
		for(int x=0;x<_sizeX;x++)
		{
			int idx = getShiftedIndex(_field[fieldY+x])*4;
			int xVal = x*4;
			for(int k=0;k<4;k++)
			{
				screenObj->_pixels[imageY + xVal+k] = _palette[idx+k];
			}
		}
	}
}

/**
 * Rotates the palette.
 */
void IndexedPaletteBackground::clocktick() {
	rotateColorIndex();
}

/**
 * Called from the constructor and does all of the work,
 * so that derived classes can call from their constructors.
 */
void IndexedPaletteBackground::initIndexedPaletteBackground(int sizeX, int sizeY) {
	initBackground(sizeX, sizeY);
	_xIndexOffset = 0;
	_yIndexOffset = 0;

	_palette = NULL;
	_paletteWidth = 0;
	_paletteHeight = 0;

	//create the color buffer.
	if(_sizeX > 0 && _sizeY > 0)
	{
		_field = new int[(_sizeX)*(_sizeY)];		
	}
	else
	{
		_field = NULL;
		exit(1);
	}
}

/**
 * Called from the destructor and does all of the work,
 * so that derived classes can call from their destructors.
 */
void IndexedPaletteBackground::destroyIndexedPaletteBackground() {
	if(_field != NULL)
	{
		delete [] _field;
		_field = NULL;
	}
	if(_palette != NULL)
	{
		delete [] _palette;
		_palette = NULL;
	}
	_paletteWidth = 0;
	_paletteHeight = 0;
}

/**
 * Given a color index, returns the shifted color index.
 * If a number less than 0 is passed in, a number that
 * corresponds to black is returned.
 */
int IndexedPaletteBackground::getShiftedIndex(int idx) {
	if(idx < 0)
	{
		return _paletteWidth*_paletteHeight;
	}
	if(_paletteWidth > 0 && _paletteHeight > 0)
	{
		return (idx+int(_xIndexOffset))%_paletteWidth + _yAdjustedOffset;
	}
	return 0;
}

/**
 * Rotates the color index.
 */
void IndexedPaletteBackground::rotateColorIndex() {
	_xIndexOffset += _xSpeed;
	if(_xIndexOffset >= _paletteWidth)
	{
		_xIndexOffset = 0;
	}
	_yIndexOffset += _ySpeed;
	if(_yIndexOffset >= _paletteHeight)
	{
		_yIndexOffset = 0;
	}
	_yAdjustedOffset = int(_yIndexOffset) * _paletteWidth;
}
