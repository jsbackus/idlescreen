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

#include <QString>

/*
 * converts a bool to a QString.
 */
QString boolToString(bool bVal) {

	return (bVal) ? QString("true") : QString("false");
}

/*
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