#include "misc_funcs.h"
#include "Vector2D.h"

// Set default value of the global epsilon
double Vector2D::DEFAULT_EPSILON = 0.0000001;

/**
 * Sets the default epsilon value.
 */
void Vector2D::setDefaultEpsilon(const double& epsilon) {
  DEFAULT_EPSILON = epsilon;
}

/**
 * Gets the default epsilon value.
 */
double Vector2D::getDefaultEpsilon() {
  return DEFAULT_EPSILON;
}

/**
 * Sets the epsilon value for this object.
 */
void Vector2D::setEpsilon(const double &epsilon) {
  _epsilon = epsilon;
}

/**
 * Gets the epsilon value for this object.
 */
double Vector2D::getEpsilon() {
  return _epsilon;
}

/**
 * Constructor.
 *
 * @param x The initial x value of the vector.
 * @param y The initial y value of the vector.
 */
Vector2D::Vector2D(const double x, const double y) {
  _x = x;
  _y = y;
  _bIsNormalized = false;
  _epsilon = DEFAULT_EPSILON;
}
  
/**
 * Deconstructor
 */
Vector2D::~Vector2D() {
}

/**
 * Gets the x value.
 */
double Vector2D::getX() {
  return _x;
}
/**
 * Sets the x value.
 */
void Vector2D::setX(const double x) {
  _x = x;
  _bIsNormalized = false;
}

/**
 * Gets the y value.
 */
double Vector2D::getY() {
  return _y;
}
/**
 * Sets the y value.
 */
void Vector2D::setY(const double y) {
  _y = y;
  _bIsNormalized = false;
}

/**
 * Sets the x & y values.
 *
 * @param x The x value of the vector.
 * @param y The y value of the vector.
 */
void Vector2D::setValue(const double x, const double y) {
  _x = x;
  _y = y;
}

/**
 * Gets the magnitude of the vector.
 */
double Vector2D::magnitude() {
  if(_bIsNormalized) {
    return 1.0;
  } else {
    return sqrt(_x*_x+_y*_y);
  }
}

/**
 * Gets the normal of this vector.
 */
Vector2D Vector2D::getNormal() {
  Vector2D retVal;
  // 2D case is easy!
  retVal._x = -1.0*_y;
  retVal._y = _x;

  retVal.normalize();

  return retVal;
}

/**
 * Normalizes this vector.
 */
void Vector2D::normalize() {
  if(!_bIsNormalized) {
    double mag = magnitude();
    if(!relativeCompare(mag, 0.0, _epsilon)) {
      _x = _x/mag;
      _y = _y/mag;
      _bIsNormalized = true;
    }
  }
}

/**
 * Returns the dot product of the specified vector with this vector.
 */
double Vector2D::dot(const Vector2D other) {
  return (_x*other._x+_y*other._y);
}


/**
 * Returns true if the specified vector is parallel to this vector.
 */
bool Vector2D::isParallelTo(const Vector2D other) {
  Vector2D thisUnitV = *this;
  Vector2D otherUnitV = other;
  thisUnitV.normalize();
  otherUnitV.normalize();

  return 
    ( relativeCompare(thisUnitV.magnitude(), 1.0, _epsilon) &&
      relativeCompare(otherUnitV.magnitude(), 1.0, _epsilon) &&
      (relativeCompare(thisUnitV._x, otherUnitV._x, _epsilon) || 
       relativeCompare(thisUnitV._x, 1.0*otherUnitV._x, _epsilon)) &&
      (relativeCompare(thisUnitV._y, otherUnitV._y, _epsilon) || 
       relativeCompare(thisUnitV._y, 1.0*otherUnitV._y, _epsilon)) );
}

/**
 * Returns true if the specified vector is normal to this vector.
 */
bool Vector2D::isNormalTo(const Vector2D other) {
  Vector2D thisV = *this;
  Vector2D otherNormV = other;

  return thisV.isParallelTo(otherNormV.getNormal());
}

Vector2D& Vector2D::operator=(const Vector2D& other) {
  // no need to check for self-assignment, since we're not 
  // allocating/deallocating memory.
  _x = other._x;
  _y = other._y;
  _bIsNormalized = other._bIsNormalized;

  return *this;
}

/**
 * Calculates the angle between the specified vector and this vector.
 * Note: returns a value between [0, pi] or NaN if values are invalid.
 * If one or both of the vectors has a length of zero, a value of
 * 0.0 is returned.
 */
double Vector2D::getAngle(const Vector2D other) {
  
  // we know that mag(v1)*mag(v2)*cos(theta)=normal(v1) dot v2
  // thus theta = arccos( (normal(v1) dot v2) / ( mag(v1)*mag(v2) ) )
  Vector2D norm = *this;
  Vector2D otherUnitV = other;
  norm.normalize();
  otherUnitV.normalize(); // dealing with normal vectors makes the math easy.

  if(norm.magnitude() != 1.0 || otherUnitV.magnitude() != 1.0) {
    return 0.0;
  }

  return acos(norm.dot(otherUnitV));
}

/**
 * Rotates the vector by the angle radians.
 */
