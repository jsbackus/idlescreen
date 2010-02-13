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
#include "misc_funcs.h"
#include "Point2D.h"

// Set default value of the global epsilon
double Point2D::DEFAULT_EPSILON = 0.0000001;

/**
 * Sets the default epsilon value.
 */
void Point2D::setDefaultEpsilon(const double& epsilon) {
  DEFAULT_EPSILON = epsilon;
}

/**
 * Gets the default epsilon value.
 */
double Point2D::getDefaultEpsilon() {
  return DEFAULT_EPSILON;
}

/**
 * Sets the epsilon value for this object.
 */
void Point2D::setEpsilon(const double &epsilon) {
  _epsilon = epsilon;
}

/**
 * Gets the epsilon value for this object.
 */
double Point2D::getEpsilon() {
  return _epsilon;
}

/**
 * Constructor.
 *
 * @param x The initial x value of the vector.
 * @param y The initial y value of the vector.
 */
Point2D::Point2D(const double x, const double y) {
  _x = x;
  _y = y;
  _epsilon = DEFAULT_EPSILON;
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
void Point2D::setX(const double x) {
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
void Point2D::setY(const double y) {
  _y = y;
}

Point2D& Point2D::operator=(const Point2D& other) {
  // no need to check for self-assignment, since we're not 
  // allocating/deallocating memory.
  _x = other._x;
  _y = other._y;

  return *this;
}

/**
 * Sets both the x & y values.
 */
void Point2D::setValue(const double x, const double y) {
  _x = x;
  _y = y;
}

bool Point2D::operator==(const Point2D& other) {
  return (relativeCompare(_x, other._x, _epsilon) && 
	  relativeCompare(_y,other._y, _epsilon));
}
bool Point2D::operator!=(const Point2D& other) {
  return !(*this == other);
}
Point2D& Point2D::operator*(const double val) {
  *this *= val;
  return *this;
}
Point2D& Point2D::operator/(const double val) {
  *this /= val;
  return *this;
}
Point2D& Point2D::operator*=(const double val) {
  _x *= val;
  _y *= val;
  return *this;  
}
Point2D& Point2D::operator/=(const double val) {
  _x /= val;
  _y /= val;
  return *this;
}

/**
 * friend vector to dump to an ostream
 */
ostream& operator<<(ostream& os, const Point2D& point) {
  Point2D tmp = point;
  os<<"("<<tmp.getX()<<","<<tmp.getY()<<")";
  return os;
}
