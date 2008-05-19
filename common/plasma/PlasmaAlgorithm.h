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
 * This class is used to generate a plasma fractal and fill the specified
 * buffer.  This algorithm is intended to be used with an indexed palette,
 * therefore the contents of the buffer are index numbers.  Data points
 * that have not been calculated yet are set to -1.  Assumes a zero-based
 * index.  The size of the field should be a power of 2.  The algorithm
 * will work for non-powers of 2, however the color for some regions may
 * not get calculated, leaving them "empty".
 *
 * The algorithm works by first picking four random colors that correspond
 * to the outside corners of the field:
 * (0,0), (width-1, 0), (0, height-1), (0, height-1)
 * The color at the midpoint of each of the four sides is calculated by
 * linearly interpolating between the two endpoints.  The center point of
 * the square region is calculated by averaging the midpoints of all
 * four sides and then a random perterbation is added to this average and
 * modified by the coarseness factor.  This then divides the original region
 * into four subregions.  The calculations are repeated for each subregion,
 * which in turn generates new subregions.  This process continues until the
 * corners are adjacent.
 *
 * It is possible to generate a plasma fractal that can be tiled
 * in the vertical and/or horizontal directions by passing the appropriate
 * values to the initialize method.  When tiling in the horizontal direction
 * the algorithm randomly picks colors for these positions:
 * (0,0), (width/2, 0), (0, height-1), (width/2, height-1)
 * Colors at positions (width-1, 0) and (width-1, height-1) are set equal
 * to the corresponding corners on the other side.  In otherwords:
 * C[width-1, 0] = C[0,0]
 * C[width-1, height-1] = C[0, height-1]
 * A similar method is used when tiling in the vertical direction.  When
 * tiling in both directions, colors are randomly picked for:
 * (0,0), (width/2, 0), (0, height/2), (width/2, height/2)
 * And then these color assignments are made:
 * C[width-1, 0] = C[0,0]
 * C[width-1, height/2] = C[0, height/2]
 * C[0, height-1] = C[0,0]
 * C[width/2, height-1] = C[width/2, 0]
 * C[width-1, height-1] = C[0,0]
 *
 * The algorithm is initiated with initialize() which will result in 1,
 * 2, or 4 subregions.  Successive calls to calc() must be made until
 * calc returns true.
 *
 * Revision History:
 * 080216	jsbackus	Initial Revision.
 */

#ifndef __PLASMAALGORITHM_H__
#define __PLASMAALGORITHM_H__

class PlasmaAlgorithm {

public:

	PlasmaAlgorithm();
	~PlasmaAlgorithm();

	/**
	 * This method initializes the algorithm.  It takes a pointer to the field to be filled,
	 * the dimensions of the field, the size of the palette, the coarseness factor, whether
	 * to clamp colors (as opposed to wrapping around), whether the plasma should be tiled
	 * in the horizontal direction, and whether the plasma should be tiled in the vertical
	 * direction.  Returns true if the algorithm was successfully initialized.
	 * NOTE: Expects field to already be allocated with sufficient space!
	 */
	bool initialize(int* field, int width, int height, int numColors, float coarseness,
					bool bClampColor, bool bTileHorizontal, bool bTileVertical);

	/**
	 * This method will complete one iteration of the algorithm.
	 */
	bool calc();

	/**
	 * Calculates up to numIterations iterations of the algorithm.  If N <= 0,
	 *  algorithm continues until finished.
	 */
	bool calc(int numIterations);

	/**
	 * This is an overloaded version of calc() that will block until the algorithm
	 * has completed the generation of the plasma fractal.
	 */
	bool calcAll();

private:
	// node for the linked list
	struct plasma_coord_node
	{
		long x1,y1, x2, y2;
		plasma_coord_node* next;
	};

	float _coarseness;
	bool _bClampColor;

	int* _field;
	int _height;
	int _width;

	int _numColors;

	plasma_coord_node* _genHead;
	plasma_coord_node* _genTail;

};

#endif
