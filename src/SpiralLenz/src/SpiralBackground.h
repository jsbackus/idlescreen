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

#include "SpiralBackgroundAlgorithm.h"
#include "RectangularSpiral.h"

enum SpiralBackgroundShape {
  POLAR, RECTANGULAR, DIAMOND, TRIANGULAR
};

enum SpiralBackgroundSytle {
  SINGLE, MULTIWIDTH
};

class SpiralBackground : public IndexedPaletteBackground {

 public:
  /**
   * Constructor takes the dimensions, a pointer to the palette,
   * whether to rotate the palette, and the palette color skip.
   */
  SpiralBackground(int sizeX, int sizeY, IndexedPalette* pal,
		   bool bAnimatePalette, float paletteXSpeed,
		   float paletteYSpeed);

  ~SpiralBackground();

  /**
   * Initializes the generator algorithm to generate a single-color
   * spiral of the specified width, rotating through the primary palette
   * direction as it extends outward.  If bLerpToDefaultColor is true,
   * the palette color will be in the middle with, the default palette color
   * on the borders.  If bRandomColor is true, the first color index is 
   * randomly picked.
   * If genStepsPerTick is 0, then it will all be done in one swoop.
   */
  void genSingleColorSpiral(int genStepsPerTick, int width,
			     bool bRandomColor, bool bLerpToDefaultColor);

  /**
   * Initializes the generator algorithm to generate a multi-color spiral,
   * that remains constant through the whole spiral.  The width is the
   * width (primary axis) of the palette.  If bRandomColor is true,
   * the initial index is randomly picked and the palette will be wrapped.
   * If genStepsPerTick is 0, then it will all be done in one swoop.
   */
  void genMultiColorSpiral(int genStepsPerTick, bool bRandomColor);

  // *** Begin Extended Methods ***
  /**
   * Finishes generating the spiral then rotates the palette when done.
   */
  virtual void clocktick();
  // *** End Extended Methods ***

 private:
  /**
   * Performs one iteration of the fractal generation.
   */
  void spiralGenStep();

  int _numGenStepsPerTick;
  bool _bRandomColor;

  // specific to SINGLE style
  bool _bLerpToDefaultColor;
  int _singleWidth;
  // end specific to SINGLE style

  SpiralBackgroundShapeAlgorithm* _algorithm;
  SpiralBackgroundStyle _style;
};

#endif
