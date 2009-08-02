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
 * This algorithm will generate a rectangular spiral.
 */

#include "utility/misc_funcs.h"
#include "SpiralAlgorithm.h"

#ifndef __RECTANGULARSPIRALALGORITHM_H__
#define __RECTANGULARSPIRALALGORITHM_H__

enum RectangularSpiralAlgorithmDir { UP=0, DOWN=1, LEFT=2, RIGHT=3 };

class RectangularSpiralAlgorithm : public SpiralAlgorithm {

 public:

  /**
   * Default constructor.
   */
  RectangularSpiralAlgorithm();

  /**
   * Constructor takes initialization parameters.
   * @param colorWidth the width of the strip of color.
   * @param emptyWidth the width of the default color region.
   * @param bConstantColor whether or not to hold the color index constant.
   * @param bRandomColor whether or not the initial color is randomly selected.
   * @param bIncrementColor whether to increment the color as it spirals out.
   * @see calc()
   */
  RectangularSpiralAlgorithm(const int colorWidth, const int emptyWidth,
			     const bool bConstantColor, const bool bRandomColor,
			     const bool bIncrementColor);

  ~RectangularSpiralAlgorithm();

  /**
   * Initializes the algorithm.  Doesn't perform any actual generation.
   * @param field The array of integers in which to generate the spiral field.
   * @param screenWidth the width of the screen in pixels.
   * @param screenHeight the height of the screen in pixels.
   * @param numColors the number of colors in the palette primary direction.
   */
  void initialize(int* field, const int screenWidth, const int screenHeight,
		  const int numColors);

  /**
   * Completes one iteration of the algorithm.  Returns true if the algorithm
   * is completed.
   */
  bool calc();

 private:
  /**
   * Calculates both the corner and "straight away" segments.
   * These segments are _segmentSize x _segmentSize in size.
   * They are also oriented such that (0,0) is the bottom left corner.
   * _straightSeg is such that it flows from left to right, and
   * _cornerSeg is such that it flows from right to up if _bClockwise
   * is false, or right to down if _bClockwise is true.  Note that in
   * either case, the color index offset has not been properly modulo'd
   * and should be done so after being added to the current color index,
   * if _bConstantColor is false.  In the case where _bConstantColor is
   * true, the index offset will always be 0.
   */
  void calcSegments();

  /**
   * Rotates and draws the specified segment at (_currX,_currY) based
   * on _dir.
   * @param seg Must be either _cornerSeg or _straightSeg.
   */
  void drawSegment(int* seg);

  int _colorWidth;
  int _emptyWidth;
  bool _bConstantColor;
  bool _bRandomColor;
  bool _bIncrementColor;

  int _segmentSize;
  int* _cornerSeg;
  int* _straightSeg;

  RectangularSpiralAlgorithmDir _dir;
  bool _bClockwise;
  float _currX;
  float _currY;
  int _nextIdx;

  int _xSegs;  //> either current or next number of segments
  int _ySegs;  //> either current or next number of segments
  int _remainingSegs; //> Segments remaining to draw.
  int _xMaxSegs; //> Maximum number of segments in the x direction.
  int _yMaxSegs; //> Maximum number of segments in the y direction.

  // These are to speed up some calculations, since they only need done once.
  int _maxFieldIdx;   //> _sizeX * _sizeY - 1;
  float _halfSegSize; //> _segmentSize / 2.0

};

#endif