void Vector2D::rotate(const double angle) {
  double newX = _x*cos(angle)-_y*sin(angle);
  double newY = _x*sin(angle)+_y*cos(angle);

  _x = newX;
  _y = newY;
}


/**
 * Returns the point that is described by traveling along a scaled
 * version of this vector from the specified point.  Note that
 * this vector is not normalized beforehand.
 *
 * @param a Origination point.
 * @param scalar The amount to scale this vector by first.
 */
Point2D Vector2D::getPoint(const Point2D a, const double scalar) {
  Point2D retVal = a;
  retVal.setX(retVal.getX()+scalar*_x);
  retVal.setY(retVal.getY()+scalar*_y);

  return retVal;
}

/**
 * Checks to see if point B is on the line specified
 * by this vector and point A.
 *
 * @param a A point known to be on the line.
 * @param b The point to check.
 */
bool Vector2D::isOnLine(const Point2D a, const Point2D b) {
  // check to see if the points are the same.  If so, easy peasy!
  Point2D copyA = a;
  if(copyA == b) {
    return true;
  }

  // create a replica and ensure that its magnitude isn't 0.0.
  Vector2D replica = *this;
  if(relativeCompare(replica.magnitude(),0.0, _epsilon)) {
    return true;
  }
  // If the vector from point B to point A is parallel to vector V, then
  // point B is on the line.  
  Vector2D v2;
  v2 = b - a;
  return replica.isParallelTo(v2);
}

/**
 * Finds the point of intersection of the line specified
 * by this vector and point A and the line specified by
 * vector V2 and point B.  Returns false if the lines
 * do not intersect.  The resulting point is put into result.
 * If result is NULL, the point will not be returned, but the
 * function will still return true if the lines intersect or
 * false if they do not.
 *
 * @param pA A point on the line specified by this vector.
 * @param vB The vector describing the other line.
 * @param pB A point on the line described by vector v.
 * @param result The resulting point.
 * @return True if the lines intersect, false if they do not.
 */
bool Vector2D::getIntersectingPt(const Point2D pA, const Vector2D vB, 
				 const Point2D pB, Point2D& result) {

  Point2D rPA = pA;
  Point2D rPB = pB;
  Vector2D rVB = vB;
  double numer = rVB._x*(rPB.getY() - rPA.getY()) - 
    rVB._y*(rPB.getX() - rPA.getX());
  double denom = rVB._y*_x - rVB._x*_y;
  if(denom != 0) {
    Vector2D vF = (*this) * (numer / denom);
    result = rPA + vF;
  }
  return false;
}

bool Vector2D::operator==(const Vector2D& other) {
  return (relativeCompare(_x, other._x, _epsilon) && 
	  relativeCompare(_y, other._y, _epsilon));
}
bool Vector2D::operator!=(const Vector2D& other) {
  return !(*this == other);

}
Vector2D& Vector2D::operator*(const double val) {
  *this *= val;
  return *this;
}
Vector2D& Vector2D::operator*=(const double val) {
  _x *= val;
  _y *= val;
  _bIsNormalized = false;
  return *this;  
}

Vector2D& Vector2D::operator+(const Vector2D& other) {
  *this += other;
  return *this;
}
Vector2D& Vector2D::operator/(const double val) {
  *this /= val;
  return *this;
}
Vector2D& Vector2D::operator+=(const Vector2D& other) {
  _x += other._x;
  _y += other._y;
  _bIsNormalized = false;
  return *this;
}
Vector2D& Vector2D::operator/=(const double val) {
  _x /= val;
  _y /= val;
  _bIsNormalized = false;
  return *this;
}
Vector2D& Vector2D::operator-=(const Vector2D& other) {
  Vector2D otherInv = other;
  otherInv *= -1.0;
  *this += otherInv;
  return *this;
}
Vector2D& Vector2D::operator-(const Vector2D& other) {
  *this -= other;
  return *this;
}

// **** Begin Friend definitions ****

Vector2D operator*(const double val, const Vector2D& v) {
  Vector2D tmp = v;
  return tmp*val;
}

/**
 * A point plus a vector results in a new point.
 */
Point2D operator+(const Point2D& p, const Vector2D& v) {
  Vector2D repV = v;
  Point2D retVal = repV.getPoint(p,1.0);
  return retVal;
}

Point2D operator+(const Vector2D& v, const Point2D& p) {
  Vector2D repV = v;
  Point2D retVal = repV.getPoint(p,1.0);
  return retVal;
}

Point2D operator-(const Point2D& p, const Vector2D& v) {
  Vector2D repV = v;
  Point2D retVal = repV.getPoint(p,-1.0);
  return retVal;
}

/**
 * point subtraction results in a vector
 */
Vector2D operator-(const Point2D& final, const Point2D& initial) {
  Point2D f = final;
  Point2D i = initial;
  Vector2D retVal(f.getX() - i.getX(),f.getY() - i.getY());
  return retVal;
}

/**
 * friend vector to dump to an ostream
 */
ostream& operator<<(ostream& os, const Vector2D& vector) {
  Vector2D tmp = vector;
  os<<"("<<tmp.getX()<<","<<tmp.getY()<<")";
  return os;
}
