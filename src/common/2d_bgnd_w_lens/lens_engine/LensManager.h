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
 * This class manages all lens sprites, creating them, moving
 * them, and destroying/repositioning them when they're
 * off screen.
 */

#ifndef __LENSMANAGER_H__
#define __LENSMANAGER_H__

#include <QStringList>
#include <QString>

#include "../globaldefs.h"
#include "LensObject.h"

#include "../ConfigManager.h"

class LensManager {

public:
	//constructor takes a reference to the screen object and
	//to the config manager
	LensManager(screen_struct* screenObj, ConfigManager* confMgr);
	~LensManager();

	//flips through the list of lenses, render all that are on screen.
	void drawLenses();

	//called every clock tick.  Flips through the list calling animate()
	//on all lenses.  If a lens is no longer visible, it is either
	//destroyed or repositioned depending on whether or not it is
	//a SphericalLens.
	void clockTick();

private:

	//creates a new temporary lense
	//LensObject* createTempLens();

	LensObject** _lenses;
	int _numLenses;
	int _maxNumLenses;

	screen_struct* _screenObj;
	ConfigManager* _confMgr;

};
#endif