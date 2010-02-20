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
//#include <iostream>
#include <sstream>
#include <string>
using namespace std;

#include "Test_Vector2D.h"

void Test_Vector2D::Test_GetSetX() {
  Vector2D testVector;

  // set to a positive number
  testVector.setX(5.1);
  QVERIFY(testVector.getX() == 5.1);
  QVERIFY(testVector.getY() == 0.0);

  // set to default
  testVector.setX();
  QVERIFY(testVector.getX() == 0.0);
  QVERIFY(testVector.getY() == 0.0);

  // set to a negative number
  testVector.setX(-1000.4);
  QVERIFY(testVector.getX() == -1000.4);
  QVERIFY(testVector.getY() == 0.0);
}

void Test_Vector2D::Test_GetSetY() {
  Vector2D testVector;

  // set to a positive number
  testVector.setY(7.2);
  QVERIFY(testVector.getX() == 0.0);
  QVERIFY(testVector.getY() == 7.2);

  // set to default
  testVector.setY();
  QVERIFY(testVector.getX() == 0.0);
  QVERIFY(testVector.getY() == 0.0);

  // set to a negative number
  testVector.setY(-1413.9);
  QVERIFY(testVector.getX() == 0.0);
  QVERIFY(testVector.getY() == -1413.9);
}

void Test_Vector2D::Test_SetValue() {
  Vector2D testVector;
  double xVal;
  double yVal;
  
  // set to positive numbers
  xVal = 12.19;
  yVal = 0.194;
  testVector.setValue(xVal,yVal);
  QVERIFY(testVector.getX() == xVal);
  QVERIFY(testVector.getY() == yVal);

  // set to default
  testVector.setValue();
  QVERIFY(testVector.getX() == 0.0);
  QVERIFY(testVector.getY() == 0.0);

  // set to a negative number
  xVal = -0.294;
  yVal = -194.109;
  testVector.setValue(xVal,yVal);
  QVERIFY(testVector.getX() == xVal);
  QVERIFY(testVector.getY() == yVal);
}

