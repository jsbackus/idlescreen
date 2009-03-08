/**
 * Copyright (c) 2009 Jeff Backus.
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

// begin debug
//#include <iostream>
//using namespace std;
// end debug

#include <QtXml/QDomElement>
#include <QtXml/QDomText>

//#include "../../common/utility/misc_funcs.h"

#include "utility/misc_funcs.h"
#include "PlasmaGeneratorProfile.h"

PlasmaGeneratorProfile::PlasmaGeneratorProfile() {

  //defaults
  _name = "Empty";
  _palName = "";
  _coarseness = 1.0;
  _bClampColorIndex = true;
  _bTileHorizontal = false;
  _bTileVertical = false;
  _height = 0;
  _width = 0;
  _targetFileName = "out.bmp";
  _policy = ASK;
}

PlasmaGeneratorProfile::~PlasmaGeneratorProfile() {
  
}

/* Attempts to load this background profile object from the
 * specified QDomNode.
 */
PlasmaGeneratorProfile* PlasmaGeneratorProfile::load(QDomNode &node) {
  float tmpF;
  int tmpI;
  QDomElement tempElem;

  //sanity check on the input
  if(node.nodeName() != getXMLTagName())
    return NULL;

  PlasmaGeneratorProfile* retVal = new PlasmaGeneratorProfile();
  if(retVal == NULL)
    return NULL;

  tempElem = node.firstChildElement("name");
  if(!tempElem.isNull()) {
    retVal->_name = tempElem.text();
  }

  tempElem = node.firstChildElement("palette");
  if(!tempElem.isNull()) {
    retVal->_palName = tempElem.text();
  }
	
  tempElem = node.firstChildElement("coarseness");
  if(!tempElem.isNull()) {
    tmpF = tempElem.text().toFloat();
    if(tmpF > 0.00 && tmpF < 10000.00) {
      retVal->_coarseness = tmpF;
    }
  }

  tempElem = node.firstChildElement("clamp_color");
  if(!tempElem.isNull()) {
    QString param = tempElem.text();
    retVal->_bClampColorIndex = stringToBool(param);
  }

  tempElem = node.firstChildElement("tile_horizontal");
  if(!tempElem.isNull()) {
    QString param = tempElem.text();
    retVal->_bTileHorizontal = stringToBool(param);
  }

  tempElem = node.firstChildElement("tile_vertical");
  if(!tempElem.isNull()) {
    QString param = tempElem.text();
    retVal->_bTileVertical = stringToBool(param);
  }

  tempElem = node.firstChildElement("height");
  if(!tempElem.isNull()) {
    tmpI = tempElem.text().toInt();
    if(tmpI > 0 && tmpI < 10000) {
      retVal->_height = tmpI;
    }
  }

  tempElem = node.firstChildElement("width");
  if(!tempElem.isNull()) {
    tmpI = tempElem.text().toInt();
    if(tmpI > 0 && tmpI < 10000) {
      retVal->_width = tmpI;
    }
  }

  tempElem = node.firstChildElement("target");
  if(!tempElem.isNull()) {
    retVal->_targetFileName = tempElem.text();
  }
	
  tempElem = node.firstChildElement("policy");
  if(!tempElem.isNull()) {
    if(tempElem.text().compare(QString("ALWAYS"), Qt::CaseInsensitive) == 0) {
      retVal->_policy = ALWAYS;
    } else 
      if(tempElem.text().compare(QString("NEVER"), Qt::CaseInsensitive) == 0) {
	retVal->_policy = NEVER;
      } else {
	retVal->_policy = ASK;
      }
  }
  return retVal;
}


/*
 * Returns a QDomNode object that represents this profile.
 */
QDomNode PlasmaGeneratorProfile::save(QDomDocument* doc) {
  QString tempStr;
  QDomText tempNode;
  QDomElement tempElem;
  QDomElement retVal = doc->createElement(getXMLTagName());

  //name
  tempNode = doc->createTextNode(_name);
  tempElem = doc->createElement("name");
  tempElem.appendChild(tempNode);
  retVal.appendChild(tempElem);

  //palette name
  tempNode = doc->createTextNode(_palName);
  tempElem = doc->createElement("palette");
  tempElem.appendChild(tempNode);
  retVal.appendChild(tempElem);

  //coarseness
  tempStr.setNum(_coarseness);
  tempNode = doc->createTextNode(tempStr);
  tempElem = doc->createElement("coarseness");
  tempElem.appendChild(tempNode);
  retVal.appendChild(tempElem);
	
  //clamp color index?
  tempNode = doc->createTextNode(boolToString(_bClampColorIndex));
  tempElem = doc->createElement("clamp_color");
  tempElem.appendChild(tempNode);
  retVal.appendChild(tempElem);

  //tile horizontally?
  tempNode = doc->createTextNode(boolToString(_bTileHorizontal));
  tempElem = doc->createElement("tile_horizontal");
  tempElem.appendChild(tempNode);
  retVal.appendChild(tempElem);

  //tile vertically?
  tempNode = doc->createTextNode(boolToString(_bTileVertical));
  tempElem = doc->createElement("tile_vertical");
  tempElem.appendChild(tempNode);
  retVal.appendChild(tempElem);

  //height
  tempStr.setNum(_height);
  tempNode = doc->createTextNode(tempStr);
  tempElem = doc->createElement("height");
  tempElem.appendChild(tempNode);
  retVal.appendChild(tempElem);

  //width
  tempStr.setNum(_width);
  tempNode = doc->createTextNode(tempStr);
  tempElem = doc->createElement("width");
  tempElem.appendChild(tempNode);
  retVal.appendChild(tempElem);

  //target file name
  tempNode = doc->createTextNode(_targetFileName);
  tempElem = doc->createElement("target");
  tempElem.appendChild(tempNode);
  retVal.appendChild(tempElem);

  //overwrite policy
  switch(_policy) {
  case ALWAYS:
    tempNode = doc->createTextNode("ALWAYS");
    break;
  case NEVER:
    tempNode = doc->createTextNode("NEVER");
    break;
  default:
    tempNode = doc->createTextNode("ASK");
  }
  tempElem = doc->createElement("policy");
  tempElem.appendChild(tempNode);
  retVal.appendChild(tempElem);

  return retVal;
}

