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

// begin debug
#include<iostream>
using namespace std;
//end debug

#include <QtXml/QDomElement>
#include <QtXml/QDomText>

#include "utility/misc_funcs.h"

//tmp
#include "IndexedPalette/IndexedPaletteProfile.h"
//endtmp

#include "AcidRainBackgroundProfile.h"

AcidRainBackgroundProfile::AcidRainBackgroundProfile() {
  _xmlTypeValue = "AcidRainBackground";

  //defaults
  _name = "Empty";

  _styles = NULL;
  _numStyles = 0;
  _maxNumStyles = 0;

  _maxRainDensity = 0.0;
  _gravity = 0.0;
  _maxHorizAccel = 0.0;
  _maxHorizAccelDelta = 0.0;
  _recoilElasticity = 0.0;

  // make an initially empty list of default size
  growStyleList();
}

AcidRainBackgroundProfile::~AcidRainBackgroundProfile() {
  if(_styles != NULL) {
    delete [] _styles;
    _styles = NULL;
  }
}

/** 
 * Attempts to load this background profile object from the
 * specified QDomNode.
 */
BackgroundProfile* AcidRainBackgroundProfile::load(QDomNode &node) {

  float tmpF;
  int tmpI;
  QString tmpStr;
  QDomElement tempElem;

  //sanity check on the input
  if(node.nodeName() != getXMLTagName())
    return NULL;

  //make sure that the type value matches
  tempElem = node.firstChildElement("type");
  if(tempElem.isNull() || tempElem.text() != _xmlTypeValue)
    return NULL;

  AcidRainBackgroundProfile* retVal = new AcidRainBackgroundProfile();
  if(retVal == NULL)
    return NULL;

  tempElem = node.firstChildElement("name");
  if(!tempElem.isNull()) {
    retVal->_name = tempElem.text();
  }

  tempElem = node.firstChildElement("max_rain_density");
  if(!tempElem.isNull()) {
    tmpI = tempElem.text().toInt();
    if(tmpI >= 0 && tmpI <= 100) {
      retVal->_maxRainDensity = tmpI;
    }
  }

  tempElem = node.firstChildElement("gravity");
  if(!tempElem.isNull()) {
    tmpF = tempElem.text().toFloat();
    if(tmpF > -100.0 && tmpF < 100.0) {
      retVal->_gravity = tmpF;
    }
  }

  tempElem = node.firstChildElement("recoil_elasticity");
  if(!tempElem.isNull()) {
    tmpF = tempElem.text().toFloat();
    if(tmpF > -100.0 && tmpF < 100.0) {
      retVal->_recoilElasticity = tmpF;
    }
  }

  tempElem = node.firstChildElement("max_horiz_accel");
  if(!tempElem.isNull()) {
    tmpF = tempElem.text().toFloat();
    if(tmpF > -100.0 && tmpF < 100.0) {
      retVal->_maxHorizAccel = tmpF;
    }
  }

  tempElem = node.firstChildElement("max_horiz_accel_delta");
  if(!tempElem.isNull()) {
    tmpF = tempElem.text().toFloat();
    if(tmpF > -100.0 && tmpF < 100.0) {
      retVal->_maxHorizAccelDelta = tmpF;
    }
  }

  // get style information

  // get the number of styles in order to set the initial
  // size of the list.  Styles will be added using the
  // public functions, so the list will grow as necessary.
  tempElem = node.firstChildElement("num_styles");
  if(!tempElem.isNull()) {
    tmpI = tempElem.text().toInt();
    if(tmpI >= 0 && tmpI < 100) {
      retVal->growStyleList(tmpI);
    }
  }

  // read in all crawly_style_entry objects and add each one to the 
  // list of styles.  Order isn't important, nor is whether the
  // actual number of styes matches the number in the config file.
  QDomElement tempNode = node.firstChildElement("rain_style").
    lastChildElement("rain_style_entry");

  int index = 0;
  while(!tempNode.isNull()) {	
    retVal->addStyle();

    tempElem = tempNode.firstChildElement("palette");
    if(!tempElem.isNull()) {
      retVal->setPaletteName(index, tempElem.text());
    }

    tempElem = tempNode.firstChildElement("min_length");
    if(!tempElem.isNull()) {
      tmpI = tempElem.text().toInt();
      if(tmpI < 30000) {
	retVal->setMinLength(index, tmpI);
      }
    }

    tempElem = tempNode.firstChildElement("max_length");
    if(!tempElem.isNull()) {
      tmpI = tempElem.text().toInt();
      if(tmpI < 30000) {
	retVal->setMaxLength(index, tmpI);
      }
    }

    tempElem = tempNode.firstChildElement("thickness");
    if(!tempElem.isNull()) {
      tmpI = tempElem.text().toInt();
      if(0 < tmpI && tmpI < 30000) {
	retVal->setThickness(index, tmpI);
      }
    }

    tempElem = tempNode.firstChildElement("min_initial_v");
    if(!tempElem.isNull()) {
      tmpF = tempElem.text().toFloat();
      if(0.0 < tmpF && tmpF < 100.0) {
	retVal->setMinInitialV(index, tmpF);
      }
    }

    tempElem = tempNode.firstChildElement("max_initial_v");
    if(!tempElem.isNull()) {
      tmpF = tempElem.text().toFloat();
      if(0.0 < tmpF && tmpF < 100.0) {
	retVal->setMaxInitialV(index, tmpF);
      }
    }

    tempElem = tempNode.firstChildElement("palette_speed");
    if(!tempElem.isNull()) {
      tmpF = tempElem.text().toFloat();
      if(0.0 < tmpF && tmpF < 10000.0) {
	retVal->setPaletteSpeed(index, tmpF);
      }
    }

    tempElem = tempNode.firstChildElement("head_constant_color");
    if(!tempElem.isNull()) {
      tmpStr = tempElem.text();
      retVal->setHeadConstantColor(index, stringToBool(tmpStr));
    }

    tempElem = tempNode.firstChildElement("head_random_color");
    if(!tempElem.isNull()) {
      tmpStr = tempElem.text();
      retVal->setHeadRandomColor(index, stringToBool(tmpStr));
    }

    index++;
    tempNode = tempNode.previousSiblingElement("rain_style_entry");
  }

  return retVal;
}