void Test_Vector2D::Test_Constructor() {
  // set to positive numbers
  Vector2D testVectorA(5.4,7.2);
  QVERIFY(testVectorA.getX() == 5.4);
  QVERIFY(testVectorA.getY() == 7.2);

  // set to default
  Vector2D testVectorB;
  QVERIFY(testVectorB.getX() == 0.0);
  QVERIFY(testVectorB.getY() == 0.0);

  // set to a negative number
  Vector2D testVectorC(-5923.1,-21734.1);
  QVERIFY(testVectorC.getX() == -5923.1);
  QVERIFY(testVectorC.getY() == -21734.1);
}
void Test_Vector2D::Test_Magnitude() {
  double xVal;
  double yVal;

  // both positive numbers
  xVal = 1.92;
  yVal = 0.32;
  Vector2D testVectorA(xVal, yVal);
  // we are assuming that the math function sqrt is ok...
  QVERIFY(testVectorA.magnitude() == sqrt(xVal*xVal+yVal*yVal));

  // x pos, y neg numbers
  xVal = 274.13;
  yVal = -192.68;
  Vector2D testVectorB(xVal, yVal);
  // we are assuming that the math function sqrt is ok...
  QVERIFY(testVectorB.magnitude() == sqrt(xVal*xVal+yVal*yVal));

  // x neg, y pos numbers
  xVal = -0.719;
  yVal = 1.768;
  Vector2D testVectorC(xVal, yVal);
  // we are assuming that the math function sqrt is ok...
  QVERIFY(testVectorC.magnitude() == sqrt(xVal*xVal+yVal*yVal));

  // both negative numbers
  xVal = -173.43;
  yVal = -297.67;
  Vector2D testVectorD(xVal, yVal);
  // we are assuming that the math function sqrt is ok...
  QVERIFY(testVectorD.magnitude() == sqrt(xVal*xVal+yVal*yVal));

  // set to default
  Vector2D testVectorE;
  QVERIFY(testVectorE.magnitude() == sqrt(0.0));
}
void Test_Vector2D::Test_Normalize() {
  double xVal;
  double yVal;

  // both positive numbers
  xVal = 1.92;
  yVal = 0.32;
  Vector2D testVectorA(xVal, yVal);
  testVectorA.normalize();
  QVERIFY(testVectorA.magnitude() == 1.0);

  // x pos, y neg numbers
  xVal = 274.13;
  yVal = -192.68;
  Vector2D testVectorB(xVal, yVal);
  testVectorB.normalize();
  QVERIFY(testVectorB.magnitude() == 1.0);

  // x neg, y pos numbers
  xVal = -0.719;
  yVal = 1.768;
  Vector2D testVectorC(xVal, yVal);
  testVectorC.normalize();
  QVERIFY(testVectorC.magnitude() == 1.0);

  // both negative numbers
  xVal = -173.43;
  yVal = -297.67;
  Vector2D testVectorD(xVal, yVal);
  testVectorD.normalize();
  QVERIFY(testVectorD.magnitude() == 1.0);

  // set to default
  Vector2D testVectorE;
  testVectorE.normalize();
  QVERIFY(testVectorE.magnitude() == 0.0);
}
void Test_Vector2D::Test_GetNormal() {
  double xVal;
  double yVal;
  double mag;
  Vector2D testVector;

  // both positive numbers
  xVal = 1.92;
  yVal = 0.32;
  mag = sqrt(xVal*xVal+yVal*yVal);
  Vector2D testVectorA(xVal, yVal);
  testVector = testVectorA.getNormal();
  QVERIFY(testVector.getX() == yVal * -1.0/mag);
  QVERIFY(testVector.getY() == xVal/mag);

  // x pos, y neg numbers
  xVal = 274.13;
  yVal = -192.68;
  mag = sqrt(xVal*xVal+yVal*yVal);
  Vector2D testVectorB(xVal, yVal);
  testVector = testVectorB.getNormal();
  QVERIFY(testVector.getX() == yVal * -1.0/mag);
  QVERIFY(testVector.getY() == xVal/mag);

  // x neg, y pos numbers
  xVal = -0.719;
  yVal = 1.768;
  mag = sqrt(xVal*xVal+yVal*yVal);
  Vector2D testVectorC(xVal, yVal);
  testVector = testVectorC.getNormal();
  QVERIFY(testVector.getX() == yVal * -1.0/mag);
  QVERIFY(testVector.getY() == xVal/mag);

  // both negative numbers
  xVal = -173.43;
  yVal = -297.67;
  mag = sqrt(xVal*xVal+yVal*yVal);
  Vector2D testVectorD(xVal, yVal);
  testVector = testVectorD.getNormal();
  QVERIFY(testVector.getX() == yVal * -1.0/mag);
  QVERIFY(testVector.getY() == xVal/mag);
  
  // set to default
  Vector2D testVectorE;
  testVector = testVectorE.getNormal();
  QVERIFY(testVector.getX() == 0.0);
  QVERIFY(testVector.getY() == 0.0);
}
void Test_Vector2D::Test_Dot() {
  double xValA, xValB;
  double yValA, yValB;
  double dot;
  Vector2D testVectorA;
  Vector2D testVectorB;

  // all positive numbers
  xValA = 1.92;
  yValA = 0.32;
  xValB = 272.1;
  yValB = 173.9;
  dot = xValA*xValB+yValA*yValB;
  testVectorA.setValue(xValA, yValA);
  testVectorB.setValue(xValB, yValB);
  QVERIFY(testVectorA.dot(testVectorB) == dot);

  // x pos, one y neg numbers
  xValA = 731.4;
  yValA = 173.61;
  xValB = 14.93;
  yValB = -56.173;
  dot = xValA*xValB+yValA*yValB;
  testVectorA.setValue(xValA, yValA);
  testVectorB.setValue(xValB, yValB);
  QVERIFY(testVectorA.dot(testVectorB) == dot);

  // one x neg, y pos numbers
  xValA = -0.7193;
  yValA = 0.361;
  xValB = 1.934;
  yValB = 1.431;
  dot = xValA*xValB+yValA*yValB;
  testVectorA.setValue(xValA, yValA);
  testVectorB.setValue(xValB, yValB);
  QVERIFY(testVectorA.dot(testVectorB) == dot);

  // all negative numbers
  xValA = -17.31;
  yValA = -16.21;
  xValB = -0.932;
  yValB = -0.871;
  dot = xValA*xValB+yValA*yValB;
  testVectorA.setValue(xValA, yValA);
  testVectorB.setValue(xValB, yValB);
  QVERIFY(testVectorA.dot(testVectorB) == dot);
  
  // one set to default
  xValA = 1.92;
  yValA = 0.32;
  xValB = 0.0;
  yValB = 0.0;
  dot = xValA*xValB+yValA*yValB;
  testVectorA.setValue(xValA, yValA);
  testVectorB.setValue();
  QVERIFY(testVectorA.dot(testVectorB) == dot);

  // both set to default
  dot = 0.0;
  testVectorA.setValue();
  testVectorB.setValue();
  QVERIFY(testVectorA.dot(testVectorB) == dot);
}

void Test_Vector2D::Test_IsParallelTo() {
  Vector2D testVectorA;
  Vector2D testVectorB;

  // both positive slopes
  testVectorA.setValue(3.3,5.7);
  testVectorB.setValue(3.3*9.7,5.7*9.7);
  QVERIFY(testVectorA.isParallelTo(testVectorB));
  QVERIFY(testVectorB.isParallelTo(testVectorA));

  // one positive, one negative
  testVectorA.setValue(-4.7,6.9);
  testVectorB.setValue(4.7*191.6,6.9*191.6);
  QVERIFY(testVectorA.isParallelTo(testVectorB));
  QVERIFY(testVectorB.isParallelTo(testVectorA));

  // both negative
  testVectorA.setValue(-12.1,17.73);
  testVectorB.setValue(-12.1*4.6,17.73*4.6);
  QVERIFY(testVectorA.isParallelTo(testVectorB));
  QVERIFY(testVectorB.isParallelTo(testVectorA));

  // not parallel
  testVectorA.setValue(6.7,9.1);
  testVectorB.setValue(6.7*13.4,9.1*8.3);
  QVERIFY(!testVectorA.isParallelTo(testVectorB));
  QVERIFY(!testVectorB.isParallelTo(testVectorA));

  // one set to default
  testVectorA.setValue(17.1,4.3);
  testVectorB.setValue();
  QVERIFY(!testVectorA.isParallelTo(testVectorB));
  QVERIFY(!testVectorB.isParallelTo(testVectorA));

  // both set to default
  testVectorA.setValue();
  testVectorB.setValue();
  QVERIFY(!testVectorA.isParallelTo(testVectorB));
  QVERIFY(!testVectorB.isParallelTo(testVectorA));
}

