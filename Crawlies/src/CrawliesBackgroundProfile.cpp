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
//#include<iostream>
//using namespace std;
//end debug

#include <QtXml/QDomElement>
#include <QtXml/QDomText>

#include "utility/misc_funcs.h"

//tmp
#include "IndexedPalette/IndexedPaletteProfile.h"
//endtmp

#include "CrawliesBackgroundProfile.h"

CrawliesBackgroundProfile::CrawliesBackgroundProfile() {
  _xmlTypeValue = "CrawliesBackground";

  //defaults
  _name = "Empty";

  _styles = NULL;
  _numStyles = 0;
  _maxNumStyles = 0;
  _maxNumCrawlies = 0;
  _spawnChance = 0;

  // make an initially empty list of default size
  growStyleList();
}

CrawliesBackgroundProfile::~CrawliesBackgroundProfile() {
  if(_styles != NULL) {
    delete [] _styles;
    _styles = NULL;
  }
}

/** 
 * Attempts to load this background profile object from the
 * specified QDomNode.
 */
BackgroundProfile* CrawliesBackgroundProfile::load(QDomNode &node) {
  // right now, just use defaults

  CrawliesBackgroundProfile* retVal = new CrawliesBackgroundProfile();
  if(retVal == NULL)
     return NULL;

  retVal->_name = "Classic RGB Profile";

  retVal->_spawnChance = 4;
  retVal->_maxNumCrawlies = 20;

  retVal->growStyleList();

  retVal->_styles[0].minLength = 31;
  retVal->_styles[0].maxLength = 32;
  retVal->_styles[0].minSpriteSpeed = 1.0;
  retVal->_styles[0].maxSpriteSpeed = 3.0;
  retVal->_styles[0].palSpeed = 0;
  retVal->_styles[0].bHeadConstantColor = false;//true;
  retVal->_styles[0].bHeadRandomColor = true;//false;
  retVal->_styles[0].pal = "Classic RGB Palette";

  retVal->_numStyles = 1;

  return retVal;
}

/**
 * Returns a QDomNode object that represents this profile.
 */
QDomNode CrawliesBackgroundProfile::save(QDomDocument* doc) {
  QDomElement retVal = doc->createElement(getXMLTagName());
  
  return retVal;
}

/**
 * Creates and returns a new background object.
 */
Background* CrawliesBackgroundProfile::getNewBackgroundObj(int height, int width, QHash<QString, IndexedPaletteProfile*>* palHash) {

  if(palHash == NULL)
    return NULL;

  // create a new CrawliesManager to return
  CrawliesManager* retVal = new CrawliesManager(width, height, _maxNumCrawlies,
						_spawnChance);
  if(retVal == NULL)
    return NULL;

  // populate styles list
  for(int i=0; i<_numStyles; i++) {
    retVal->addCrawliesStyle(palHash->value(_styles[i].pal)->createPalette(),
			     _styles[i].minLength, 
			     _styles[i].maxLength, _styles[i].minSpriteSpeed,
			     _styles[i].maxSpriteSpeed, _styles[i].palSpeed,
			     _styles[i].bHeadConstantColor,
			     _styles[i].bHeadRandomColor);
  }

  return retVal;
}

/**
 * get/set max number of crawlies.
 */
int CrawliesBackgroundProfile::getMaxNumberCrawlies() {
  return _maxNumCrawlies;
}
void CrawliesBackgroundProfile::setMaxNumberCrawlies(int numCrawlies) {
  _maxNumCrawlies = numCrawlies;
}

/**
 * Get/set spawn chance.  This number is the denominator, i.e.
 * percent chance = 1/numCrawlies * 100%.
 */
int CrawliesBackgroundProfile::getSpawnChance() {
  return _spawnChance;
}
void CrawliesBackgroundProfile::setSpawnChance(int spawnChance) {
  _spawnChance = spawnChance;
}

/**
 * Get/set functions related for style subprofiles.
 */
int CrawliesBackgroundProfile::getNumStyles() {
  return _numStyles;
}
void CrawliesBackgroundProfile::clearStyleList() {
  if(_styles != NULL) {
    delete [] _styles;
    _styles = NULL;
    _maxNumStyles = 0;
    _numStyles = 0;
  }
  growStyleList();
}
void CrawliesBackgroundProfile::addStyle() {
  if(_numStyles == _maxNumStyles || _styles == NULL)
    growStyleList();
  _numStyles = _numStyles+1;
}
void CrawliesBackgroundProfile::deleteStyle(int styleIdx) {
  if(_styles == NULL)
    return;
  for(int i=styleIdx;i<_numStyles-1;i++) {
    _styles[i] = _styles[i+1];
  }
  _numStyles--;
}

