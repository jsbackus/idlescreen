
// debug
#include <iostream>
// end debug
#include <math.h>
using namespace std;

#include "PolarSpiralAlgorithm.h"

/**
 * Constructor.
 */
PolarSpiralAlgorithm::PolarSpiralAlgorithm() {

  // parameters
  _colorWidth = 1;
  _emptyWidth = 0;
  _bConstantColor = true;
  _bRandomColor = false;
  _bIncrementColor = true;

  // internals
  _nextIdx = 0;
  _currX = 0.0;
  _currY = 0.0;
  _rCoeff = 0.0;
  _currR = 0.0;
  _currTheta = 0.0;
  _maxR = 0.0;

  // misc constants
  _maxFieldIdx = 0;
  _screenXOffset = 0;
  _screenYOffset = 0;
}

  /**
   * Constructor takes initialization parameters.
   * @param colorWidth the width of the strip of color.
   * @param emptyWidth the width of the default color region.
   * @param bConstantColor whether or not to hold the color index constant.
   * @param bRandomColor whether or not the initial color is randomly selected.
   * @param bIncrementColor whether to increment the color as it spirals out.
   * @see calc()
   */
PolarSpiralAlgorithm::PolarSpiralAlgorithm(const int colorWidth, 
					 const int emptyWidth,
					 const bool bConstantColor,
					 const bool bRandomColor,
					 const bool bIncrementColor) {

  // empty constants until initialize(...) is called
  _maxFieldIdx = 0;
  _screenXOffset = 0;
  _screenYOffset = 0;
  _maxR = 0.0;

  // initial values for internals
  _nextIdx = 0;
  _currX = 0.0;
  _currY = 0.0;

  // set parameters
  _colorWidth = colorWidth;
  _emptyWidth = emptyWidth;
  _bConstantColor = bConstantColor;
  _bRandomColor = bRandomColor;
  _bIncrementColor = bIncrementColor;

  if(_colorWidth < 0)
    _colorWidth = 0;
  if(_emptyWidth < 0)
    _emptyWidth = 0;  

  // calculate misc constants
  _segmentWidth = _colorWidth + (_emptyWidth<<1);

  // calculate spiral constants and initial values
  _rCoeff = double(_segmentWidth)/(2.0*SPIRALALGORITHM_PI);  
  _rCoeff *= (jrand() % 2 == 0) ? 1.0 : -1.0;  // whether clockwise or not.
  _currR = 0.0;

  // for now, just start at 0.
  _currTheta = 0.0;
  _lastThetaStep = SPIRALALGORITHM_PI/16.0;

}


PolarSpiralAlgorithm::~PolarSpiralAlgorithm() {
}

/**
 * Initializes the algorithm.  Doesn't perform any actual generation.
 * @param field The array of integers in which to generate the spiral field.
 * @param screenWidth the width of the screen in pixels.
 * @param screenHeight the height of the screen in pixels.
 * @param numColors the number of colors in the palette primary direction.
 */
void PolarSpiralAlgorithm::initialize(int* field, const int screenWidth, 
				     const int screenHeight, 
				     const int numColors) {
  initializeBase(field, screenWidth, screenHeight, numColors);

  if(_height <= 0)
    return;
  if(_width <= 0)
    return;
  if(_numColors <= 0)
    return;

  // calculate remaining internal constants
  _maxFieldIdx = screenWidth * screenHeight - 1;
  _screenXOffset = screenWidth>>1;
  _screenYOffset = screenHeight>>1;

  // calculate polar stopping condition.
  _maxR = sqrt( _screenXOffset*_screenXOffset + 
		_screenYOffset*_screenYOffset);
  if(_rCoeff < 0.0) {
    _maxR *= -1.0;
  }

  // pick initial color
  if(_bRandomColor) {
    _nextIdx = jrand() % numColors;
  } else {
    _nextIdx = 0;
  }

  // begin debug
  _stepCount = 128; //16;
  // end debug

}

/**
 * Completes one iteration of the algorithm.  Returns true if the algorithm
 * is completed.
 */
bool PolarSpiralAlgorithm::calc() {
  if(_field == NULL)
    return true;
  
  if(_stepCount < 0)
    return true;
  //_stepCount--;
  
  bool retVal = calcSpiral();
  
  // set pixel to next color, if it is within the boundaries.
  int px = roundDtoI(_currX)+_screenXOffset;
  int py = roundDtoI(_currY)+_screenYOffset;
  int fieldIdx = px+py*_width;
  if(0 <= px && px < _width && 0 <= py && py < _height &&
     fieldIdx < _maxFieldIdx) {
    _field[fieldIdx] = _nextIdx;
  }

  // update color
  if(!_bConstantColor) {
    if(_bIncrementColor) {
      _nextIdx = (_nextIdx+1)%_numColors;
    } else {
      _nextIdx = _nextIdx - 1;
      if(_nextIdx < 0) {
	_nextIdx = _numColors - _nextIdx;
      }
    }
  }

  return retVal;
}

/**
 * Calculates a new X,Y using a polar spiral.  Utilizes an adaptive
 * algorithm to adjust thetaStep such that the new (x,y) value will
 * be adjacent but not on the previous (x,y) value.
 * 
 * @return True if the calculation is complete.
 */
bool PolarSpiralAlgorithm::calcSpiral() {
  //cout<<"Entering calcSpiral"<<endl;
  double newX;
  double newY;
  double newR;
  double newTheta;
  double newThetaStep = _lastThetaStep;

  int iOldX = roundDtoI(_currX);
  int iOldY = roundDtoI(_currY);
  int iNewX;
  int iNewY;
 
  int escape_count = 0;
  bool bDone = false;
  while(!bDone) {
    // update Theta
    newTheta = _currTheta + newThetaStep;

    // calculate the new R, X and Y
    newR = _rCoeff * newTheta;
    newX = newR * cos(newTheta);
    newY = newR * sin(newTheta);

    // check to see if we've calculated for an adjacent pixel
    iNewX = roundDtoI(newX);
    iNewY = roundDtoI(newY);
    /*
    cout<<"newX: "<<newX<<","<<iNewX<<endl;
    cout<<"newY: "<<newY<<","<<iNewY<<endl;
    cout<<"oldX: "<<*x<<","<<iOldX<<endl;
    cout<<"oldY: "<<*y<<","<<iOldY<<endl;
    */

    bDone = true;
    if(iNewX == iOldX && iNewY == iOldY) {
      // theta too small
      newThetaStep*=2.0;
      bDone = false;
    } else if(iNewX <= iOldX-2 || iOldX+2 <= iNewX ||
	      iNewY <= iOldY-2 || iOldY+2 <= iNewY) {
      // theta too large
      newThetaStep*=0.5;
      bDone = false;
    }

    if(++escape_count >= 100) {
      /*
      cout<<"PolarSpiralAlgorithm::calcSpiral() Escape count reached!"<<endl;
      cout<<"maxR: "<<_maxR<<endl;
      */
      //exit(1);
      //*r = _maxR+1.0;
      _maxR = -1;
      bDone = true;
    }
  }

  // !!! NOTE NEED TO ADD IN CODE TO HANDLE _colorWidth

  // send out values.
  _currX = newX;
  _currY = newY;
  _currR = newR;
  _currTheta = newTheta;
  _lastThetaStep = newThetaStep;

  return (_rCoeff < 0.0) ? (newR <= _maxR) : (newR >= _maxR);
}