void Test_Vector2D::Test_IsNormalTo() {
  Vector2D testVectorA;
  Vector2D testVectorB;

  // direct normal
  testVectorA.setValue(3.3,5.7);
  testVectorB.setValue(5.7,-3.3);
  QVERIFY(testVectorA.isNormalTo(testVectorB));
  QVERIFY(testVectorB.isNormalTo(testVectorA));

  // with one scaled
  testVectorA.setValue(3.3,5.7);
  testVectorB.setValue(5.7*9.7,-3.3*9.7);
  QVERIFY(testVectorA.isNormalTo(testVectorB));
  QVERIFY(testVectorB.isNormalTo(testVectorA));

  // direct normal, the other direction
  testVectorA.setValue(4.7,6.9);
  testVectorB.setValue(-6.9,4.7);
  QVERIFY(testVectorA.isNormalTo(testVectorB));
  QVERIFY(testVectorB.isNormalTo(testVectorA));

  // normal with multiplier, the other direction
  testVectorA.setValue(4.7,6.9);
  testVectorB.setValue(-6.9*119.31,4.7*119.31);
  QVERIFY(testVectorA.isNormalTo(testVectorB));
  QVERIFY(testVectorB.isNormalTo(testVectorA));

  // try a set of parallel vectors...
  testVectorA.setValue(12.1,17.73);
  testVectorB.setValue(12.1*4.6,17.73*4.6);
  QVERIFY(!testVectorA.isNormalTo(testVectorB));
  QVERIFY(!testVectorB.isNormalTo(testVectorA));

  // not normal
  testVectorA.setValue(6.7,9.1);
  testVectorB.setValue(-9.1*13.4,6.7*8.3);
  QVERIFY(!testVectorA.isNormalTo(testVectorB));
  QVERIFY(!testVectorB.isNormalTo(testVectorA));

  // one set to default
  testVectorA.setValue(17.1,4.3);
  testVectorB.setValue();
  QVERIFY(!testVectorA.isNormalTo(testVectorB));
  QVERIFY(!testVectorB.isNormalTo(testVectorA));

  // both set to default
  testVectorA.setValue();
  testVectorB.setValue();
  QVERIFY(!testVectorA.isNormalTo(testVectorB));
  QVERIFY(!testVectorB.isNormalTo(testVectorA));
}

void Test_Vector2D::Test_GetAngle() {
  Vector2D testVectorA;
  Vector2D testVectorB;

  // Parallel vectors
  double angle = 0.0;
  double mag = 3.5;
  testVectorA.setValue(1.0,0.0);
  testVectorB.setValue(mag*cos(angle),mag*sin(angle));
  QVERIFY(relativeCompare(testVectorA.getAngle(testVectorB),angle, 1.0e-6));
  QVERIFY(relativeCompare(testVectorB.getAngle(testVectorA),angle, 1.0e-6));

  // Perpendicular vectors
  angle = 1.570763;
  mag = 17.4;
  testVectorA.setValue(1.0,0.0);
  testVectorB.setValue(mag*cos(angle),mag*sin(angle));
  QVERIFY(relativeCompare(testVectorA.getAngle(testVectorB),angle, 1.0e-6));
  QVERIFY(relativeCompare(testVectorB.getAngle(testVectorA),angle, 1.0e-6));

  // Acute vectors
  angle = 0.478115;
  mag = 0.653;
  testVectorA.setValue(1.0,0.0);
  testVectorB.setValue(mag*cos(angle),mag*sin(angle));
  QVERIFY(relativeCompare(testVectorA.getAngle(testVectorB),angle, 1.0e-6));
  QVERIFY(relativeCompare(testVectorB.getAngle(testVectorA),angle, 1.0e-6));

  // Obtuse vectors
  angle = 3.071099;
  mag = 19273.1;
  testVectorA.setValue(1.0,0.0);
  testVectorB.setValue(mag*cos(angle),mag*sin(angle));
  QVERIFY(relativeCompare(testVectorA.getAngle(testVectorB),angle, 1.0e-6));
  QVERIFY(relativeCompare(testVectorB.getAngle(testVectorA),angle, 1.0e-6));

  // both set to default
  angle = 0.0;
  testVectorA.setValue();
  testVectorB.setValue();
  QVERIFY(relativeCompare(testVectorA.getAngle(testVectorB),angle, 1.0e-6));
  QVERIFY(relativeCompare(testVectorB.getAngle(testVectorA),angle, 1.0e-6));
}

