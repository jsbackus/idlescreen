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
#include "CrawliesSprite.h"

/**
 * Constructor.
 */
CrawliesSprite::CrawliesSprite() {
  initSprite();
}

/**
 * Constructor that takes initialization parameters.
 *
 * @param width The screen width.
 * @param height The screen height.
 * @param startX The starting X coordinate
 * @param startY The starting Y coordinate
 * @param pal The palette to use for this Crawly.
 * @param length The length of this Crawly.  If -1, use the palette length.
 * @param thickness The thickness of the crawly.
 * @param spriteSpeed The speed at which this crawly moves.
 * @param palSpeed The speed at which the secondary palette rotates.
 * @param bHeadConstantColor Whether the head keeps the same pal index.
 * @param bHeadRandomColor Whether the head color is random or 0.
 */
CrawliesSprite::CrawliesSprite(int width, int height, int startX,
			       int startY, IndexedPalette* pal, 
			       int length, int thickness,
			       float spriteSpeed,
			       float palSpeed, bool bHeadConstantColor,
			       bool bHeadRandomColor) {
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
  _spriteSpeed = spriteSpeed;
  _palSpeed = palSpeed;
  _bHeadConstantColor = bHeadConstantColor;

  convPalette(pal);

  // initialize segment list.
  if(length < 1) {
    _numSegments = _palWidth;
  } else {
    _numSegments = length;
  }

  if(_numSegments == 0)
    return;

  _segments = new crawlies_tuple[_numSegments];
  if(_segments == NULL)
    return;

  // set positions of all segments, and calculate
  // tail color.
  if(bHeadRandomColor) {
    _tailColorIdx = jrand() % _palWidth;
  } else {
    _tailColorIdx = 0;
  }

  for(int i=0; i<_numSegments; i++) {
    _segments[i].x = startX;
    _segments[i].y = startY;
    _tailColorIdx++;
  }

  // if the length of the crawly is longer than the palette, we'll
  // need to wrap.  In either case, we've gone 1 to far, so back up.
  _tailColorIdx = (_tailColorIdx-1)%_palWidth;

  _lastDir = NONE;

  _bAlive = true;
}

CrawliesSprite::~CrawliesSprite() {
  _bAlive = false;
  if(_pal != NULL) {
    delete [] _pal;
    _pal = NULL;
  }
  if(_segments != NULL) {
    delete [] _segments;
    _segments = NULL;
  }
}

/**
 * Draws the sprite.
 * @param screenObj Screen buffer to draw to.
 */
