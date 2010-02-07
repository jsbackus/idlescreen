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

#ifndef __TEST_POINT2D_H__
#define __TEST_POINT2D_H__

#include <QtTest/QtTest>
#include "../Point2D.h"

class Test_Point2D: public QObject {
  Q_OBJECT

 private slots:
  void Test_GetSetX();
  void Test_GetSetY();
  void Test_SetValue();
  void Test_Constructor();
  void Test_OperatorSet();
  void Test_OperatorEqual();
  void Test_OperatorNotEqual();
  void Test_OperatorMultiply();
  void Test_OperatorDivide();
  void Test_OperatorMultiplyEqual();
  void Test_OperatorDivideEqual();
  void Test_OperatorOstream();

 private:
};

#endif
