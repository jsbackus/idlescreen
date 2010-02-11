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
 * This set of classes is for unit testing Point2D.
 */

#ifndef __TEST_VECTOR2D_H__
#define __TEST_VECTOR2D_H__

#include <QtTest/QtTest>
#include "../Vector2D.h"

class Test_Vector2D: public QObject {
  Q_OBJECT

 private slots:
  void Test_GetSetX();
  void Test_GetSetY();
  void Test_SetValue();
  void Test_Constructor();
  void Test_Magnitude();
  void Test_Normalize();
  void Test_GetNormal();
  void Test_Dot();
  void Test_IsParallelTo();
  void Test_IsNormalTo();
  void Test_GetAngle();
  void Test_Rotate();
  void Test_GetPoint();
  void Test_IsOnLine();
  void Test_GetIntersectingPt();

  void Test_OperatorSet();
  void Test_OperatorEqual();
  void Test_OperatorNotEqual();
  void Test_OperatorScalarMultiplyEqual();
  void Test_OperatorScalarMultiply();
  void Test_OperatorScalarDivideEqual();
  void Test_OperatorScalarDivision();
  void Test_VectorAddEqual();
  void Test_VectorAddition();
  void Test_VectorSubEqual();
  void Test_VectorSubtraction();
  void Test_PointVectorAddition();
  void Test_PointVectorSubtraction();
  void Test_PointSubtraction();
  void Test_OperatorOstream();

 private:
};

#endif
