#include "SpiralAlgorithm.h"

/**
 * Constructor.
 */
SpiralAlgorithm::SpiralAlgorithm() {
  _field = NULL;
  _width = 0;
  _height = 0;
  _numColors = 0;
}

SpiralAlgorithm::~SpiralAlgorithm() {
  // someone else is responsible for destroying _field
  _field = NULL;
}

  /**
   * Initializes the algorithm.  Doesn't perform any actual generation.
   * @param field The array of integers in which to generate the spiral field.
   * @param screenWidth the width of the screen in pixels.
   * @param screenHeight the height of the screen in pixels.
   * @param numColors the number of colors in the palette primary direction.
   * @see calc()
   */
void SpiralAlgorithm::initialize(int* field, const int screenWidth, 
				 const int screenHeight,
				 const int numColors) {

  initializeBase(field, screenWidth, screenHeight, numColors);
}

/**
 * Completes one iteration of the algorithm.  Returns true if the algorithm
 * is completed.
 */
bool SpiralAlgorithm::calc() {
  // base does nada.
  return true;
}

/**
 * Completes up to numIterations iterations of the algorithm.  If N <= 0,
 * the algorithm will continue until finished.
 */
bool SpiralAlgorithm::calc(const int numIterations) {
	for(int i=0; i<numIterations; i++) {
		if(calc())
			return true;
	}
	return false;  
}

/**
 * This is a convenience function that is the same as calling calc(-1).
 */
bool SpiralAlgorithm::calcAll() {
  return calc(-1);
}

  /**
   * Initializes the base algorithm.  Doesn't perform any actual generation.
   * @param field The array of integers in which to generate the spiral field.
   * @param screenWidth the width of the screen in pixels.
   * @param screenHeight the height of the screen in pixels.
   * @param numColors the number of colors in the palette primary direction.
   * @see initialize()
   */
void SpiralAlgorithm::initializeBase(int* field, const int screenWidth,
				     const int screenHeight, 
				     const int numColors) {
  _field = field;
  _width = screenWidth;
  _height = screenHeight;
  _numColors = numColors;
}

