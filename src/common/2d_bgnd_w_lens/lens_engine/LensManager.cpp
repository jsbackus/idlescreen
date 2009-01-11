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

#include "LensManager.h"

//constructor takes a reference to the screen object and
//to the config manager
LensManager::LensManager(screen_struct* screenObj, ConfigManager* confMgr) {
	_lenses = NULL;
	_numLenses = 0;

	_confMgr = confMgr;
	_screenObj = screenObj;

	if(_confMgr == NULL || _screenObj == NULL)
		return;

	//initialize lens related code
	_confMgr->initializeLenses(_screenObj);

	//allocate space for lenses
	_maxNumLenses = _confMgr->getMaxNumLenses();

	if(_maxNumLenses > 0) {
		_lenses = new LensObject*[_maxNumLenses];
	}
}

LensManager::~LensManager() {
	if(_lenses != NULL) {
		for(int i=0;i<_numLenses;i++) {
			delete _lenses[i];
			_lenses[i] = NULL;
		}
		delete [] _lenses;
		_lenses = NULL;
		_numLenses = 0;
		_maxNumLenses = 0;
	}
}

//flips through the list of lenses, render all that are on screen.
void LensManager::drawLenses() {
	for(int i=0;i<_numLenses;i++) {
		_lenses[i]->drawLens();
	}
}

//called every clock tick.  Flips through the list calling animate()
//on all lenses.  If a lens is no longer visible, it is either
//destroyed or repositioned depending on whether or not it is
//a SphericalLens.
void LensManager::clockTick() {
	int i;

	if(_maxNumLenses == 0)
		return;

	//first, animate all visible lenses
	for(i=0;i<_numLenses;i++) {
		_lenses[i]->animate();
	}

	//search for temporary lenses that are no longer visible
	i=0;
	while(i<_numLenses) {
		if(!_lenses[i]->isVisible()) {
			//lens is no longer visible, so remove from list
			delete _lenses[i];
			if(i != _numLenses-1) {
				_lenses[i] = _lenses[_numLenses-1];
			}
			_lenses[_numLenses-1] = NULL;
			_numLenses--;
		}
		i++;
	}

	//decide whether or not to create a new lens
	//pick a random number between 0 and max num of lenses.
	//if the random number is less than the max number - current
	//number, create a new one
	i = rand()%_maxNumLenses;
	if(i < (_maxNumLenses - _numLenses)) {
		_lenses[_numLenses] = _confMgr->getNewLens();
		if(_lenses[_numLenses] != NULL) {
			_numLenses++;
		}
	}
}

