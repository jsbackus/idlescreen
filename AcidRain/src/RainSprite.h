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

/**
 * This class represents a generic rain sprite.  It is a super class
 * of FallingRainSprite and BouncingRainSprite.
 */

#ifndef __RAINSPRITE_H__
#define __RAINSPRITE_H__

#include "2d_bgnd_w_lens/globaldefs.h"
#include "IndexedPalette/IndexedPalette.h"

struct rainsprite_tuple {
  int x, y;
};

class RainSprite {
 public:
  /**
   * Constructor.
   */
  RainSprite();
  virtual ~RainSprite();

  /**
   * Draws the sprite.
   * @param screenObj Screen buffer to draw to.
   */
  virtual void drawSprite(screen_struct* screenObj);

  /**
   * Animate the sprite.
   *
   * @param horizontalAcceleration The acceleration due to wind.
   */
  virtual void moveSprite(float horizontalAcceleration);

  /**
   * Whether the sprite is still "alive" or valid.
   *
   * @return True if alive.
   */
  bool isAlive();

  /** 
   * Returns the number of pixels used by this sprite.  This
   * is used for the density calculations by the AcidRainManager.
   */
  int getNumPixelsUsed();

 protected:
  /**
   * Initializes the data structures.
   */
  void initBaseSprite();
 
  int _screenWidth;
  int _screenHeight;
  int* _pal;  //!< Not created, but passed into the derived class and set!
  int _palWidth;
  int _palHeight;
  float _palYOffset;
  int _colorIdx;
  int _thickness;
  float _vX, _vY;
  float _pX, _pY;
  float _gravity;
  float _palSpeed;
  bool _bHeadConstantColor;

  bool _bAlive;
  int _numPixelsUsed;
};

#endif