void CrawliesSprite::drawCrawly(screen_struct* screenObj) {
  if(!isAlive())
    return;
  if(screenObj == NULL)
    return;

  // start drawing, tail first (remember tail is index 0).
  int curColorIdx = _tailColorIdx;
  for(int i=0; i<_numSegments; i++) {
    int x = _segments[i].x;
    int y = _segments[i].y;
    if( x >= 0 && x < _screenWidth && y >= 0 && y < _screenHeight) {
      int palIdx = (curColorIdx + ((int)_palYOffset)*_palWidth)*4;
      for(int i=0;i<_thickness;i++) {
	int ty = (y+i)*_screenWidth*4;
	for(int j=0;j<_thickness;j++) {
	  int tx = (x+j)*4;
	  for(int k=0; k<4;k++) {
	    screenObj->_pixels[tx+ty+k] = _pal[palIdx+k];
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
}

/**
 * Animate the sprite.
 */
void CrawliesSprite::moveCrawly() {
  if(!isAlive())
    return;

  // rotate through secondary palette
  _palYOffset += _palSpeed;
  while(_palYOffset >= _palHeight) {
    _palYOffset -= _palHeight;
  }

  // move body if _curStep is large enough
  _curStep += _spriteSpeed;

  // we only want the integer part of _curStep.  Once we
  // have it, subtract it out of _curStep.
  int clockStep = (int)_curStep;
  _curStep -= (float)clockStep;

  // move the worm one step per clockstep.  The head can change
  // direction at each clock step.  We do this so that there isn't
  // a break between segments.  Not the most efficient...
  while(clockStep > 0) {
    // check to see if head and tail equal.  If so, then this worm is alive,
    // otherwise, assume it is dead.
    _bAlive = (_segments[0].x == _segments[_numSegments-1].x &&
	       _segments[0].y == _segments[_numSegments-1].y);

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

    // pick direction and move head
    bool bDone = false;
    crawlies_dir dir = NONE;
    while(!bDone) {
      int hdIdx = _numSegments-1;
      if(_lastDir == NONE) {
	// if we started off screen, then the only option is to move on screen.
	// we need to assume that the manager did its job and didn't start us
	// beyond a corner.

	if(_segments[hdIdx].x < 0) {
	  // we're off to the left, so move right.
	  dir = RIGHT;
	} else if(_segments[hdIdx].x >= _screenWidth) {
	  // we're off to the right, so move left.
	  dir = LEFT;
	} else if(_segments[hdIdx].y < 0) {
	  // we're below the screen, so move up.
	  dir = UP;
	} else if(_segments[hdIdx].y >= _screenHeight) {
	  dir = DOWN;
	} else {
	  // we're on the screen but haven't moved yet, so pick a direction.
	  dir = (crawlies_dir)(jrand()%4);
	}
      } else {
	dir = (crawlies_dir)(jrand()%4);
      }
      switch(dir) {
      case LEFT:
	if(_lastDir != RIGHT) {
	  _segments[hdIdx].x -= _thickness;
	  _lastDir = dir;
	  bDone = true;
	}
	break;
      case RIGHT:
	if(_lastDir != LEFT) {
	  _segments[hdIdx].x += _thickness;
	  _lastDir = dir;
	  bDone = true;
	}
	break;
      case UP:
	if(_lastDir != DOWN) {
	  _segments[hdIdx].y += _thickness;
	  _lastDir = dir;
	  bDone = true;
	}
	break;
      case DOWN:
	if(_lastDir != UP) {
	  _segments[hdIdx].y -= _thickness;
	  _lastDir = dir;
	  bDone = true;
	}
	break;
      case NONE:
      default:
	break;
      }
    }
    
    // increment head color if not constant
    if(!_bHeadConstantColor) {
      _tailColorIdx = (_tailColorIdx+1)%_palWidth;
    }

    clockStep--;
  }
}

  /**
   * Whether the sprite is still "alive" or valid.
   *
   * @return True if alive.
   */
  bool CrawliesSprite::isAlive() {
    return _bAlive && (_segments != NULL);
  }

  /**
   * Initializes the data structures.
   */
  void CrawliesSprite::initSprite() {
    _bAlive = false;
    _numSegments = 0;
    _segments = NULL;
    _bHeadConstantColor = false;
    _screenWidth = 0;
    _screenHeight = 0;
    _pal = NULL;
    _palWidth = 0;
    _palHeight = 0;
    _curStep = 0.0;
    _palYOffset = 0.0;
    _spriteSpeed = 0.0;
    _palSpeed = 0.0;
    _thickness = 0;
    _tailColorIdx = 0;
    _lastDir = NONE;
  }

  /**
   * Converts the palette from IndexedPalette* to int*
   */
  void CrawliesSprite::convPalette(IndexedPalette* pal) {
    if(pal == NULL)
      return;

    if(_pal != NULL) {
      delete [] _pal;
      _pal = NULL;
    }

    _palWidth = pal->getWidth();
    _palHeight = pal->getHeight();
    _palYOffset = 0.0;

    if(_palWidth <= 0 || _palHeight <= 0)
      return;

    // create space for the 2D palette
    _pal = new int[(_palWidth*_palHeight)*4];
    if(_pal == NULL)
      return;

    // copy palette
    GLubyte buff[4];
    for(int y=0; y<_palHeight; y++) {
      for(int x=0; x<_palWidth; x++) {
	pal->getColor(x,y,&buff[0], 4);
	for(int j=0; j<4;j++) {
	  _pal[(x+y*_palWidth)*4+j] = buff[j];
	}
      }
    }
  }
