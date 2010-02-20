
// debug
#include <iostream>
// end debug
#include <math.h>
using namespace std;

#include "NgonSpiralAlgorithm.h"

/**
 * Constructor.
 */
NgonSpiralAlgorithm::NgonSpiralAlgorithm() {

  // parameters
  _n = 0;
  _colorWidth = 1;
  _emptyWidth = 0;
  _bConstantColor = true;
  _bRandomColor = false;
  _bIncrementColor = true;
  _maxFieldIdx = 0;

  _origin.setValue(0,0);
  _axes = NULL;
  _curAxis = -1;
  _ringNum = -1;
  _lineWidth = -1.0;
  _bevelAngle = 0.0;
  _bevelLength = 0.0;
  _curColorIdx = 0;
  _curColorCount = 0.0;
  _growLength = 0.0;
}

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
NgonSpiralAlgorithm::NgonSpiralAlgorithm(const int N, const int colorWidth, 
					 const int emptyWidth,
					 const bool bConstantColor,
					 const bool bRandomColor,
					 const bool bIncrementColor) {

  // empty constants until initialize(...) is called
  _maxFieldIdx = 0;
  /*
    _screenXOffset = 0;
    _screenYOffset = 0;
  */

  // set parameters
  _n = N;
  _colorWidth = colorWidth;
  _colorWidthDbl = (double)colorWidth;
  _emptyWidth = emptyWidth;
  _bConstantColor = bConstantColor;
  _bRandomColor = bRandomColor;
  _bIncrementColor = bIncrementColor;

  if(_colorWidth < 0)
    _colorWidth = 0;
  if(_emptyWidth < 0)
    _emptyWidth = 0;  

  // calculate misc constants
  //_segmentWidth = _colorWidth + (_emptyWidth<<1);
  _lineWidth = double(_colorWidth + (_emptyWidth<<1));
  cout<<"Setting linewidth to "<<_lineWidth<<endl;

  // allocate space for axes & their normals
  if(!(3 <= _n && _n < 1024))
    return;
  _axes = new Vector2D[_n];
  _normals = new Vector2D[_n];
  if(_axes == NULL || _normals == NULL) {
    _n = 0;
    return;
  }

  // create & calculate axes
  double delta = 2.0 * M_PI / ((double)_n);
  if(relativeCompare(delta, M_PI/2.0, 1e-6)) {
    _growLength = ((double)_lineWidth);
  } else if( M_PI/2.0 < delta) {
    _growLength = ((double)_lineWidth) / sin(M_PI - delta);
  } else {
    //_growLength = ((double)_lineWidth) / sin(delta);
    //_growLength = ((double)_lineWidth) / (2.0*sin(delta));
    _growLength = ((double)_lineWidth);
    //_growLength = ((double)_lineWidth) * sin(delta);
  }
  _bevelAngle = delta/2.0;
  double angle = 0.0; //((double)(jrand()%360)) / 2.0*PI;
  int i;
  for(i=0; i<_n; i++) {
    _axes[i].setValue(1.0,0.0);
    _axes[i].rotate(angle);
    _normals[i] = _axes[i].getNormal();
    cout<<"i: "<<i<<", angle: "<<angle<<", axis: "<<_axes[i]<<", n: "
	<<_normals[i]<<endl;
    angle += delta;
  }
  _bevelLength = sin(_bevelAngle) * _lineWidth/2.0;

  _curColorCount = 0.0;
  _curAxis = 0;
  _ringNum = 0.0;
}


NgonSpiralAlgorithm::~NgonSpiralAlgorithm() {
  if(_axes != NULL) {
    delete [] _axes;
    _axes = NULL;
  }
  if(_normals != NULL) {
    delete [] _normals;
    _normals = NULL;
  }
}