void Test_Vector2D::Test_Rotate() {
  Vector2D testVector;
  double angle;
  double x;
  double y;
  // Parallel vectors
  angle = 0.0;
  x = 1.345;
  y = 2.714;
  testVector.setValue(x,y);
  testVector.rotate(angle);
  QVERIFY(relativeCompare(testVector.getX(),x*cos(angle)-y*sin(angle), 1.0e-6));
  QVERIFY(relativeCompare(testVector.getY(),x*sin(angle)+y*cos(angle), 1.0e-6));

  // perpendicular vectors
  angle = 1.570796;
  x = 1.345;
  y = 2.714;
  testVector.setValue(x,y);
  testVector.rotate(angle);
  QVERIFY(relativeCompare(testVector.getX(),x*cos(angle)-y*sin(angle), 1.0e-6));
  QVERIFY(relativeCompare(testVector.getY(),x*sin(angle)+y*cos(angle), 1.0e-6));

  // 180 degrees
  angle = 3.1415927;
  x = 1.345;
  y = 2.714;
  testVector.setValue(x,y);
  testVector.rotate(angle);
  QVERIFY(relativeCompare(testVector.getX(),x*cos(angle)-y*sin(angle), 1.0e-6));
  QVERIFY(relativeCompare(testVector.getY(),x*sin(angle)+y*cos(angle), 1.0e-6));

  // some positive angle
  angle = 0.558505;
  x = 1.345;
  y = 2.714;
  testVector.setValue(x,y);
  testVector.rotate(angle);
  QVERIFY(relativeCompare(testVector.getX(),x*cos(angle)-y*sin(angle), 1.0e-6));
  QVERIFY(relativeCompare(testVector.getY(),x*sin(angle)+y*cos(angle), 1.0e-6));

  // some negative angle
  angle = -0.558505;
  x = 1.345;
  y = 2.714;
  testVector.setValue(x,y);
  testVector.rotate(angle);
  QVERIFY(relativeCompare(testVector.getX(),x*cos(angle)-y*sin(angle), 1.0e-6));
  QVERIFY(relativeCompare(testVector.getY(),x*sin(angle)+y*cos(angle), 1.0e-6));

  // set to default
  angle = 1.954769;
  testVector.setValue();
  testVector.rotate(angle);
  QVERIFY(relativeCompare(testVector.getX(),0.0, 1.0e-6));
  QVERIFY(relativeCompare(testVector.getY(),0.0, 1.0e-6));
}

void Test_Vector2D::Test_GetPoint() {
  Vector2D vector;
  Point2D origin;
  Point2D result;
  double scalar;

  // simple case
  vector.setValue(1.0,1.0);
  origin.setValue(2.0,5.0);
  scalar = 12.0;
  result = vector.getPoint(origin, scalar);
  QVERIFY(relativeCompare(result.getX(),vector.getX()*scalar+origin.getX(), 
			  1.0e-6));
  QVERIFY(relativeCompare(result.getY(),vector.getY()*scalar+origin.getY(), 
			  1.0e-6));

  // fractional case
  vector.setValue(10.0,10.0);
  origin.setValue(17.3,91.4);
  scalar = 0.5;
  result = vector.getPoint(origin, scalar);
  QVERIFY(relativeCompare(result.getX(),vector.getX()*scalar+origin.getX(), 
			  1.0e-6));
  QVERIFY(relativeCompare(result.getY(),vector.getY()*scalar+origin.getY(), 
			  1.0e-6));

  // negative case
  vector.setValue(1.0,1.0);
  vector.normalize();
  origin.setValue(0.0,0.0);
  scalar = -1914.1341;
  result = vector.getPoint(origin, scalar);
  QVERIFY(relativeCompare(result.getX(),vector.getX()*scalar+origin.getX(), 
			  1.0e-6));
  QVERIFY(relativeCompare(result.getY(),vector.getY()*scalar+origin.getY(), 
			  1.0e-6));

  // default case
  vector.setValue();
  origin.setValue();
  scalar = 12.0;
  result = vector.getPoint(origin, scalar);
  QVERIFY(relativeCompare(result.getX(),0.0, 1.0e-6));
  QVERIFY(relativeCompare(result.getY(),0.0, 1.0e-6));

}

void Test_Vector2D::Test_GetDistanceToPoint() {
  Vector2D vector;
  Point2D origin;
  Point2D testPoint;

  // positive slope, point on line
  vector.setValue(1.0,1.0);
  origin.setValue(2.0,5.0);
  testPoint.setValue(5.3,8.3);
  QVERIFY(relativeCompare(vector.getDistanceToPoint(origin, testPoint), 
						    3.3, 1e-6));

  // positive slope, point not on line
  vector.setValue(1.0,1.0);
  origin.setValue(2.0,5.0);
  testPoint.setValue(5.3,7.3);
  QVERIFY(isnan(vector.getDistanceToPoint(origin, testPoint)));

  // vertical vector
  vector.setValue(4.0,0.0);
  origin.setValue(2.0,5.0);
  testPoint.setValue(0.3,5.0);
  QVERIFY(relativeCompare(vector.getDistanceToPoint(origin, testPoint), 
						    -1.7/4.0, 1e-6));

  // horizontal vector
  vector.setValue(0.0,1.7);
  origin.setValue(7.1,5.0);
  testPoint.setValue(7.1,8.3);
  QVERIFY(relativeCompare(vector.getDistanceToPoint(origin, testPoint), 
						    3.3/1.7, 1e-6));

  // default vector
  // positive slope, point on line
  vector.setValue();
  origin.setValue(2.0,5.0);
  testPoint.setValue(5.3,8.3);
  QVERIFY(isnan(vector.getDistanceToPoint(origin, testPoint)));
}

