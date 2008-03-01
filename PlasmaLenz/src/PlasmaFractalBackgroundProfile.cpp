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

#include <QtXml/QDomElement>
#include <QtXml/QDomText>

#include "../../common/utility/misc_funcs.h"
#include "PlasmaFractalBackgroundProfile.h"
#include "PlasmaFractal.h"

PlasmaFractalBackgroundProfile::PlasmaFractalBackgroundProfile() {
	_xmlTypeValue = "PlasmaFractalBackground";

	//defaults
	_name = "Empty";
	_palName = "";
	_coarseness = 1.0;
	_genStepsPerTick = 2048;
	_bAnimatePalette = true;
	_paletteXSpeed = 1;
	_paletteYSpeed = 1;
	_bClampColorIndex = true;
}

PlasmaFractalBackgroundProfile::~PlasmaFractalBackgroundProfile() {
}

/* Attempts to load this background profile object from the
 * specified QDomNode.
 */
BackgroundProfile* PlasmaFractalBackgroundProfile::load(QDomNode &node) {
	float tmpF;
	int tmpI;
	QDomElement tempElem;

	//sanity check on the input
	if(node.nodeName() != getXMLTagName())
		return NULL;

	//make sure that the type value matches
	tempElem = node.firstChildElement("type");
	if(tempElem.isNull() || tempElem.text() != _xmlTypeValue)
		return NULL;

	PlasmaFractalBackgroundProfile* retVal = new PlasmaFractalBackgroundProfile();

	tempElem = node.firstChildElement("name");
	if(!tempElem.isNull()) {
		retVal->_name = tempElem.text();
	}

	tempElem = node.firstChildElement("coarseness");
	if(!tempElem.isNull()) {
		tmpF = tempElem.text().toFloat();
		if(tmpF > 0.00 && tmpF < 10000.00) {
			retVal->_coarseness = tmpF;
		}
	}

	tempElem = node.firstChildElement("steps_per_tick");
	if(!tempElem.isNull()) {
		tmpI = tempElem.text().toInt();
		if(tmpI > 0 && tmpI < 30000) {
			retVal->_genStepsPerTick = tmpI;
		}
	}

	tempElem = node.firstChildElement("anmiate_palette");
	if(!tempElem.isNull()) {
		retVal->_bAnimatePalette = stringToBool(tempElem.text());
	}

	tempElem = node.firstChildElement("palette_x_speed");
	if(!tempElem.isNull()) {
		tmpI = tempElem.text().toInt();
		if(tmpI >= 0 && tmpI < 30000) {
			retVal->_paletteXSpeed = tmpI;
		}
	}

	tempElem = node.firstChildElement("palette_y_speed");
	if(!tempElem.isNull()) {
		tmpI = tempElem.text().toInt();
		if(tmpI >= 0 && tmpI < 30000) {
			retVal->_paletteYSpeed = tmpI;
		}
	}

	tempElem = node.firstChildElement("clamp_color");
	if(!tempElem.isNull()) {
		retVal->_bClampColorIndex = stringToBool(tempElem.text());
	}

	tempElem = node.firstChildElement("palette");
	if(!tempElem.isNull()) {
		retVal->_palName = tempElem.text();
	}
	
	return retVal;
}


/*
 * Returns a QDomNode object that represents this profile.
 */
QDomNode PlasmaFractalBackgroundProfile::save(QDomDocument* doc) {
	QString tempStr;
	QDomText tempNode;
	QDomElement tempElem;
	QDomElement retVal = doc->createElement(getXMLTagName());

	//type
	tempNode = doc->createTextNode(_xmlTypeValue);
	tempElem = doc->createElement("type");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	//name
	tempNode = doc->createTextNode(_name);
	tempElem = doc->createElement("name");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	//coarseness
	tempStr.setNum(_coarseness);
	tempNode = doc->createTextNode(tempStr);
	tempElem = doc->createElement("coarseness");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);
	
	//steps per tick
	tempStr.setNum(_genStepsPerTick);
	tempNode = doc->createTextNode(tempStr);
	tempElem = doc->createElement("steps_per_tick");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	//animate palette?
	tempNode = doc->createTextNode(boolToString(_bAnimatePalette));
	tempElem = doc->createElement("anmiate_palette");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	//palette X speed
	tempStr.setNum(_paletteXSpeed);
	tempNode = doc->createTextNode(tempStr);
	tempElem = doc->createElement("palette_x_speed");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	//palette Y speed
	tempStr.setNum(_paletteYSpeed);
	tempNode = doc->createTextNode(tempStr);
	tempElem = doc->createElement("palette_y_speed");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	//clamp color index?
	tempNode = doc->createTextNode(boolToString(_bClampColorIndex));
	tempElem = doc->createElement("clamp_color");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	//palette name
	tempNode = doc->createTextNode(_palName);
	tempElem = doc->createElement("palette");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	return retVal;
}

