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

#include "../../utility/misc_funcs.h"
#include "LensProfile.h"

LensProfile::LensProfile() {
	_xmlTypeValue = "LensProfile";

	_name = "Empty";
	_palName = "";
	_outlineWidth = 1;

	_bRandTint = false;
	_bUseComplement = true;
	_bUseTint = false;
	_bUseOutlineComplement = true;
	_bUseOutlineTint = false;
	_lastColorIndex = 0;

	_minSpeed = 2.0;
	_maxSpeed = 6.0;
	_bRandSpeed = true;

	_minAngle = PI/4.0;
	_maxAngle = float((3*PI)/4.0);
	_bRandAngle = true;

	_sides[0] = Bottom;
	_lastSide = 0;
	_numSides = 1;
	_bRandSide = false;

	_bInitialized = false;
	_pal = NULL;
	_screenObj = NULL;
}

LensProfile::~LensProfile() {
	_screenObj = NULL;
	if(_pal != NULL) {
		delete _pal;
		_pal = NULL;
	}
}

/* Attempts to load this background profile object from the
 * specified QDomNode.
 */
LensProfile* LensProfile::load(QDomNode &node) {
	return NULL;
}

/*
 * Returns a QDomNode object that represents this profile.
 */
QDomNode LensProfile::save(QDomDocument* doc) {
	QDomElement retVal = doc->createElement(getXMLTagName());
	saveBase(doc, retVal);
	return retVal;
}

/*
 * Creates and returns a new background object.
 */
LensObject* LensProfile::getNewLensObject() {
	return NULL;
}

/*
 * Returns the maximum number of lenses this profile will allow.
 */
int LensProfile::getMaxNumLenses() {
	return 0;
}

/*
 * Get/set the name of this profile.
 */
void LensProfile::setName(const QString &name) {
	_name = name;
}

QString LensProfile::getName() {
	return _name;
}

/*
 * Returns the tag name used by Dom elements for all
 * subclasses of BackgroundProfile.
 */
QString LensProfile::getXMLTagName() {
	return "LensProfile";
}

/*
 * Returns the type of BackgroundProfile.
 */
QString LensProfile::getLensType() {
	return _xmlTypeValue;
}

/*
 * Initializes the profile so that it can generate new lenses with getNewLensObject().
 */
void LensProfile::initialize(QHash<QString, IndexedPaletteProfile*>* palHash, screen_struct* screenObj) {
	initializeBase(palHash,screenObj);
}

void LensProfile::initializeBase(QHash<QString, IndexedPaletteProfile*>* palHash, screen_struct* screenObj) {
	_screenObj = screenObj;
	IndexedPaletteProfile* palProfile = palHash->value(_palName);
	if(palProfile != NULL) {
		_pal = palProfile->createPalette();
	}

	_bInitialized = true;
}

