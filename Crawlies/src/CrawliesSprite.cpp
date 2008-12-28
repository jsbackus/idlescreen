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

#include "CrawliesSprite.h"

/**
 * Constructor.
 */
void CrawliesSprite::CrawliesSprite() {
  initSprite();
}

/**
 * Constructor that takes initialization parameters.
 *
 * @param sizeX The screen width.
 * @param sizeY The screen height.
 * @param startX The starting X coordinate
 * @param startY The starting Y coordinate
 * @param pal The palette to use for this Crawly.
 * @param length The length of this Crawly.  If -1, use the palette length.
 * @param spriteSpeed The speed at which this crawly moves.
 * @param palSpeed The speed at which the secondary palette rotates.
 * @param bHeadConstantColor Whether the head keeps the same pal index.
 * @param bHeadRandomColor Whether the head color is random or 0.
 */
void CrawliesSprite::CrawliesSprite(int sizeX, int sizeY, int startX,
				    int startY, IndexedPalette* pal, 
				    int length, float spriteSpeed,
				    float palSpeed, bool bHeadConstantColor,
				    bool bHeadRandomColor) {
  initSprite();

  if(pal == NULL)
    return;
  if(sizeX <= 0)
    return;
  if(sizeY <= 0)
    return;

  _screenWidth = sizeX;
  _screenHeight = sizeY;
  _spriteSpeed = spriteSpeed;
  _palSpeed = palSpeed;
  _bHeadConstantColor = bHeadConstantColor;

  convPalette(pal);

  // initialize segment list.
  if(length < 1) {
    _numSegments = _palWidth;
  } else {
    _numSegments == length;
  }

  if(_numSegments == 0)
    return;

  _segments = new crawlies_tuple[_numSegments];
  if(_segments == NULL)
    return;

  // set positions of all segments, and calculate
  // tail color.
  if(bHeadRandomColor) {
    _tailColorIdx = rand() % _palWidth;
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
  _tailColorIdx = (_tailColorIdx-1)%palWidth;

  _lastDir = NONE;

  _bAlive = true;
}

CrawliesSprite::~CrawliesSprite() {
  _bAlive = false;
  _pal = NULL;
  if(_segments != NULL) {
    delete [] _segments;
    _segments = NULL;
  }
}

/**
 * Draws the sprite.
 * @param screenObj Screen buffer to draw to.
 */
void CrawliesSprite::drawSprite(screen_struct* screenObj) {
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
	x *= 4;
	y *= _screenWidth*4;
	int palIdx = curColorIdx + ((int)_palYOffset)*_palWidth;
	for(int j=0; j<4;j++) {
	  screenObj->_pixels[x+y+j] = _pal[j];
	}
    }
  }
}

/**
 * Animate the sprite.
 */
void CrawliesSprite::clocktick() {
  if(!isAlive())
    return;

  // rotate through secondary palette
  _palYOffset += _palSpeed;
  while(_palYOffset >= _palHeight) {
    _palYOffset -= (float)_palHeight;
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
    _bAlive == (_segments[0].x == _segments[_numSegments-1].x &&
		_segments[0].y == _segments[_numSegments-1].y);

    // for segments 0 to N-1, we just shift down.
    for(int i=0; i<_numSegments-1; i++) {
      _segment[i] = _segment[i+1];
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
	  int hdIdx = _numSegments-1;
    while(!bDone) {
      dir = rand()%4;
      switch(dir) {
      case LEFT:
	if(_lastDir != RIGHT) {
	  _segments[hdIdx].x -= 1;
	  _lastDir = dir;
	  bDone = true;
	}
	break;
      case RIGHT:
	if(_lastDir != LEFT) {
	  _segments[hdIdx].x += 1;
	  _lastDir = dir;
	  bDone = true;
	}
	break;
      case UP:
	if(_lastDir != DOWN) {
	  _segments[hdIdx].y += 1;
	  _lastDir = dir;
	  bDone = true;
	}
	break;
      case DOWN:
	if(_lastDir != UP) {
	  _segments[hdIdx].y -= 1;
	  _lastDir = dir;
	  bDone = true;
	}
	break;
    }
    
    // increment head color if not constant
    if(!_bHeadConstantColor) {
      _lastColorIdx = (_lastColorIdx+1)%_palWidth;
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
  _spriteSpeed = 0.0;
  _palSpeed = 0.0;
}

/**
 * Converts the palette from IndexedPalette* to int*
 */
void CrawliesSprite::convPalette(IndexedPalette* pal) {
  if(pal == NULL)
    return;

  if(_pal != NULL) {
    delete [] _palette;
    _palette = NULL;
  }

  _palWidth = pal->getWidth();
  _palHeight = pal->getHeight();
  _palYOffset = 0;

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
