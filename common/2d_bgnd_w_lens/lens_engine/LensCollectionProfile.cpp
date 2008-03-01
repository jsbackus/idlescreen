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
#include "LensCollectionProfile.h"

LensCollectionProfile::LensCollectionProfile(QHash<QString, LensProfile*>* lensHash) {
	_xmlTypeValue = "LensCollectionProfile";

	_lensHash = lensHash;
	_screenObj = NULL;
	_lastProfile = -1;

	_maxNumLenses = 5;
	_bRandProfile = false;
}

LensCollectionProfile::~LensCollectionProfile() {
	_lensHash = NULL;
}

/* Attempts to load this background profile object from the
 * specified QDomNode.
 */
LensProfile* LensCollectionProfile::load(QDomNode &node) {
	int tmpI;
	LensCollectionProfile* retVal = new LensCollectionProfile(_lensHash);

	QDomElement tempElem;

	//sanity check on the input
	if(node.nodeName() != getXMLTagName())
		return NULL;

	//make sure that the type value matches
	tempElem = node.firstChildElement("type");
	if(tempElem.isNull() || tempElem.text() != _xmlTypeValue)
		return NULL;

	tempElem = node.firstChildElement("name");
	if(!tempElem.isNull()) {
		retVal->_name = tempElem.text();
	}

	tempElem = node.firstChildElement("instances_max");
	if(!tempElem.isNull()) {
		tmpI = tempElem.text().toInt();
		if(tmpI > 0 && tmpI < 10000) {
			retVal->_maxNumLenses = tmpI;
		}
	}

	tempElem = node.firstChildElement("random_profile");
	if(!tempElem.isNull()) {
		retVal->_bRandProfile = stringToBool(tempElem.text());
	}

	//Read in the profile list.  We want to preserve the order of this list, indicated by the
	//stored location.  We will start with the last node in the hopes that the order is preserved
	//in the XML file and during read back.  That way, we only have to grow the list once.  We
	//don't want to just use the _imageHistorySize, because there may be fewer entries and we
	//don't want empty strings at the end of the list.
	QDomElement tempNode = node.firstChildElement("profile_list").lastChildElement("profile_entry");
	while(!tempNode.isNull()) {	
		QString name;
		int index;

		tempElem = tempNode.firstChildElement("name");
		if(!tempElem.isNull()) {
			name = tempElem.text();
		}

		tempElem = tempNode.firstChildElement("location");
		if(!tempElem.isNull()) {
			tmpI = tempElem.text().toInt();
			if(tmpI >= 0) {
				index = tmpI;
			}
		}

		//grow the list to the appropriate size.  Should only be executed once - assuming
		//the order of entries is preserved within the XML file and when reading back.
		while(index >= retVal->_profiles.size()) {
			retVal->_profiles.append("");
		}

		//insert the string at the appropriate location
		retVal->_profiles.replace(index, name);

		tempNode = tempNode.previousSiblingElement("profile_entry");
	}

	return retVal;
}

/*
 * Returns a QDomNode object that represents this profile.
 */
QDomNode LensCollectionProfile::save(QDomDocument* doc) {
	QString tempStr;
	QDomText tempNode;
	QDomElement tempElem;
	QDomElement retVal = doc->createElement(getXMLTagName());

	tempNode = doc->createTextNode(_xmlTypeValue);
	tempElem = doc->createElement("type");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	tempNode = doc->createTextNode(_name);
	tempElem = doc->createElement("name");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	tempStr.setNum(_maxNumLenses);
	tempNode = doc->createTextNode(tempStr);
	tempElem = doc->createElement("instances_max");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	tempNode = doc->createTextNode(boolToString(_bRandProfile));
	tempElem = doc->createElement("random_profile");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	//image history list.  If the expanded list of images is empty,
	//store _configHistoryList, otherwise translate from the list
	//of indices
	QDomElement profileElem = doc->createElement("profile_list");
	QDomElement entryElem;
	for(int i=0;i<_profiles.size(); i++) {
		entryElem = doc->createElement("profile_entry");
		//name
		tempNode = doc->createTextNode(_profiles.at(i));
		tempElem = doc->createElement("name");
		tempElem.appendChild(tempNode);
		entryElem.appendChild(tempElem);

		//history location
		tempStr.setNum(i);
		tempNode = doc->createTextNode(tempStr);
		tempElem = doc->createElement("location");
		tempElem.appendChild(tempNode);
		entryElem.appendChild(tempElem);

		profileElem.appendChild(entryElem);
	}
	retVal.appendChild(profileElem);

	return retVal;
}

/*
 * Creates and returns a new LensObject object.
 */
LensObject* LensCollectionProfile::getNewLensObject() {
	if(_screenObj == NULL)
		return NULL;

	if(_bRandProfile) {
		int newProfileNum = _lastProfile;
		while(newProfileNum == _lastProfile) {
			newProfileNum = rand() % _profiles.size();
		}
		_lastProfile = newProfileNum;
	} else {
		_lastProfile = (_lastProfile+1) % _profiles.size();
	}
	LensProfile* tmpProfile = _lensHash->value(_profiles.at(_lastProfile));
	if(tmpProfile == NULL)
		return NULL;

	return tmpProfile->getNewLensObject();
}

/*
 * Returns the maximum number of lenses this profile will allow.
 */
int LensCollectionProfile::getMaxNumLenses() {
	return _maxNumLenses;
}

/*
 * Initializes the profile so that it can generate new lenses with getNewLensObject().
 */
void LensCollectionProfile::initialize(QHash<QString, IndexedPaletteProfile*>* palHash, screen_struct* screenObj) {
	LensProfile* tmpProfile;

	_screenObj = screenObj;

	for(int i=0;i<_profiles.size();i++) {
		tmpProfile = _lensHash->value(_profiles.at(i));
		if(tmpProfile != NULL && !tmpProfile->isInitialized()) {
			tmpProfile->initialize(palHash, screenObj);
		}
	}
	_bInitialized = true;
}
