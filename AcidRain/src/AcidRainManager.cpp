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

//begin debug
//#include <iostream>
//using namespace std;
//end debug

#include "utility/misc_funcs.h"
#include "AcidRainManager.h"

/**
 * Generic constructor.
 */
AcidRainManager::AcidRainManager() {
  initData();
}

/**
 * Constructor that takes all of the initialization information that
 * isn't specific to each palette.  For palette specific information,
 * see addCrawliesStyle().
 *
 * @param sizeX The width of the screen in pixels.
 * @param sizeY The height of the screen in pixels.
 * @param maxCrawlies The maximum number of worms allowed on the screen.
 * @param spawnChance There is a 1/spawnChance chance for a spawn.
 */
AcidRainManager::AcidRainManager(int sizeX, int sizeY, int maxCrawlies,
				 int spawnChance) {
  initData();
  initBackground(sizeX, sizeY);
  _maxNumCrawlies = maxCrawlies;
  _crawlies = new CrawliesSprite*[maxCrawlies];
  _numCrawlies = 0;
  _spawnChance = spawnChance;
  _numStyles = 0;
  _bSetupFinished = true;
}

AcidRainManager::~AcidRainManager() {
  if(_styles != NULL) {
    delete [] _styles;
    _styles = NULL;
  }
  if(_crawlies != NULL) {
    for(int i=0; i<_numCrawlies;i++) {
      delete _crawlies[i];
      _crawlies[i] = NULL;
    }
    delete [] _crawlies;
    _crawlies = NULL;
  }
}

/**
 * This procedure will add a new crawly style to the list of styles
 * available for the worms.  Must be called at least once before the
 * first call to clocktick()!
 *
 * For consistant sized worms, minLength and maxLength should be set
 * equal.  To use the length of the palette for either quatity, set
 * equal to -1.
 * 
 * If the sprite speed is supposed to be constant, minSpeed and maxSpeed
 * must be set equal.
 *
 * @param pal A pointer to the palette to associate with this style.
 * @param minLength The minimum crawly length.
 * @param maxLength the maximum crawly length.
 * @param thickness The thickness of the crawly.
 * @param minSpriteSpeed The minimum worm speed.
 * @param maxSpriteSpeed The maximum worm speed.
 * @param palSpeed The secondary palette rotation speed.
 * @param bHeadConstantColor Whether the head keeps the same color index.
 * @param bHeadRandomColor Whether the head color is random when created.
 */
void AcidRainManager::addCrawliesStyle(IndexedPalette* pal, int minLength, 
				       int maxLength, 
				       int thickness,
				       float minSpriteSpeed,
				       float maxSpriteSpeed,
				       float palSpeed, 
				       bool bHeadConstantColor, 
				       bool bHeadRandomColor) {
  if(pal == NULL)
    return;
  if(_styles == NULL)
    return;

  if(_numStyles == _maxNumStyles) {
    growStyleList();
  }

  // clone palette
  _styles[_numStyles].pal = *pal;

  // copy other parameters
  _styles[_numStyles].minLength = minLength;
  _styles[_numStyles].maxLength = maxLength;
  _styles[_numStyles].thickness = thickness;
  _styles[_numStyles].minSpriteSpeed = minSpriteSpeed;
  _styles[_numStyles].maxSpriteSpeed = maxSpriteSpeed;
  _styles[_numStyles].palSpeed = palSpeed;
  _styles[_numStyles].bHeadConstantColor = bHeadConstantColor;
  _styles[_numStyles].bHeadRandomColor = bHeadRandomColor;

  _numStyles++;
}

/**
 * Sets the "setup finished" flag.
 *
 * @param Whether setup is finished.
 */
void AcidRainManager::setFinished(bool bFinished) {
  _bSetupFinished = bFinished;
}

/**
 * Draws the whole Background.
 */
void AcidRainManager::drawBackground(screen_struct* screenObj) {
  if(screenObj == NULL)
    return;

  if(!isSetupFinished()) {
    return;
  }

  // first, erase the buffer.
  for(int y=0;y<screenObj->_sizeY;y++) {
    int yOffset = y*_sizeX*4;
    for(int x=0;x<screenObj->_sizeX;x++) {
      int xOffset = x*4;
      for(int j=0; j<3; j++) {
	screenObj->_pixels[yOffset+xOffset+j] = 0;
      }
      screenObj->_pixels[yOffset+xOffset+3] = 255;
    }
  }

  // now go through the list of crawlies
  if(_crawlies == NULL)
    return;
  for(int i=0; i<_numCrawlies; i++) {
    if(_crawlies[i] != NULL && _crawlies[i]->isAlive()) {
      // we've found a valid, live crawly, so render!
      _crawlies[i]->drawCrawly(screenObj);
    }
  }
}

/**
 * Finishes generating the fractal then rotates the palette when done.
 */
