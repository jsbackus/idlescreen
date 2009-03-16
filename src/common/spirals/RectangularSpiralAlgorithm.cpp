#include "RectangularSpiralAlgorithm.h"

/**
 * Constructor.
 */
RectangularSpiralAlgorithm::RectangularSpiralAlgorithm() {
  _cornerSeg = NULL;
  _straightSeg = NULL;
}

RectangularSpiralAlgorithm::~RectangularSpiralAlgorithm() {
  if(_cornerSeg != NULL) {
    delete [] _cornerSeg;
    _cornerSeg = NULL;
  }
  if(_straightSeg != NULL) {
    delete [] _straightSeg;
    _straightSeg = NULL;
  }
}

/**
 * Initializes the algorithm.  Doesn't perform any actual generation.
 * @param field The array of integers in which to generate the spiral field.
 * @param screenWidth the width of the screen in pixels.
 * @param screenHeight the height of the screen in pixels.
 * @param numColors the number of colors in the palette primary direction.
 * @param colorWidth the width of the strip of color.
 * @param emptyWidth the width of the default color region.
 * @param bConstantColor whether or not to hold the color index constant.
 * @param bRandomColor whether or not the initial color is randomly selected.
 * @see calc()
 */
void RectangularSpiralAlgorithm::initialize(int* field, int screenWidth, 
					    int screenHeight, int numColors,
					    int colorWidth, int emptyWidth,
					    bool bConstantColor, 
					    bool bRandomColor) {
  initializeBase(field, screenWidth, screenHeight, numColors, colorWidth,
		 colorHeight, bConstantColor);

  if(_height <= 0)
    return;
  if(_width <= 0)
    return;
  if(_numColors <= 0)
    return;

  calcSegs();

  // set initial starting position
  _currX = screenWidth / 2;
  _currY = screenHeight / 2;

  // pick initial color
  if(bRandomColor) {
    _nextIdx = jrand() % numColors;
  } else {
    _nextIdx = 0;
  }

  // pick direction
  _dir = jrand() % 4;
  _bClockwise = (jrand() % 2 == 0);

  // set initial distances
  switch(_dir) {
  case UP:
    _bXPositive = _bClockwise;
    _xSegs = 2;

    _bYPositive = true;
    _ySegs = 1;
    _remainingSegs = 1;
    break;
  case DOWN:
    _bXPositive = !_bClockwise;
    _xSegs = 2;

    _bYPositive = false;
    _ySegs = 1;
    _remainingSegs = 1;
    break;
  case LEFT:
    _bXPositive = false;
    _xSegs = 1;
    _remainingSegs = 1;

    _bYPositive = _bClockwise;
    _ySegs = 2;
    break;
  case RIGHT:
  default:
    _bXPositive = true;
    _xSegs = 1;
    _remainingSegs = 1;

    _bYPositive = !_bClockwise;
    _ySegs = 2;
  }

}

/**
 * Completes one iteration of the algorithm.  Returns true if the algorithm
 * is completed.
 */