/**
 * Returns a QDomNode object that represents this profile.
 */
QDomNode AcidRainBackgroundProfile::save(QDomDocument* doc) {
  QString tempStr;
  QDomText tempNode;
  QDomElement tempElem;
  QDomElement retVal = doc->createElement(getXMLTagName());

  // type
  tempNode = doc->createTextNode(_xmlTypeValue);
  tempElem = doc->createElement("type");
  tempElem.appendChild(tempNode);
  retVal.appendChild(tempElem);

  // name
  tempNode = doc->createTextNode(_name);
  tempElem = doc->createElement("name");
  tempElem.appendChild(tempNode);
  retVal.appendChild(tempElem);

  // maximum rain density
  tempStr.setNum(_maxRainDensity);
  tempNode = doc->createTextNode(tempStr);
  tempElem = doc->createElement("max_rain_density");
  tempElem.appendChild(tempNode);
  retVal.appendChild(tempElem);

  // gravity
  tempStr.setNum(_gravity);
  tempNode = doc->createTextNode(tempStr);
  tempElem = doc->createElement("gravity");
  tempElem.appendChild(tempNode);
  retVal.appendChild(tempElem);

  // recoil elasticity
  tempStr.setNum(_recoilElasticity);
  tempNode = doc->createTextNode(tempStr);
  tempElem = doc->createElement("recoil_elasticity");
  tempElem.appendChild(tempNode);
  retVal.appendChild(tempElem);

  // max horizontal acceleration
  tempStr.setNum(_maxHorizAccel);
  tempNode = doc->createTextNode(tempStr);
  tempElem = doc->createElement("max_horiz_accel");
  tempElem.appendChild(tempNode);
  retVal.appendChild(tempElem);

  // max horizontal acceleration delta
  tempStr.setNum(_maxHorizAccelDelta);
  tempNode = doc->createTextNode(tempStr);
  tempElem = doc->createElement("max_horiz_accel_delta");
  tempElem.appendChild(tempNode);
  retVal.appendChild(tempElem);

  // store the style information
  if(_styles == NULL) {
    // should never get to this point with these set to
    // non-zero, but meh, doesn't hurt to be safe.
    _numStyles = 0;
    _maxNumStyles = 0;
  }

  // number of styles
  tempStr.setNum(_numStyles);
  tempNode = doc->createTextNode(tempStr);
  tempElem = doc->createElement("num_styles");
  tempElem.appendChild(tempNode);
  retVal.appendChild(tempElem);

  QDomElement styleElem = doc->createElement("rain_style");
  QDomElement entryElem;
  int i;
  for(i=0;i<_numStyles; i++) {
    entryElem = doc->createElement("rain_style_entry");

    // palette name
    tempNode = doc->createTextNode(_styles[i].pal);
    tempElem = doc->createElement("palette");
    tempElem.appendChild(tempNode);
    entryElem.appendChild(tempElem);

    // minimum length
    tempStr.setNum(_styles[i].minLength);
    tempNode = doc->createTextNode(tempStr);
    tempElem = doc->createElement("min_length");
    tempElem.appendChild(tempNode);
    entryElem.appendChild(tempElem);
    
    // maximum length
    tempStr.setNum(_styles[i].maxLength);
    tempNode = doc->createTextNode(tempStr);
    tempElem = doc->createElement("max_length");
    tempElem.appendChild(tempNode);
    entryElem.appendChild(tempElem);

    // thickness
    tempStr.setNum(_styles[i].thickness);
    tempNode = doc->createTextNode(tempStr);
    tempElem = doc->createElement("thickness");
    tempElem.appendChild(tempNode);
    entryElem.appendChild(tempElem);

    // min sprite speed
    tempStr.setNum(_styles[i].minInitialV);
    tempNode = doc->createTextNode(tempStr);
    tempElem = doc->createElement("min_initial_v");
    tempElem.appendChild(tempNode);
    entryElem.appendChild(tempElem);

    // max sprite speed
    tempStr.setNum(_styles[i].maxInitialV);
    tempNode = doc->createTextNode(tempStr);
    tempElem = doc->createElement("max_initial_v");
    tempElem.appendChild(tempNode);
    entryElem.appendChild(tempElem);

    // palette (secondary) speed
    tempStr.setNum(_styles[i].palSpeed);
    tempNode = doc->createTextNode(tempStr);
    tempElem = doc->createElement("palette_speed");
    tempElem.appendChild(tempNode);
    entryElem.appendChild(tempElem);

    // head constant color?
    tempNode = doc->createTextNode(boolToString(_styles[i].bHeadConstantColor));
    tempElem = doc->createElement("head_constant_color");
    tempElem.appendChild(tempNode);
    entryElem.appendChild(tempElem);

    // head random color?
    tempNode = doc->createTextNode(boolToString(_styles[i].bHeadRandomColor));
    tempElem = doc->createElement("head_random_color");
    tempElem.appendChild(tempNode);
    entryElem.appendChild(tempElem);

    // add to the style element
    styleElem.appendChild(entryElem);
  }

  retVal.appendChild(styleElem);
  
  return retVal;
}

