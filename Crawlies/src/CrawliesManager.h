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
 * profile class is CrawliesBackgroundProfile.h.
 *
 * Note: it is important that the manager not place any worms beyond
 * corners!  i.e. at (-1,-1), (-1, height), (width, -1), (width, height).
 */

#include <stdlib.h>
#include <ctime>

using namespace std;

#ifndef __CRAWLIESMANAGER_H__
#define __CRAWLIESMANAGER_H__

#define CRAWLY_STYLE_CHUNK_SIZE 5

#include "2d_bgnd_w_lens/globaldefs.h"
#include "2d_bgnd_w_lens/Background.h"

#include "CrawliesSprite.h"

struct crawly_style {
  IndexedPalette pal;
  int minLength;
  int maxLength;
  float minSpriteSpeed;
  float maxSpriteSpeed;
  float palSpeed;
  bool bHeadConstantColor;
  bool bHeadRandomColor;
};

class CrawliesManager : public Background {

 public:

  /**
   * Generic constructor.
   */
  CrawliesManager();

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
  CrawliesManager(int sizeX, int sizeY, int maxCrawlies, int spawnChance);

  ~CrawliesManager();

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
   * @param minSpriteSpeed The minimum worm speed.
   * @param maxSpriteSpeed The maximum worm speed.
   * @param palSpeed The secondary palette rotation speed.
   * @param bHeadConstantColor Whether the head keeps the same color index.
   * @param bHeadRandomColor Whether the head color is random when created.
   */
  void addCrawliesStyle(IndexedPalette* pal, int minLength, int maxLength, 
			float minSpriteSpeed, float maxSpriteSpeed,
			float palSpeed, bool bHeadConstantColor, 
			bool bHeadRandomColor);

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
   * Creates a new sprite from one of the palette styles available and
   * adds it to the list of live sprites.
   */
  void spawnCrawly();

  /**
   * Checks for "life" and moves living crawlies.
   */
  void moveCrawlies();

  /**
   * Grows the list of crawly styles.
   */
  void growStyleList(int size=CRAWLY_STYLE_CHUNK_SIZE);

  crawly_style* _styles;
  int _numStyles;
  int _maxNumStyles;

  CrawliesSprite** _crawlies;
  int _numCrawlies;
  int _maxNumCrawlies;
  int _spawnChance;
};

#endif
