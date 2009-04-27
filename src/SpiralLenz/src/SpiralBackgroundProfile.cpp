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

#include "utility/misc_funcs.h"
#include "SpiralBackgroundProfile.h"
#include "SpiralBackground.h"
#include "spirals/SpiralAlgorithm.h"

SpiralBackgroundProfile::SpiralBackgroundProfile() {
  _xmlTypeValue = "SpiralBackground";

  //defaults
  _name = "Empty";
  _palName = "";
  _genStepsPerTick = 100;
  _bAnimatePalette = true;
  _paletteXSpeed = 1;
  _paletteYSpeed = 1;
}

SpiralBackgroundProfile::~SpiralBackgroundProfile() {
  // no es nada
}

/**
 * Attempts to load this background profile object from the
 * specified QDomNode.
 */
BackgroundProfile* SpiralBackgroundProfile::load(QDomNode &node) {
  QDomElement tempElem;

  //sanity check on the input
  if(node.nodeName() != getXMLTagName())
    return NULL;

  //make sure that the type value matches
  tempElem = node.firstChildElement("type");
  if(tempElem.isNull() || tempElem.text() != _xmlTypeValue)
    return NULL;

  SpiralBackgroundProfile* retVal = new SpiralBackgroundProfile();
  if(retVal == NULL)
    return NULL;

  // load base
  retVal->loadBase(node);

  // derivative classes would load data here...

  // return new profile
  return retVal;
}


/**
 * Attempts to load the base profile object from the
 * specified QDomNode.
 */
void SpiralBackgroundProfile::loadBase(QDomNode &node) {
  int tmpI;
  QDomElement tempElem;

  tempElem = node.firstChildElement("name");
  if(!tempElem.isNull()) {
    QString str = tempElem.text();
    setName(str);
  }

  tempElem = node.firstChildElement("steps_per_tick");
  if(!tempElem.isNull()) {
    tmpI = tempElem.text().toInt();
    setGenStepsPerTick(tmpI);
  }

  tempElem = node.firstChildElement("anmiate_palette");
  if(!tempElem.isNull()) {
    QString param = tempElem.text();
    setAnimatePalette(stringToBool(param));
  }

  tempElem = node.firstChildElement("palette_x_speed");
  if(!tempElem.isNull()) {
    tmpI = tempElem.text().toInt();
    setPaletteXSpeed(tmpI);
  }

  tempElem = node.firstChildElement("palette_y_speed");
  if(!tempElem.isNull()) {
    tmpI = tempElem.text().toInt();
    setPaletteYSpeed(tmpI);
  }

  tempElem = node.firstChildElement("palette");
  if(!tempElem.isNull()) {
    setPaletteName(tempElem.text());
  }
}

/**
 * Returns a QDomNode object that represents this profile.
 */
QDomNode SpiralBackgroundProfile::save(QDomDocument* doc) {
  QString tempStr;
  QDomText tempNode;
  QDomElement tempElem;
  QDomElement retVal = doc->createElement(getXMLTagName());

  //type
  tempNode = doc->createTextNode(_xmlTypeValue);
  tempElem = doc->createElement("type");
  tempElem.appendChild(tempNode);
  retVal.appendChild(tempElem);

  // save base class values
  saveBase(&retVal, doc);

  // derivative clases would save data here

  // return node.
  return retVal;
}


/**
 * Returns a QDomNode object that represents the base profile.
 */
void SpiralBackgroundProfile::saveBase(QDomElement* node, QDomDocument* doc) {

  QString tempStr;
  QDomText tempNode;
  QDomElement tempElem;

  //name
  tempNode = doc->createTextNode(_name);
  tempElem = doc->createElement("name");
  tempElem.appendChild(tempNode);
  node->appendChild(tempElem);

  //steps per tick
  tempStr.setNum(_genStepsPerTick);
  tempNode = doc->createTextNode(tempStr);
  tempElem = doc->createElement("steps_per_tick");
  tempElem.appendChild(tempNode);
  node->appendChild(tempElem);

  //animate palette?
  tempNode = doc->createTextNode(boolToString(_bAnimatePalette));
  tempElem = doc->createElement("anmiate_palette");
  tempElem.appendChild(tempNode);
  node->appendChild(tempElem);

  //palette X speed
  tempStr.setNum(_paletteXSpeed);
  tempNode = doc->createTextNode(tempStr);
  tempElem = doc->createElement("palette_x_speed");
  tempElem.appendChild(tempNode);
  node->appendChild(tempElem);

  //palette Y speed
  tempStr.setNum(_paletteYSpeed);
  tempNode = doc->createTextNode(tempStr);
  tempElem = doc->createElement("palette_y_speed");
  tempElem.appendChild(tempNode);
  node->appendChild(tempElem);

  //palette name
  tempNode = doc->createTextNode(_palName);
  tempElem = doc->createElement("palette");
  tempElem.appendChild(tempNode);
  node->appendChild(tempElem);
}

