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

#include "Test_Point2D.h"

void Test_Point2D::Test_GetSetX() {
  Point2D testPoint;

  // set to a positive number
  testPoint.setX(5.1);
  QVERIFY(testPoint.getX() == 5.1);
  QVERIFY(testPoint.getY() == 0.0);

  // set to default
  testPoint.setX();
  QVERIFY(testPoint.getX() == 0.0);
  QVERIFY(testPoint.getY() == 0.0);

  // set to a negative number
  testPoint.setX(-1000.4);
  QVERIFY(testPoint.getX() == -1000.4);
  QVERIFY(testPoint.getY() == 0.0);
}

void Test_Point2D::Test_GetSetY() {
  Point2D testPoint;

  // set to a positive number
  testPoint.setY(7.2);
  QVERIFY(testPoint.getX() == 0.0);
  QVERIFY(testPoint.getY() == 7.2);

  // set to default
  testPoint.setY();
  QVERIFY(testPoint.getX() == 0.0);
  QVERIFY(testPoint.getY() == 0.0);

  // set to a negative number
  testPoint.setY(-1413.9);
  QVERIFY(testPoint.getX() == 0.0);
  QVERIFY(testPoint.getY() == -1413.9);
}

void Test_Point2D::Test_SetValue() {
  Point2D testPoint;
  double xVal;
  double yVal;
  
  // set to positive numbers
  xVal = 12.19;
  yVal = 0.194;
  testPoint.setValue(xVal,yVal);
  QVERIFY(testPoint.getX() == xVal);
  QVERIFY(testPoint.getY() == yVal);

  // set to default
  testPoint.setValue();
  QVERIFY(testPoint.getX() == 0.0);
  QVERIFY(testPoint.getY() == 0.0);

  // set to a negative number
  xVal = -0.294;
  yVal = -194.109;
  testPoint.setValue(xVal,yVal);
  QVERIFY(testPoint.getX() == xVal);
  QVERIFY(testPoint.getY() == yVal);
}

void Test_Point2D::Test_Constructor() {
  // set to positive numbers
  Point2D testPointA(5.4,7.2);
  QVERIFY(testPointA.getX() == 5.4);
  QVERIFY(testPointA.getY() == 7.2);

  // set to default
  Point2D testPointB;
  QVERIFY(testPointB.getX() == 0.0);
  QVERIFY(testPointB.getY() == 0.0);

  // set to a negative number
  Point2D testPointC(-5923.1,-21734.1);
  QVERIFY(testPointC.getX() == -5923.1);
  QVERIFY(testPointC.getY() == -21734.1);
}

void Test_Point2D::Test_OperatorSet() {
  Point2D testPoint;

  // set to positive numbers
  Point2D testPointA(5.4,7.2);
  testPoint = testPointA;
  QVERIFY(testPoint.getX() == testPointA.getX());
  QVERIFY(testPoint.getY() == testPointA.getY());

  // set to default
  Point2D testPointB;
  testPoint = testPointB;
  QVERIFY(testPoint.getX() == testPointB.getX());
  QVERIFY(testPoint.getY() == testPointB.getY());

  // set to a negative number
  Point2D testPointC(-5923.1,-21734.1);
  testPoint = testPointC;
  QVERIFY(testPoint.getX() == testPointC.getX());
  QVERIFY(testPoint.getY() == testPointC.getY());
}

void Test_Point2D::Test_OperatorEqual() {
  Point2D testPointA(5.4,7.2);
  Point2D testPointB;
  Point2D testPointC(-5923.1,-21734.1);

  Point2D testPoint = testPointA;
  QVERIFY(testPoint == testPointA);
  QVERIFY(!(testPoint == testPointB));
  QVERIFY(!(testPoint == testPointC));

  testPoint = testPointB;
  QVERIFY(!(testPoint == testPointA));
  QVERIFY(testPoint == testPointB);
  QVERIFY(!(testPoint == testPointC));

  testPoint = testPointC;
  QVERIFY(!(testPoint == testPointA));
  QVERIFY(!(testPoint == testPointB));
  QVERIFY(testPoint == testPointC);
}

void Test_Point2D::Test_OperatorNotEqual() {
  Point2D testPointA(5.4,7.2);
  Point2D testPointB;
  Point2D testPointC(-5923.1,-21734.1);

  Point2D testPoint = testPointA;
  QVERIFY(!(testPoint != testPointA));
  QVERIFY(testPoint != testPointB);
  QVERIFY(testPoint != testPointC);

  testPoint = testPointB;
  QVERIFY(testPoint != testPointA);
  QVERIFY(!(testPoint != testPointB));
  QVERIFY(testPoint != testPointC);

  testPoint = testPointC;
  QVERIFY(testPoint != testPointA);
  QVERIFY(testPoint != testPointB);
  QVERIFY(!(testPoint != testPointC));
}