void Test_Vector2D::Test_IsOnLine() {
  Vector2D vector;
  Point2D origin;
  Point2D testPoint;

  // positive slope
  vector.setValue(1.0,1.0);
  origin.setValue(2.0,5.0);
  testPoint.setValue(4.7,7.7);
  QVERIFY(vector.isOnLine(origin,testPoint));

  // negative slope
  vector.setValue(1.0,1.0);
  origin.setValue(2.0,5.0);
  testPoint.setValue(-1.3,1.7);
  QVERIFY(vector.isOnLine(origin,testPoint));

  // not on line
  vector.setValue(1.0,1.0);
  origin.setValue(2.0,5.0);
  testPoint.setValue(4.7,5.7);
  QVERIFY(!vector.isOnLine(origin,testPoint));

  // same point
  vector.setValue(1.0,1.0);
  origin.setValue(2.0,5.0);
  testPoint.setValue(2.0,5.0);
  QVERIFY(vector.isOnLine(origin,testPoint));

  // default vector
  vector.setValue();
  origin.setValue(2.0,5.0);
  testPoint.setValue(2.0,5.0);
  QVERIFY(vector.isOnLine(origin,testPoint));
}

void Test_Vector2D::Test_Intersections() {
  Vector2D vA;
  Vector2D vB;
  Point2D pA;
  Point2D pB;
  Point2D pI;
  bool bResult;

  // 60 degree case, multiplier
  vA.setValue(1.0,1.0);
  pA.setValue(2.0,3.0);
  vB.setValue(1.0,1.0);
  vB.rotate(1.047198);
  pB.setValue(-4.0,5.0);
  QVERIFY(relativeCompare(vA.getIntersectionMul(pA, vB, pB),-4.3093987, 1e-6));

  // Normal case
  vA.setValue(1.0,1.0);
  pA.setValue(2.0,3.0);
  vB.setValue(-1.0,1.0);
  pB.setValue(-4.0,1.0);
  bResult=vA.getIntersectionPt(pA, vB, pB, pI);
  QVERIFY(bResult);
  QVERIFY(relativeCompare(pI.getX(),-2.0, 1e-6));
  QVERIFY(relativeCompare(pI.getY(),-1.0, 1e-6));

  // Parallel case
  vA.setValue(1.0,2.0);
  pA.setValue(4.0,1.0);
  vB.setValue(8.0,16.0);
  pB.setValue(17.0,15.0);
  bResult=vA.getIntersectionPt(pA, vB, pB, pI);
  QVERIFY(!bResult);
  QVERIFY(isnan(pI.getX()));
  QVERIFY(isnan(pI.getY()));

  // Normal case, same point
  vA.setValue(1.0,4.0);
  pA.setValue(7.0,9.0);
  vB.setValue(-4.0,1.0);
  pB.setValue(7.0,9.0);
  bResult=vA.getIntersectionPt(pA, vB, pB, pI);
  QVERIFY(bResult);
  QVERIFY(relativeCompare(pI.getX(),7.0, 1e-6));
  QVERIFY(relativeCompare(pI.getY(),9.0, 1e-6));

  // Parallel case, same point
  vA.setValue(1.0,1.0);
  pA.setValue(2.0,3.0);
  vB.setValue(1.0,1.0);
  pB.setValue(-4.0,5.0);
  bResult=vA.getIntersectionPt(pA, vB, pB, pI);
  QVERIFY(!bResult);
  QVERIFY(isnan(pI.getX()));
  QVERIFY(isnan(pI.getY()));

  // 60 degree case
  vA.setValue(1.0,1.0);
  pA.setValue(2.0,3.0);
  vB.setValue(1.0,1.0);
  vB.rotate(1.047198);
  pB.setValue(-4.0,5.0);
  bResult=vA.getIntersectionPt(pA, vB, pB, pI);
  QVERIFY(bResult);
  QVERIFY(relativeCompare(pI.getX(),-2.3093987, 1e-6));
  QVERIFY(relativeCompare(pI.getY(),-1.3093987, 1e-6));

  // 60 degree case, same point
  vA.setValue(1.0,1.0);
  pA.setValue(2.0,3.0);
  vB.setValue(1.0,1.0);
  vB.rotate(1.047198);
  pB.setValue(2.0,3.0);
  bResult=vA.getIntersectionPt(pA, vB, pB, pI);
  QVERIFY(bResult);
  QVERIFY(relativeCompare(pI.getX(),2.0, 1e-6));
  QVERIFY(relativeCompare(pI.getY(),3.0, 1e-6));

  // first vector is default
  vA.setValue();
  pA.setValue(2.0,3.0);
  vB.setValue(-1.0,1.0);
  pB.setValue(-4.0,5.0);
  bResult=vA.getIntersectionPt(pA, vB, pB, pI);
  QVERIFY(!bResult);
  QVERIFY(isnan(pI.getX()));
  QVERIFY(isnan(pI.getY()));

  // second vector is default
  vA.setValue(1.7,4.9);
  pA.setValue(2.0,3.0);
  vB.setValue();
  pB.setValue(-4.0,5.0);
  bResult=vA.getIntersectionPt(pA, vB, pB, pI);
  QVERIFY(!bResult);
  QVERIFY(isnan(pI.getX()));
  QVERIFY(isnan(pI.getY()));
}

void Test_Vector2D::Test_OperatorSet() {
  Vector2D testVector;

  // set to positive numbers
  Vector2D testVectorA(5.4,7.2);
  testVector = testVectorA;
  QVERIFY(testVector.getX() == testVectorA.getX());
  QVERIFY(testVector.getY() == testVectorA.getY());

  // set to default
  Vector2D testVectorB;
  testVector = testVectorB;
  QVERIFY(testVector.getX() == testVectorB.getX());
  QVERIFY(testVector.getY() == testVectorB.getY());

  // set to a negative number
  Vector2D testVectorC(-5923.1,-21734.1);
  testVector = testVectorC;
  QVERIFY(testVector.getX() == testVectorC.getX());
  QVERIFY(testVector.getY() == testVectorC.getY());
}