void LensProfile::loadBase(QDomNode &node) {
	float tmpF;
	int tmpI;
	QDomElement tempElem;

	//sanity check on the input
	if(node.nodeName() != getXMLTagName())
		return;

	//make sure that the type value matches
	tempElem = node.firstChildElement("type");
	if(tempElem.isNull() || tempElem.text() != _xmlTypeValue)
		return;

	tempElem = node.firstChildElement("name");
	if(!tempElem.isNull()) {
		_name = tempElem.text();
	}

	tempElem = node.firstChildElement("palette_name");
	if(!tempElem.isNull()) {
		_palName = tempElem.text();
	}

	tempElem = node.firstChildElement("outline_width");
	if(!tempElem.isNull()) {
		tmpI = tempElem.text().toInt();
		if(tmpI >= 0 && tmpI < 10000) {
			_outlineWidth = tmpI;
		}
	}

	tempElem = node.firstChildElement("random_tint");
	if(!tempElem.isNull()) {
		QString param = tempElem.text();
		_bRandTint = stringToBool(param);
	}

	tempElem = node.firstChildElement("use_primary_tint");
	if(!tempElem.isNull()) {
		QString param = tempElem.text();
		_bUseTint = stringToBool(param);
	}

	tempElem = node.firstChildElement("primary_tint_is_complement");
	if(!tempElem.isNull()) {
		QString param = tempElem.text();
		_bUseComplement = stringToBool(param);
	}

	tempElem = node.firstChildElement("use_outline_tint");
	if(!tempElem.isNull()) {
		QString param = tempElem.text();
		_bUseOutlineTint = stringToBool(param);
	}

	tempElem = node.firstChildElement("outline_tint_is_complement");
	if(!tempElem.isNull()) {
		QString param = tempElem.text();
		_bUseOutlineComplement = stringToBool(param);
	}

	tempElem = node.firstChildElement("min_speed");
	if(!tempElem.isNull()) {
		tmpF = tempElem.text().toFloat();
		if(tmpF > 0.00 && tmpF < 1000.00) {
			_minSpeed = tmpF;
		}
	}

	tempElem = node.firstChildElement("max_speed");
	if(!tempElem.isNull()) {
		tmpF = tempElem.text().toFloat();
		if(tmpF > 0.00 && tmpF < 1000.00) {
			_maxSpeed = tmpF;
		}
	}

	tempElem = node.firstChildElement("random_speed");
	if(!tempElem.isNull()) {
		QString param = tempElem.text();
		_bRandSpeed = stringToBool(param);
	}

	tempElem = node.firstChildElement("min_angle");
	if(!tempElem.isNull()) {
		tmpF = tempElem.text().toFloat();
		if(tmpF > 0.00 && tmpF < 1000.00) {
			_minAngle = tmpF;
		}
	}

	tempElem = node.firstChildElement("max_angle");
	if(!tempElem.isNull()) {
		tmpF = tempElem.text().toFloat();
		if(tmpF > 0.00 && tmpF < 1000.00) {
			_maxAngle = tmpF;
		}
	}

	tempElem = node.firstChildElement("random_angle");
	if(!tempElem.isNull()) {
		QString param = tempElem.text();
		_bRandAngle = stringToBool(param);
	}

	tempElem = node.firstChildElement("random_side");
	if(!tempElem.isNull()) {
		QString param = tempElem.text();
		_bRandSide = stringToBool(param);
	}

	_numSides = 0;
	QDomElement tempNode = node.firstChildElement("side_types").lastChildElement("side_entry");
	while(!tempNode.isNull()) {	
		QString sideStr;
		int index;

		tempElem = tempNode.firstChildElement("side");
		if(!tempElem.isNull()) {
			sideStr = tempElem.text();
		}

		tempElem = tempNode.firstChildElement("location");
		if(!tempElem.isNull()) {
			tmpI = tempElem.text().toInt();
			if(tmpI >= 0) {
				index = tmpI;
			} else {
				index = 0;
			}
		}

		//insert the string at the appropriate location
		_sides[index] = stringToSide(sideStr);

		//increase the number of sides, if necessary
		if(_numSides <= index) {
			_numSides = index+1;
		}

		tempNode = tempNode.previousSiblingElement("side_entry");
	}
}

