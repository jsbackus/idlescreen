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
 * This class represents a generic vector in two dimensions.  All internal
 * values are stored as doubles.
 */

#include <iostream>
#include <math.h>

using namespace std;

#include "misc_funcs.h"
#include "Point2D.h"

#ifndef __VECTOR2D_H__
#define __VECTOR2D_H__

class Vector2D {

 public:

  /**
   * Constructor.
   *
   * @param x The initial x value of the vector.
   * @param y The initial y value of the vector.
   */
  Vector2D(const double x=0.0, const double y=0.0);

  /**
   * Deconstructor
   */
  ~Vector2D();
  
  /**
   * Gets the x value.
   */
  double getX();

  /**
   * Sets the x value.
   */
  void setX(const double x=0.0);

  /**
   * Gets the y value.
   */
  double getY();

  /**
   * Sets the y value.
   */
  void setY(const double y=0.0);

  /**
   * Sets the x & y values.
   *
   * @param x The x value of the vector.
   * @param y The y value of the vector.
   */
  void setValue(const double x=0.0, const double y=0.0);

  /**
   * Gets the magnitude of the vector.
   */
  double magnitude();

  /**
   * Gets the normal of this vector.
   */
  Vector2D getNormal();

  /**
   * Normalizes this vector.
   */
  void normalize();

  /**
   * Returns the dot product of the specified vector with this vector.
   */
  double dot(const Vector2D other);

  /**
   * Returns true if the specified vector is parallel to this vector.
   */
  bool isParallelTo(const Vector2D other);

  /**
   * Returns true if the specified vector is normal to this vector.
   */
  bool isNormalTo(const Vector2D other);

  /**
   * Sets the default epsilon value.
   */
  static void setDefaultEpsilon(const double& epsilon);
  
  /**
   * Gets the default epsilon value.
   */
  static double getDefaultEpsilon();

  /**
   * Sets the epsilon value for this object.
   */
  void setEpsilon(const double &epsilon);

  /**
   * Gets the epsilon value for this object.
   */
  double getEpsilon();

  Vector2D& operator=(const Vector2D& other);
  bool operator==(const Vector2D& other);
  bool operator!=(const Vector2D& other);

#ifdef __NEEDS_DEBUGGING__
  /**
   * Calculates the angle between the specified vector and this vector.
   */
  double getAngle(const Vector2D other);

  /**
   * Checks to see if point B is on the line specified
   * by this vector and point A.
   *
   * @param a A point known to be on the line.
   * @param b The point to check.
   */
  bool isOnLine(const Point2D a, const point2D b);

  /**
   * Checks to see if point B is on the line specified
   * by this vector and point A.
   *
   * @param a A point known to be on the line.
   * @param b The point to check.
   */
  Point2D getPoint(const Point2D a, const double distance=1.0);

  /**
   * Finds the point of intersection of the line specified
   * by this vector and point A and the line specified by
   * vector V2 and point B.  Returns false if the lines
   * do not intersect.  The resulting point is put into result.
   *
   * @param a A point on the line specified by this vector.
   * @param v2 The vector describing the other line.
   * @param b A point on the line described by vector v.
   * @param result The resulting point.
   * @return True if the lines intersect, false if they do not.
   */
  bool getIntersectingPt(const Point2D pA, const Vector2D vB, 
			 const Point2D pB, Vector2D* result);

  Vector2D& operator+(const Vector2D& other);
  Vector2D& operator-(const Vector2D& other);
  Vector2D& operator*(const double val);
  Vector2D& operator*(const double val, Vector2D& v);
  Vector2D& operator/(const double val);
  Vector2D& operator+=(const Vector2D& other);
  Vector2D& operator*=(const double val);
  Vector2D& operator/=(const double val);
  Vector2D& operator-=(const Vector2D& other);

  friend Vector2D& operator-(const Point2D& initial, const Point2D& final);
  friend Point2D& operator+(const Point2D& p, const Vector2D& v);
  friend Point2D& operator+(const Vector2D& v, const Point2D& p);
#endif
  friend ostream& operator<<(ostream& os, const Vector2D& vector);

 private:
  double _x;
  double _y;

  bool _bIsNormalized; //!< cuts down on excess calculations.

  double _epsilon;
  static double DEFAULT_EPSILON;
};

#endif
