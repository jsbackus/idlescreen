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
 * This class represents the falling rain.  It is responsible for
 * moving and drawing the sprite.  The AcidRainManager class is responsible
 * for creating and maintaining the sprites.
 */

#ifndef __FALLINGRAINSPRITE_H__
#define __FALLINGRAINSPRITE_H__

#include "RainSprite.h"

struct recoil_sprite_node {
  RainSprite* sprite;
  recoil_sprite_node* next;
};

class FallingRainSprite : public RainSprite {
 public:
  /**
   * Constructor.
   */
  FallingRainSprite();

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
  FallingRainSprite(int width, int height, int startX, float gravity,
		    int* pal, int palWidth, int palHeight,
		    int length, int thickness,
		    float initialV, float palSpeed, int initialPalYOffset,
		    bool bHeadConstantColor, bool bHeadRandomColor,
		    float recoilElasticity);

  ~FallingRainSprite();

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

  /**
   * When this sprite starts to break apart on recoil, this will
   * return one such sprite on each call, or NULL if none are available.
   */
  RainSprite* getRecoilSprite();


 private:
  /**
   * Initializes the data structures.
   */
  void initSprite();

  int _numSegments;
  rainsprite_tuple* _segments; //!< Sprite segments.  Index 0 is tail!

  float _recoilElasticity;
  recoil_sprite_node* _recoilSprites;
};

#endif