/*
 * Creates and returns a new background object.
 */
Background* PlasmaFractalBackgroundProfile::getNewBackgroundObj(int height, int width, QHash<QString, IndexedPaletteProfile*>* palHash) {
	return new PlasmaFractal(width, height,
		palHash->value(_palName)->createPalette(), _coarseness, _genStepsPerTick,
		_bAnimatePalette, _paletteXSpeed, _paletteYSpeed, _bClampColorIndex);
}

/*
 * get/set palette name.
 */
QString PlasmaFractalBackgroundProfile::getPaletteName() {
	return _palName;
}

void PlasmaFractalBackgroundProfile::setPaletteName(QString paletteName) {
	_palName = paletteName;
}

/*
 * get/set coarseness.
 */
float PlasmaFractalBackgroundProfile::getCoarseness() {
	return _coarseness;
}

void PlasmaFractalBackgroundProfile::setCoarseness(float coarseness) {
	_coarseness = coarseness;
}

/*
 * get/set steps per tick
 */
int PlasmaFractalBackgroundProfile::getGenStepsPerTick() {
	return _genStepsPerTick;
}

void PlasmaFractalBackgroundProfile::setGenStepsPerTick(int genStepsPerTick) {
	_genStepsPerTick = genStepsPerTick;
}

/*
 * Get/set animate palette.
 */
bool PlasmaFractalBackgroundProfile::getAnimatePalette() {
	return _bAnimatePalette;
}

void PlasmaFractalBackgroundProfile::setAnimatePalette(bool bAnimatePalette) {
	_bAnimatePalette = bAnimatePalette;
}

/*
 * get/set palette x speed.
 */
float PlasmaFractalBackgroundProfile::getPaletteXSpeed() {
	return _paletteXSpeed;
}

void PlasmaFractalBackgroundProfile::setPaletteXSpeed(float speed) {
	_paletteXSpeed = speed;
}

/*
 * get/set palette y speed.
 */
float PlasmaFractalBackgroundProfile::getPaletteYSpeed() {
	return _paletteYSpeed;
}

void PlasmaFractalBackgroundProfile::setPaletteYSpeed(float speed) {
	_paletteYSpeed = speed;
}

/*
 * Get/set clamp color index.
 */
bool PlasmaFractalBackgroundProfile::getClampColorIndex() {
	return _bClampColorIndex;
}

void PlasmaFractalBackgroundProfile::setClampColorIndex(bool bClampColorIndex) {
	_bClampColorIndex = bClampColorIndex;
}

/*
 * Overloaded assignment operator
 */
PlasmaFractalBackgroundProfile& PlasmaFractalBackgroundProfile::operator=(PlasmaFractalBackgroundProfile& other) {
	_name = other._name;
	_palName = other._palName;
	_coarseness = other._coarseness;
	_genStepsPerTick = other._genStepsPerTick;
	_bAnimatePalette = other._bAnimatePalette;
	_paletteXSpeed = other._paletteXSpeed;
	_paletteYSpeed = other._paletteYSpeed;
	_bClampColorIndex = other._bClampColorIndex;

	return *this;
}

/*
 * Creates a new object with this object's settings.
 */
BackgroundProfile* PlasmaFractalBackgroundProfile::clone() {
	PlasmaFractalBackgroundProfile* retVal = new PlasmaFractalBackgroundProfile();

	*retVal = *this;

	return (BackgroundProfile*)retVal;
}

