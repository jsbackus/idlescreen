/**
 * Constructor.
 *
 * @param x The initial x value of the vector.
 * @param y The initial y value of the vector.
 */
Point2D::Point2D(const double x=0.0, const double y=0.0) {
  _x = x;
  _y = y;
}
  
/**
 * Deconstructor
 */
Point2D::~Point2D() {
}

/**
 * Gets the x value.
 */
double Point2D::getX() {
  return _x;
}
/**
 * Sets the x value.
 */
void Point2D::setX(const double x=0.0) {
  _x = x;
}

/**
 * Gets the y value.
 */
double Point2D::getY() {
  return _y;
}
/**
 * Sets the y value.
 */
void Point2D::setY(const double y=0.0) {
  _y = y;
}

Point2D& Point2D::operator=(const Point2D& other) {
  // no need to check for self-assignment, since we're not 
  // allocating/deallocating memory.
  _x = other._x;
  _y = other._y;

  return *this;
}
Point2D& Point2D::operator==(const Point2D& other) {
  return (_x == other._x && _y == other._y);
}
Point2D& Point2D::operator!=(const Point2D& other) {
  return !(*this == other);
}
Point2D& Point2D::operator*(const double val) {
  Point2D retVal = *this;
  retVal *= val;
  return retVal;
}
Point2D& Point2D::operator/(const double val) {
  Point2D retVal = *this;
  retVal /= val;
  return retVal;
}
Point2D& Point2D::operator*=(const double val) {
  Point2D retVal = *this;
  retVal._x *= val;
  retVal._y *= val;
  return retVal;  
}
Point2D& Point2D::operator/=(const double val) {
  Point2D retVal = *this;
  retVal._x /= val;
  retVal._y /= val;
  return retVal;
}