void AcidRainManager::clocktick() {
  if(!isSetupFinished()) {
    return;
  }
  if(_sprites == NULL) {
    return;
  }

  // first, move the crawlies we have and calculate density
  int i=0;
  int usedPixels = 0;
  while(i<_numSprites) {
    if(_sprites[i] != NULL && _sprites[i]->isAlive()) {
      // we've found a valid, live crawly, so move!
      _sprites[i]->moveCrawly();
      // add it to the density calculation
      _sprites[i]->

      i++;
    } else {
      // current location does not contain a valid, living
      // crawly, so move the crawly at the end of the list
      // to this location, decrement the size of the list
      // and try again.
      delete _sprites[i];
      _sprites[i] = _sprites[_numSprites-1];
      _sprites[_numSprites-1] == NULL;
      _numSprites--;
    }
  }

  // next, if there is space, consider making a new one.
  if(_numCrawlies < _maxNumCrawlies && jrand()%_spawnChance == 0) {
    spawnCrawly();
  }
}

/**
 * Inits data structures.
 */
void AcidRainManager::initData() {
  _bSetupFinished = false;
  _styles = NULL;
  _maxNumStyles = 0;
  _numStyles = 0;
  _crawlies = NULL;
  _numCrawlies = 0;
  _maxNumCrawlies = 0;

  growStyleList();
}

/**
 * Creates a new sprite from one of the palette styles available and
 * adds it to the list of live crawlies.
 */
void AcidRainManager::spawnSprite() {
  if(_sprites == NULL)
    return;
  if(_styles == NULL)
    return;

  // determine which style we are going to use
  int idx = jrand()%_numStyles;

  // determine starting position
  int startX = (jrand()%(_sizeX));

  // determine the length
  int minLength = _styles[idx].minLength;
  if(minLength <= 0) {
    minLength = _styles[idx].pal.getWidth();
  }
  int maxLength = _styles[idx].maxLength;
  if(maxLength <= 0) {
    maxLength = _styles[idx].pal.getWidth();
  }
  int length;
  if(minLength == maxLength) {
    length = minLength;
  } else {
    length = (jrand()%(maxLength-minLength))+minLength;
  }

  // determine sprite initial velocity  
  float minV = _styles[idx].minInitialV;
  float maxV = _styles[idx].maxInitialV;
  float spriteV;
  if(minV == maxV) {
    spriteV = minV;
  } else {
    spriteV = ((float)(jrand()%((int)((maxV-minV)*100.0))))/100.0;
  }

  FallingRainSprite* tmpSprite = 
    new FallingRainSprite(_sizeX, _sizeY, startX, _gravity,&_styles[idx].pal,
			  length, _styles[idx].thickness, spriteV,
			  _styles[idx].palSpeed, _palYOffset,
			  _styles[idx].bHeadConstantColor,
			  _styles[idx].bHeadRandomColor);

  // append to list of sprites
  if(_numSprites == _maxNumSprites) {
    growSpriteList(int size=RAIN_SPRITE_CHUNK_SIZE);
  }
  _sprites[_numSprites] = tmpSprite;
  if(_sprites[_numSprites] == NULL)
    return;
  _numSprites++;
  tmpSprite = NULL;
}

/**
 * Grows the list of crawly styles.
 */
void AcidRainManager::growStyleList(int size) {
  if(_styles == NULL) {
    _maxNumStyles = size;
    _styles = new rainsprite_style[_maxNumStyles];
    if(_styles == NULL) {
      _maxNumStyles = 0;
      return;
    }
    _numStyles = 0;
  } else {
    // create a temp list and copy
    rainsprite_style* tmpList = new rainsprite_style[_maxNumStyles+size];
    if(tmpList == NULL) {
      return;
    }
    for(int i=0;i<_numStyles;i++) {
      tmpList[i] = _styles[i];
    }
    delete [] _styles;
    _styles = tmpList;
    tmpList = NULL;
    _maxNumStyles += size;
  }
}

/**
 * Grows the list of sprites.
 */
void AcidRainManager::growSpriteList(int size) {
  if(_sprites == NULL) {
    _maxNumSprites = size;
    _sprites = new FallingRainSprite*[_maxNumSprites];
    if(_sprites == NULL) {
      _maxNumSprites = 0;
      return;
    }
    _numSprites = 0;
    // null out list
    for(int i=0; i<_maxNumSprites;i++) {
      _sprites[i] = NULL;
    }
  } else {
    // create a temp list and copy
    FallingRainSprite** tmpList = new FallingRainSprite*[_maxNumStyles+size];
    if(tmpList == NULL) {
      return;
    }
    int i;
    for(i=0;i<_numSprites;i++) {
      tmpList[i] = _sprites[i];
      _sprites[i] = NULL;
    }
    // null out rest of list
    for(i=_numSprites;i<_maxNumSprites;i++) {
      tmpList[i] = NULL;
    }
    delete [] _sprites;
    _sprites = tmpList;
    tmpList = NULL;
    _maxNumSprites += size;
  }
}


/**
 * Converts the palette from IndexedPalette* to int*
 */
void FallingRainSprite::convPalette(IndexedPalette* pal) {
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