/**
 * Creates and returns a new background object.
 */
Background* AcidRainBackgroundProfile::getNewBackgroundObj(int height, int width, QHash<QString, IndexedPaletteProfile*>* palHash) {

  if(palHash == NULL)
    return NULL;

  // create a new AcidRainManager to return
  AcidRainManager* retVal = new AcidRainManager(width, height, _maxRainDensity,
						_gravity, _maxHorizAccel,
						_maxHorizAccelDelta,
						_recoilElasticity);
  if(retVal == NULL)
    return NULL;

  // populate styles list
  for(int i=0; i<_numStyles; i++) {
    retVal->addRainStyle(palHash->value(_styles[i].pal)->createPalette(),
			 _styles[i].minLength, 
			 _styles[i].maxLength, 
			 _styles[i].thickness,
			 _styles[i].minInitialV,
			 _styles[i].maxInitialV, _styles[i].palSpeed,
			 _styles[i].bHeadConstantColor,
			 _styles[i].bHeadRandomColor);
  }
  return retVal;
}

/**
 * get/set max rain density as an integer number between 0-100.
 */
int AcidRainBackgroundProfile::getMaxRainDensity() {
  return _maxRainDensity;
}
void AcidRainBackgroundProfile::setMaxRainDensity(int density) {
  _maxRainDensity = density;
}

