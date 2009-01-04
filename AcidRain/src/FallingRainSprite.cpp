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
#include<iostream>
using namespace std;
//end debug

#include <math.h>

#include "utility/misc_funcs.h"
#include "FallingRainSprite.h"
#include "BouncingRainSprite.h"

/**
 * Constructor.
 */
FallingRainSprite::FallingRainSprite() {
  initSprite();
}

/**
 * Constructor that takes initialization parameters.
 *
 * @param width The screen width.
 * @param height The screen height.
 * @param startX The starting X coordinate
 * @param gravity The acceleration due to gravity.
 * @param pal The palette to use for this.
 * @param palWidth The width of the palette to use.
 * @param palHeight the height of the palette to use.
 * @param length The length of this sprite.  If -1, use the palette length.
 * @param thickness The thickness of the sprite.
 * @param initialV The initial falling velocity of this sprite.
 * @param palSpeed The speed at which the secondary palette rotates.
 * @param initialPalYOffset The initial secondary axis index.
 * @param bHeadConstantColor Whether the head keeps the same pal index.
 * @param bHeadRandomColor Whether the head color is random or 0.
 * @param recoilElasticity The amount of "bounce" the drops have.
 */
FallingRainSprite::FallingRainSprite(int width, int height, int startX,
				     float gravity, int* pal,
				     int palWidth, int palHeight, int length, 
				     int thickness, float initialV,
				     float palSpeed, int initialPalYOffset,
				     bool bHeadConstantColor,
				     bool bHeadRandomColor,
				     float recoilElasticity) {
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
  _gravity = -1.0*fabs(gravity);
  _vY = -1.0*fabs(initialV);
  _pal = pal;
  _palWidth = palWidth;
  _palHeight = palHeight;
  _recoilElasticity = recoilElasticity;

  // initialize segment list.
  if(length < 1) {
    _numSegments = _palWidth;
  } else {
    _numSegments = length;
  }

  if(_numSegments == 0)
    return;

  _numPixelsUsed = _numSegments * _thickness * _thickness;

  _segments = new rainsprite_tuple[_numSegments];
  if(_segments == NULL)
    return;

  // set positions of all segments, and calculate
  // tail color.
  if(bHeadRandomColor) {
    _colorIdx = jrand() % _palWidth;
  } else {
    _colorIdx = 0;
  }

  for(int i=0; i<_numSegments; i++) {
    _segments[i].x = startX;
    _segments[i].y = height; // off top of screen
    _colorIdx++;
  }

  // if the length of the sprite is longer than the palette, we'll
  // need to wrap.  In either case, we've gone 1 to far, so back up.
  _colorIdx = (_colorIdx-1)%_palWidth;

  _bAlive = true;
}

FallingRainSprite::~FallingRainSprite() {
  _bAlive = false;
  if(_segments != NULL) {
    delete [] _segments;
    _segments = NULL;
  }
}

/**
 * Draws the sprite.
 * @param screenObj Screen buffer to draw to.
 */
void FallingRainSprite::drawSprite(screen_struct* screenObj) {
  if(!isAlive())
    return;
  if(screenObj == NULL)
    return;

  // start drawing, tail first (remember tail is index 0).
  int curColorIdx = _colorIdx;
  for(int i=0; i<_numSegments; i++) {
    int x = _segments[i].x;
    int y = _segments[i].y;
    int palIdx = (curColorIdx + ((int)_palYOffset)*_palWidth)*4;
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
    //curColorIdx = (curColorIdx+1)%_palWidth;
    curColorIdx--;
    if(curColorIdx < 0) {
      curColorIdx = _palWidth-1;
    }
  }
}

/**
 * Animate the sprite.
 *
 * @param horizontalAcceleration The acceleration due to wind.
 */
