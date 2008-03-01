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

#include "GenericLensObject.h"

//constructor calls setupLens to create the data structures.
GenericLensObject::GenericLensObject(screen_struct* screenObj, int sizeX, int sizeY, int outlineWidth) {
	//set up base class
	setupLens(screenObj, sizeX, sizeY, outlineWidth);
}

GenericLensObject::~GenericLensObject() {
	destroyLens();
}

//retrieves the field and bitmap data structures.  Up to caller to 
//ensure buffer boundaries are not exceeded!
void GenericLensObject::getDataStructures(bool** bitmap, int** field) {
	*bitmap = _bitmap;
	*field = _field;
}

//calculates the outline
void GenericLensObject::calculateOutline() {
	//lastly, calculate outline
	calcOutline();
}