/**
 * Get/set the acceleration "downward".  Must be a positive number!
 */
float AcidRainBackgroundProfile::getGravity() {
  return _gravity;
}
void AcidRainBackgroundProfile::setGravity(float gravity) {
  _gravity = gravity;
}

/**
 * Get/set the amount of rain "bounce".  Must be a positive number!
 */
float AcidRainBackgroundProfile::getRecoilElasticity() {
  return _recoilElasticity;
}
void AcidRainBackgroundProfile::setRecoilElasticity(float recoil) {
  _recoilElasticity = recoil;
}

/**
 * Get/set the max horizontal acceleration and delta acceleration.
 */
float AcidRainBackgroundProfile::getMaxHorizontalAcceleration() {
  return _maxHorizAccel;
}
void AcidRainBackgroundProfile::setMaxHorizontalAcceleration(float accel) {
  _maxHorizAccel = accel;
}
float AcidRainBackgroundProfile::getMaxHorizontalAccelerationDelta() {
  return _maxHorizAccelDelta;
}
void AcidRainBackgroundProfile::setMaxHorizontalAccelerationDelta(float delta) {
  _maxHorizAccelDelta = delta;
}

/**
 * Get/set functions related for style subprofiles.
 */
int AcidRainBackgroundProfile::getNumStyles() {
  return _numStyles;
}
void AcidRainBackgroundProfile::clearStyleList() {
  if(_styles != NULL) {
    delete [] _styles;
    _styles = NULL;
    _maxNumStyles = 0;
    _numStyles = 0;
  }
  growStyleList();
}
void AcidRainBackgroundProfile::addStyle() {
  if(_numStyles == _maxNumStyles || _styles == NULL)
    growStyleList();
  _numStyles = _numStyles+1;
}
void AcidRainBackgroundProfile::deleteStyle(int styleIdx) {
  if(_styles == NULL)
    return;
  for(int i=styleIdx;i<_numStyles-1;i++) {
    _styles[i] = _styles[i+1];
  }
  _numStyles--;
}

QString AcidRainBackgroundProfile::getPaletteName(int styleIdx) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return "";

  return _styles[styleIdx].pal;
}
void AcidRainBackgroundProfile::setPaletteName(int styleIdx, QString palName) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return;

  _styles[styleIdx].pal = palName;
}

int AcidRainBackgroundProfile::getMinLength(int styleIdx) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return -1;
  return _styles[styleIdx].minLength;
}
void AcidRainBackgroundProfile::setMinLength(int styleIdx, int minLength) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return;
  _styles[styleIdx].minLength = minLength;
}
int AcidRainBackgroundProfile::getMaxLength(int styleIdx) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return -1;

  return _styles[styleIdx].maxLength;
}
void AcidRainBackgroundProfile::setMaxLength(int styleIdx, int maxLength) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return;

  _styles[styleIdx].maxLength = maxLength;
}

int AcidRainBackgroundProfile::getThickness(int styleIdx) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return 0;

  return _styles[styleIdx].thickness;
}
void AcidRainBackgroundProfile::setThickness(int styleIdx, int thickness) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return;
  
  _styles[styleIdx].thickness = thickness;
}

float AcidRainBackgroundProfile::getMinInitialV(int styleIdx) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return 0.0;

  return _styles[styleIdx].minInitialV;
}
void AcidRainBackgroundProfile::setMinInitialV(int styleIdx, float velocity) 
{
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return;

  _styles[styleIdx].minInitialV = velocity;
}
float AcidRainBackgroundProfile::getMaxInitialV(int styleIdx) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return 0.0;

  return _styles[styleIdx].maxInitialV;
}
void AcidRainBackgroundProfile::setMaxInitialV(int styleIdx, float velocity) 
{
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return;

  _styles[styleIdx].maxInitialV = velocity;
}

