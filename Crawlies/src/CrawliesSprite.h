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
 * This class represents the worm sprite.  It is responsible for
 * moving and drawing the sprite.  The CrawliesManager class is responsible
 * for creating and maintaining the sprites.
 */

#ifndef __CRAWLIESSPRITE_H__
#define __CRAWLIESSPRITE_H__

#include "2d_bgnd_w_lens/globaldefs.h"
#include "IndexedPalette/IndexedPalette.h"

struct crawlies_tuple {
  int x, y;
};

enum crawlies_dir {
  LEFT=0, RIGHT=1, UP=2, DOWN=3, NONE=4
};

class CrawliesSprite {
 public:
  /**
   * Constructor.
   */
  CrawliesSprite();

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
  CrawliesSprite(int width, int height, int startX, int startY,
		 IndexedPalette* pal, int length, int thickness,
		 float spriteSpeed, float palSpeed, bool bHeadConstantColor,
		 bool bHeadRandomColor);

  ~CrawliesSprite();

  /**
   * Draws the sprite.
   * @param screenObj Screen buffer to draw to.
   */
  void drawCrawly(screen_struct* screenObj);

  /**
   * Animate the sprite.
   */
  void moveCrawly();

  /**
   * Whether the sprite is still "alive" or valid.
   *
   * @return True if alive.
   */
  bool isAlive();

 private:
  /**
   * Initializes the data structures.
   */
  void initSprite();

  /**
   * Converts the palette from IndexedPalette* to int*
   */
  void convPalette(IndexedPalette* pal);
 
  int _screenWidth;
  int _screenHeight;
  int* _pal;
  int _palWidth;
  int _palHeight;
  float _curStep;
  float _palYOffset;
  int _tailColorIdx;
  int _thickness;
  float _spriteSpeed, _palSpeed;
  crawlies_dir _lastDir;
  bool _bHeadConstantColor;
  bool _bAlive;
  int _numSegments;
  crawlies_tuple* _segments; //!< Worm segments.  Index 0 is tail!
};

#endif
