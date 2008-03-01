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

#include "IndexedPaletteProfile.h"
#include "../utility/misc_funcs.h"

IndexedPaletteProfile::IndexedPaletteProfile() {

	//set defaults
	_name = "New Palette";
	_bInterpolateColors = false;
	_bWrapColors = false;
	_width = 0;
	_height = 0;
	for(int i=0;i<3;i++) {
		_defaultColor[i] = 0;
	}
}

/* Attempts to load this IndexedPaletteProfile object from the
 * specified QDomNode.
 */
IndexedPaletteProfile* IndexedPaletteProfile::load(QDomNode &node) {

	QDomElement tempElem;
	int tmpI;

	//sanity check on the input
	if(node.nodeName() != getXMLTagName())
		return NULL;

	IndexedPaletteProfile* retVal = new IndexedPaletteProfile();

	tempElem = node.firstChildElement("name");
	if(!tempElem.isNull()) {
		retVal->_name = tempElem.text();
	}

	tempElem = node.firstChildElement("width");
	if(!tempElem.isNull()) {
		tmpI = tempElem.text().toInt();
		if(tmpI > 0 && tmpI < 10240) {
			retVal->_width = tmpI;
		}
	}

	tempElem = node.firstChildElement("height");
	if(!tempElem.isNull()) {
		tmpI = tempElem.text().toInt();
		if(tmpI > 0 && tmpI < 10240) {
			retVal->_height = tmpI;
		}
	}

	tempElem = node.firstChildElement("interpolate_colors");
	if(!tempElem.isNull()) {
		retVal->_bInterpolateColors = stringToBool(tempElem.text());
	}

	tempElem = node.firstChildElement("wrap_colors");
	if(!tempElem.isNull()) {
		retVal->_bWrapColors = stringToBool(tempElem.text());
	}

	QDomElement colorElem = node.firstChildElement("default_color");
	if(!colorElem.isNull()) {
		tempElem = colorElem.firstChildElement("red");
		if(!tempElem.isNull()) {
			tmpI = tempElem.text().toInt();
			if(tmpI >= 0 && tmpI <= 255) {
				retVal->_defaultColor[0] = tmpI;
			}
		}

		tempElem = colorElem.firstChildElement("blue");
		if(!tempElem.isNull()) {
			tmpI = tempElem.text().toInt();
			if(tmpI >= 0 && tmpI <= 255) {
				retVal->_defaultColor[1] = tmpI;
			}
		}

		tempElem = colorElem.firstChildElement("green");
		if(!tempElem.isNull()) {
			tmpI = tempElem.text().toInt();
			if(tmpI >= 0 && tmpI <= 255) {
				retVal->_defaultColor[2] = tmpI;
			}
		}
	}

	colorElem = node.firstChildElement("color_list").firstChildElement("color_entry");
	while(!colorElem.isNull()) {	
		color_data_struct tmpColor;

		tempElem = colorElem.firstChildElement("red");
		if(!tempElem.isNull()) {
			tmpI = tempElem.text().toInt();
			if(tmpI >= 0 && tmpI <= 255) {
				tmpColor.r = tmpI;
			}
		}

		tempElem = colorElem.firstChildElement("green");
		if(!tempElem.isNull()) {
			tmpI = tempElem.text().toInt();
			if(tmpI >= 0 && tmpI <= 255) {
				tmpColor.g = tmpI;
			}
		}

		tempElem = colorElem.firstChildElement("blue");
		if(!tempElem.isNull()) {
			tmpI = tempElem.text().toInt();
			if(tmpI >= 0 && tmpI <= 255) {
				tmpColor.b = tmpI;
			}
		}

		tempElem = colorElem.firstChildElement("alpha");
		if(!tempElem.isNull()) {
			tmpI = tempElem.text().toInt();
			if(tmpI >= 0 && tmpI <= 255) {
				tmpColor.a = tmpI;
			}
		}

		tempElem = colorElem.firstChildElement("x_coordinate");
		if(!tempElem.isNull()) {
			tmpI = tempElem.text().toInt();
			if(tmpI >= 0 && tmpI < retVal->_width) {
				tmpColor.x = tmpI;
			}
		}

		tempElem = colorElem.firstChildElement("y_coordinate");
		if(!tempElem.isNull()) {
			tmpI = tempElem.text().toInt();
			if(tmpI >= 0 && tmpI < retVal->_height) {
				tmpColor.y = tmpI;
			}
		}

		retVal->_colorList.append(tmpColor);

		colorElem = colorElem.nextSiblingElement("color_entry");
	}

	return retVal;
}