/**
 * Creates and returns a new background object.
 */
Background* SpiralBackgroundProfile::getNewBackgroundObj(int height, int width,
							 QHash<QString, IndexedPaletteProfile*>* palHash) {

  SpiralAlgorithm* alg = new SpiralAlgorithm();
  if(alg == NULL)
    return NULL;

  return new SpiralBackground(width, height, alg,
			      palHash->value(_palName)->createPalette(),
			      _bAnimatePalette, _paletteXSpeed, _paletteYSpeed,
			      _genStepsPerTick);
}

/**
 * get/set palette name.
 */
QString SpiralBackgroundProfile::getPaletteName() {
  return _palName;
}

void SpiralBackgroundProfile::setPaletteName(QString paletteName) {
  _palName = paletteName;
}

/**
 * get/set steps per tick
 */
int SpiralBackgroundProfile::getGenStepsPerTick() {
  return _genStepsPerTick;
}

void SpiralBackgroundProfile::setGenStepsPerTick(int genStepsPerTick) {
    _genStepsPerTick = genStepsPerTick;
}

/**
 * Get/set animate palette.
 */
bool SpiralBackgroundProfile::getAnimatePalette() {
  return _bAnimatePalette;
}

void SpiralBackgroundProfile::setAnimatePalette(bool bAnimatePalette) {
  _bAnimatePalette = bAnimatePalette;
}

/**
 * get/set palette x speed.
 */
float SpiralBackgroundProfile::getPaletteXSpeed() {
  return _paletteXSpeed;
}

void SpiralBackgroundProfile::setPaletteXSpeed(float speed) {
  if(0.0 <= speed && speed <= 30000.0)
    _paletteXSpeed = speed;
}

/**
 * get/set palette y speed.
 */
float SpiralBackgroundProfile::getPaletteYSpeed() {
  return _paletteYSpeed;
}

void SpiralBackgroundProfile::setPaletteYSpeed(float speed) {
  if(0.0 <= speed && speed <= 30000.0)
    _paletteYSpeed = speed;
}

/**
 * Called whenever palette names change
 */
void SpiralBackgroundProfile::paletteNameChanged(QString oldName,
						 QString newName) {
  // if the palette name matches the old name, change the name.
  if(_palName == oldName) {
    _palName = newName;
  }
}
	
/**
 * Called whenever a palette is removed.
 */
void SpiralBackgroundProfile::paletteRemoved(QString palName) {
  // if this palette has been removed, set current palette name to ""
  if(_palName == palName) {
    _palName = QString("");
  }
}

/**
 * Copies the base values of the specified profile to this profile.
 */
void SpiralBackgroundProfile::baseCopy(SpiralBackgroundProfile& other) {
  _name = other._name;
  _palName = other._palName;
  _genStepsPerTick = other._genStepsPerTick;
  _bAnimatePalette = other._bAnimatePalette;
  _paletteXSpeed = other._paletteXSpeed;
  _paletteYSpeed = other._paletteYSpeed;
  
}

/**
 * Overloaded assignment operator
 */
SpiralBackgroundProfile& SpiralBackgroundProfile::operator=(SpiralBackgroundProfile& other) {
  
  baseCopy(other);

  return *this;
}

/**
 * Creates a new object with this object's settings.
 */
BackgroundProfile* SpiralBackgroundProfile::clone() {
  SpiralBackgroundProfile* retVal = new SpiralBackgroundProfile();

  *retVal = *this;

  return (BackgroundProfile*)retVal;
}



