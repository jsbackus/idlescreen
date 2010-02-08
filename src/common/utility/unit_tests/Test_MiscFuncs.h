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
 * This set of classes is for unit testing the functions in misc_funcs.
 */

#ifndef __TEST_MISC_FUNCS_H__
#define __TEST_MISC_FUNCS_H__

#include <QtTest/QtTest>
#include "../misc_funcs.h"

class Test_MiscFuncs: public QObject {
  Q_OBJECT

 private slots:
  void Test_Jrand();
  void Test_BoolToString();
  void Test_StringToBool();
  void Test_DotProduct();
  void Test_RandStr();
  void Test_BoolToCheckState();
  void Test_RoundFToI();
  void Test_RoundDToI();
  void Test_RelativeCompareF();
  void Test_RelativeCompareD();

 private:
};

#endif