/*
 * Returns a QDomNode object that represents this palette.
 */
QDomNode IndexedPaletteProfile::save(QDomDocument* doc) {
	QString tempStr;
	QDomText tempNode;
	QDomElement tempElem;
	QDomElement retVal = doc->createElement(getXMLTagName());

	//name
	tempNode = doc->createTextNode(_name);
	tempElem = doc->createElement("name");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	//palette width
	tempStr.setNum(_width);
	tempNode = doc->createTextNode(tempStr);
	tempElem = doc->createElement("width");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	//palette height
	tempStr.setNum(_height);
	tempNode = doc->createTextNode(tempStr);
	tempElem = doc->createElement("height");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	//interpolate colors?
	tempNode = doc->createTextNode(boolToString(_bInterpolateColors));
	tempElem = doc->createElement("interpolate_colors");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	//wrap colors when interpolating?
	tempNode = doc->createTextNode(boolToString(_bWrapColors));
	tempElem = doc->createElement("wrap_colors");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	//default color
	QDomElement colorElem = doc->createElement("default_color");

		//Red aspect
		tempStr.setNum(_defaultColor[0]);
		tempNode = doc->createTextNode(tempStr);
		tempElem = doc->createElement("red");
		tempElem.appendChild(tempNode);
		colorElem.appendChild(tempElem);
	
		//Green aspect
		tempStr.setNum(_defaultColor[1]);
		tempNode = doc->createTextNode(tempStr);
		tempElem = doc->createElement("green");
		tempElem.appendChild(tempNode);
		colorElem.appendChild(tempElem);
	
		//Blue aspect
		tempStr.setNum(_defaultColor[2]);
		tempNode = doc->createTextNode(tempStr);
		tempElem = doc->createElement("blue");
		tempElem.appendChild(tempNode);
		colorElem.appendChild(tempElem);
	
	retVal.appendChild(colorElem);

	//color list
	QDomElement colorElemList = doc->createElement("color_list");
	for(int i=0;i<_colorList.size();i++) {
		color_data_struct tmpColor = _colorList.at(i);
		colorElem = doc->createElement("color_entry");

		//Red value
		tempStr.setNum(tmpColor.r);
		tempNode = doc->createTextNode(tempStr);
		tempElem = doc->createElement("red");
		tempElem.appendChild(tempNode);
		colorElem.appendChild(tempElem);
	
		//Green value
		tempStr.setNum(tmpColor.g);
		tempNode = doc->createTextNode(tempStr);
		tempElem = doc->createElement("green");
		tempElem.appendChild(tempNode);
		colorElem.appendChild(tempElem);
	
		//Blue value
		tempStr.setNum(tmpColor.b);
		tempNode = doc->createTextNode(tempStr);
		tempElem = doc->createElement("blue");
		tempElem.appendChild(tempNode);
		colorElem.appendChild(tempElem);
	
		//Alpha value
		tempStr.setNum(tmpColor.a);
		tempNode = doc->createTextNode(tempStr);
		tempElem = doc->createElement("alpha");
		tempElem.appendChild(tempNode);
		colorElem.appendChild(tempElem);

		//X coordinate
		tempStr.setNum(tmpColor.x);
		tempNode = doc->createTextNode(tempStr);
		tempElem = doc->createElement("x_coordinate");
		tempElem.appendChild(tempNode);
		colorElem.appendChild(tempElem);

		//Y coordinate
		tempStr.setNum(tmpColor.y);
		tempNode = doc->createTextNode(tempStr);
		tempElem = doc->createElement("y_coordinate");
		tempElem.appendChild(tempNode);
		colorElem.appendChild(tempElem);

		colorElemList.appendChild(colorElem);
	}
	retVal.appendChild(colorElemList);

	return retVal;
}
/*
 * Creates and returns a new IndexedPalette based on this palette profile.
 */