/**
 * Initializes the algorithm.  Doesn't perform any actual generation.
 * @param field The array of integers in which to generate the spiral field.
 * @param screenWidth the width of the screen in pixels.
 * @param screenHeight the height of the screen in pixels.
 * @param numColors the number of colors in the palette primary direction.
 */
void NgonSpiralAlgorithm::initialize(int* field, const int screenWidth, 
				     const int screenHeight, 
				     const int numColors) {
  initializeBase(field, screenWidth, screenHeight, numColors);

  if(_height <= 0)
    return;
  if(_width <= 0)
    return;
  if(_numColors <= 0)
    return;

  _maxFieldIdx = screenWidth*screenHeight-1;

  _origin.setValue((screenWidth>>1)+_lineWidth/2.0+0.5, 
		   (screenHeight>>1)+_lineWidth/2.0+0.5 );

  // pick initial color
  if(_bRandomColor) {
    _curColorIdx = jrand() % numColors;
  } else {
    _curColorIdx = 0;
  }
  _curColorCount = 0.0;

  // begin debug
  _stepCount = 16;
  // end debug
}

/**
 * Completes one iteration of the algorithm.  Returns true if the algorithm
 * has completed.
 */
bool NgonSpiralAlgorithm::calc() {
  if(_field == NULL)
    return true;

  // begin debug
  cout<<"Count: "<<_stepCount<<endl;
  /*
  if(_stepCount < 0)
    return true;
  _stepCount--;
  */
  // end debug
  
  // draw a segment
  Point2D beginPoint;
  Point2D endPoint;

  cout<<"Cur Axis: "<<_curAxis<<", ring: "<<_ringNum
      <<", grow length: "<<_growLength<<endl;
  // check to see if the first segment has been drawn, since it is
  // the only "odd" one.
  if(_ringNum < 1.0) {
    // nope, so draw a segment from the origin to the first midpoint.
    beginPoint = _origin;
    //Vector2D tmpV = _axes[0];
    //tmpV.rotate(0.5*_bevelAngle);
    endPoint = _axes[0].getPoint(_origin, _growLength);
    // set up for next round.
    _lastPoint = endPoint;
    _curAxis = 0;
    _ringNum = _growLength;
    _ringCounter = _n-2;
  } else {
    // start at last midpoint
    beginPoint = _lastPoint;
    // find the next midpoint on the next axis.  If the current axis is the
    // last axis in the set, then increment the ring number too.
    int nextAxis = (_curAxis+1)%_n;
    _ringCounter--;
    if(_ringCounter <= 0) {
      _ringNum+=_growLength;
      _ringCounter = _n-2;
    }
    _lastPoint = _axes[nextAxis].getPoint(beginPoint, _ringNum);
    // change axis state for next iteration
    _curAxis = nextAxis;
  }
  // draw the line segment
  return !drawSegment(beginPoint, _lastPoint);
}

/**
 * Draws a segment from pointA to pointB.  Returns true if any indices were
 * set within _field.
 */
