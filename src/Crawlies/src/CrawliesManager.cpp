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
#include "CrawliesManager.h"

/**
 * Generic constructor.
 */
CrawliesManager::CrawliesManager() {
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
 * @param spawnChance There is (spawnChance in 100) chance for a spawn.
 * @param dirChangeChance Chance of changing direction between 0 & 100.
 */
CrawliesManager::CrawliesManager(int sizeX, int sizeY, int maxCrawlies,
				 int spawnChance, int dirChangeChance) {
  initData();
  initBackground(sizeX, sizeY);
  _maxNumCrawlies = maxCrawlies;
  _maxNumCrawliesFloat = (float) _maxNumCrawlies;
  _crawlies = new CrawliesSprite*[maxCrawlies];
  _numCrawlies = 0;
  _spawnChance = ((float)spawnChance)/100.0;
  _currentSpawnChance = spawnChance;
  _numStyles = 0;
  _bSetupFinished = true;
  _dirChangeChance = dirChangeChance;
}

CrawliesManager::~CrawliesManager() {
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
void CrawliesManager::addCrawliesStyle(IndexedPalette* pal, int minLength, 
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
void CrawliesManager::setFinished(bool bFinished) {
  _bSetupFinished = bFinished;
}

/**
 * Draws the whole Background.
 */
void CrawliesManager::drawBackground(screen_struct* screenObj) {
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
void CrawliesManager::clocktick() {
  if(!isSetupFinished()) {
    return;
  }


  // first, move the crawlies we have.
  moveCrawlies();

  // next, if there is space, consider making a new one.
  if((_numCrawlies < _maxNumCrawlies) && (jrand()%100 < _currentSpawnChance)) {
    spawnCrawly();
  }
}

/**
 * Inits data structures.
 */
void CrawliesManager::initData() {
  _bSetupFinished = false;
  _styles = NULL;
  _maxNumStyles = 0;
  _numStyles = 0;
  _crawlies = NULL;
  _numCrawlies = 0;
  _maxNumCrawlies = 0;
  _dirChangeChance = 0;

  growStyleList();
}

/**
 * Creates a new sprite from one of the palette styles available and
 * adds it to the list of live crawlies.
 */
void CrawliesManager::spawnCrawly() {
  if(_crawlies == NULL)
    return;
  if(_styles == NULL)
    return;

  // determine which style we are going to use
  int idx = jrand()%_numStyles;

  // determine starting position
  int startX = (jrand()%(_sizeX+2)) - 1;
  int startY = (jrand()%(_sizeY+2)) - 1;

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

  // determine sprite speed
  float minSpeed = _styles[idx].minSpriteSpeed;
  float maxSpeed = _styles[idx].maxSpriteSpeed;
  float spriteSpeed;
  if(minSpeed == maxSpeed) {
    spriteSpeed = minSpeed;
  } else {
    spriteSpeed = ((float)(jrand()%((int)((maxSpeed-minSpeed)*100.0))))/100.0;
  }

  CrawliesSprite* tmpCrawly = new CrawliesSprite(_sizeX, _sizeY, startX, 
						 startY,&_styles[idx].pal,
						 length, _styles[idx].thickness,
						 spriteSpeed,
						 _styles[idx].palSpeed,
						 _styles[idx].bHeadConstantColor,
						 _styles[idx].bHeadRandomColor,
						 _dirChangeChance);

  // append to list of crawlies
  _crawlies[_numCrawlies] = tmpCrawly;
  if(_crawlies[_numCrawlies] == NULL)
    return;
  _numCrawlies++;
  tmpCrawly = NULL;

  _currentSpawnChance = (int)( ((float) (_maxNumCrawlies - _numCrawlies)) /
			       _maxNumCrawliesFloat * _spawnChance +
			       _maxNumCrawliesFloat);
}

/**
 * Checks for "life" and moves living crawlies.
 */
void CrawliesManager::moveCrawlies() {
  if(_crawlies == NULL) {
    return;
  }

  int i=0;
  while(i<_numCrawlies) {
    if(_crawlies[i] != NULL && _crawlies[i]->isAlive()) {
      // we've found a valid, live crawly, so move!
      _crawlies[i]->moveCrawly();
      i++;
    } else {
      // current location does not contain a valid, living
      // crawly, so move the crawly at the end of the list
      // to this location, decrement the size of the list
      // and try again.
      delete _crawlies[i];
      _crawlies[i] = _crawlies[_numCrawlies-1];
      _numCrawlies--;
    }
  }
}

/**
 * Grows the list of crawly styles.
 */
void CrawliesManager::growStyleList(int size) {
  if(_styles == NULL) {
    _maxNumStyles = size;
    _styles = new crawly_style[_maxNumStyles];
    if(_styles == NULL) {
      _maxNumStyles = 0;
      return;
    }
    _numStyles = 0;
  } else {
    // create a temp list and copy
    crawly_style* tmpList = new crawly_style[_maxNumStyles+size];
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

