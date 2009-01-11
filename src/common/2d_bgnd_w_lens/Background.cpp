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

#include "Background.h"

/**
 * Draws the whole Fractal.  Creates a texture
 * based on the information in _field and _palette.
 */
void Background::drawBackground(screen_struct* screenObj) {
	return;
}

/**
 * Whether or not the background should animate
 * itself (if it supports it)
 */
void Background::enableAnimation(bool bEnableAnimation) {
	_bEnableAnimation = bEnableAnimation;
}

/**
 * Call this every tick to animate.
 * Must be overloaded by derived class.
 */
void Background::clocktick() {
	return;
}

/**
 * Returns true if the background has finished whatever
 * setup it needs to do, animated or otherwise.
 */
bool Background::isSetupFinished() {
	return _bSetupFinished;
}

/**
 * Constructor takes the background dimensions as
 * arguments.
 */
Background::Background(int sizeX, int sizeY) {
	_sizeX = sizeX;
	_sizeY = sizeY;
	_bEnableAnimation = false;
	_bSetupFinished = true;
}

Background::~Background(void) {
	
}

/**
 * Default constructor...
 */
Background::Background() {
	_sizeX = 0;
	_sizeY = 0;
	_bEnableAnimation = false;
	_bSetupFinished = true;
}

/**
 * Called from the constructor and does all of the work,
 * so that derived classes can call from their constructors.
 * sizeX and sizeY are the dimensions of the background, not
 * necessarily the size of the screen object, which has to
 * be a power of 2.
 */
void Background::initBackground(int sizeX, int sizeY) {
	_sizeX = sizeX;
	_sizeY = sizeY;
	_bEnableAnimation = false;
	_bSetupFinished = true;
}

int Background::getScreenWidth() {
	return _sizeX;
}

int Background::getScreenHeight() {
	return _sizeY;
}

int Background::getTextureWidth() {
	int retVal = 2;

	while(retVal < _sizeX) {
		retVal *= 2;
	}

	return retVal;
}

int Background::getTextureHeight() {
	int retVal = 2;

	while(retVal < _sizeY) {
		retVal *= 2;
	}

	return retVal;
}