QString CrawliesBackgroundProfile::getPalette(int styleIdx) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return "";

  return _styles[styleIdx].pal;
}
void CrawliesBackgroundProfile::setPalette(int styleIdx, QString palName) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return;

  _styles[styleIdx].pal = palName;
}

int CrawliesBackgroundProfile::getMinLength(int styleIdx) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return -1;
  return _styles[styleIdx].minLength;
}
void CrawliesBackgroundProfile::setMinLength(int styleIdx, int minLength) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return;
  _styles[styleIdx].minLength = minLength;
}
int CrawliesBackgroundProfile::getMaxLength(int styleIdx) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return -1;

  return _styles[styleIdx].maxLength;
}
void CrawliesBackgroundProfile::setMaxLength(int styleIdx, int maxLength) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return;

  _styles[styleIdx].maxLength = maxLength;
}

float CrawliesBackgroundProfile::getMinSpriteSpeed(int styleIdx) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return 0.0;

  return _styles[styleIdx].minSpriteSpeed;
}
void CrawliesBackgroundProfile::setMinSpriteSpeed(int styleIdx, float minSpeed) 
{
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return;

  _styles[styleIdx].minSpriteSpeed = minSpeed;
}
float CrawliesBackgroundProfile::getMaxSpriteSpeed(int styleIdx) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return 0.0;

  return _styles[styleIdx].maxSpriteSpeed;
}
void CrawliesBackgroundProfile::setMaxSpriteSpeed(int styleIdx, float maxSpeed) 
{
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return;

  _styles[styleIdx].maxSpriteSpeed = maxSpeed;
}

float CrawliesBackgroundProfile::getPaletteSpeed(int styleIdx) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return 0.0;

  return _styles[styleIdx].palSpeed;
}
void CrawliesBackgroundProfile::setPaletteSpeed(int styleIdx, float palSpeed) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return;

  _styles[styleIdx].palSpeed = palSpeed;
}

bool CrawliesBackgroundProfile::isHeadConstantColor(int styleIdx)  {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return true;

  return _styles[styleIdx].bHeadConstantColor;
}

void CrawliesBackgroundProfile::setHeadConstantColor(int styleIdx, 
						     bool bConstant) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return;

  _styles[styleIdx].bHeadConstantColor = bConstant;
}

bool CrawliesBackgroundProfile::isHeadRandomColor(int styleIdx) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return false;

  return _styles[styleIdx].bHeadRandomColor;
}
void CrawliesBackgroundProfile::setHeadRandomColor(int styleIdx, bool bRandom) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return;

  _styles[styleIdx].bHeadRandomColor = bRandom;
}

/**
 * Overloaded assignment operator.
 */
CrawliesBackgroundProfile& CrawliesBackgroundProfile::operator=(CrawliesBackgroundProfile& other) {
  _name = other._name;

  _maxNumCrawlies = other._maxNumCrawlies;
  _spawnChance = other._spawnChance;

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
BackgroundProfile* CrawliesBackgroundProfile::clone() {
  CrawliesBackgroundProfile* retVal = new CrawliesBackgroundProfile();

  *retVal = *this;

  return (BackgroundProfile*)retVal;
}

/**
 * Grows the list of crawly styles.
 */
void CrawliesBackgroundProfile::growStyleList(int size) {
  if(_styles == NULL) {
    _maxNumStyles = size;
    _styles = new crawly_profile_style[_maxNumStyles];
    if(_styles == NULL) {
      _maxNumStyles = 0;
      return;
    }
    _numStyles = 0;
  } else {
    // create a temp list and copy
    crawly_profile_style* tmpList=new crawly_profile_style[_maxNumStyles+size];
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



/** !! Begin Old Code !! **/

/* Attempts to load this background profile object from the
 * specified QDomNode.
 */
/*
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

  tempElem = node.firstChildElement("animate_palette");
  if(!tempElem.isNull()) {
    QString param = tempElem.text();
    retVal->_bAnimatePalette = stringToBool(param);
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
    QString param = tempElem.text();
    retVal->_bClampColorIndex = stringToBool(param);
  }

  tempElem = node.firstChildElement("palette");
  if(!tempElem.isNull()) {
    retVal->_palName = tempElem.text();
  }
	
  return retVal;
}
*/

/*
 * Returns a QDomNode object that represents this profile.
 */
/*
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
  tempElem = doc->createElement("animate_palette");
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
*/