void Test_Vector2D::Test_OperatorEqual() {
  Vector2D testVectorA(5.4,7.2);
  Vector2D testVectorB;
  Vector2D testVectorC(-5923.1,-21734.1);

  Vector2D testVector = testVectorA;
  QVERIFY(testVector == testVectorA);
  QVERIFY(!(testVector == testVectorB));
  QVERIFY(!(testVector == testVectorC));

  testVector = testVectorB;
  QVERIFY(!(testVector == testVectorA));
  QVERIFY(testVector == testVectorB);
  QVERIFY(!(testVector == testVectorC));

  testVector = testVectorC;
  QVERIFY(!(testVector == testVectorA));
  QVERIFY(!(testVector == testVectorB));
  QVERIFY(testVector == testVectorC);
}

void Test_Vector2D::Test_OperatorNotEqual() {
  Vector2D testVectorA(5.4,7.2);
  Vector2D testVectorB;
  Vector2D testVectorC(-5923.1,-21734.1);

  Vector2D testVector = testVectorA;
  QVERIFY(!(testVector != testVectorA));
  QVERIFY(testVector != testVectorB);
  QVERIFY(testVector != testVectorC);

  testVector = testVectorB;
  QVERIFY(testVector != testVectorA);
  QVERIFY(!(testVector != testVectorB));
  QVERIFY(testVector != testVectorC);

  testVector = testVectorC;
  QVERIFY(testVector != testVectorA);
  QVERIFY(testVector != testVectorB);
  QVERIFY(!(testVector != testVectorC));
}

void Test_Vector2D::Test_OperatorScalarMultiplyEqual() {
  Vector2D testVector;
  double scalar;
  double x;
  double y;

  // simple case
  x = 17.234;
  y = 1.1091;
  scalar = 0.9324;
  testVector.setValue(x,y);
  testVector *= scalar;
  QVERIFY(relativeCompare(testVector.getX(),x*scalar,1.0e-6));
  QVERIFY(relativeCompare(testVector.getY(),y*scalar,1.0e-6));

  // scalar is 0.0
  x = 17.234;
  y = 1.1091;
  scalar = 0.0;
  testVector.setValue(x,y);
  testVector *= scalar;
  QVERIFY(relativeCompare(testVector.getX(),0.0,1.0e-6));
  QVERIFY(relativeCompare(testVector.getY(),0.0,1.0e-6));

  // vector is default
  scalar = 0.9324;
  testVector.setValue();
  testVector *= scalar;
  QVERIFY(relativeCompare(testVector.getX(),0.0,1.0e-6));
  QVERIFY(relativeCompare(testVector.getY(),0.0,1.0e-6));
}

void Test_Vector2D::Test_OperatorScalarMultiply() {
  Vector2D testVector;
  Vector2D result;
  double scalar;
  double x;
  double y;

  // Test V * S
  // simple case
  x = 17.234;
  y = 1.1091;
  scalar = 0.9324;
  testVector.setValue(x,y);
  result = testVector * scalar;
  QVERIFY(relativeCompare(result.getX(),x*scalar,1.0e-6));
  QVERIFY(relativeCompare(result.getY(),y*scalar,1.0e-6));

  // scalar is 0.0
  x = 17.234;
  y = 1.1091;
  scalar = 0.0;
  testVector.setValue(x,y);
  result = testVector * scalar;
  QVERIFY(relativeCompare(result.getX(),x*scalar,1.0e-6));
  QVERIFY(relativeCompare(result.getY(),y*scalar,1.0e-6));

  // vector is default
  scalar = 0.9324;
  testVector.setValue();
  result = testVector * scalar;
  QVERIFY(relativeCompare(result.getX(),0.0,1.0e-6));
  QVERIFY(relativeCompare(result.getY(),0.0,1.0e-6));

  // Test S * V
  // simple case
  x = -0.9134;
  y = 21.234;
  scalar = 17.143;
  testVector.setValue(x,y);
  result = scalar * testVector;
  QVERIFY(relativeCompare(result.getX(),x*scalar,1.0e-6));
  QVERIFY(relativeCompare(result.getY(),y*scalar,1.0e-6));

  // scalar is 0.0
  x = -0.9134;
  y = 21.234;
  scalar = 0.0;
  testVector.setValue(x,y);
  result = scalar * testVector;
  QVERIFY(relativeCompare(result.getX(),x*scalar,1.0e-6));
  QVERIFY(relativeCompare(result.getY(),y*scalar,1.0e-6));

  // vector is default
  scalar = 17.143;
  testVector.setValue();
  result = scalar * testVector;
  QVERIFY(relativeCompare(result.getX(),0.0,1.0e-6));
  QVERIFY(relativeCompare(result.getY(),0.0,1.0e-6));
}

