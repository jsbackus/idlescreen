
// debug
//#include <iostream>
//using namespace std;
// end debug

#include "RectangularSpiralAlgorithm.h"

/**
 * Constructor.
 */
RectangularSpiralAlgorithm::RectangularSpiralAlgorithm() {
  _cornerSeg = NULL;
  _straightSeg = NULL;

  _colorWidth = 1;
  _emptyWidth = 0;
  _bConstantColor = true;
  _bRandomColor = false;
  _bClockwise = false;
  _bIncrementColor = true;
  _currX = 0;
  _currY = 0;
  _nextIdx = 0;
  _xSegs = 0;
  _ySegs = 0;
  _remainingSegs = 0;
  _xMaxSegs = 0;
  _yMaxSegs = 0;
  _maxFieldIdx = 0;
  _halfSegSize = 0;
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
RectangularSpiralAlgorithm::RectangularSpiralAlgorithm(const int colorWidth, 
						       const int emptyWidth,
						       const bool bConstantColor,
						       const bool bRandomColor,
						       const bool bIncrementColor) {
  _cornerSeg = NULL;
  _straightSeg = NULL;

  _currX = 0;
  _currY = 0;
  _nextIdx = 0;
  _xSegs = 0;
  _ySegs = 0;
  _remainingSegs = 0;
  _xMaxSegs = 0;
  _yMaxSegs = 0;
  _maxFieldIdx = 0;
  _halfSegSize = 0;
  _bClockwise = false;

  _colorWidth = colorWidth;
  _emptyWidth = emptyWidth;
  _bConstantColor = bConstantColor;
  _bRandomColor = bRandomColor;
  _bIncrementColor = bIncrementColor;

  if(_colorWidth < 0)
    _colorWidth = 0;
  if(_emptyWidth < 0)
    _emptyWidth = 0;  
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
 * @see calc()
 */
void RectangularSpiralAlgorithm::initialize(int* field, const int screenWidth, 
					    const int screenHeight, 
					    const int numColors) {
  initializeBase(field, screenWidth, screenHeight, numColors);

  if(_height <= 0)
    return;
  if(_width <= 0)
    return;
  if(_numColors <= 0)
    return;

  // set initial starting position
  _currX = screenWidth / 2;
  _currY = screenHeight / 2;
  _maxFieldIdx = screenWidth * screenHeight - 1;

  // pick initial color
  if(_bRandomColor) {
    _nextIdx = jrand() % numColors;
  } else {
    _nextIdx = 0;
  }

  // pick direction
  _dir = (RectangularSpiralAlgorithmDir) (jrand() % 4);
  _bClockwise = (jrand() % 2 == 0);

  // begin debug
  //cout<<"Forcing dir!"<<endl;
  //_dir = RIGHT;
  //_bClockwise = true;
  // end debug

  // set initial distances
  switch(_dir) {
  case UP:
    _xSegs = 1;

    _ySegs = 0;
    _remainingSegs = 1;
    break;
  case DOWN:
    _xSegs = 1;

    _ySegs = 0;
    _remainingSegs = 1;
    break;
  case LEFT:
    _xSegs = 0;
    _remainingSegs = 1;

    _ySegs = 1;
    break;
  case RIGHT:
  default:
    _xSegs = 0;
    _remainingSegs = 1;

    _ySegs = 1;
  }

  // calculate segments
  calcSegments();

  // calculate stopping condition.
  _xMaxSegs = _width / _segmentSize;
  _yMaxSegs = _height / _segmentSize;

}

/**
 * Completes one iteration of the algorithm.  Returns true if the algorithm
 * is completed.
 */
bool RectangularSpiralAlgorithm::calc() {
  if(_field == NULL)
    return true;
  if(_cornerSeg == NULL)
    return true;
  if(_straightSeg == NULL)
    return true;

  /**
   * Algorithm:
   * - Process a segment
   * - Update _currX and/or _currY
   * - Update color index, if necessary
   * - Decrement _remainingSegs
   * - if _remainingSegs == 1:
   *   - change _dir
   *   - update current/next segs and _remainingSegs.
   */

  // draw a segment
  if(_remainingSegs <= 1) {
    // draw a corner segment
    drawSegment(_cornerSeg);

    //float halfSegSize = float(_segmentSize)/2;

    // update position and reset _remainingSegs
    switch(_dir) {
    case UP:
      _currY += _halfSegSize;
      if(_bClockwise) {
	_currX += _halfSegSize;
	_dir = RIGHT;
      } else {
	_currX -= _halfSegSize;
	_dir = LEFT;
      }
      _ySegs++;
      _remainingSegs = _xSegs;
      break;
    case DOWN:
      _currY -= _halfSegSize;
      if(_bClockwise) {
	_currX -= _halfSegSize;
	_dir = LEFT;
      } else {
	_currX += _halfSegSize;
	_dir = RIGHT;
      }
      _ySegs++;
      _remainingSegs = _xSegs;
      break;
    case LEFT:
      _currX -= _halfSegSize;
      if(_bClockwise) {
	_currY += _halfSegSize;
	_dir = UP;
      } else {
	_currY -= _halfSegSize;
	_dir = DOWN;
      }
      _xSegs++;
      _remainingSegs = _ySegs;
      break;
    case RIGHT:
    default:
      _currX += _halfSegSize;
      if(_bClockwise) {
	_currY -= _halfSegSize;
	_dir = DOWN;
      } else {
	_currY += _halfSegSize;
	_dir = UP;
      }
      _xSegs++;
      _remainingSegs = _ySegs;
    }
    // debug
    //return true;
    // end debug
  } else {
    // draw a straight segment
    drawSegment(_straightSeg);

    // update position
    switch(_dir) {
    case UP:
      _currY += _segmentSize;
      break;
    case DOWN:
      _currY -= _segmentSize;
      break;
    case LEFT:
      _currX -= _segmentSize;
      break;
    case RIGHT:
    default:
      _currX += _segmentSize;
    }
    _remainingSegs--;    
  }
  
  // update color index
  if(!_bConstantColor) {
    if(_bIncrementColor) {
      _nextIdx = (_nextIdx+_segmentSize)%_numColors;
    } else {
      _nextIdx = _nextIdx - _segmentSize;
      if(_nextIdx < 0) {
	_nextIdx = _numColors - _nextIdx;
      }
    }
  }

  //return true;
  return (_xSegs > _xMaxSegs) && (_ySegs > _yMaxSegs);
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
  _halfSegSize = float(_segmentSize)/2.0;

  // create space for the two segments
  _straightSeg = new int[_segmentSize * _segmentSize];
  if(_straightSeg == NULL)
    return;
  _cornerSeg = new int[_segmentSize * _segmentSize];
  if(_cornerSeg == NULL)
    return;

  int x, y, yCalc;

  // init segment space
  for(y=0;y<_segmentSize;y++) {
    yCalc = y*_segmentSize;
    for(x=0;x<_segmentSize;x++) {
      _straightSeg[yCalc+x] = -1;
      _cornerSeg[yCalc+x] = -1;
    }
  }

  // build the straight segment.  See comment in header file for more 
  // information.
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
	  if(_bIncrementColor) {
	    _straightSeg[yCalc+x] = x;
	  } else {
	    _straightSeg[yCalc+x] = _segmentSize - x - 1;
	  }
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
      for(x = 0; x<_segmentSize; x++) {
	if(x < y) {
	  _cornerSeg[yCalc+x] = _straightSeg[yCalc+x];
	} else {
	  _cornerSeg[yCalc+x] = _straightSeg[(_segmentSize -x -1)*_segmentSize
					     + (_segmentSize -y -1)];
	}
      }
    } 
  } else {
    for(y = 0; y<_segmentSize; y++) {
      yCalc = y*_segmentSize;
      for(x = 0; x<_segmentSize; x++) {
	if(x < _segmentSize-y-1) {
	  _cornerSeg[yCalc+x] = _straightSeg[yCalc+x];
	} else {
	  _cornerSeg[yCalc+x] = _straightSeg[x*_segmentSize + y];
	}
      }
    }     
  }
}

