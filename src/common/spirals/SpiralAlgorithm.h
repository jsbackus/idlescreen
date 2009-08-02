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
 * This is the base class for the spiral algorithms.
 */

#ifndef __SPIRALALGORITHM_H__
#define __SPIRALALGORITHM_H__

#ifdef _M_IX86
#include <windows.h>
#else
//#include <stream.h>
#include <stdlib.h>
#endif

#define SPIRALALGORITHM_PI 3.14159265f

class SpiralAlgorithm {

 public:

  /**
   * Constructor.
   */
  SpiralAlgorithm();

  virtual ~SpiralAlgorithm();

  /**
   * Initializes the algorithm.  Doesn't perform any actual generation.
   * @param field The array of integers in which to generate the spiral field.
   * @param screenWidth the width of the screen in pixels.
   * @param screenHeight the height of the screen in pixels.
   * @param numColors the number of colors in the palette primary direction.
   * @see calc()
   */
  virtual void initialize(int* field, const int screenWidth, 
			  const int screenHeight, const int numColors);

  /**
   * Completes one iteration of the algorithm.  Returns true if the algorithm
   * is completed.
   */
  virtual bool calc();

  /**
   * Completes up to numIterations iterations of the algorithm.  If N <= 0,
   * the algorithm will continue until finished.
   * @param numIterations the number of iterations to perform.
   * @return true if the spiral generation is complete.
   * @see calc();
   */
  bool calc(const int numIterations);

  /**
   * This is a convenience function that is the same as calling calc(-1).
   * @see calc();
   */
  bool calcAll();

 protected:
  /**
   * Initializes the base algorithm.  Doesn't perform any actual generation.
   * @param field The array of integers in which to generate the spiral field.
   * @param screenWidth the width of the screen in pixels.
   * @param screenHeight the height of the screen in pixels.
   * @param numColors the number of colors in the palette primary direction.
   * @param bIncrementColor whether the colors increment as they spiral out.
   * @see initialize()
   */
  void initializeBase(int* field, const int screenWidth, const int screenHeight,
		      const int numColors);

  int _width;
  int _height;
  int _numColors;
  int* _field;
};

#endif
