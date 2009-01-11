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


#include "PlasmaFractal.h"

/**
 * Constructor takes the dimensions, a pointer to the palette,
 * the courseness, whether to rotate the palette,
 * how steps to go through between clock ticks,
 * the palette color skip and whether to clamp the color index or to wrap it.
 * If genStepsPerTick is 0, then it will all be done in one swoop.
 */
PlasmaFractal::PlasmaFractal(int sizeX, int sizeY, IndexedPalette* pal, float coarseness, int genStepsPerTick, bool bAnimatePalette, float paletteXSpeed, float paletteYSpeed, bool bClampColorIndex) {
	initIndexedPaletteBackground(sizeX,sizeY);
	setPalette(pal);
	_coarseness = coarseness;
	_bEnableAnimation = bAnimatePalette;
	_bColorClamp = bClampColorIndex;
	_numGenStepsPerTick = genStepsPerTick;
	_xSpeed = paletteXSpeed;
	_ySpeed = paletteYSpeed;

	_bSetupFinished = false;

	genPlasma();

}

PlasmaFractal::~PlasmaFractal() {
	destroyIndexedPaletteBackground();
}

/**
 * Finishes generating the fractal then rotates the palette when done.
 */
void PlasmaFractal::clocktick() {
	if(_paletteWidth > 0)
	{
		if(!_bSetupFinished)
		{
			plasmaGenStep();
		}
		else
		{
			rotateColorIndex();
		}
	}
}

/**
 * Generates a new Plasma Fractal
 */
void PlasmaFractal::genPlasma() {
	_algorithm.initialize(_field, _sizeX, _sizeY, _paletteWidth, _coarseness, _bColorClamp, false, false);
}
/**
 * Performs one iteration of the fractal generation.
 */
void PlasmaFractal::plasmaGenStep() {
	if(_algorithm.calc(_numGenStepsPerTick))
		_bSetupFinished = true;
}