void Test_Vector2D::Test_OperatorScalarDivideEqual() {
  Vector2D testVector;
  double scalar;
  double x;
  double y;

  // simple case
  x = 17.234;
  y = 1.1091;
  scalar = 0.9324;
  testVector.setValue(x,y);
  testVector /= scalar;
  QVERIFY(relativeCompare(testVector.getX(),x/scalar,1.0e-6));
  QVERIFY(relativeCompare(testVector.getY(),y/scalar,1.0e-6));

  // scalar is 0.0
  x = 17.234;
  y = 1.1091;
  scalar = 0.0;
  testVector.setValue(x,y);
  testVector /= scalar;
  QVERIFY(testVector.getX() == x/scalar);
  QVERIFY(testVector.getY() == y/scalar);

  // vector is default
  scalar = 0.9324;
  testVector.setValue();
  testVector /= scalar;
  QVERIFY(relativeCompare(testVector.getX(),0.0,1.0e-6));
  QVERIFY(relativeCompare(testVector.getY(),0.0,1.0e-6));
}

void Test_Vector2D::Test_OperatorScalarDivision()  {
  Vector2D testVector;
  Vector2D result;
  double scalar;
  double x;
  double y;

  // simple case
  x = 17.234;
  y = 1.1091;
  scalar = 0.9324;
  testVector.setValue(x,y);
  result = testVector / scalar;
  QVERIFY(relativeCompare(result.getX(),x/scalar,1.0e-6));
  QVERIFY(relativeCompare(result.getY(),y/scalar,1.0e-6));

  // scalar is 0.0
  x = 17.234;
  y = 1.1091;
  scalar = 0.0;
  testVector.setValue(x,y);
  result = testVector / scalar;
  QVERIFY(testVector.getX() == x/scalar);
  QVERIFY(testVector.getY() == y/scalar);

  // vector is default
  scalar = 0.9324;
  testVector.setValue();
  result = testVector / scalar;
  QVERIFY(relativeCompare(result.getX(),0.0,1.0e-6));
  QVERIFY(relativeCompare(result.getY(),0.0,1.0e-6));
}

void Test_Vector2D::Test_VectorAddEqual() {
  Vector2D testVectorA;
  Vector2D testVectorB;
  double x;
  double y;

  // simple case
  x = 1.7324;
  y = 9.1534;
  testVectorA.setValue(x,y);
  testVectorB.setValue(0.8234,17.2341);
  testVectorA += testVectorB;
  QVERIFY(relativeCompare(testVectorA.getX(),testVectorB.getX()+x,1.0e-6));
  QVERIFY(relativeCompare(testVectorA.getY(),testVectorB.getY()+y,1.0e-6));

  // rhs is default
  x = 1.7324;
  y = 9.1534;
  testVectorA.setValue(x,y);
  testVectorB.setValue();
  testVectorA += testVectorB;
  QVERIFY(relativeCompare(testVectorA.getX(),x,1.0e-6));
  QVERIFY(relativeCompare(testVectorA.getY(),y,1.0e-6));

  // lhs is default
  testVectorA.setValue();
  testVectorB.setValue(0.8234,17.2341);
  testVectorA += testVectorB;
  QVERIFY(relativeCompare(testVectorA.getX(),testVectorB.getX(),1.0e-6));
  QVERIFY(relativeCompare(testVectorA.getY(),testVectorB.getY(),1.0e-6));
}

void Test_Vector2D::Test_VectorAddition() {
  Vector2D testVectorA;
  Vector2D testVectorB;
  Vector2D result;
  double x;
  double y;

  // simple case
  x = 1.7324;
  y = 9.1534;
  testVectorA.setValue(x,y);
  testVectorB.setValue(0.8234,17.2341);
  result = testVectorA + testVectorB;
  QVERIFY(relativeCompare(result.getX(),x+testVectorB.getX(),1.0e-6));
  QVERIFY(relativeCompare(result.getY(),y+testVectorB.getY(),1.0e-6));

  // rhs is default
  x = 1.7324;
  y = 9.1534;
  testVectorA.setValue(x,y);
  testVectorB.setValue();
  result = testVectorA + testVectorB;
  QVERIFY(relativeCompare(result.getX(),x+testVectorB.getX(),1.0e-6));
  QVERIFY(relativeCompare(result.getY(),y+testVectorB.getY(),1.0e-6));

  // lhs is default
  testVectorA.setValue();
  testVectorB.setValue(0.8234,17.2341);
  result = testVectorA + testVectorB;
  QVERIFY(relativeCompare(result.getX(),0.0+testVectorB.getX(),1.0e-6));
  QVERIFY(relativeCompare(result.getY(),0.0+testVectorB.getY(),1.0e-6));
}

void Test_Vector2D::Test_VectorSubEqual() {
  Vector2D testVectorA;
  Vector2D testVectorB;
  Vector2D result;
  double x;
  double y;

  // simple case
  x = 1.7324;
  y = 9.1534;
  testVectorA.setValue(x,y);
  testVectorB.setValue(0.8234,17.2341);
  testVectorA -= testVectorB;
  QVERIFY(relativeCompare(testVectorA.getX(),x-testVectorB.getX(),1.0e-6));
  QVERIFY(relativeCompare(testVectorA.getY(),y-testVectorB.getY(),1.0e-6));

  // rhs is default
  x = 1.7324;
  y = 9.1534;
  testVectorA.setValue(x,y);
  testVectorB.setValue();
  testVectorA -= testVectorB;
  QVERIFY(relativeCompare(testVectorA.getX(),x-testVectorB.getX(),1.0e-6));
  QVERIFY(relativeCompare(testVectorA.getY(),y-testVectorB.getY(),1.0e-6));

  // lhs is default
  testVectorA.setValue();
  testVectorB.setValue(0.8234,17.2341);
  testVectorA -= testVectorB;
  QVERIFY(relativeCompare(testVectorA.getX(),0.0-testVectorB.getX(),1.0e-6));
  QVERIFY(relativeCompare(testVectorA.getY(),0.0-testVectorB.getY(),1.0e-6));
}