bool NgonSpiralAlgorithm::drawSegment(const Point2D& pointA, 
				      const Point2D& pointB) {
  cout<<"Drawing Segment from "<<pointA<<" to "<<pointB<<endl;
  bool bPixelDrawn = false;
  Point2D pB = pointB;
  Point2D pA = pointA;
  Vector2D pathV = pB - pA;
  pathV.normalize();
  // calculate normal to the path
  Vector2D nV = pathV.getNormal();
  nV.normalize();
  // calculate bevel vector
  Vector2D bevV = nV;
  bevV.normalize();
  bevV.rotate(1.0*_bevelAngle);
  // these points represent the actual drawing
  Point2D pStart;
  Point2D pEnd;
  double posOuter;
  double posInner;
  double traverseLength;

  cout<<"Path: "<<pathV<<", Normal: "<<nV<<", Bevel: "<<bevV<<endl;

  Point2D bevA = pA;
  Point2D bevB = pB;
  if(_colorWidth > 1) {
    // Calculate the first end of the bevel.
    bevA = pA + _bevelLength*pathV;
    // Calculate the end of the bevel on the other end.
    bevB = pB - _bevelLength*pathV;

    /* 1: draw bevel on starting endpoint */
    // Traverse the width of the color part of the line, starting
    // at the point on the normal that is (color width/2)-(current increment) 
    // length away from the main path draw a line to the intersection of the
    // bevel vector along the vector that is parallel to path.
    for(posOuter = 0.0; posOuter < _colorWidthDbl; 
	posOuter += NGONSPIRALALGORITHM_STEPSIZE) {
      // calculate the point on the normal.
      pStart = bevA + (_colorWidthDbl/2.0 - posOuter)*nV;

      // calculate the point on the bevel vector.
      if(!pathV.getIntersectionPt(pStart, bevV, pA, pEnd)) {
	cout<<"pathV doesn't intersect bevV!"<<endl;
	return false;
      }

      // calculate the traversal length
      traverseLength = pathV.getDistanceToPoint(pStart, pEnd);
      // traverse from start to finish setting pixels along the way  - remember, 
      // we're going backwards!
      for(posInner = 0.0; posInner > traverseLength; 
	  posInner -= NGONSPIRALALGORITHM_STEPSIZE) {
	bPixelDrawn = setPixel(pathV.getPoint(pStart, posInner)) || bPixelDrawn;
      }
    }
    updateColorCount(_bevelLength);
    cout<<"first bevel done"<<endl;
  }
  /* 2: draw segment between beveled ends */
  // Traverse the length of the segment between bevA and bevB filling in the
  // color portion of the segment.
  traverseLength = pathV.getDistanceToPoint(bevA, bevB);
  cout<<"TraverseLength: "<<traverseLength<<endl;
  //for(posOuter = 0.0; posOuter < traverseLength; 
  for(posOuter = 0.0; posOuter < traverseLength; 
      posOuter += NGONSPIRALALGORITHM_STEPSIZE) {
    //cout<<"posOuter: "<<posOuter<<endl;
    
    // calculate the point on the path.
    pStart = bevA + posOuter*pathV;
    //cout<<">> "<<pStart<<" -> ";
    // now go to the edge of the color region.
    pStart = pStart + (_colorWidthDbl/2.0)*nV;
    //cout<<pStart<<endl;

    // traverse from start to finish setting pixels along the way
    for(posInner = 0.0; posInner < _colorWidthDbl; 
	posInner += NGONSPIRALALGORITHM_STEPSIZE) {
      //cout<<"posInner: "<<posInner<<", pStart: "<<pStart<<endl;
      bPixelDrawn = setPixel(pStart) || bPixelDrawn;
      updateColorCount(NGONSPIRALALGORITHM_STEPSIZE);
      pStart = pStart - NGONSPIRALALGORITHM_STEPSIZE*nV;
    }
  }
  cout<<"seg done"<<endl;

  /* 3: draw bevel on tail endpoint */
  if(_colorWidth > 1) {
    // Adjust bevV for the new side.
    bevV.rotate(-1.0*_bevelAngle);
    // Traverse the width of the color part of the line, starting
    // at the point on the normal that is (color width/2)-(current increment) 
    // length away from the main path draw a line to the intersection of the
    // bevel vector along the vector that is parallel to path.
    for(posOuter = 0.0; posOuter < _colorWidthDbl; 
	posOuter += NGONSPIRALALGORITHM_STEPSIZE) {
      // calculate the point on the normal.
      pStart = bevB + (_colorWidthDbl/2.0 - posOuter)*nV;

      // calculate the point on the bevel vector.
      if(!pathV.getIntersectionPt(pStart, bevV, pB, pEnd)) {
	cout<<"pathV doesn't intersect bevV!"<<endl;
	return false;
      }

      // calculate the traversal length
      traverseLength = pathV.getDistanceToPoint(pStart, pEnd);
      // traverse from start to finish setting pixels along the way - going
      // forwards this time!
      for(posInner = 0.0; posInner < traverseLength; 
	  posInner += NGONSPIRALALGORITHM_STEPSIZE) {
	bPixelDrawn = setPixel(pathV.getPoint(pStart, posInner)) || bPixelDrawn;
      }
    }
    updateColorCount(_bevelLength);
  }
  return bPixelDrawn;
}

