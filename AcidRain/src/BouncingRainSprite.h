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
 * This class represents the recoiling rain.  It is responsible for
 * moving and drawing the sprite.  The AcidRainManager class is responsible
 * for maintaining the sprites, however, FallingRainSprite will create
 * these sprites.  Uses base class _pX and _pY for position.
 */

#ifndef __BOUNCINGRAINSPRITE_H__
#define __BOUNCINGRAINSPRITE_H__

#include "RainSprite.h"

class BouncingRainSprite : public RainSprite {
 public:
  /**
   * Constructor.
   */
  BouncingRainSprite();

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
  BouncingRainSprite(int width, int height, float startX, float startY,
		     float gravity,
		     int** pal, int palWidth, int palHeight, int palIdx,
		     int thickness, float initialVX, float initialVY,
		     float palSpeed, int initialPalYOffset,
		     bool bHeadConstantColor);

  ~BouncingRainSprite();

  // begin base class virtual function redefinitions
  /**
   * Draws the sprite.
   * @param screenObj Screen buffer to draw to.
   */
  void drawSprite(screen_struct* screenObj);

  /**
   * Animate the sprite.
   *
   * @param horizontalAcceleration The acceleration due to wind.
   */
  void moveSprite(float horizontalAcceleration);
  // end base class redefinitions

 private:
  /**
   * Initializes the data structures.
   */
  void initSprite();

};

#endif
