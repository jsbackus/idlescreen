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

#include "../../misc_funcs.h"
#include "CircularLensProfile.h"

CircularLensProfile::CircularLensProfile() {
	_xmlTypeValue = "CircularLensProfile";

	_minRadius = float(0.15);
	_maxRadius = float(0.30);
	_bRandSize = true;
	_maxNumLenses = 5;
}

CircularLensProfile::~CircularLensProfile() {
}

/* Attempts to load this background profile object from the
 * specified QDomNode.
 */
LensProfile* CircularLensProfile::load(QDomNode &node) {
	float tmpF;
	int tmpI;
	CircularLensProfile* retVal = new CircularLensProfile();
	retVal->loadBase(node);

	QDomElement tempElem;

	tempElem = node.firstChildElement("min_radius");
	if(!tempElem.isNull()) {
		tmpF = tempElem.text().toFloat();
		if(tmpF > 0.00 && tmpF < 1.00) {
			retVal->_minRadius = tmpF;
		}
	}

	tempElem = node.firstChildElement("max_radius");
	if(!tempElem.isNull()) {
		tmpF = tempElem.text().toFloat();
		if(tmpF > 0.00 && tmpF < 1.00) {
			retVal->_maxRadius = tmpF;
		}
	}

	tempElem = node.firstChildElement("random_size");
	if(!tempElem.isNull()) {
		_bRandSize = stringToBool(tempElem.text());
	}

	tempElem = node.firstChildElement("instances_max");
	if(!tempElem.isNull()) {
		tmpI = tempElem.text().toInt();
		if(tmpI > 0 && tmpI < 10000) {
			retVal->_maxNumLenses = tmpI;
		}
	}

	return retVal;
}

/*
 * Returns a QDomNode object that represents this profile.
 */
QDomNode CircularLensProfile::save(QDomDocument* doc) {
	QString tempStr;
	QDomText tempNode;
	QDomElement tempElem;
	QDomElement retVal = doc->createElement(getXMLTagName());

	saveBase(doc, retVal);

	tempStr.setNum(_minRadius);
	tempNode = doc->createTextNode(tempStr);
	tempElem = doc->createElement("min_radius");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	tempStr.setNum(_maxRadius);
	tempNode = doc->createTextNode(tempStr);
	tempElem = doc->createElement("max_radius");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	tempNode = doc->createTextNode(boolToString(_bRandSize));
	tempElem = doc->createElement("random_size");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	tempStr.setNum(_maxNumLenses);
	tempNode = doc->createTextNode(tempStr);
	tempElem = doc->createElement("instances_max");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	return retVal;
}

/*
 * Creates and returns a new LensObject object.
 */
LensObject* CircularLensProfile::getNewLensObject() {
	if(_screenObj == NULL)
		return NULL;

	int sizeX = _screenObj->_sizeX;
	int sizeY = _screenObj->_sizeY;
	float sizePercentX;
	float sizePercentY;

	if(sizeX < sizeY) {
		sizeY = sizeX;
	} else {
		sizeX = sizeY;
	}

	if(_bRandSize) {
		int minI, maxI;
		minI = int(float(sizeX)*_minRadius*2);
		maxI = int(float(sizeX)*_maxRadius*2);
		sizeY = sizeX = (rand() % (maxI - minI)) + minI;

		sizePercentX = float(sizeX) / float(_screenObj->_sizeX);
		sizePercentY = float(sizeY) / float(_screenObj->_sizeY);
	} else {
		sizeX = int(float(sizeX)*_maxRadius*2);
		sizePercentX = _maxRadius*2;

		sizeY = int(float(sizeY)*_maxRadius*2);
		sizePercentY = _maxRadius*2;
	}

	GenericLensObject* retVal = getNewGenericLens(sizePercentX, sizePercentY);

	int* field = NULL;
	bool* bitmap = NULL;

	retVal->getDataStructures(&bitmap, &field);

	int screenX = _screenObj->_sizeX;
	int radius;

	if(field != NULL && bitmap != NULL) {
		if(sizeX < sizeY) {
			radius = sizeX/2 - 1 - _outlineWidth;
		} else {
			radius = sizeY/2 - 1 - _outlineWidth;
		}

		int rsqrd = radius * radius;
		int midX = sizeX / 2;
		int midY = sizeY / 2;

		for(int y=0; y < sizeY; y++) {
			int yLoc = y*sizeX;
			int yVal = y*screenX;
			int posY = y-midY;
			int ysqrd = posY*posY;
			for(int x=0; x<sizeX; x++) {
				int posX = x-midX;
				field[yLoc+x] = (yVal+x)*4;
				if(posX*posX + ysqrd <= rsqrd) {
					bitmap[yLoc+x] = true;
				} else {
					bitmap[yLoc+x] = false;
				}
			}
		}
	
		//lastly, calculate outline
		retVal->calculateOutline();

		field = NULL;
		bitmap = NULL;
	}

	return retVal;
}

/*
 * Returns the maximum number of lenses this profile will allow.
 */
int CircularLensProfile::getMaxNumLenses() {
	return _maxNumLenses;
}

/*
 * Creates a new object with this object's settings.
 */
LensProfile* CircularLensProfile::clone() {
	CircularLensProfile* retVal = new CircularLensProfile();

	// deep copy needs implemented!
	*retVal = *this;

	return (LensProfile*)retVal;
}
