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
 * This is a generic N-gon spiral algorithm.
 */

#include "utility/Vector2D.h"
#include "utility/Point2D.h"
#include "utility/misc_funcs.h"
#include "SpiralAlgorithm.h"

#ifndef __NGONSPIRALALGORITHM_H__
#define __NGONSPIRALALGORITHM_H__

#define NGONSPIRALALGORITHM_STEPSIZE 0.5

class NgonSpiralAlgorithm : public SpiralAlgorithm {

 public:

  /**
   * Default constructor.
   */
  NgonSpiralAlgorithm();

  /**
   * Constructor takes initialization parameters.
   * @param N the number of sides the Ngon has.  N=0 for a polar spiral.
   * @param colorWidth the width of the strip of color.
   * @param emptyWidth the width of the default color region.
   * @param bConstantColor whether or not to hold the color index constant.
   * @param bRandomColor whether or not the initial color is randomly selected.
   * @param bIncrementColor whether to increment the color as it spirals out.
   * @see calc()
   */
  NgonSpiralAlgorithm(const int N, const int colorWidth, const int emptyWidth,
		      const bool bConstantColor, const bool bRandomColor,
		      const bool bIncrementColor);

  ~NgonSpiralAlgorithm();

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
   * has completed.
   */
  bool calc();

 private:
  /**
   * Draws a segment from pointA to pointB.  Returns true if any indices were
   * set within _field.
   */
  bool drawSegment(const Point2D& pointA, const Point2D& pointB);

  /**
   * Updates the color count by the specified amount.
   */
  void updateColorCount(const double amnt);

  /**
   * Sets the specified coordinates to the current color index.  Returns
   * true if a pixel was actually set.
   */
  bool setPixel(const Point2D& p);
  
  // begin parameters
  int _n;
  int _colorWidth;
  double _colorWidthDbl;
  int _emptyWidth;
  bool _bConstantColor;
  bool _bRandomColor;
  bool _bIncrementColor;
  int _maxFieldIdx;   //> _sizeX * _sizeY - 1;
  // end parameters

  // begin state data
  Point2D _origin;
  Point2D _lastPoint;
  Vector2D* _axes;
  Vector2D* _normals;
  int _curAxis;
  double _ringNum;
  int _ringCounter;
  double _lineWidth;
  double _growLength;
  double _bevelAngle;
  double _bevelLength;
  double _curColorCount;
  int _curColorIdx;

  /*
  // begin old...
  double _currX;
  double _currY;
  int _nextIdx;
  */

  // begin constants to speed up calculations
  int _screenXOffset;
  int _screenYOffset;
  int _segmentWidth;
  // end constants

  // end old...

  // begin debug
  int _stepCount;
  
#ifdef BLARGH
  // begin spiral specific
  double _rCoeff; // accounts for clockwise/counterclockwise!
  double _currR;
  double _currTheta;
  double _thetaStep;
  double _maxR;   //!< polar stopping condition
  // end spiral specific

  /**
   * Calculates a new X,Y using a polar spiral.  Utilizes an adaptive
   * algorithm to adjust thetaStep such that the new (x,y) value will
   * be adjacent but not on the previous (x,y) value.
   * @param x the previous X value.  On return, it is the new X value.
   * @param y the previous Y value.  On return, it is the new Y value.
   * @param r the previous R value.  On return, it is the new R value.
   * @param theta the previous Theta value.  It is updated on return.
   * @param thetaStep the previous Theta step value.  Updated on return.
   * @return True if the calculation is complete.
   */
  bool calcSpiral(double* x, double* y, double* r, double* theta, 
		  double* thetaStep);

  /*
  int* _cornerSeg;
  int* _straightSeg;

  NgonSpiralAlgorithmDir _dir;
  bool _bClockwise;

  int _xSegs;  //> either current or next number of segments
  int _ySegs;  //> either current or next number of segments
  int _remainingSegs; //> Segments remaining to draw.
  int _xMaxSegs; //> Maximum number of segments in the x direction.
  int _yMaxSegs; //> Maximum number of segments in the y direction.

  // These are to speed up some calculations, since they only need done once.
  float _halfSegSize; //> _segmentSize / 2.0
  */
#endif
};

#endif