void FallingRainSprite::moveSprite(float horizontalAcceleration) {
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

  float dX = 0.0;
  float dY = 0.0;
  float absY = fabs(_pY);
  float absX = fabs(_pX);

  // Check to see if mag(X) is larger than 1 and it is larger than Y
  if(absX > absY && (_pX <= -1.0 || _pX >= 1.0) ) {
    dX = _pX/absX; // make sure we have the right sign...
    if(_pY <= -1.0 || _pY >= 1.0) {
      // change in position along Y is non-zero, so we need
      // to move along at a ratio of dY = Y/X
      dY = fabs(_pY / _pX)*(_pY/absY);
    }
  } else if(absY > absX && (_pY <= -1.0 || _pY >= 1.0) ) {
    dY = _pY/absY; // make sure we have the right sign...
    if(_pX <= -1.0 || _pX >= 1.0) {
      // change in position along X is non-zero, so we need
      // to move along at a ratio of dX = X/Y
      dX = fabs(_pX / _pY)*(_pX/absX);
    }
  } else if(absY == absX) {
    dX = _pX/absX;
    dY = _pY/absY;
  }

  float hX = _segments[_numSegments-1].x;
  float hY = _segments[_numSegments-1].y;
  float scrWidth = (float)_screenWidth;
  float thick = (float)_thickness;

  // while there are still whole steps to take along either
  // axis, move the sprite trail along.  Maybe not the most
  // efficient, but it does keep breaks from appearing.
  while( (_pX > 1.0 || _pX < -1.0 || _pY > 1.0 || _pY < -1.0) 
	 && _numSegments > 0) { 
    
    _bAlive = (_numSegments > 0 &&
	       _segments[0].x == _segments[_numSegments-1].x &&
	       _segments[0].y == _segments[_numSegments-1].y);

    if(_numSegments != 0) {

      // for segments 0 to N-1, we just shift down.
      for(int i=0; i<_numSegments-1; i++) {
	_segments[i] = _segments[i+1];
	// Check to see if this segment is on screen.  If so,
	// indicate it is still alive.
	if(0 <= _segments[i].x && _segments[i].x < _screenWidth &&
	   0 <= _segments[i].y && _segments[i].y < _screenHeight) {
	  _bAlive = true;
	}
      }
      
      // update position differential
      _pX -= dX;
      _pY -= dY;
    
      // move head.
      hY += dY*thick;
      hX += dX*thick;
      
      // Wrap X around screen if need be.
      while (hX < 0) {
	hX += scrWidth;
      }
      while (hX >= scrWidth) {
	hX -= scrWidth; 
      }
      
      // update head position
      _segments[_numSegments-1].x = (int)hX;
      _segments[_numSegments-1].y = (int)hY;

      // increment head color if not constant
      if(!_bHeadConstantColor) {
	_colorIdx = (_colorIdx+1)%_palWidth;
      }
      
      // BEGIN TO DO 
      // Need to look for when the head segment hits y=0, calculate
      // the new velocities, and then calculate the new position.
      // need to account for the fact that the position may have shot
      // past y=0!

      // if head reaches the bottom, create new recoil sprite
      while(_numSegments > 0 && _segments[_numSegments-1].y < 0) {
	// calculate random recoil direction with a velocity equal in
	// magnitude to the velocity at which it hit
	float recoilMag = sqrt(_vX*_vX+_vY*_vY)*_recoilElasticity;
	float recoilAng = ((float)(jrand()%((int)(PI*10000))))/10000.0;
	float vX = recoilMag*cos(recoilAng);
	float vY = recoilMag*sin(recoilAng);

	// calculate position
	float pY = 0.0;
	float pX = float(_segments[_numSegments-1].x);

	// there is a bug in here, but it looks okay without this...
	/*
	// we need to backtrack for pX to the point where pY == 0.0
	pX = (((float)_segments[_numSegments-1].y) / (dY*thick)) * 
	dX*thick*pX + pX;

	// adjust position by the remaining clockSteps
	pX += ((float)clockStep)*vX;
	pY += ((float)clockStep)*vY;
	*/

	/// end revamp!
	// wrap
	while(pX < 0) {
	  pX += scrWidth;
	}
	while(pX >= scrWidth) {
	  pX -= scrWidth;
	}

	BouncingRainSprite* tmpSprite =
	  new BouncingRainSprite(_screenWidth, _screenHeight, pX, pY,
				 _gravity, _pal, _palWidth, _palHeight,
				 _colorIdx, _thickness, vX, vY, _palSpeed,
				 _palYOffset, _bHeadConstantColor);

	if(tmpSprite != NULL) {

	  // add it to the list
	  recoil_sprite_node* tmpNode = new recoil_sprite_node;
	  if(tmpNode != NULL) {
	    tmpNode->next = _recoilSprites;
	    tmpNode->sprite = tmpSprite;
	    _recoilSprites = tmpNode;
	    tmpNode = NULL;
	  }
	}
	_numSegments--;
      }
      if(_numSegments == 0 || _segments[_numSegments-1].y < 0) {
	// we're not alive anymore
	_bAlive = false;
      }
    }
  }
}

/**
 * Initializes the data structures.
 */
void FallingRainSprite::initSprite() {
  initBaseSprite();
  _numSegments = 0;
  _segments = NULL;
  _recoilElasticity = 1.0;
  _recoilSprites = NULL;
}

/**
 * When this sprite starts to break apart on recoil, this will
 * return one such sprite on each call, or NULL if none are available.
 */
RainSprite* FallingRainSprite::getRecoilSprite() {
  if(_recoilSprites != NULL) {
    RainSprite* retVal = _recoilSprites->sprite;
    recoil_sprite_node* tmpNode = _recoilSprites;
    _recoilSprites = _recoilSprites->next;
    tmpNode->next = NULL;
    tmpNode->sprite = NULL;
    delete tmpNode;
    return retVal;
  }
  return NULL;
}