/*
 * Creates and returns a new PlasmaGenerator object.  Does not
 * generate the plasma.
 */
PlasmaGenerator* PlasmaGeneratorProfile::getGenerator(QHash<QString, IndexedPaletteProfile*>* palHash) {

  if(palHash == NULL)
    return NULL;

  PlasmaGenerator* retVal = new PlasmaGenerator();
  if(retVal == NULL)
    return NULL;

  IndexedPaletteProfile* palProfile = palHash->value(_palName);
  if(palProfile == NULL) {
    delete retVal;
    retVal = NULL;
    return NULL;
  }

  IndexedPalette* pal = palProfile->createPalette();
  if(pal == NULL) {
    delete retVal;
    retVal = NULL;
    return NULL;
  }

  retVal->setHeight(_height);
  retVal->setWidth(_width);
  retVal->setPalette(*pal);
  retVal->setClampColor(_bClampColorIndex);
  retVal->setTileHorizontal(_bTileHorizontal);
  retVal->setTileVertical(_bTileVertical);
  retVal->setCoarseness(_coarseness);

  // clean up
  delete pal;
  pal = NULL;

  return retVal;
}

/*
 * get/set palette name.
 */
QString PlasmaGeneratorProfile::getPaletteName() {
  return _palName;
}

void PlasmaGeneratorProfile::setPaletteName(QString paletteName) {
  _palName = paletteName;
}

/*
 * get/set coarseness.
 */
float PlasmaGeneratorProfile::getCoarseness() {
  return _coarseness;
}

void PlasmaGeneratorProfile::setCoarseness(float coarseness) {
  _coarseness = coarseness;
}

/*
 * Get/set clamp color index.
 */
bool PlasmaGeneratorProfile::getClampColorIndex() {
  return _bClampColorIndex;
}

void PlasmaGeneratorProfile::setClampColorIndex(bool bClampColorIndex) {
  _bClampColorIndex = bClampColorIndex;
}

/*
 * Overloaded assignment operator
 */
PlasmaGeneratorProfile& PlasmaGeneratorProfile::operator=(PlasmaGeneratorProfile& other) {
  _name = other._name;
  _palName = other._palName;
  _coarseness = other._coarseness;
  _bClampColorIndex = other._bClampColorIndex;
  _bTileHorizontal = other._bTileHorizontal;
  _bTileVertical = other._bTileVertical;
  _height = other._height;
  _width = other._width;
  _targetFileName = other._targetFileName;
  _policy = other._policy;

  return *this;
}

/*
 * Creates a new object with this object's settings.
 */
PlasmaGeneratorProfile* PlasmaGeneratorProfile::clone() {
  PlasmaGeneratorProfile* retVal = new PlasmaGeneratorProfile();

  *retVal = *this;

  return retVal;
}

/**
 * Called whenever palette names change
 */
void PlasmaGeneratorProfile::paletteNameChanged(QString oldName,
							QString newName) {
  // if the palette name matches the old name, change the name.
  if(_palName == oldName) {
    _palName = newName;
  }
}
	
/**
 * Called whenever a palette is removed.
 */
void PlasmaGeneratorProfile::paletteRemoved(QString palName) {
  // if this palette has been removed, set current palette name to ""
  if(_palName == palName) {
    _palName = QString("");
  }
}

/**
 * Gets the current filename target.
 */
QString PlasmaGeneratorProfile::getFileTarget() {
  return _targetFileName;
}

/**
 * Sets the current filename target.
 */
void PlasmaGeneratorProfile::setFileTarget(QString filename) {
  _targetFileName = filename;
}

/**
 * Gets file overwrite policy.
 */
PlasmaGeneratorOverwritePolicy PlasmaGeneratorProfile::getOverwritePolicy() {
  return _policy;
}

/**
 * Sets the file overwrite policy.
 */
void PlasmaGeneratorProfile::setOverwritePolicy(PlasmaGeneratorOverwritePolicy policy) {
  _policy = policy;
}


/*
 * Get/set the name of this profile.
 */
void PlasmaGeneratorProfile::setName(QString &name) {
  _name = name;
}
QString PlasmaGeneratorProfile::getName() {
  return _name;
}

/*
 * Returns the tag name used by Dom elements for all
 * subclasses of BackgroundProfile.
 */
QString PlasmaGeneratorProfile::getXMLTagName() {
  return QString("PlasmaGeneratorProfile");
}
