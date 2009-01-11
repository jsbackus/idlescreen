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

#include "MasterProfile.h"
MasterProfile::MasterProfile() {
	_name = "New Profile";

	_screenWidth = 256;
	_screenHeight = 256;

	//_backgroundProfileName;
	//_lensProfileName;

	_timerMillis = 50;
}

MasterProfile::~MasterProfile() {

}

/* Attempts to load this background profile object from the
 * specified QDomNode.
 */
MasterProfile* MasterProfile::load(QDomNode &node) {
	
	QDomElement tempElem;

	//sanity check on the input
	if(node.nodeName() != getXMLTagName())
		return NULL;

	MasterProfile* retVal = new MasterProfile();

	tempElem = node.firstChildElement("name");
	if(!tempElem.isNull()) {
		retVal->_name = tempElem.text();
	}

	tempElem = node.firstChildElement("width");
	if(!tempElem.isNull()) {
		retVal->_screenWidth = tempElem.text().toInt();
	}

	tempElem = node.firstChildElement("height");
	if(!tempElem.isNull()) {
		retVal->_screenHeight = tempElem.text().toInt();
	}

	tempElem = node.firstChildElement("timer_millis");
	if(!tempElem.isNull()) {
		retVal->_timerMillis = tempElem.text().toInt();
	}

	tempElem = node.firstChildElement("background_profile");
	if(!tempElem.isNull()) {
		retVal->_backgroundProfileName = tempElem.text();
	}

	tempElem = node.firstChildElement("lens_profile");
	if(!tempElem.isNull()) {
		retVal->_lensProfileName = tempElem.text();
	}

	return retVal;
}

/*
 * Returns a QDomNode object that represents this profile.
 */
QDomNode MasterProfile::save(QDomDocument* doc) {
	QString tempStr;
	QDomText tempNode;
	QDomElement tempElem;
	QDomElement retVal = doc->createElement(getXMLTagName());

	//name
	tempNode = doc->createTextNode(_name);
	tempElem = doc->createElement("name");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	//screen width
	tempStr.setNum(_screenWidth);
	tempNode = doc->createTextNode(tempStr);
	tempElem = doc->createElement("width");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	//screen height
	tempStr.setNum(_screenHeight);
	tempNode = doc->createTextNode(tempStr);
	tempElem = doc->createElement("height");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	//timer millis
	tempStr.setNum(_timerMillis);
	tempNode = doc->createTextNode(tempStr);
	tempElem = doc->createElement("timer_millis");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	//background profile name
	tempNode = doc->createTextNode(_backgroundProfileName);
	tempElem = doc->createElement("background_profile");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	//lens collection name
	tempNode = doc->createTextNode(_lensProfileName);
	tempElem = doc->createElement("lens_profile");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	return retVal;
}

/*
 * Get/set the name of this profile.
 */
void MasterProfile::setName(QString &name) {
	_name = name;
}

QString MasterProfile::getName() {
	return _name;
}

/*
 * Returns the tag name used by Dom elements for all
 * subclasses of BackgroundProfile.
 */
QString MasterProfile::getXMLTagName() {
	return "MasterProfile";
}

int MasterProfile::getScreenWidth() {
	return _screenWidth;
}

int MasterProfile::getScreenHeight() {
	return _screenHeight;
}

int MasterProfile::getTextureWidth() {
	int retVal = 2;

	while(retVal < _screenWidth) {
		retVal *= 2;
	}

	return retVal;
}

int MasterProfile::getTextureHeight() {
	int retVal = 2;

	while(retVal < _screenHeight) {
		retVal *= 2;
	}

	return retVal;
}

void MasterProfile::setScreenWidth(int width) {
	_screenWidth = width;
}

void MasterProfile::setScreenHeight(int height) {
	_screenHeight = height;
}

QString MasterProfile::getBackgroundProfileName() {
	return _backgroundProfileName;
}

void MasterProfile::setBackgroundProfileName(QString name) {
	_backgroundProfileName = name;
}

QString MasterProfile::getLensProfileName() {
	return _lensProfileName;
}
void MasterProfile::setLensProfileName(QString name) {
	_lensProfileName = name;
}

int MasterProfile::getTimerMillis() {
	return _timerMillis;
}

void MasterProfile::setTimerMillis(int millis) {
	_timerMillis = millis;
}
/*
 * Overloaded assignment operator
 */
MasterProfile& MasterProfile::operator=(MasterProfile& other) {
	_name = other._name;

	_screenWidth = other._screenWidth;
	_screenHeight = other._screenHeight;

	_backgroundProfileName = other._backgroundProfileName;
	_lensProfileName = other._lensProfileName;

	_timerMillis = other._timerMillis;

	return *this;
}

