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


#include "SpiralBackground.h"

/**
 * Constructor takes the dimensions, a pointer to the palette,
 * whether to rotate the palette, and the palette color skip.
 */
SpiralBackground::SpiralBackground(int sizeX, int sizeY, IndexedPalette* pal,
				   bool bAnimatePalette, float paletteXSpeed,
				   float paletteYSpeed) {

  // base class specific
  initIndexedPaletteBackground(sizeX,sizeY);
  setPalette(pal);
  _bEnableAnimation = bAnimatePalette;
  _xSpeed = paletteXSpeed;
  _ySpeed = paletteYSpeed;

  _bSetupFinished = false;

  // general defaults
  _numGenStepsPerTick = 0;
  _bRandomColor = false;
  _algorithm = NULL;
  _style = SINGLE;

  // SINGLE style specific
  _bLerpToDefault = false;
  _singleWidth = 1;
  // end SINGLE style


  // initialize field to -1 (no color)
  for(int i=0;i<_height*_width;i++)
    {
      _field[i] = -1;
    }

}

SpiralBackground::~SpiralBackground() {
  destroyIndexedPaletteBackground();
}

/**
 * Initializes the generator algorithm to generate a single-color
 * spiral of the specified width, rotating through the primary palette
 * direction as it extends outward.  If bLerpToDefaultColor is true,
 * the palette color will be in the middle with, the default palette color
 * on the borders.  If bRandomColor is true, the first color index is 
 * randomly picked.
 * If genStepsPerTick is 0, then it will all be done in one swoop.
 */
void SpiralBackground::genSingleColorSpiral(SpiralBackgroundShape shape,
					    int genStepsPerTick, int width,
					    bool bRandomColor,
					    bool bLerpToDefaultColor) {
  _numGenStepsPerTick = genStepsPerTick;
  _bRandomColor = bRandomColor;

  _style = SINGLE;
  _singleWidth = width;
  _bLerpToDefaultColor = bLerpToDefaultColor;

  //_algorithm =
  //_shape = shape;
}

/**
 * Initializes the generator algorithm to generate a multi-color spiral,
 * that remains constant through the whole spiral.  The width is the
 * width (primary axis) of the palette.  If bRandomColor is true,
 * the initial index is randomly picked and the palette will be wrapped.
 * If genStepsPerTick is 0, then it will all be done in one swoop.
 */
void SpiralBackground::genMultiColorSpiral(SpiralBackgroundShape shape,
					   int genStepsPerTick, 
					   bool bRandomColor) {
  _numGenStepsPerTick = genStepsPerTick;
  _bRandomColor = bRandomColor;

  _style = MULTIWIDTH;

  //_shape = shape;
  //_algorithm = 
}

/**
 * Finishes generating the spiral then rotates the palette when done.
 */
void SpiralBackground::clocktick() {
  if(_paletteWidth > 0)
    {
      if(!_bSetupFinished)
	{
	  spiralGenStep();
	}
      else
	{
	  rotateColorIndex();
	}
    }
}

/**
 * Performs one iteration of the fractal generation.
 */
void SpiralBackground::spiralGenStep() {
  if(_algorithm.calc(_numGenStepsPerTick))
    _bSetupFinished = true;
}