void LensProfile::saveBase(QDomDocument* doc, QDomNode &target) {
	QString tempStr;
	QDomText tempNode;
	QDomElement tempElem;

	tempNode = doc->createTextNode(_xmlTypeValue);
	tempElem = doc->createElement("type");
	tempElem.appendChild(tempNode);
	target.appendChild(tempElem);

	tempNode = doc->createTextNode(_name);
	tempElem = doc->createElement("name");
	tempElem.appendChild(tempNode);
	target.appendChild(tempElem);

	tempNode = doc->createTextNode(_palName);
	tempElem = doc->createElement("palette_name");
	tempElem.appendChild(tempNode);
	target.appendChild(tempElem);

	tempStr.setNum(_outlineWidth);
	tempNode = doc->createTextNode(tempStr);
	tempElem = doc->createElement("outline_width");
	tempElem.appendChild(tempNode);
	target.appendChild(tempElem);

	tempNode = doc->createTextNode(boolToString(_bRandTint));
	tempElem = doc->createElement("random_tint");
	tempElem.appendChild(tempNode);
	target.appendChild(tempElem);

	tempNode = doc->createTextNode(boolToString(_bUseTint));
	tempElem = doc->createElement("use_primary_tint");
	tempElem.appendChild(tempNode);
	target.appendChild(tempElem);

	tempNode = doc->createTextNode(boolToString(_bUseComplement));
	tempElem = doc->createElement("primary_tint_is_complement");
	tempElem.appendChild(tempNode);
	target.appendChild(tempElem);

	tempNode = doc->createTextNode(boolToString(_bUseOutlineTint));
	tempElem = doc->createElement("use_outline_tint");
	tempElem.appendChild(tempNode);
	target.appendChild(tempElem);

	tempNode = doc->createTextNode(boolToString(_bUseOutlineComplement));
	tempElem = doc->createElement("outline_tint_is_complement");
	tempElem.appendChild(tempNode);
	target.appendChild(tempElem);

	tempStr.setNum(_minSpeed);
	tempNode = doc->createTextNode(tempStr);
	tempElem = doc->createElement("min_speed");
	tempElem.appendChild(tempNode);
	target.appendChild(tempElem);

	tempStr.setNum(_maxSpeed);
	tempNode = doc->createTextNode(tempStr);
	tempElem = doc->createElement("max_speed");
	tempElem.appendChild(tempNode);
	target.appendChild(tempElem);

	tempNode = doc->createTextNode(boolToString(_bRandSpeed));
	tempElem = doc->createElement("random_speed");
	tempElem.appendChild(tempNode);
	target.appendChild(tempElem);

	tempStr.setNum(_minAngle);
	tempNode = doc->createTextNode(tempStr);
	tempElem = doc->createElement("min_angle");
	tempElem.appendChild(tempNode);
	target.appendChild(tempElem);

	tempStr.setNum(_maxAngle);
	tempNode = doc->createTextNode(tempStr);
	tempElem = doc->createElement("max_angle");
	tempElem.appendChild(tempNode);
	target.appendChild(tempElem);

	tempNode = doc->createTextNode(boolToString(_bRandAngle));
	tempElem = doc->createElement("random_angle");
	tempElem.appendChild(tempNode);
	target.appendChild(tempElem);

	tempNode = doc->createTextNode(boolToString(_bRandSide));
	tempElem = doc->createElement("random_side");
	tempElem.appendChild(tempNode);
	target.appendChild(tempElem);

	QDomElement sideListElem = doc->createElement("side_types");
	QDomElement entryElem;
	for(int i=0;i<_numSides; i++) {
		entryElem = doc->createElement("side_entry");

		//side
		tempNode = doc->createTextNode(sideToString(_sides[i]));
		tempElem = doc->createElement("side");
		tempElem.appendChild(tempNode);
		entryElem.appendChild(tempElem);

		//location
		tempStr.setNum(i);
		tempNode = doc->createTextNode(tempStr);
		tempElem = doc->createElement("location");
		tempElem.appendChild(tempNode);
		entryElem.appendChild(tempElem);

		sideListElem.appendChild(entryElem);
	}
	target.appendChild(sideListElem);
}

/*
 * Sets the following bits of data in the specified GenericLensObject:
 * sizeX, sizeY
 * outlineWidth
 * screenObj
 * primary and outline tints (including complement coloring)
 * size
 * starting position
 * tragectory
 * speed
 */