IndexedPalette* IndexedPaletteProfile::createPalette() {
	IndexedPalette* retVal = new IndexedPalette();

	retVal->setDefaultColor(_defaultColor[0], _defaultColor[1], _defaultColor[2]);
	retVal->empty(_width, _height);

	for(int i=0;i<_colorList.size();i++) {
		color_data_struct tmpStruct = _colorList.at(i);
		retVal->setColor(tmpStruct.x, tmpStruct.y,tmpStruct.r, tmpStruct.g, tmpStruct.b, tmpStruct.a);
	}

	if(_bInterpolateColors) {
		retVal->interpolate(_bWrapColors);
	}

	return retVal;
}

/*
 * Get/set the name of this profile.
 */
void IndexedPaletteProfile::setName(QString &name) {
	_name = name;
}

QString IndexedPaletteProfile::getName() {
	return _name;
}

/*
 * Get/set the specified color.  If the color has been defined,
 * the default color will be returned.
 */
void IndexedPaletteProfile::setColor(int x, int y, int r, int g, int b, int a) {
	if(x >= _width || y >= _height)
		return;

	color_data_struct tmpColor;

	//search to see if the specified color is in the list
	int i=0;
	while(i < _colorList.size()) {
		tmpColor = _colorList.at(i);
		if(tmpColor.x == x && tmpColor.y == y) {
			tmpColor.r = r;
			tmpColor.g = g;
			tmpColor.b = b;
			tmpColor.a = a;
			_colorList.replace(i, tmpColor);
			return;
		}
		i++;
	}

	//couldn't find it.  Add it to the end
	tmpColor.r = r;
	tmpColor.g = g;
	tmpColor.b = b;
	tmpColor.a = a;
	tmpColor.x = x;
	tmpColor.y = y;

	_colorList.append(tmpColor);
};

void IndexedPaletteProfile::setColor(int x, int y, QRgb color) {
	if(x >= _width || y >= _height)
		return;

	color_data_struct tmpColor;

	//search to see if the specified color is in the list
	int i=0;
	while(i < _colorList.size()) {
		tmpColor = _colorList.at(i);
		if(tmpColor.x == x && tmpColor.y == y) {
			tmpColor.r = qRed(color);
			tmpColor.g = qGreen(color);
			tmpColor.b = qBlue(color);
			tmpColor.a = qAlpha(color);
			_colorList.replace(i, tmpColor);
			return;
		}
		i++;
	}

	tmpColor.r = qRed(color);
	tmpColor.g = qGreen(color);
	tmpColor.b = qBlue(color);
	tmpColor.a = qAlpha(color);
	tmpColor.x = x;
	tmpColor.y = y;

	_colorList.append(tmpColor);
};

void IndexedPaletteProfile::getColor(int x, int y, int* r, int* g, int* b, int* a) {
	if(x >= _width || y >= _height) {
		*r = _defaultColor[0];
		*g = _defaultColor[1];
		*b = _defaultColor[2];
		*a = 0;
		return;
	}

	int i=0;
	while(i < _colorList.size()) {
		color_data_struct tmpColor = _colorList.at(i);
		if(tmpColor.x == x && tmpColor.y == y) {
			*r = tmpColor.r;
			*g = tmpColor.g;
			*b = tmpColor.b;
			*a = tmpColor.a;
			return;
		}
		i++;
	}

	*r = _defaultColor[0];
	*g = _defaultColor[1];
	*b = _defaultColor[2];
	*a = 0;
};

