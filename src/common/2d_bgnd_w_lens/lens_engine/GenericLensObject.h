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

/**
 * This class extends LensObject, but allows access to the bitmap and
 * field data structures.  This is so that lens profiles can modify
 * these data structures without needed a subclass for each type of lens.
 * It's a bit of a hack, but it simplifies the class structure.
 */

#ifndef __GENERICLENSOBJECT_H__
#define __GENERICLENSOBJECT_H__

#include "LensObject.h"


class GenericLensObject : public LensObject {
public:
	//constructor calls setupLens to create the data structures.
	GenericLensObject(screen_struct* screenObj, int sizeX, int sizeY, int outlineWidth=1);
	~GenericLensObject();

	//retrieves the field and bitmap data structures.  Up to caller to 
	//ensure buffer boundaries are not exceeded!
	void getDataStructures(bool** bitmap, int** field);

	//calculates the outline
	void calculateOutline();
};

#endif