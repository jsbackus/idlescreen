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

#include "LensObject.h"

//constructor and destructor
LensObject::LensObject() {
	_screenObj = NULL;
	_field = NULL;
	_bitmap = NULL;
	_screenBuf = NULL;
	_posX = 0;
	_posY = 0;
	_sizeX = 0;
	_sizeY = 0;
	_dirX = 0;
	_dirY = 0;
	_bVisible = false;
	_bOutlineTint = false;
	_bOutlineComplementColors = false;
}

LensObject::LensObject(screen_struct* screenObj, int sizeX, int sizeY, int outlineWidth) {
	_screenObj = NULL;
	_field = NULL;
	_outline = NULL;
	_bitmap = NULL;
	_screenBuf = NULL;
	_posX = 0;
	_posY = 0;
	_sizeX = 0;
	_sizeY = 0;
	_dirX = 0;
	_dirY = 0;
	_bVisible = false;
	_bOutlineTint = false;
	_bOutlineComplementColors = false;

	setupLens(screenObj,sizeX,sizeY, outlineWidth);

	//fill bitmap to true and field to direct map
	for(int y=1;y<_sizeY-1;y++) {
		int yLoc = y*_sizeX;
		int yVal = y*_screenObj->_sizeX;
		for(int x=1;x<_sizeX-1;x++) {
			_field[yLoc+x] = (yVal+x)*4;
			_bitmap[yLoc+x] = true;
		}
	}
	calcOutline();
}


LensObject::~LensObject() {
	destroyLens();
}

//Renders the lens
void LensObject::drawLens() {
	renderLens(_posX, _posY, _sizeX, _sizeY, _field, _bitmap, _outline);
}

//moves the lens
void LensObject::animate() {
	moveLens();
}

//sets up all datastructures.  Must be called before subclasses set field and outline values.
void LensObject::setupLens(screen_struct* screenObj, int sizeX, int sizeY, int outlineWidth){
	if(sizeX > 0 && sizeY > 0 && screenObj != NULL) {
		_screenObj = screenObj;
		_sizeX = sizeX;
		_sizeY = sizeY;
		_posX = 0;
		_posY = 0;
		_dirX = 0;
		_dirY = 0;
		_speed = 0;
		_bTint = false;
		_bComplementColors = false;
		_bOutlineTint = false;
		_bOutlineComplementColors = false;
		_bVisible = true;
		_outlineWidth = outlineWidth;

		_field = new int[_sizeX*_sizeY];
		_screenBuf = new GLubyte[_sizeX*_sizeY*4];
		_outline = new bool[_sizeX*_sizeY];
		_bitmap = new bool[_sizeX*_sizeY];
		if(_field == NULL || _outline == NULL || _bitmap == NULL) {
			destroyLens();
		}

		for(int i=0;i<_sizeX*_sizeY;i++) {
			_outline[i] = false;
			_bitmap[i] = false;
		}
	}
}

//cleans up
void LensObject::destroyLens() {
	_screenObj = NULL;
	if(_field != NULL) {
		delete [] _field;
		_field = NULL;
	}
	if(_screenBuf != NULL) {
		delete [] _screenBuf;
		_screenBuf = NULL;
	}
	if(_outline != NULL) {
		delete [] _outline;
		_outline = NULL;
	}
	if(_bitmap != NULL) {
		delete [] _bitmap;
		_bitmap = NULL;
	}
	_sizeX = 0;
	_sizeY = 0;
	_posX = 0;
	_posY = 0;
	_dirX = 0;
	_dirY = 0;
	_speed = 0;
}

//sets the tint.  If NULL then tinting
//is disabled.  tint must be 4 GLubytes
//long!
void LensObject::setTint(GLubyte* tint) {
	_bTint = false;
	if(tint != NULL) {
		_bTint = true;
		for(int i=0; i<4;i++) {
			_tint[i] = float(float(tint[i])/255.0);
		}
	}
}

//sets the lens to complement colors
void LensObject::setComplement(bool bComplement) {
	_bComplementColors = bComplement;
}

//returns whether this lens is still visible
bool LensObject::isVisible() {
	return _bVisible;
}