float AcidRainBackgroundProfile::getPaletteSpeed(int styleIdx) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return 0.0;

  return _styles[styleIdx].palSpeed;
}
void AcidRainBackgroundProfile::setPaletteSpeed(int styleIdx, float palSpeed) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return;

  _styles[styleIdx].palSpeed = palSpeed;
}

bool AcidRainBackgroundProfile::isHeadConstantColor(int styleIdx)  {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return true;

  return _styles[styleIdx].bHeadConstantColor;
}

void AcidRainBackgroundProfile::setHeadConstantColor(int styleIdx, 
						     bool bConstant) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return;

  _styles[styleIdx].bHeadConstantColor = bConstant;
}

bool AcidRainBackgroundProfile::isHeadRandomColor(int styleIdx) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return false;

  return _styles[styleIdx].bHeadRandomColor;
}
void AcidRainBackgroundProfile::setHeadRandomColor(int styleIdx, bool bRandom) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return;

  _styles[styleIdx].bHeadRandomColor = bRandom;
}

rain_profile_style AcidRainBackgroundProfile::getStyle(int styleIdx) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles) {
    rain_profile_style empty;
    empty.pal = "";
    empty.minLength = 0;
    empty.maxLength = 0;
    empty.thickness = 0;
    empty.minInitialV = 0.0;
    empty.maxInitialV = 0.0;
    empty.palSpeed = 0;
    empty.bHeadConstantColor = false;
    empty.bHeadRandomColor = false;
    return empty;
  }

  return _styles[styleIdx];
}

void AcidRainBackgroundProfile::setStyle(int styleIdx, 
					 rain_profile_style& style) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return;

  // to sanity check data, we'll use all other set routines instead
  // of just copying.
  setPaletteName(styleIdx, style.pal);
  setMinLength(styleIdx, style.minLength);
  setMaxLength(styleIdx, style.maxLength);
  setThickness(styleIdx, style.thickness);
  setMinInitialV(styleIdx, style.minInitialV);
  setMaxInitialV(styleIdx, style.maxInitialV);
  setPaletteSpeed(styleIdx, style.palSpeed);
  setHeadConstantColor(styleIdx, style.bHeadConstantColor);
  setHeadRandomColor(styleIdx, style.bHeadRandomColor);
}


/**
 * Overloaded assignment operator.
 */
AcidRainBackgroundProfile& AcidRainBackgroundProfile::operator=(AcidRainBackgroundProfile& other) {
  _name = other._name;

  _maxRainDensity = other._maxRainDensity;
  _gravity = other._gravity;
  _maxHorizAccel = other._maxHorizAccel;
  _maxHorizAccelDelta = other._maxHorizAccelDelta;
  _recoilElasticity = other._recoilElasticity;

  int i;

  if(_styles != NULL) {
    delete [] _styles;
    _styles = NULL;
  }

  if(other._maxNumStyles > 0) {
    growStyleList(other._maxNumStyles);
  }

  if(other._styles != NULL) {
    for(i=0;i<other._numStyles;i++) {
      _styles[i] = other._styles[i];
    }
  }

  _numStyles = other._numStyles;

  return *this;
}

/**
 * Creates a new object with this object's settings.
 */
BackgroundProfile* AcidRainBackgroundProfile::clone() {
  AcidRainBackgroundProfile* retVal = new AcidRainBackgroundProfile();

  *retVal = *this;

  return (BackgroundProfile*)retVal;
}

/**
 * Grows the list of crawly styles.
 */
void AcidRainBackgroundProfile::growStyleList(int size) {
  if(_styles == NULL) {
    _maxNumStyles = size;
    _styles = new rain_profile_style[_maxNumStyles];
    if(_styles == NULL) {
      _maxNumStyles = 0;
      return;
    }
    _numStyles = 0;
  } else {
    // create a temp list and copy
    rain_profile_style* tmpList=new rain_profile_style[_maxNumStyles+size];
    if(tmpList == NULL) {
      return;
    }
    for(int i=0;i<_numStyles;i++) {
      tmpList[i] = _styles[i];
    }
    delete [] _styles;
    _styles = tmpList;
    tmpList = NULL;
    _maxNumStyles += size;
  }
}

