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
 * This algorithm generates a polar spiral.
 */

#include "utility/misc_funcs.h"
#include "SpiralAlgorithm.h"

#ifndef __POLARSPIRALALGORITHM_H__
#define __POLARSPIRALALGORITHM_H__

class PolarSpiralAlgorithm : public SpiralAlgorithm {

 public:

  /**
   * Default constructor.
   */
  PolarSpiralAlgorithm();

  /**
   * Constructor takes initialization parameters.
   * @param colorWidth the width of the strip of color.
   * @param emptyWidth the width of the default color region.
   * @param bConstantColor whether or not to hold the color index constant.
   * @param bRandomColor whether or not the initial color is randomly selected.
   * @param bIncrementColor whether to increment the color as it spirals out.
   * @see calc()
   */
  PolarSpiralAlgorithm(const int colorWidth, const int emptyWidth,
		      const bool bConstantColor, const bool bRandomColor,
		      const bool bIncrementColor);

  ~PolarSpiralAlgorithm();

  /**
   * Initializes the algorithm.  Doesn't perform any actual generation.
   * @param field The array of integers in which to generate the spiral field.
   * @param screenWidth the width of the screen in pixels.
   * @param screenHeight the height of the screen in pixels.
   * @param numColors the number of colors in the palette primary direction.
   */
  void initialize(int* field, const int screenWidth, 
		  const int screenHeight, const int numColors);

  /**
   * Completes one iteration of the algorithm.  Returns true if the algorithm
   * is completed.
   */
  bool calc();

 private:
  /**
   * Calculates a new X,Y using a polar spiral.  Utilizes an adaptive
   * algorithm to adjust thetaStep such that the new (x,y) value will
   * be adjacent but not on the previous (x,y) value.
   * @return True if the calculation is complete.
   */
  bool calcSpiral();

  // begin parameters
  int _colorWidth;
  int _emptyWidth;
  bool _bConstantColor;
  bool _bRandomColor;
  bool _bIncrementColor;
  // end parameters

  double _rCoeff; // accounts for clockwise/counterclockwise!
  double _currR;
  double _currTheta;
  double _lastThetaStep;
  double _maxR;   //!< polar stopping condition

  double _currX;
  double _currY;
  int _nextIdx;

  // begin constants to speed up calculations
  int _screenXOffset;
  int _screenYOffset;
  int _segmentWidth;
  int _maxFieldIdx;   //> _sizeX * _sizeY - 1;
  // end constants

  // begin debug
  int _stepCount;
};

#endif