//Gives the lens a new position and direction
void LensObject::setPosition(int posX, int posY) {
	if(_posX > -1*_sizeX && _posY > -1*_sizeY &&
		_posX < _screenObj->_sizeX && _posY < _screenObj->_sizeY - 1)
	{
		_bVisible = true;
		_posX = posX;
		_posY = posY;
	}
}

//gives the lens a new direction
void LensObject::setDirection(float dirX, float dirY) {
	float mag = dirX*dirX + dirY*dirY;

	//check for div by 0.
	if(mag == 0.0)
		return;

	mag = sqrt(mag);
	_dirX = dirX/mag;
	_dirY = dirY/mag;
}

//sets the speed
void LensObject::setSpeed(float speed) {
	_speed = speed;
}

//convenience function that uses what data contained within base class
void LensObject::calcOutline() {
	calcOutline(_sizeX,_sizeY, _bitmap, _outline);
}

/**
 * Calculates the outline from the bitmap.
 * Looks for any false values next to a true
 * value.  To make it more generic, it accepts
 * size data and pointers to ouline and
 * bitmap buffers.
 */
void LensObject::calcOutline(int sizeX, int sizeY, bool* bitmap, bool* outline) {

	if(bitmap == NULL || outline == NULL)
		return;

	for(int i=0;i<_outlineWidth;i++) {
		for(int y=0;y<sizeY;y++) {
			for(int x=0;x<sizeX;x++) {
				int loc = y*sizeX + x;
				//assume false
				outline[loc] = false;

				if(!bitmap[loc]) {

					//check bottom left
					if(y>i && x > i && bitmap[loc - (i+1)*sizeX - 1 - i])
						outline[loc] = true;

					//check bottom mid
					if(y>i && bitmap[loc - (i+1)*sizeX])
						outline[loc] = true;

					//check bottom right
					if(y>i && x < sizeX - 1 - i && bitmap[loc - (i+1)*sizeX + 1 + i])
						outline[loc] = true;

					//check left
					if(x > i && bitmap[loc - 1])
						outline[loc] = true;

					//check right
					if(x < sizeX - 1 - i && bitmap[loc + 1])
						outline[loc] = true;

					//check top left
					if(y < sizeY - 1 - i && x > 0 && bitmap[loc + (i+1)*sizeX - 1 - i])
						outline[loc] = true;

					//check top mid
					if(y < sizeY - 1 - i  && bitmap[loc + (i+1)*sizeX])
						outline[loc] = true;

					//check top right
					if(y < sizeY - 1 - i  && x < sizeX - 1 - i && bitmap[loc + (i+1)*sizeX + 1 + i])
						outline[loc] = true;

				} else {
					//for true values next to the edges, go ahead
					//and make them outlines
					if(x >= sizeX - i - 1 || x <= i || y >= sizeY - i - 1 || y <= i)
						outline[loc] = true;
				}
			}
		}
	}
}

//sets the outline color.  If NULL, then
//outline is disabled.
//color must be 4 GLubytes long!
void LensObject::setOutlineTint(GLubyte* color) {
	_bOutlineTint = false;
	if(color != NULL) {
		_bOutlineTint = true;
		for(int i=0; i<4;i++) {
			_outlineTint[i] = float(color[i])/float(255.0);
		}
	}
}

//whether the outline should use complementary
//colors
void LensObject::setOutlineComplement(bool bComplement) {
	_bOutlineComplementColors = bComplement;
}

/**
 * Routine that actually draws.  In order to make it more generic for
 * any subclasses, it accepts position, size, field, outline, and bitmap data
 * as arguments instead of using the data located inside the class.
 */
