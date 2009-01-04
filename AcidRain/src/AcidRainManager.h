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
 * This class is responsible for creating and maintaining the worm
 * sprites, including drawing and moving them.  The corresponding
 * profile class is AcidRainBackgroundProfile.h.
 *
 * Note: it is important that the manager not place any worms beyond
 * corners!  i.e. at (-1,-1), (-1, height), (width, -1), (width, height).
 */

#include <stdlib.h>
#include <ctime>

using namespace std;

#ifndef __ACIDRAINMANAGER_H__
#define __ACIDRAINMANAGER_H__

#define RAIN_STYLE_CHUNK_SIZE 5
#define RAIN_SPRITE_CHUNK_SIZE 20

#include "2d_bgnd_w_lens/globaldefs.h"
#include "2d_bgnd_w_lens/Background.h"

#include "RainSprite.h"
#include "FallingRainSprite.h"

struct rainsprite_style {
  int* pal;
  int palWidth;
  int palHeight;
  int minLength;
  int maxLength;
  int thickness;
  float minInitialV;
  float maxInitialV;
  float palSpeed;
  bool bHeadConstantColor;
  bool bHeadRandomColor;
};

class AcidRainManager : public Background {

 public:

  /**
   * Generic constructor.
   */
  AcidRainManager();

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
  AcidRainManager(int sizeX, int sizeY, int maxDensity, float gravity,
		  float maxHorizontalAcceleration,
		  float maxHorizontalAccelerationDelta,
		  float recoilElasticity);

  ~AcidRainManager();

  /**
   * This procedure will add a new rain style to the list of styles
   * available for the sprites.  Must be called at least once before the
   * first call to clocktick()!
   *
   * For consistant sized sprites, minLength and maxLength should be set
   * equal.  To use the length of the palette for either quatity, set
   * equal to -1.
   * 
   * If the sprite initial speed is supposed to be constant, minInitialV 
   * and maxInitialV must be set equal.
   *
   * @param pal A pointer to the palette to associate with this style.
   * @param minLength The minimum sprite length.
   * @param maxLength the maximum sprite length.
   * @param thickness The thickness of the sprite.
   * @param minInitialV The minimum sprite initial velocity.
   * @param maxInitialV The maximum sprite initial velocity.
   * @param palSpeed The secondary palette rotation speed.
   * @param bHeadConstantColor Whether the head keeps the same color index.
   * @param bHeadRandomColor Whether the head color is random when created.
   */
  void addRainStyle(IndexedPalette* pal, int minLength, int maxLength, 
		    int thickness, float minInitialV,
		    float maxInitialV, float palSpeed,
		    bool bHeadConstantColor, bool bHeadRandomColor);

  /**
   * Sets the "setup finished" flag.
   *
   * @param Whether setup is finished.
   */
  void setFinished(bool bFinished);

  // *** Begin Extended Methods ***
  /**
   * Draws the whole Background.
   */
  void drawBackground(screen_struct* screenObj);

  /**
   * Finishes generating the fractal then rotates the palette when done.
   */
  void clocktick();
  // *** End Extended Methods ***

 private:
  /**
   * Inits data structures.
   */
  void initData();

  /**
   * Creates a new falling sprite from the styles available  and adds
   * it to the list.
   */
  void spawnSprite();

  /**
   * Grows the list of rain styles.
   */
  void growStyleList(int size=RAIN_STYLE_CHUNK_SIZE);

  /**
   * Grows the list of falling sprites.
   */
  void growFallingSpriteList(int size=RAIN_SPRITE_CHUNK_SIZE);

  /**
   * Grows the list of bouncing sprites.
   */
  void growBouncingSpriteList(int size=RAIN_SPRITE_CHUNK_SIZE);

  /**
   * Converts the palette from IndexedPalette* to int* and places
   * information into the specified style.
   */
  void convPalette(IndexedPalette* pal, rainsprite_style* style);

  rainsprite_style* _styles;
  int _numStyles;
  int _maxNumStyles;

  FallingRainSprite** _fallingSprites;
  int _numFallingSprites;
  int _maxNumFallingSprites;

  RainSprite** _bouncingSprites;
  int _numBouncingSprites;
  int _maxNumBouncingSprites;

  int _spawnChance;

  int _palYOffset;
  float _maxDensity;
  float _curDensity;
  float _screenArea;

  float _horizAccel;
  float _maxHorizAccel;
  float _maxHorizAccelDelta;
  float _recoilElasticity;
  float _gravity;
};

#endif