void Test_Point2D::Test_OperatorMultiply() {
  double xVal = 5.4;
  double yVal = 7.2;
  double mul;
  Point2D testPoint(xVal, yVal);

  QVERIFY(testPoint.getX() == xVal);
  QVERIFY(testPoint.getY() == yVal);

  // Positive number
  mul = 4.91;
  testPoint = testPoint * mul;
  xVal *= mul;
  yVal *= mul;
  QVERIFY(testPoint.getX() == xVal);
  QVERIFY(testPoint.getY() == yVal);

  // Negative number
  mul = -0.15;
  testPoint = testPoint * mul;
  xVal *= mul;
  yVal *= mul;
  QVERIFY(testPoint.getX() == xVal);
  QVERIFY(testPoint.getY() == yVal);

  // Zero
  mul = 0.0;
  testPoint = testPoint * mul;
  xVal *= mul;
  yVal *= mul;
  QVERIFY(testPoint.getX() == xVal);
  QVERIFY(testPoint.getY() == yVal);
}

void Test_Point2D::Test_OperatorDivide() {
  double xVal = 32.149;
  double yVal = 91.43;
  double div;
  Point2D testPoint(xVal, yVal);

  QVERIFY(testPoint.getX() == xVal);
  QVERIFY(testPoint.getY() == yVal);

  // Positive number
  div = 0.7291;
  testPoint = testPoint / div;
  xVal /= div;
  yVal /= div;
  QVERIFY(testPoint.getX() == xVal);
  QVERIFY(testPoint.getY() == yVal);

  // Negative number
  div = -15.941;
  testPoint = testPoint / div;
  xVal /= div;
  yVal /= div;
  QVERIFY(testPoint.getX() == xVal);
  QVERIFY(testPoint.getY() == yVal);

  // One
  div = 1.0;
  testPoint = testPoint / div;
  xVal /= div;
  yVal /= div;
  QVERIFY(testPoint.getX() == xVal);
  QVERIFY(testPoint.getY() == yVal);

  // Zero
  div = 0.0;
  testPoint = testPoint / div;
  xVal /= div;
  yVal /= div;
  QVERIFY(testPoint.getX() == xVal);
  QVERIFY(testPoint.getY() == yVal);
}

void Test_Point2D::Test_OperatorMultiplyEqual() {
  double xVal = 1.19;
  double yVal = 910.14;
  double mul;
  Point2D testPoint(xVal, yVal);

  QVERIFY(testPoint.getX() == xVal);
  QVERIFY(testPoint.getY() == yVal);

  // Positive number
  mul = 0.1234613;
  testPoint *= mul;
  xVal *= mul;
  yVal *= mul;
  QVERIFY(testPoint.getX() == xVal);
  QVERIFY(testPoint.getY() == yVal);

  // Negative number
  mul = -7234.19;
  testPoint *= mul;
  xVal *= mul;
  yVal *= mul;
  QVERIFY(testPoint.getX() == xVal);
  QVERIFY(testPoint.getY() == yVal);

  // Zero
  mul = 0.0;
  testPoint *= mul;
  xVal *= mul;
  yVal *= mul;
  QVERIFY(testPoint.getX() == xVal);
  QVERIFY(testPoint.getY() == yVal);
}

void Test_Point2D::Test_OperatorDivideEqual() {
  double xVal = 12.1;
  double yVal = 0.7143;
  double div;
  Point2D testPoint(xVal, yVal);

  QVERIFY(testPoint.getX() == xVal);
  QVERIFY(testPoint.getY() == yVal);

  // Positive number
  div = 71.45;
  testPoint /= div;
  xVal /= div;
  yVal /= div;
  QVERIFY(testPoint.getX() == xVal);
  QVERIFY(testPoint.getY() == yVal);

  // Negative number
  div = -0.941613;
  testPoint /= div;
  xVal /= div;
  yVal /= div;
  QVERIFY(testPoint.getX() == xVal);
  QVERIFY(testPoint.getY() == yVal);

  // One
  div = 1.0;
  testPoint /= div;
  xVal /= div;
  yVal /= div;
  QVERIFY(testPoint.getX() == xVal);
  QVERIFY(testPoint.getY() == yVal);

  // Zero
  div = 0.0;
  testPoint /= div;
  xVal /= div;
  yVal /= div;
  QVERIFY(testPoint.getX() == xVal);
  QVERIFY(testPoint.getY() == yVal);
}

void Test_Point2D::Test_OperatorOstream() {
  double xVal;
  double yVal;

  // set to positive numbers
  xVal = 56.4;
  yVal = 17.2;

  Point2D testPoint(xVal, yVal);
  ostringstream testOSSA(ostringstream::out);
  testOSSA.precision(15);
  testOSSA << testPoint;
  string testStrA = testOSSA.str();

  ostringstream controlOSSA(ostringstream::out);
  controlOSSA.precision(15);
  controlOSSA << "("<<xVal<<","<<yVal<<")";
  string controlStrA = controlOSSA.str();
  
  QVERIFY(testStrA == controlStrA);

  // set to negative numbers
  xVal = -324151.1;
  yVal = -0.1932414;

  testPoint.setValue(xVal, yVal);
  ostringstream testOSSB(ostringstream::out);
  testOSSB.precision(15);
  testOSSB << testPoint;
  string testStrB = testOSSB.str();

  ostringstream controlOSSB(ostringstream::out);
  controlOSSB.precision(15);
  controlOSSB << "("<<xVal<<","<<yVal<<")";
  string controlStrB = controlOSSB.str();
  
  QVERIFY(testStrB == controlStrB);
}
