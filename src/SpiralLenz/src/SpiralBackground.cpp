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
SpiralBackground::SpiralBackground(int sizeX, int sizeY, 
				   SpiralAlgorithm* algorithm,
				   IndexedPalette* pal, bool bAnimatePalette,
				   float paletteXSpeed, float paletteYSpeed,
				   int genStepsPerTick) {

  // general defaults
  _algorithm = NULL;

  // base class specific
  initIndexedPaletteBackground(sizeX,sizeY);
  setPalette(pal);
  _bEnableAnimation = bAnimatePalette;
  _xSpeed = paletteXSpeed;
  _ySpeed = paletteYSpeed;
  _algorithm = algorithm;
  _numGenStepsPerTick = genStepsPerTick;

  _bSetupFinished = false;

  // initialize field to -1 (no color)
  if(_field != NULL) {
    for(int i=0;i<_sizeX*_sizeY;i++) {
      _field[i] = -1;
    }
  }

  // initialize the algorithm
  if(_algorithm != NULL) {
    _algorithm->initialize(_field, sizeX, sizeY, _paletteWidth);
  }
}

SpiralBackground::~SpiralBackground() {
  destroyIndexedPaletteBackground();
}

/**
 * Finishes generating the spiral then rotates the palette when done.
 */
void SpiralBackground::clocktick() {
  if(_paletteWidth > 0) {
    if(!_bSetupFinished && _algorithm != NULL) {
      if(_algorithm->calc(_numGenStepsPerTick))
	_bSetupFinished = true;
    } else {
      rotateColorIndex();
    }
  }
}

