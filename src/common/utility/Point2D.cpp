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
#include "Point2D.h"

/**
 * Constructor.
 *
 * @param x The initial x value of the vector.
 * @param y The initial y value of the vector.
 */
Point2D::Point2D(const double x, const double y) {
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
  return (_x == other._x && _y == other._y);
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