/**
 * Rotates and draws the specified segment at (_currX,_currY) based
 * on _dir.
 */
void RectangularSpiralAlgorithm::drawSegment(int* seg) {
  if(seg == NULL)
    return;

  // first, we need to translate the _currX and _currY positions to
  // the lower left-hand corner.  Remember: (_currX,_currY) is the
  // midpoint of the side opposite the direction we're going.  i.e.
  // (_currX,_currY) is the midpoint of the left side when going right.
  float xCornerFloat, yCornerFloat;

  switch(_dir) {
  case UP:
    xCornerFloat = _currX - _halfSegSize;
    yCornerFloat = _currY;
    break;
  case DOWN:
    xCornerFloat = _currX - _halfSegSize;
    yCornerFloat = _currY - float(_segmentSize);
    break;
  case LEFT:
    xCornerFloat = _currX - float(_segmentSize);
    yCornerFloat = _currY - _halfSegSize;
    break;
  case RIGHT:
  default:
    xCornerFloat = _currX;
    yCornerFloat = _currY - _halfSegSize;
  }

  // there seems to be a rounding error
  if(xCornerFloat < 0.0) {
    xCornerFloat -= 0.5;
  }
  if(yCornerFloat < 0.0) {
    yCornerFloat -= 0.5;
  }
  int xCorner = xCornerFloat;
  int yCorner = yCornerFloat;

  // adjust yCorner to be ready for copying
  int yCornerAdjusted = yCorner * _width; 

  // now copy from seg to _field.  Modifying (x,y) in order
  // to rotate:
  // Rt: field(x,y) = seg( x, y)
  // Up: field(x,y) = seg(-y, x)
  // Lf: field(x,y) = seg(-x,-y)
  // Dn: field(x,y) = seg( y,-x)

  for(int i=0; i<_segmentSize; i++) {
    int fieldY = i*_width;
    for(int j=0; j<_segmentSize; j++) {
      // set (x,y) = rot(j,i)
      int x;
      int y;
      switch(_dir) {
      case UP:
	x = i;
	y = _segmentSize-j-1;
	break;
      case DOWN:
	x = _segmentSize-i-1;
	y = j;
	break;
      case LEFT:
	x = _segmentSize-j-1;
	y = _segmentSize-i-1;
	break;
      case RIGHT:
      default:
	x = j;
	y = i;
      }

      // copy
      int ySeg = y*_segmentSize;
      int fieldIdx = yCornerAdjusted+fieldY+xCorner+j;
      if(0 <= xCorner+j && xCorner+j < _width &&
	 0 <= yCorner+i && yCorner+i < _height &&
	 0 <= ySeg+x && ySeg+x < _segmentSize*_segmentSize-1) {
	if(seg[ySeg + x] < 0) {
	  _field[fieldIdx] = -1;
	} else {
	  _field[fieldIdx] = (_nextIdx + seg[ySeg + x]) % _numColors;
	}
      }
    }
  }
}
