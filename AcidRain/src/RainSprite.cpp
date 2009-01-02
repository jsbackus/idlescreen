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

#include "RainSprite.h"

/**
 * Constructor.
 */
RainSprite::RainSprite() {
  initBaseSprite();
}

RainSprite::~RainSprite() {
  // we won't delete the palette because AcidRainManager is supposed to be
  // handling that.
  _pal == NULL;
}

/**
 * Draws the sprite.
 * @param screenObj Screen buffer to draw to.
 */
void RainSprite::drawSprite(screen_struct* screenObj) {
  // nada
}

/**
 * Animate the sprite.
 *
 * @param horizontalAcceleration The acceleration due to wind.
 */
void RainSprite::moveSprite(float horizontalAcceleration) {
  // nada
}

/**
 * Whether the sprite is still "alive" or valid.
 *
 * @return True if alive.
 */
bool RainSprite::isAlive() {
  // this value gets set by the derived class!
  return _bAlive;
}

/** 
 * Returns the number of pixels used by this sprite.  This
 * is used for the density calculations by the AcidRainManager.
 */
int RainSprite::getNumPixelsUsed() {
  // this value gets set by the derived class!
  return _numPixelsUsed;
}

/**
 * Initializes the data structures.
 */
void RainSprite::initBaseSprite() {
  _bAlive = false;
  _numPixelsUsed = 0;
  _screenWidth = 0;
  _screenHeight = 0;
  _pal = NULL;
  _palWidth = 0;
  _palHeight = 0;
  _palYOffset = 0.0;
  _colorIdx = 0;
  _thickness = 0;
  _vX = 0.0;
  _vY = 0.0;
  _pX = 0.0;
  _pY = 0.0;
  _gravity = 0.0;
  _palSpeed = 0.0;
  _bHeadConstantColor = true;
}