//creates a new temporary lense
/*
LensObject* LensManager::createTempLens() {

	lens_profile_struct* profile = _confMgr->getLensProfile(rand()%_confMgr->getNumLensProfiles());

	if(profile == NULL)
		return NULL;

	//set lens size
	int sizeX = _screenObj->_sizeX;
	int sizeY = _screenObj->_sizeY;
	float xMult = 1.0;
	float yMult = 1.0;
	if(profile->_bRandSize) {
		xMult = float(rand() % int((profile->_maxSizeX - profile->_minSizeX)*100.0))/100.0 + profile->_minSizeX;
		yMult = float(rand() % int((profile->_maxSizeY - profile->_minSizeY)*100.0))/100.0 + profile->_minSizeY;
	} else {
		xMult = profile->_maxSizeX;
		yMult = profile->_maxSizeY;
	}

	LensObject* lens;
	int lensType = profile->_lensTypes[rand() % profile->_numLensTypes];
	//lensType = USE_TRIANGULAR_LENS;
	switch(lensType) {
		case USE_SPHERICAL_LENS:
			//sphere only uses X multiplier (since it is "square")
			yMult = xMult;
			sizeX = int(float(sizeX)*xMult);
			sizeY = int(float(sizeY)*yMult);
			lens = new SphericalLens(profile->_distortionFactor, _screenObj, sizeX, sizeY, profile->_outlineWidth);
			break;
		case USE_CIRCULAR_LENS:
			//circle only uses X multiplier (since it is "square")
			yMult = xMult;
			sizeX = int(float(sizeX)*xMult);
			sizeY = int(float(sizeY)*yMult);
			lens = new CircularLens(_screenObj, sizeX, sizeY, profile->_outlineWidth);
			break;
		case USE_TRIANGULAR_LENS:
			sizeX = int(float(sizeX)*xMult);
			sizeY = int(float(sizeY)*yMult);
			lens = new TriangularLens(profile->_forceIsosceles, _screenObj, sizeX, sizeY, profile->_outlineWidth);
			break;
		case USE_RECTANGULAR_LENS:
			sizeX = int(float(sizeX)*xMult);
			sizeY = int(float(sizeY)*yMult);
			lens = new RectangularLens(_screenObj, sizeX, sizeY, profile->_outlineWidth);
		default:
			break;
	}

	//tints and complements
	lens->setComplement(profile->_bComplement);
	lens->setOutlineComplement(profile->_bOutlineComplement);
	if(profile->_bUseTint) {
		lens->setTint(&profile->_tint[0]);
	} else {
		lens->setTint(NULL);
	}
	if(profile->_bUseOutlineTint) {
		lens->setOutlineTint(&profile->_outlineTint[0]);
	} else {
		lens->setOutlineTint(NULL);
	}

	//speed
	if(profile->_bRandSpeed) {
		float speed = float( rand() % int((profile->_maxSpeed - profile->_minSpeed)*100.0)) / 100.0 + profile->_minSpeed;
		lens->setSpeed(speed);
	} else {
		lens->setSpeed(profile->_maxSpeed);
	}

	int startSide = 1;
	//pick starting side
	if(profile->_bRandStart) {
		startSide = profile->_sideStartOrder[rand()%profile->_numSides];
	} else {
		profile->_lastStartSide++;
		if(profile->_lastStartSide >= profile->_numSides) {
			profile->_lastStartSide = 0;
		}
		startSide = profile->_sideStartOrder[profile->_lastStartSide];
	}

	//starting angle
	float angle;
	float dir = 1.0;
	if(profile->_bRandAngle) {
		angle = float(rand() % int((profile->_maxAngle-profile->_minAngle)*100.0)) / 100.0 + profile->_minAngle;
	} else {
		angle = profile->_maxAngle;
	}

	if(angle > PI/2.0) {
		dir = dir * -1.0;
	}

	//pick position and set up direction
	float normX, normY;  //keep normalized!
	int startX, startY;

	switch(startSide) {
		case LEFT_SIDE:
			//left side
			normX = sin(angle);
			normY = -1.0*cos(angle);
			startX = int(float(_screenObj->_sizeX)*-1.0*xMult);  //remember: position is bottom left corner
			startY = (rand() % int(float(_screenObj->_sizeY)*0.90)) + int(float(_screenObj->_sizeY)*0.05);
			break;
		case RIGHT_SIDE:
			//right side
			normX = -1.0*sin(angle);
			normY = cos(angle);
			startX = _screenObj->_sizeX-1;
			startY = (rand() % int(float(_screenObj->_sizeY)*0.90)) + int(float(_screenObj->_sizeY)*0.05);
			break;
		case TOP_SIDE:
			//top
			normX = -1.0*cos(angle);
			normY = -1.0*sin(angle);
			startX = (rand() % int(float(_screenObj->_sizeX)*0.90)) + int(float(_screenObj->_sizeX)*0.05);
			startY = _screenObj->_sizeY-1;
			break;
		case BOTTOM_SIDE:
		default:
			//bottom
			normX = cos(angle);
			normY = sin(angle);
			startX = (rand() % int(float(_screenObj->_sizeX)*0.90)) + int(float(_screenObj->_sizeX)*0.05);
			startY = int(float(_screenObj->_sizeY)*-1.0*yMult);  //remember: position is bottom left corner
			break;
	}

	lens->setPosition(startX, startY);
	lens->setDirection(normX, normY);

	return lens;
	
}
*/