/**
 * Updates the color count by the specified amount.
 */
void NgonSpiralAlgorithm::updateColorCount(const double amnt) {
  if(!_bConstantColor) {
    _curColorCount += amnt;
    if(_colorWidthDbl <= _curColorCount) {
      _curColorCount -= _colorWidthDbl;
      // update the color index.
      if(_bIncrementColor) {
	_curColorIdx++;
	if(_numColors <= _curColorIdx) {
	  _curColorIdx = 0;
	}
      } else {
	_curColorIdx--;
	if(_curColorIdx <= 0) {
	  _curColorIdx = _numColors-1;
	}
      }
    }
  }
}

/**
 * Sets the specified coordinates to the current color index.
 */
bool NgonSpiralAlgorithm::setPixel(const Point2D& p) {
  if(_field == NULL)
    return false;

  Point2D lclP = p;

  int x = (int)(lclP.getX() + 0.5);
  int y = (int)(lclP.getY() + 0.5);
  int pos = y*_width + x;

  //cout<<"Setting pixel: ("<<x<<", "<<y<<")";

  // validate that the pixel is on the screen.
  if( (0 <= x && x < _width) &&
      (0 <= y && y < _height) &&
      pos < _maxFieldIdx) {
    
    // it is!  set the pixel to the current color index.
    _field[pos] = _curColorIdx;

    //cout<<" -> Success!"<<endl;

    return true;
  }

  //cout<<" -> Failure!"<<endl;

  return false;
}
  
#ifdef BLARGH
/**
 * Completes one iteration of the algorithm.  Returns true if the algorithm
 * is completed.
 */
bool NgonSpiralAlgorithm::calc() {
  if(_field == NULL)
    return true;

  if(_stepCount < 0)
    return true;
  //_stepCount--;
  // if N is valid, use the N-gon spiral calculations.
  // Otherwise, use the polar spiral calculations
  bool retVal;
  if(2 < _n && _n <= NGONSPIRAL_MAX_N) {
    retVal = true; // < Temporary!
  } else {
    retVal = calcSpiral(&_currX, &_currY, &_currR, &_currTheta, 
			&_lastThetaStep);
  }

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
 * @param x the previous X value.  On return, it is the new X value.
 * @param y the previous Y value.  On return, it is the new Y value.
 * @param r the previous R value.  On return, it is the new R value.
 * @param theta the previous Theta value.  It is updated on return.
 * @param thetaStep the previous Theta step value.  Updated on return.
 * @return True if the calculation is complete.
 */
bool NgonSpiralAlgorithm::calcSpiral(double* x, double* y, double* r, 
				     double* theta, double* thetaStep) {
  //cout<<"Entering calcSpiral"<<endl;
  double newX;
  double newY;
  double newR;
  double newTheta;
  double newThetaStep = *thetaStep;

  int iOldX = roundDtoI(*x);
  int iOldY = roundDtoI(*y);
  int iNewX;
  int iNewY;
 
  int escape_count = 0;
  bool bDone = false;
  while(!bDone) {
    // update Theta
    newTheta = *theta + newThetaStep;

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
	cout<<"NgonSpiralAlgorithm::calcSpiral() Escape count reached!"<<endl;
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
  *x = newX;
  *y = newY;
  *r = newR;
  *theta = newTheta;
  *thetaStep = newThetaStep;

  return (_rCoeff < 0.0) ? (newR <= _maxR) : (newR >= _maxR);
}

#endif
