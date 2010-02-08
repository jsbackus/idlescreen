/**
 * Copyright (c) 2008 Jeff Backus.
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
#include <stdlib.h>
#include <ctime>

//#include <iostream>
using namespace std;

#include <QString>


// Reseed countdown for rnd()
int COUNT_TO_NEXT_RESEED = -1;

/**
 * Returns a random integer just like rand() does, however it will
 * periodically reseed the generator using the current time.
 */
int jrand() {
  if(COUNT_TO_NEXT_RESEED <= 0) {
    //seed the random number generator.
    srand((unsigned)time( NULL ));

    while(COUNT_TO_NEXT_RESEED <= 0) {
      COUNT_TO_NEXT_RESEED = rand();
    }
  }

  COUNT_TO_NEXT_RESEED--;
  return rand();
}

/**
 * converts a bool to a QString.
 */
QString boolToString(bool bVal) {

	return (bVal) ? QString("true") : QString("false");
}

/**
 * Converts a QString to a bool.  TRUE is returned
 * whenever the provided string equals "true", otherwise
 * false is returned.  Not case-sensitive.
 */
bool stringToBool(QString &strVal) {
	if(strVal.isEmpty())
		return false;
	
	strVal.toLower();

	return (strVal == "true");
}

//returns (v1x,v1y) dot (v2x, v2y)
float dotProduct(float v1x, float v1y, float v2x, float v2y) {
	return (v1x*v2x + v1y*v2y);	
}

/**
 * Creates a string of random numbers and letters of the
 * specified length.
 */
QString randStr(int length) {
	QString retVal = "";
	QString validChars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	for(int i=0;i<length;i++) {
		int idx = jrand()%validChars.length();
		retVal.append(validChars.at(idx));
	}

	return retVal;
}

/**
 * Converts the specified bool to Qt::CheckState.
 */
Qt::CheckState boolToCheckState(bool bVal) {
  if(bVal)
    return Qt::Checked;
  
  return Qt::Unchecked;
}

/**
 * Rounds a float to the nearest whole number and casts to integer.
 */
int roundFtoI(const float& val) {
  return (val < 0.0) ? int(val-0.5) : int(val+0.5);
}

/**
 * Rounds a double to the nearest whole number and casts to integer.
 */
int roundDtoI(const double& val) {
  return (val < 0.0) ? int(val-0.5) : int(val+0.5);
}

/**
 * Compares to doubles using the specified precision.
 */
bool relativeCompare(const double& A, const double& B, 
		     const double& epsilon) {
  double tmpA = (A > 0.0) ? A : -1.0 * A;
  double tmpB = (B > 0.0) ? B : -1.0 * B;
  double delta = (tmpA > tmpB) ? tmpA - tmpB : tmpB - tmpA;
  return (delta <= epsilon);
}

/**
 * Compares to floats using the specified precision.
 */
bool relativeCompare(const float& A, const float& B, 
		     const float& epsilon) {
  float tmpA = (A > 0.0) ? A : -1.0 * A;
  float tmpB = (B > 0.0) ? B : -1.0 * B;
  float delta = (tmpA > tmpB) ? tmpA - tmpB : tmpB - tmpA;
  return (delta <= epsilon);
}
