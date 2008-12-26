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
 * This class generates a plasma fractal with the
 * specified palette.  The fractal generation
 * can be done in stages so that the user isn't
 * left staring at a blank screen for several minutes.
 * After the fractal is generated, it will rotate the
 * palette.
 */

#include <stdlib.h>
#include <ctime>

using namespace std;

#ifndef __PLASMAFRACTAL_H__
#define __PLASMAFRACTAL_H__

//#include "../../common/Plasma/PlasmaAlgorithm.h"
//#include "../../common/2d_bgnd_w_lens/globaldefs.h"
//#include "../../common/2d_bgnd_w_lens/IndexedPaletteBackground.h"

#include "plasma/PlasmaAlgorithm.h"
#include "2d_bgnd_w_lens/globaldefs.h"
#include "2d_bgnd_w_lens/IndexedPaletteBackground.h"

//12-26-08: Note-> Extend Background, not IndexedPaletteBackground.

class PlasmaFractal : public IndexedPaletteBackground {

public:
	/**
	 * Constructor takes the dimensions, a pointer to the palette,
	 * the courseness, whether to rotate the palette,
	 * how steps to go through between clock ticks,
	 * the palette color skip and whether to clamp the color index or to wrap it.
	 * If genStepsPerTick is 0, then it will all be done in one swoop.
	 */
	PlasmaFractal(int sizeX, int sizeY, IndexedPalette* pal, float coarseness, int genStepsPerTick, bool bAnimatePalette, float paletteXSpeed, float paletteYSpeed, bool bClampColorIndex);

	~PlasmaFractal();

	// *** Begin Extended Methods ***
	/**
	 * Finishes generating the fractal then rotates the palette when done.
	 */
	virtual void clocktick();
	// *** End Extended Methods ***

private:
	/**
	 * Generates a new Plasma Fractal
	 */
	void genPlasma();

	/**
	 * Performs one iteration of the fractal generation.
	 */
	void plasmaGenStep();

	float _coarseness;
	bool _bColorClamp;

	int _numGenStepsPerTick;

	PlasmaAlgorithm _algorithm;
	
};

#endif