void Test_Vector2D::Test_VectorSubtraction() {
  Vector2D testVectorA;
  Vector2D testVectorB;
  Vector2D result;
  double x;
  double y;

  // simple case
  x = 1.7324;
  y = 9.1534;
  testVectorA.setValue(x,y);
  testVectorB.setValue(0.8234,17.2341);
  result = testVectorA - testVectorB;
  QVERIFY(relativeCompare(result.getX(),x-testVectorB.getX(),1.0e-6));
  QVERIFY(relativeCompare(result.getY(),y-testVectorB.getY(),1.0e-6));

  // rhs is default
  x = 1.7324;
  y = 9.1534;
  testVectorA.setValue(x,y);
  testVectorB.setValue();
  result = testVectorA - testVectorB;
  QVERIFY(relativeCompare(result.getX(),x-testVectorB.getX(),1.0e-6));
  QVERIFY(relativeCompare(result.getY(),y-testVectorB.getY(),1.0e-6));

  // lhs is default
  testVectorA.setValue();
  testVectorB.setValue(0.8234,17.2341);
  result = testVectorA - testVectorB;
  QVERIFY(relativeCompare(result.getX(),0.0-testVectorB.getX(),1.0e-6));
  QVERIFY(relativeCompare(result.getY(),0.0-testVectorB.getY(),1.0e-6));
}

void Test_Vector2D::Test_PointVectorAddition() {
  Point2D p;
  Point2D result;
  Vector2D v;

  // Test_GetPoint() tests most of the functionality...

  // P+V case
  p.setValue(27.123, 191.143);
  v.setValue(13.2341, 17.0981);
  result = p+v;
  QVERIFY(relativeCompare(result.getX(),p.getX()+v.getX(), 1.0e-6));
  QVERIFY(relativeCompare(result.getY(),p.getY()+v.getY(), 1.0e-6));

  // V+P case
  p.setValue(0.324, -0.4231);
  v.setValue(1.723, 9.2341);
  result = v+p;
  QVERIFY(relativeCompare(result.getX(),p.getX()+v.getX(), 1.0e-6));
  QVERIFY(relativeCompare(result.getY(),p.getY()+v.getY(), 1.0e-6));
}

void Test_Vector2D::Test_PointVectorSubtraction() {
  Point2D p;
  Point2D result;
  Vector2D v;

  // Test_GetPoint() tests most of the functionality...

  // P-V case
  p.setValue(27.123, 191.143);
  v.setValue(13.2341, 17.0981);
  result = p-v;
  QVERIFY(relativeCompare(result.getX(),p.getX()-v.getX(), 1.0e-6));
  QVERIFY(relativeCompare(result.getY(),p.getY()-v.getY(), 1.0e-6));

}

void Test_Vector2D::Test_PointSubtraction() {
  Point2D a;
  Point2D b;
  Vector2D result;

  // positive case
  a.setValue(1.4, 5.7);
  b.setValue(6.7, 23.9);
  result = b - a;
  QVERIFY(relativeCompare(result.getX(),b.getX()-a.getX(), 1.0e-6));
  QVERIFY(relativeCompare(result.getY(),b.getY()-a.getY(), 1.0e-6));

  // negative case
  a.setValue(6.7, 23.9);
  b.setValue(1.4, 5.7);
  result = b - a;
  QVERIFY(relativeCompare(result.getX(),b.getX()-a.getX(), 1.0e-6));
  QVERIFY(relativeCompare(result.getY(),b.getY()-a.getY(), 1.0e-6));

  // both defaults case
  a.setValue();
  b.setValue();
  result = b - a;
  QVERIFY(relativeCompare(result.getX(),b.getX()-a.getX(), 1.0e-6));
  QVERIFY(relativeCompare(result.getY(),b.getY()-a.getY(), 1.0e-6));
}

void Test_Vector2D::Test_OperatorOstream() {
  double xVal;
  double yVal;

  // set to positive numbers
  xVal = 56.4;
  yVal = 17.2;

  Vector2D testVector(xVal, yVal);
  ostringstream testOSSA(ostringstream::out);
  testOSSA.precision(15);
  testOSSA << testVector;
  string testStrA = testOSSA.str();

  ostringstream controlOSSA(ostringstream::out);
  controlOSSA.precision(15);
  controlOSSA << "("<<xVal<<","<<yVal<<")";
  string controlStrA = controlOSSA.str();
  
  QVERIFY(testStrA == controlStrA);

  // set to negative numbers
  xVal = -324151.1;
  yVal = -0.1932414;

  testVector.setValue(xVal, yVal);
  ostringstream testOSSB(ostringstream::out);
  testOSSB.precision(15);
  testOSSB << testVector;
  string testStrB = testOSSB.str();

  ostringstream controlOSSB(ostringstream::out);
  controlOSSB.precision(15);
  controlOSSB << "("<<xVal<<","<<yVal<<")";
  string controlStrB = controlOSSB.str();
  
  QVERIFY(testStrB == controlStrB);
}