GenericLensObject* LensProfile::getNewGenericLens(float sizePercentX, float sizePercentY) {

	if(!_bInitialized)
		return NULL;

	int sizeX = int(sizePercentX * float(_screenObj->_sizeX));
	int sizeY = int(sizePercentY * float(_screenObj->_sizeY));
	GenericLensObject* retVal = new GenericLensObject(_screenObj, sizeX, sizeY, _outlineWidth);

	//tints and complements
	retVal->setComplement(_bUseComplement);
	retVal->setOutlineComplement(_bUseOutlineComplement);
	if(_pal != NULL && (_bUseTint || _bUseOutlineTint)) {
		GLubyte buff[3];

		if(_bRandTint) {
			int newColorIndex = _lastColorIndex;
			while(newColorIndex == _lastColorIndex) {
				newColorIndex = rand() % _pal->getWidth();
			}
			_lastColorIndex = newColorIndex;
		} else {
			_lastColorIndex = (_lastColorIndex+1) % _pal->getWidth();
		}

		if(_bUseTint) {
			_pal->getColor(_lastColorIndex, 0, &buff[0],4);
			retVal->setTint(&buff[0]);
		} else {
			retVal->setTint(NULL);
		}

		if(_bUseOutlineTint) {
			_pal->getColor(_lastColorIndex, 1, &buff[0],4);
			retVal->setOutlineTint(&buff[0]);
		} else {
			retVal->setOutlineTint(NULL);
		}
	}

	//speed
	if(_bRandSpeed) {
		float speed = float( rand() % int((_maxSpeed - _minSpeed)*100.0)) / 100.0 + _minSpeed;
		retVal->setSpeed(speed);
	} else {
		retVal->setSpeed(_maxSpeed);
	}

	side_type startSide;
	//pick starting side
	if(_bRandSide) {
		int newSide = _lastSide;
		while(newSide == _lastSide) {
			newSide = rand()%_numSides;
		}
		startSide = _sides[newSide];
		_lastSide = newSide;
	} else {
		_lastSide++;
		if(_lastSide >= _numSides) {
			_lastSide = 0;
		}
		startSide = _sides[_lastSide];
	}

	//starting angle
	float angle;
	float dir = 1.0;
	if(_bRandAngle) {
		angle = float(rand() % int((_maxAngle-_minAngle)*100.0)) / 100.0 + _minAngle;
	} else {
		angle = _maxAngle;
	}

	if(angle > PI/2.0) {
		dir = dir * -1.0;
	}

	//pick position and set up direction
	float normX, normY;  //keep normalized!
	int startX, startY;

	switch(startSide) {
		case Left:
			//left side
			normX = sin(angle);
			normY = -1.0*cos(angle);
			startX = int(float(_screenObj->_sizeX)*-1.0*sizePercentX);  //remember: position is bottom left corner
			startY = (rand() % int(float(_screenObj->_sizeY)*0.90)) + int(float(_screenObj->_sizeY)*0.05);
			break;
		case Right:
			//right side
			normX = -1.0*sin(angle);
			normY = cos(angle);
			startX = _screenObj->_sizeX-1;
			startY = (rand() % int(float(_screenObj->_sizeY)*0.90)) + int(float(_screenObj->_sizeY)*0.05);
			break;
		case Top:
			//top
			normX = -1.0*cos(angle);
			normY = -1.0*sin(angle);
			startX = (rand() % int(float(_screenObj->_sizeX)*0.90)) + int(float(_screenObj->_sizeX)*0.05);
			startY = _screenObj->_sizeY-1;
			break;
		case Bottom:
		default:
			//bottom
			normX = cos(angle);
			normY = sin(angle);
			startX = (rand() % int(float(_screenObj->_sizeX)*0.90)) + int(float(_screenObj->_sizeX)*0.05);
			startY = int(float(_screenObj->_sizeY)*-1.0*sizePercentY);  //remember: position is bottom left corner
			break;
	}

	retVal->setPosition(startX, startY);
	retVal->setDirection(normX, normY);

	return retVal;
}

//convert between string and side_type
QString LensProfile::sideToString(side_type side) {
	switch(side) {
		case Left:
			return QString("Left");
		case Right:
			return QString("Right");
		case Top:
			return QString("Top");
		case Bottom:
			return QString("Bottom");
		default:
			return QString("NoSide");
	}
}

side_type LensProfile::stringToSide(QString side) {
	if(side == "Left")
		return Left;
	if(side == "Right")
		return Right;
	if(side == "Top")
		return Top;
	if(side == "Bottom")
		return Bottom;
	return NoSide;
}

/*
 * Whether this profile has been initialized or not.
 */
bool LensProfile::isInitialized() {
	return _bInitialized;
}

/*
 * Creates a new object with this object's settings.
 */
LensProfile* LensProfile::clone() {
	return new LensProfile();
}

// deep copies the specified target
void LensProfile::baseCopy(LensProfile& other) {
  if(other._pal != NULL) {
    _pal = new IndexedPalette();
    *_pal = *other._pal;
  } else {
    _pal = NULL;
  }

	_name = other._name;
	_palName = other._palName;
	_outlineWidth = other._outlineWidth;

	_bRandTint = other._bRandTint;
	_bUseComplement = other._bUseComplement;
	_bUseTint = other._bUseTint;
	_bUseOutlineComplement = other._bUseOutlineComplement;
	_bUseOutlineTint = other._bUseOutlineTint;
	_lastColorIndex = other._lastColorIndex;

	_minSpeed = other._minSpeed;
	_maxSpeed = other._maxSpeed;
	_bRandSpeed = other._bRandSpeed;

	_minAngle = other._minAngle;
	_maxAngle = other._maxAngle;
	_bRandAngle = other._bRandAngle;

	_numSides = other._numSides;
	for(int i=0; i<_numSides; i++) {
		_sides[i] = other._sides[i];
	}
	_lastSide = other._lastSide;
	_bRandSide = other._bRandSide;
	_bInitialized = other._bInitialized;
}

/*
 * Overloaded assignment operator
 */
LensProfile& LensProfile::operator=(LensProfile& other) {

	baseCopy(other);

	return *this;
}