QRgb IndexedPaletteProfile::getColor(int x, int y) {
	if(x >= _width || y >= _height) {
		return qRgba(_defaultColor[0], _defaultColor[1], _defaultColor[2], 0);
	}

	int i=0;
	while(i < _colorList.size()) {
		color_data_struct tmpColor = _colorList.at(i);
		if(tmpColor.x == x && tmpColor.y == y) {
			return qRgba(tmpColor.r, tmpColor.g, tmpColor.b, tmpColor.a);
		}
		i++;
	}

	return qRgba(_defaultColor[0], _defaultColor[1], _defaultColor[2], 0);
};

/*
 * Whether the specified color has been previously defined.
 */
bool IndexedPaletteProfile::isColorDefined(int x, int y) {
	if(x >= _width || y >= _height)
		return false;

	int i=0;
	while(i < _colorList.size()) {
		color_data_struct tmpColor = _colorList.at(i);
		if(tmpColor.x == x && tmpColor.y == y)
			return true;
		i++;
	}

	return false;
}

/*
 * Get/set default color.
 */
void IndexedPaletteProfile::setDefaultColor(int r, int g, int b) {
	_defaultColor[0] = r;
	_defaultColor[1] = g;
	_defaultColor[2] = b;
}

void IndexedPaletteProfile::setDefaultColor(QRgb color) {
	_defaultColor[0] = qRed(color);
	_defaultColor[1] = qGreen(color);
	_defaultColor[2] = qBlue(color);
}

void IndexedPaletteProfile::getDefaultColor(int* r, int* g, int* b) {
	*r = _defaultColor[0];
	*g = _defaultColor[1];
	*b = _defaultColor[2];
}

QRgb IndexedPaletteProfile::getDefaultColor(void) {
	return qRgba(_defaultColor[0], _defaultColor[1], _defaultColor[2], 0);
}

/*
 * Get/set width and height
 */
void IndexedPaletteProfile::setWidth(int width) {
	//if the new width is smaller, strip out colors outside the new width
	if(width < _width) {
		//start at end so that we don't have to check the size each time
		for(int i=_colorList.size()-1; i>=0;i--) {
			color_data_struct tmpColor = _colorList.at(i);
			if(tmpColor.x >= width) {
				_colorList.removeAt(i);
			}
		}
	}
	_width = width;
}

int IndexedPaletteProfile::getWidth() {
	return _width;
}

void IndexedPaletteProfile::setHeight(int height) {
	//if the new height is smaller, strip out colors outside the new height
	if(height < _height) {
		//start at end so that we don't have to check the size each time
		for(int i=_colorList.size()-1; i>=0;i--) {
			color_data_struct tmpColor = _colorList.at(i);
			if(tmpColor.y >= height) {
				_colorList.removeAt(i);
			}
		}
	}
	_height = height;
}

int IndexedPaletteProfile::getHeight() {
	return _height;
}

/*
 * Get/set interpolate colors
 */
void IndexedPaletteProfile::setInterpolateColors(bool bInterpolateColors) {
	_bInterpolateColors = bInterpolateColors;
}

bool IndexedPaletteProfile::getInterpolateColors() {
	return _bInterpolateColors;
}

/*
 * get/set wrap colors
 */
void IndexedPaletteProfile::setWrapColors(bool bWrapColors) {
	_bWrapColors = bWrapColors;
}

bool IndexedPaletteProfile::getWrapColors() {
	return _bWrapColors;
}

/*
 * Returns the tag name used by Dom elements.
 */
QString IndexedPaletteProfile::getXMLTagName() {
	return "IndexedPaletteProfile";
}

/*
 * Overloaded assignment operator
 */
IndexedPaletteProfile& IndexedPaletteProfile::operator=(IndexedPaletteProfile& other) {

	_name = other._name;
	_bInterpolateColors = other._bInterpolateColors;
	_bWrapColors = other._bWrapColors;
	_width = other._width;
	_height = other._height;

	for(int i=0; i<3; i++) {
		_defaultColor[i] = other._defaultColor[i];
	}

	//deep copy of list
	for(i=0; i<other._colorList.size();i++){
		color_data_struct tmpColor = other._colorList.at(i);
		_colorList.append(tmpColor);
	}

	return *this;
}

