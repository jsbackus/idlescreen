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
#include "RectangularLensProfile.h"

RectangularLensProfile::RectangularLensProfile() {
	_xmlTypeValue = "RectangularLensProfile";

	_minSizeX = float(0.15);
	_maxSizeX = float(0.30);
	_minSizeY = float(0.15);
	_maxSizeY = float(0.30);
	_bRandSize = true;
	_maxNumLenses = 5;
}

RectangularLensProfile::~RectangularLensProfile() {
}

/* Attempts to load this background profile object from the
 * specified QDomNode.
 */
LensProfile* RectangularLensProfile::load(QDomNode &node) {
	float tmpF;
	int tmpI;
	RectangularLensProfile* retVal = new RectangularLensProfile();
	retVal->loadBase(node);

	QDomElement tempElem;

	tempElem = node.firstChildElement("min_width");
	if(!tempElem.isNull()) {
		tmpF = tempElem.text().toFloat();
		if(tmpF > 0.00 && tmpF < 1.00) {
			retVal->_minSizeX = tmpF;
		}
	}

	tempElem = node.firstChildElement("max_width");
	if(!tempElem.isNull()) {
		tmpF = tempElem.text().toFloat();
		if(tmpF > 0.00 && tmpF < 1.00) {
			retVal->_maxSizeX = tmpF;
		}
	}

	tempElem = node.firstChildElement("min_height");
	if(!tempElem.isNull()) {
		tmpF = tempElem.text().toFloat();
		if(tmpF > 0.00 && tmpF < 1.00) {
			retVal->_minSizeY = tmpF;
		}
	}

	tempElem = node.firstChildElement("max_height");
	if(!tempElem.isNull()) {
		tmpF = tempElem.text().toFloat();
		if(tmpF > 0.00 && tmpF < 1.00) {
			retVal->_maxSizeY = tmpF;
		}
	}

	tempElem = node.firstChildElement("random_size");
	if(!tempElem.isNull()) {
		retVal->_bRandSize = stringToBool(tempElem.text());
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
QDomNode RectangularLensProfile::save(QDomDocument* doc) {
	QString tempStr;
	QDomText tempNode;
	QDomElement tempElem;
	QDomElement retVal = doc->createElement(getXMLTagName());

	saveBase(doc, retVal);

	tempStr.setNum(_minSizeX);
	tempNode = doc->createTextNode(tempStr);
	tempElem = doc->createElement("min_width");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	tempStr.setNum(_maxSizeX);
	tempNode = doc->createTextNode(tempStr);
	tempElem = doc->createElement("max_width");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	tempStr.setNum(_minSizeY);
	tempNode = doc->createTextNode(tempStr);
	tempElem = doc->createElement("min_height");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	tempStr.setNum(_maxSizeY);
	tempNode = doc->createTextNode(tempStr);
	tempElem = doc->createElement("max_height");
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
LensObject* RectangularLensProfile::getNewLensObject() {
	if(_screenObj == NULL)
		return NULL;

	int sizeX;
	int sizeY;
	float sizePercentX;
	float sizePercentY;

	if(_bRandSize) {
		int minI, maxI;
		minI = int(float(_screenObj->_sizeX)*_minSizeX);
		maxI = int(float(_screenObj->_sizeX)*_maxSizeX);
		sizeX = (rand() % (maxI - minI)) + minI;
		minI = int(float(_screenObj->_sizeY)*_minSizeY);
		maxI = int(float(_screenObj->_sizeY)*_maxSizeY);
		sizeY = (rand() % (maxI - minI)) + minI;

		sizePercentX = float(sizeX) / float(_screenObj->_sizeX);
		sizePercentY = float(sizeY) / float(_screenObj->_sizeY);
	} else {
		sizeX = int(float(_screenObj->_sizeX)*_maxSizeX);
		sizePercentX = _maxSizeX;

		sizeY = int(float(_screenObj->_sizeY)*_maxSizeY);
		sizePercentY = _maxSizeY;
	}

	GenericLensObject* retVal = getNewGenericLens(sizePercentX, sizePercentY);

	int* field = NULL;
	bool* bitmap = NULL;

	retVal->getDataStructures(&bitmap, &field);

	int screenX = _screenObj->_sizeX;

	if(field != NULL && bitmap != NULL) {
		for(int y=_outlineWidth; y < sizeY - _outlineWidth; y++) {
			int yLoc = y*sizeX;
			int yVal = y*screenX;
			for(int x=_outlineWidth; x<sizeX - _outlineWidth; x++) {
				field[yLoc+x] = (yVal+x)*4;
				bitmap[yLoc+x] = true;
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
int RectangularLensProfile::getMaxNumLenses() {
	return _maxNumLenses;
}
