/**
 * Copyright (c) 2009 Jeff Backus.
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
 * This class generates a spiral background.  After generation,
 * it will rotate the palette with the specified primary and
 * secondary speeds.
 */

//#include <stdlib.h>
//#include <ctime>

//using namespace std;

#ifndef __SPIRALBACKGROUND_H__
#define __SPIRALBACKGROUND_H__

#include "2d_bgnd_w_lens/globaldefs.h"
#include "2d_bgnd_w_lens/IndexedPaletteBackground.h"

#include "spirals/SpiralAlgorithm.h"

class SpiralBackground : public IndexedPaletteBackground {

 public:
  /**
   * Constructor takes the dimensions, a pointer to the algorithm,
   * a pointer to the palette, whether to rotate the palette, 
   * and the palette color skip.  This class takes responsibility for
   * destroying both the algorithm and the palette.
   * @param sizeX The width of the background.
   * @param sizeY The height of the background.
   * @param algorithm A pointer to the spiral algorithm.
   * @param pal A pointer to the palette.
   * @param bAnimatePalette Whether or not to animate the palette.
   * @param paletteXSpeed The palette rotational speed in the primary direction.
   * @param paletteYSpeed The palette rotational speed in the secondary dir.
   * @param genStepsPerTick The number of generation steps per tick.
   */
  SpiralBackground(int sizeX, int sizeY, SpiralAlgorithm* algorithm,
		   IndexedPalette* pal, bool bAnimatePalette,
		   float paletteXSpeed, float paletteYSpeed,
		   int genStepsPerTick);

  ~SpiralBackground();

  // *** Begin Extended Methods ***
  /**
   * Finishes generating the spiral then rotates the palette when done.
   */
  void clocktick();
  // *** End Extended Methods ***

 private:
  int _numGenStepsPerTick;

  SpiralAlgorithm* _algorithm;
};

#endif
