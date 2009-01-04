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
#include <iostream>
using namespace std;
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
 * see addRainStyle().
 *
 * @param sizeX The width of the screen in pixels.
 * @param sizeY The height of the screen in pixels.
 * @param maxDensity The maximum rain density allowed on the screen as %.
 * @param gravity The "downward" acceleration to use.  Must be positive!
 * @param maxHorizontalAcceleration The maximum "wind" acceleration
 * @param maxHorizontalAcclerationDelta The maximum change in "wind" accel.
 * @param recoilElasticity The amount of "bounce" the rain has.
 */
AcidRainManager::AcidRainManager(int sizeX, int sizeY, int maxDensity,
				 float gravity,
				 float maxHorizontalAcceleration,
				 float maxHorizontalAccelerationDelta,
				 float recoilElasticity) {
  initData();
  initBackground(sizeX, sizeY);

  _screenArea = ((float)sizeX)*((float)sizeY);

  _maxDensity = ((float)maxDensity)/100.0;
  _gravity = gravity;
  _maxHorizAccel = maxHorizontalAcceleration;
  _maxHorizAccelDelta = maxHorizontalAccelerationDelta;
  _recoilElasticity = recoilElasticity;

  _bSetupFinished = true; //false;
}

AcidRainManager::~AcidRainManager() {
  if(_styles != NULL) {
    for(int i=0; i<_numStyles;i++) {
      delete [] _styles[i].pal;
      _styles[i].pal = NULL;
    }
    delete [] _styles;
    _styles = NULL;
  }
  if(_fallingSprites != NULL) {
    for(int i=0; i<_numFallingSprites;i++) {
      delete _fallingSprites[i];
      _fallingSprites[i] = NULL;
    }
    delete [] _fallingSprites;
    _fallingSprites = NULL;
  }
  if(_bouncingSprites != NULL) {
    for(int i=0; i<_numBouncingSprites;i++) {
      delete _bouncingSprites[i];
      _bouncingSprites[i] = NULL;
    }
    delete [] _bouncingSprites;
    _bouncingSprites = NULL;
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
 * @param minInitialV The minimum sprite initial velocity.
 * @param maxInitialV The maximum sprite initial velocity.
 * @param palSpeed The secondary palette rotation speed.
 * @param bHeadConstantColor Whether the head keeps the same color index.
 * @param bHeadRandomColor Whether the head color is random when created.
 */
void AcidRainManager::addRainStyle(IndexedPalette* pal, int minLength, 
				   int maxLength, 
				   int thickness,
				   float minInitialV,
				   float maxInitialV,
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

  // convert palette
  convPalette(pal, &_styles[_numStyles]);

  // copy other parameters
  _styles[_numStyles].minLength = minLength;
  _styles[_numStyles].maxLength = maxLength;
  _styles[_numStyles].thickness = thickness;
  _styles[_numStyles].minInitialV = minInitialV;
  _styles[_numStyles].maxInitialV = maxInitialV;
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
  if(_fallingSprites == NULL) {
    return;
  }
  if(_bouncingSprites == NULL) {
    return;
  }

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

  // go through the list of falling sprites
  if(_fallingSprites != NULL) {
    for(int i=0; i<_numFallingSprites; i++) {
      if(_fallingSprites[i] != NULL && _fallingSprites[i]->isAlive()) {
	// we've found a valid, live crawly, so render!
	_fallingSprites[i]->drawSprite(screenObj);
      }
    }
  }

  // go through the list of other sprites
  if(_bouncingSprites != NULL) {
    for(int i=0; i<_numBouncingSprites; i++) {
      if(_bouncingSprites[i] != NULL && _bouncingSprites[i]->isAlive()) {
	// we've found a valid, live crawly, so render!
	_bouncingSprites[i]->drawSprite(screenObj);
      }
    }
  }
}

/**
 * Animates the sprites and calculates density.
 */
void AcidRainManager::clocktick() {
  if(!isSetupFinished()) {
    return;
  }
  if(_fallingSprites == NULL) {
    return;
  }
  if(_bouncingSprites == NULL) {
    return;
  }

  // adjust horizontal acceleration
  float accelDelta = ((float)((jrand()%2000)-1000))*
    (_maxHorizAccelDelta/1000.0);
  if(accelDelta + _horizAccel > 1.0*_maxHorizAccel) {
    _horizAccel -= accelDelta;
  } else if(accelDelta + _horizAccel < -1.0*_maxHorizAccel) {
    _horizAccel -= accelDelta;
  } else {
    _horizAccel += accelDelta;
  }

  // move the falling sprites we have and calculate their density
  int i=0;
  int usedPixels = 0;

  while(i<_numFallingSprites) {
    if(_fallingSprites[i] != NULL && _fallingSprites[i]->isAlive()) {
      // we've found a valid, live sprite, so move!
      _fallingSprites[i]->moveSprite(_horizAccel);
      // add it to the density calculation
      usedPixels += _fallingSprites[i]->getNumPixelsUsed();
      // check to see if it throws off any bouncing sprites
      RainSprite* tmpSprite = _fallingSprites[i]->getRecoilSprite();
      while(tmpSprite != NULL) {
	if(_numBouncingSprites == _maxNumBouncingSprites) {
	  growBouncingSpriteList();
	}
	_bouncingSprites[_numBouncingSprites] = tmpSprite;
	_numBouncingSprites++;
	tmpSprite = _fallingSprites[i]->getRecoilSprite();
      }
      i++;
    } else {
      // current location does not contain a valid, living
      // crawly, so move the crawly at the end of the list
      // to this location, decrement the size of the list
      // and try again.
      delete _fallingSprites[i];
      _fallingSprites[i] = _fallingSprites[_numFallingSprites-1];
      _fallingSprites[_numFallingSprites-1] = NULL;
      _numFallingSprites--;
    }
  }

  // now animate the bouncing sprites (if any)
  i = 0;
  while(i<_numBouncingSprites) {
    if(_bouncingSprites[i] != NULL && _bouncingSprites[i]->isAlive()) {
      // we've found a valid, live sprite, so move!
      _bouncingSprites[i]->moveSprite(_horizAccel);
      // add it to the density calculation
      usedPixels += _bouncingSprites[i]->getNumPixelsUsed();
      i++;
    } else {
      // current location does not contain a valid, living
      // crawly, so move the crawly at the end of the list
      // to this location, decrement the size of the list
      // and try again.
      delete _bouncingSprites[i];
      _bouncingSprites[i] = _bouncingSprites[_numBouncingSprites-1];
      _bouncingSprites[_numBouncingSprites-1] = NULL;
      _numBouncingSprites--;
    }
  }

  // next, if there is space, consider making a new one.  To do so,
  // subtract the current density from the max density and multiply
  // by 100.  Now, take a random number between 0 and 100.  If that
  // random number is less than the difference in densities * 100,
  // then spawn.
  /*
  _curDensity = ((float)usedPixels)/_screenArea;
  cout<<"cur density: "<<_curDensity<<" of "<<_maxDensity
      <<" -> "<<(1.0-_curDensity)/_maxDensity<<endl;
  if(_curDensity < _maxDensity) {
    //int spawnChance = (int)((_maxDensity - _curDensity)*200.0);
    int spawnChance = (int)( 100.0*(1.0-_curDensity)/_maxDensity );
    cout<<"spawn chance: "<<spawnChance<<endl;
    //if(jrand()%101 < spawnChance) {
    while(jrand()%101 < spawnChance) {
      cout<<"spawned!"<<endl;
      spawnSprite();
    }
  }
  */
  // better:
  /*
  _curDensity = ((float)usedPixels)/_screenArea;
  int spawnChance = (int)( 100.0*(1.0-_curDensity)/_maxDensity );
  //int spawnChance = (int)((_maxDensity - _curDensity)*200.0);
  //cout<<"cur density: "<<_curDensity<<" of "<<_maxDensity
  //    <<" -> "<<(1.0-_curDensity)/_maxDensity<<endl;
  while(_curDensity < _maxDensity && jrand()%101 < spawnChance) {
    //cout<<"cur density: "<<_curDensity<<" of "<<_maxDensity
    //	<<" -> "<<(1.0-_curDensity)/_maxDensity<<endl;
    //int spawnChance = (int)((_maxDensity - _curDensity)*200.0);
    //cout<<"spawn chance: "<<spawnChance<<endl;
    //if(jrand()%101 < spawnChance) {
    //while(jrand()%101 < spawnChance) {
    //cout<<"spawned!"<<endl;
    usedPixels += spawnSprite();
    _curDensity = ((float)usedPixels)/_screenArea;
    spawnChance = (int)( 100.0*(1.0-_curDensity)/_maxDensity );
    //spawnChance = (int)((_maxDensity - _curDensity)*200.0);
  }
  */
  i=0; //!< simple escape
  while(shouldSpawn(usedPixels) && i < _sizeX*2) {
    i++;
    usedPixels+= spawnSprite();
  }
}

/**
 * Recalculates the current screen density and decides whether
 * or not to spawn based upon that density.
 */
bool AcidRainManager::shouldSpawn(int usedPixels) {
  _curDensity = ((float)usedPixels)/_screenArea;
  int spawnChance = (int)( 100.0*(_maxDensity-_curDensity)/_maxDensity );
  //cout<<"cur density: "<<_curDensity<<" of "<<_maxDensity
  //    <<" -> "<<spawnChance<<endl;
  return (_curDensity < _maxDensity && jrand()%100 <= spawnChance);
}


/**
 * Inits data structures.
 */
void AcidRainManager::initData() {
  _bSetupFinished = false;
  _styles = NULL;
  _maxNumStyles = 0;
  _numStyles = 0;
  _fallingSprites = NULL;
  _maxNumFallingSprites = 0;
  _numFallingSprites = 0;
  _bouncingSprites = NULL;
  _maxNumBouncingSprites = 0;
  _numBouncingSprites = 0;
  _maxDensity = 0.0;
  _curDensity = 0.0;
  _screenArea = 0.0;
  _horizAccel = 0.0;
  _maxHorizAccel = 0.0;
  _maxHorizAccelDelta = 0.0;
  _recoilElasticity = 0.0;
  _palYOffset = 0;

  growStyleList();
  growFallingSpriteList();
  growBouncingSpriteList();
}

/**
 * Creates a new falling sprite from the styles available  and adds
 * it to the list.
 * Returns the number of pixels the new sprite uses.
 */
int AcidRainManager::spawnSprite() {
  //begin debug
  //if(_numFallingSprites >= 1)
  //return;
  //end debug
  if(_fallingSprites == NULL)
    return 0;
  if(_styles == NULL)
    return 0;

  // determine which style we are going to use
  int idx = jrand()%_numStyles;

  // determine starting position
  int startX = (jrand()%(_sizeX));

  // determine the length
  int minLength = _styles[idx].minLength;
  if(minLength <= 0) {
    minLength = _styles[idx].palWidth;
  }
  int maxLength = _styles[idx].maxLength;
  if(maxLength <= 0) {
    maxLength = _styles[idx].palWidth;
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
    new FallingRainSprite(_sizeX, _sizeY, startX, _gravity,_styles[idx].pal,
			  _styles[idx].palWidth, _styles[idx].palHeight, 
			  length, _styles[idx].thickness, spriteV,
			  _styles[idx].palSpeed, _palYOffset,
			  _styles[idx].bHeadConstantColor,
			  _styles[idx].bHeadRandomColor,
			  _recoilElasticity);

  // append to list of sprites
  if(_numFallingSprites == _maxNumFallingSprites) {
    growFallingSpriteList();
  }
  _fallingSprites[_numFallingSprites] = tmpSprite;
  if(_fallingSprites[_numFallingSprites] == NULL)
    return 0;
  _numFallingSprites++;
  int retVal = tmpSprite->getNumPixelsUsed();
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
 * Grows the list of falling sprites.
 */
void AcidRainManager::growFallingSpriteList(int size) {
  if(_fallingSprites == NULL) {
    _maxNumFallingSprites = size;
    _fallingSprites = new FallingRainSprite*[_maxNumFallingSprites];
    if(_fallingSprites == NULL) {
      _maxNumFallingSprites = 0;
      return;
    }
    _numFallingSprites = 0;
    // null out list
    for(int i=0; i<_maxNumFallingSprites;i++) {
      _fallingSprites[i] = NULL;
    }
  } else {
    // create a temp list and copy
    FallingRainSprite** tmpList = 
      new FallingRainSprite*[_maxNumFallingSprites+size];
    if(tmpList == NULL) {
      return;
    }
    int i;
    for(i=0;i<_numFallingSprites;i++) {
      tmpList[i] = _fallingSprites[i];
      _fallingSprites[i] = NULL;
    }
    // null out rest of list
    for(i=_numFallingSprites;i<_maxNumFallingSprites;i++) {
      tmpList[i] = NULL;
    }
    delete [] _fallingSprites;
    _fallingSprites = tmpList;
    tmpList = NULL;
    _maxNumFallingSprites += size;
  }
}

/**
 * Grows the list of sprites.
 */
void AcidRainManager::growBouncingSpriteList(int size) {
  if(_bouncingSprites == NULL) {
    _maxNumBouncingSprites = size;
    _bouncingSprites = new RainSprite*[_maxNumBouncingSprites];
    if(_bouncingSprites == NULL) {
      _maxNumBouncingSprites = 0;
      return;
    }
    _numBouncingSprites = 0;
    // null out list
    for(int i=0; i<_maxNumBouncingSprites;i++) {
      _bouncingSprites[i] = NULL;
    }
  } else {
    // create a temp list and copy
    RainSprite** tmpList = new RainSprite*[_maxNumBouncingSprites+size];
    if(tmpList == NULL) {
      return;
    }
    int i;
    for(i=0;i<_numBouncingSprites;i++) {
      tmpList[i] = _bouncingSprites[i];
      _bouncingSprites[i] = NULL;
    }
    // null out rest of list
    for(i=_numBouncingSprites;i<_maxNumBouncingSprites;i++) {
      tmpList[i] = NULL;
    }
    delete [] _bouncingSprites;
    _bouncingSprites = tmpList;
    tmpList = NULL;
    _maxNumBouncingSprites += size;
  }
}


/**
 * Converts the palette from IndexedPalette* to int* and places
 * information into the specified style.
 */
void AcidRainManager::convPalette(IndexedPalette* pal, 
				  rainsprite_style* style) {
  if(pal == NULL)
    return;
  if(style == NULL)
    return;

  style->palWidth = pal->getWidth();
  style->palHeight = pal->getHeight();

  if(style->palWidth <= 0 || style->palHeight <= 0)
    return;

  // create space for the 2D palette
  int* tmpPal = new int[(style->palWidth*style->palHeight)*4];
  if(tmpPal == NULL)
    return;
  style->pal = tmpPal;


  // copy palette
  GLubyte buff[4];
  for(int y=0; y<style->palHeight; y++) {
    for(int x=0; x<style->palWidth; x++) {
      pal->getColor(x,y,&buff[0], 4);
      for(int j=0; j<4;j++) {
	style->pal[(x+y*style->palWidth)*4+j] = buff[j];
      }
    }
  }
}