void LensObject::renderLens(int posX, int posY, int sizeX, int sizeY, int* field, bool* bitmap, bool* outline) {
	if(_bVisible && _screenObj != NULL)
	{
		//get background colors		
		int startX = (posX < 0) ? int(-1.0*posX) : 0 ;
		int stopX = (posX + sizeX > _screenObj->_sizeX) ? _screenObj->_sizeX - int(posX) : sizeX;
		int startY = (posY < 0) ? int(-1.0*posY) : 0;
		int stopY = (posY + sizeY > _screenObj->_sizeY) ? _screenObj->_sizeY - int(posY) : sizeY;

		//this is the bottom left corner..  It's ok if this is "off screen".  startX/startY compensate.
		int botLeft = int(posY) * _screenObj->_sizeX*4 + int(posX)*4;

		int maxPos = _screenObj->_sizeX*(_screenObj->_sizeY-1)*4;
		GLubyte buff[4];
		for(int y = startY; y < stopY; y++) {
			int yImage = y*sizeX;
			int yBuff = yImage*4;
			int yPos = (y+int(posY))*_screenObj->_sizeX * 4;
			for(int x = startX; x < stopX; x++) {
				int xBuff = (x-startX)*4;
				int xPos = (x+int(posX))*4;

				//if(yPos + xPos + 4 < maxPos) {

				int pixelPos = field[yImage + x] + botLeft;
				//copy to temporary buffer
				for(int k = 0; k<4; k++) {
					if(pixelPos > 0 && pixelPos < maxPos) {
						buff[k] = _screenObj->_pixels[pixelPos + k];
					} else {
						//if we're off the screen, then don't allow any distortion.							
						buff[k] = _screenObj->_pixels[yPos + xPos + k];
					}
				}

				//if the outline bit is set and we're showing the outline,
				//then apply appropriate color transforms
				if((_bOutlineTint || _bOutlineComplementColors) && outline[x+yImage]) {
					//apply tint if needed
					if(_bOutlineTint) {
						float mag = 0.0;
						float color[3];

						for(k=0;k<3;k++) {
							float imageColor = float(buff[k])/float(255.0);
							color[k] =  imageColor * float(1.0 -  _outlineTint[3]) + _outlineTint[k]*_outlineTint[3];
							mag += color[k]*color[k];
						}
						// Really should normalize the color, but it
						// doesn't look as good.
						mag = sqrt(mag);
						for(k=0;k<3;k++) {
							//buff[k] = GLubyte((color[k]/mag)*255.0);
							buff[k] = GLubyte((color[k])*255.0);
						}
					}

					//complement if needed
					if(_bOutlineComplementColors) {
						for(k=0;k<3;k++) {
							buff[k] = 255 - buff[k];
						}
					}
				} else {
					//see if we do any color transforms
					if(bitmap[x + yImage]) {

						//apply tint if needed
						if(_bTint) {
							//float mag = 0.0;
							float color[3];

							for(k=0;k<3;k++) {
								float imageColor = float(buff[k])/float(255.0);
								color[k] =  imageColor * float(1.0 -  _tint[3]) + _tint[k]*_tint[3];
								//mag += color[k]*color[k];
							}
							// Really should normalize the color, but it
							// doesn't look as good.
							//mag = sqrt(mag);
							for(k=0;k<3;k++) {
								//buff[k] = GLubyte((color[k]/mag)*255.0);
								buff[k] = GLubyte((color[k])*255.0);
							}
						}

						//complement if needed
						if(_bComplementColors) {
							for(k=0;k<3;k++) {
								buff[k] = 255 - buff[k];
							}
						}
					}
				}

				//put back into temp buffer
				for(int k = 0; k<4; k++) {
					//copy to temporary buffer
					_screenBuf[yBuff+xBuff+k] = buff[k];
				}
			}
		}

		//copy back to screen
		for(int y = startY; y < stopY; y++) {
			int yBuff = y*sizeX*4;
			int yPos = (y+int(posY))*_screenObj->_sizeX * 4;
			for(int x = startX; x < stopX; x++) {
				int xBuff = (x-startX)*4;
				int xPos = (x+int(posX))*4;

				for(int k=0;k<4;k++) {
					_screenObj->_pixels[yPos + xPos + k] = _screenBuf[yBuff+xBuff+k];
				}
			}
		}
	}
}

/**
 * Moves the lens.
 */
void LensObject::moveLens() {
	if(_bVisible) {
		_posX += int(_dirX*_speed);
		_posY += int(_dirY*_speed);

		if(!(_posX > -1*_sizeX && _posY > -1*_sizeY &&
			_posX < _screenObj->_sizeX && _posY < _screenObj->_sizeY - 5)) {
			_bVisible = false;
		}
	}
}

//sets the visibility of the lens if it is on screen.
void LensObject::setVisible(bool bVisible) {
	_bVisible = bVisible;

	if(_bVisible && !(_posX > -1*_sizeX && _posY > -1*_sizeY &&
		_posX < _screenObj->_sizeX && _posY < _screenObj->_sizeY - 5)) {
		_bVisible = false;
	}
}
