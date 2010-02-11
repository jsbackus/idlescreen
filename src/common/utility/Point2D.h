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
 * This class represents a generic point in two dimensions.  All internal
 * values are stored as doubles.
 */

#include <iostream>
#include <math.h>

using namespace std;

#include "misc_funcs.h"

#ifndef __POINT2D_H__
#define __POINT2D_H__

class Point2D {

 public:

  /**
   * Constructor.
   *
   * @param x The initial x value of the vector.
   * @param y The initial y value of the vector.
   */
  Point2D(const double x=0.0, const double y=0.0);

  /**
   * Deconstructor
   */
  ~Point2D();
  
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
   * Sets both the x & y values.
   */
  void setValue(const double x=0.0, const double y=0.0);

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

  Point2D& operator=(const Point2D& other);
  bool operator==(const Point2D& other);
  bool operator!=(const Point2D& other);
  // these operations geometrically don't make sense, but are for ease of use.
  Point2D& operator*(const double val);
  Point2D& operator/(const double val);
  Point2D& operator*=(const double val);
  Point2D& operator/=(const double val);

  friend ostream& operator<<(ostream& os, const Point2D& point);

 private:
  double _x;
  double _y;

  double _epsilon;
  static double DEFAULT_EPSILON;
};

#endif