bool RectangularSpiralAlgorithm::calc() {
  if(_field == NULL)
    return;
  if(_cornerSeg == NULL)
    return;
  if(_straightSeg == NULL)
    return;

  int i;

  /**
   * To Do:
   * - Process a segment
   * - Update _currX and/or _currY
   * - Update color index, if necessary
   * - Decrement _remainingSegs
   * - if _remainingSegs == 0:
   *   - change _dir
   *   - update polarities, current/next segs, and _remainingSegs.
   */
  
  // update color index
  if(!_bConstantColor) {
    _nextIdx = (_nextIdx+_segmentSize)%_numColors;
  }

  // update the number of remaining segments.  If we hit 0,
  // time to change direction!
  _remainingSegs--;
  if(_remainingSegs <= 0) {
    // establish new direction
    switch(_dir) {
    case UP:
      _dir = _bClockwise ? RIGHT : LEFT;
      break;
    case DOWN:
      _dir = _bClockwise ? LEFT : RIGHT;
      break;
    case LEFT:
      _dir = _bClockwise ? UP : DOWN;
      break;
    case RIGHT:
    default:
      _dir = _bClockwise ? DOWN : UP;
    }
  }
    
  /*
    // old
  // Check the remaining distance.  If it is 0, then we are turning.
  if(_remainingSegs == 0) {
    int bevDim = _colorWidth+2*_emptyWidth;
    
    // move through the turn
    switch(_dir) {
    case UP:
      // up to right or up to left is a counter-clockwise rotation
      _currY++;
      int tmpX = _currX;
      int tmpY = _currY*_width;

      if(_currY >= 0 && _currY < _height) {
	for(i=0; i<bevDim;i++) {
	  if(tmpX >= 0 && _tmpX < _width) {
	    int bevX = bevDim - _remainingTurn;
	    int bevY = (bevDim - i - 1)*bevDim;
	    if(_bevel[bevX + bevY] >= 0) {
	      _field[tmpY+(tmpX++)] = _nextIdx + _bevel[bevX + bevY];
	    } else {
	      _field[tmpY+(tmpX++)] = -1;
	    }
	  }
	}
      }
      break;
    case DOWN:
      // down to right or down to left is a clockwise rotation
      _currY--;
      int tmpX = _currX;
      int tmpY = _currY*_width;

      if(_currY >= 0 && _currY < _height) {
	for(i=0; i<bevDim;i++) {
	  if(tmpX >= 0 && _tmpX < _width) {
	    int bevX = _remainingTurn;
	    int bevY = (i)*bevDim;
	    if(_bevel[bevX + bevY] >= 0) {
	      _field[tmpY+(tmpX++)] = _nextIdx + _bevel[bevX + bevY];
	    } else {
	      _field[tmpY+(tmpX++)] = -1;
	    }
	  }
	}
      }
      break;
    case LEFT:
      break;
    case RIGHT:
    default:
    }
*/
}

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
void RectangularSpiralAlgorithm::calcSegments() {
  if(_colorWidth <= 0)
    return;
  if(_emptyWidth < 0)
    _emptyWidth = 0;

  // set the segment dimension
  _segmentSize = _colorWidth + 2*_emptyWidth;

  // create space for the two segments
  _straightSeg = new int[_segmentSize * _segmentSize];
  if(_straightSeg == NULL)
    return;
  _cornerSeg = new int[_segmentSize * _segmentSize];
  if(_cornerSeg == NULL)
    return;

  // build the straight segment.  See comment in header file for more 
  // information.
  int x, y, yCalc, yInvCalc;
  for(y=0;y<_segmentSize; y++) {
    yCalc = y * _segmentSize;
    for(x=0;x<_segmentSize;x++) {
      if(y<_emptyWidth || y >= _colorWidth + _emptyWidth) {
	// we're in empty space, so use default color.
	_straightSeg[yCalc+x] = -1;
      } else {
	// we're in the color space.  If _bConstantColor, set
	// to 0, otherwise use x.
	if(_bConstantColor) {
	  _straightSeg[yCalc+x] = 0;
	} else {
	  _straightSeg[yCalc+x] = x;
	}
      }
    }
  }

  // now, we build the corner segment.  If _bClockwise is true,
  // then copy the top left corner as is, and rotate the bottom right
  // corner about x=y.  If _bClockwise is false, copy the bottom left
  // corner as is, and rotate the top right about x=_segmentSize-y.
  if(_bClockwise) {
    for(y = 0; y<_segmentSize; y++) {
      yCalc = y*_segmentSize;
      yInvCalc = (_segmentSize - y)*_segmentSize;
      for(x = 0; x<_segmentSize; x++) {
	if(x < y) {
	  _cornerSeg[yCalc+x] = _straightSeg[yCalc+x];
	} else {
	  _cornerSeg[yCalc+x] = _straightSeg[yInvCalc+(_segmentSize-x)];
	}
      }
    } 
  } else {
    for(y = 0; y<_segmentSize; y++) {
      yCalc = y*_segmentSize;
      yInvCalc = (_segmentSize - y)*_segmentSize;
      for(x = 0; x<_segmentSize; x++) {
	if(x < (_segmentSize-y)) {
	  _cornerSeg[yCalc+x] = _straightSeg[yCalc+x];
	} else {
	  _cornerSeg[yCalc+x] = _straightSeg[yInvCalc+(_segmentSize-x)];
	}
      }
    }     
  }
}
