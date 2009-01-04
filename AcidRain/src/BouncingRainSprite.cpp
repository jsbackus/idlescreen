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

//debug
//#include<iostream>
//using namespace std;
//end debug

#include "utility/misc_funcs.h"
#include "BouncingRainSprite.h"

/**
 * Constructor.
 */
BouncingRainSprite::BouncingRainSprite() {
  initSprite();
}

/**
 * Constructor that takes initialization parameters.
 *
 * @param width The screen width.
 * @param height The screen height.
 * @param startX The starting X coordinate
 * @param startY The starting Y coordinate
 * @param gravity The acceleration due to gravity.
 * @param pal The palette to use for this.
 * @param palWidth The width of the palette to use.
 * @param palHeight the height of the palette to use.
 * @param palIdx The starting palette primary index
 * @param thickness The thickness of the sprite.
 * @param initialVX The initial v(X) of this sprite.
 * @param initialVY The initial v(Y) of this sprite.
 * @param palSpeed The speed at which the secondary palette rotates.
 * @param initialPalYOffset The initial secondary axis index.
 * @param bHeadConstantColor Whether the head keeps the same pal index.
 */
BouncingRainSprite::BouncingRainSprite(int width, int height, float startX,
				       float startY,
				       float gravity, int* pal, int palWidth,
				       int palHeight, int palIdx,int thickness, 
				       float initialVX, float initialVY,
				       float palSpeed, int initialPalYOffset,
				       bool bHeadConstantColor) {
  initSprite();

  if(pal == NULL)
    return;
  if(width <= 0)
    return;
  if(height <= 0)
    return;
  if(thickness <= 0)
    return;

  _screenWidth = width;
  _screenHeight = height;
  _thickness = thickness;
  _palSpeed = palSpeed;
  _palYOffset = initialPalYOffset;
  _bHeadConstantColor = bHeadConstantColor;
  _gravity = -1.0*abs(gravity);
  _vX = initialVX;
  _vY = initialVY;
  _x = (int)startX;
  _pX = startX - (float)_x;
  _y = (int)startY;
  _pY = startY - (float)_y;
  _pal = pal;
  _palWidth = palWidth;
  _palHeight = palHeight;
  _colorIdx = palIdx;

  _numPixelsUsed = _thickness * _thickness;

  _bAlive = true;
}

BouncingRainSprite::~BouncingRainSprite() {
  _bAlive = false;
}

/**
 * Draws the sprite.
 * @param screenObj Screen buffer to draw to.
 */
void BouncingRainSprite::drawSprite(screen_struct* screenObj) {
  if(!isAlive())
    return;
  if(screenObj == NULL)
    return;

  int x = (int)_x;
  int y = (int)_y;
  int palIdx = (_colorIdx + ((int)_palYOffset)*_palWidth)*4;
  for(int m=0;m<_thickness;m++) {
    int ty = (y+m)*_screenWidth*4;
    for(int j=0;j<_thickness;j++) {
      int tx = (x+j)*4;
      if( (x+j) >= 0 && (x+j) < _screenWidth 
	  && (m+y) >= 0 && (m+y) < _screenHeight) {
	for(int k=0; k<4;k++) {
	  screenObj->_pixels[tx+ty+k] = _pal[palIdx+k];
	}
      }
    }
  }
  _colorIdx--;
  if(_colorIdx < 0) {
    _colorIdx = _palWidth-1;
  }
}

/**
 * Animate the sprite.
 *
 * @param horizontalAcceleration The acceleration due to wind.
 */
void BouncingRainSprite::moveSprite(float horizontalAcceleration) {
  if(!isAlive())
    return;

  // adjust velocities
  _vX += horizontalAcceleration;
  _vY += _gravity;

  // rotate through secondary palette
  _palYOffset += _palSpeed;
  while(_palYOffset >= _palHeight) {
    _palYOffset -= _palHeight;
  }

  // move body if _curStep is large enough
  _pX += _vX;
  _pY += _vY;

  // we only want the integer part of the position.  Once we
  // have it, subtract it out.
  int stepX = (int)_pX;
  _pX -= (float)stepX;
  int stepY = (int)_pY;
  _pY -= (float)stepY;

  _y += stepY;
  _x += stepX;
  // Wrap X around screen if need be.
  while (_x < 0) {
    _x += _screenWidth;
  }
  while (_x >= _screenWidth) {
    _x -= _screenWidth; 
  }
  if(_y < 0) {
    _bAlive = false;
  }
}

/**
 * Initializes the data structures.
 */
void BouncingRainSprite::initSprite() {
  initBaseSprite();
  _x = 0;
  _y = 0;
}
