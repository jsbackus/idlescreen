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

  CrawliesBackgroundProfile* retVal = new CrawliesBackgroundProfile();
  if(retVal == NULL)
     return NULL;

  tempElem = node.firstChildElement("name");
  if(!tempElem.isNull()) {
    retVal->_name = tempElem.text();
  }

  tempElem = node.firstChildElement("max_num_crawlies");
  if(!tempElem.isNull()) {
    tmpI = tempElem.text().toInt();
    if(tmpI > 0 && tmpI < 30000) {
      retVal->_maxNumCrawlies = tmpI;
    }
  }

  tempElem = node.firstChildElement("spawn_chance");
  if(!tempElem.isNull()) {
    tmpI = tempElem.text().toInt();
    if(tmpI > 0 && tmpI < 100) {
      retVal->_spawnChance = tmpI;
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
  QDomElement tempNode = node.firstChildElement("crawly_style").
    lastChildElement("crawly_style_entry");

  while(!tempNode.isNull()) {	
    int index = 0;
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

    tempElem = tempNode.firstChildElement("min_sprite_speed");
    if(!tempElem.isNull()) {
      tmpF = tempElem.text().toFloat();
      if(0.0 < tmpF && tmpF < 100.0) {
	retVal->setMinSpriteSpeed(index, tmpF);
      }
    }

    tempElem = tempNode.firstChildElement("max_sprite_speed");
    if(!tempElem.isNull()) {
      tmpF = tempElem.text().toFloat();
      if(0.0 < tmpF && tmpF < 100.0) {
	retVal->setMaxSpriteSpeed(index, tmpF);
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
    tempNode = tempNode.previousSiblingElement("crawly_style_entry");
  }

  return retVal;
}

/**
 * Returns a QDomNode object that represents this profile.
 */
QDomNode CrawliesBackgroundProfile::save(QDomDocument* doc) {
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

  // maximum number of crawlies
  tempStr.setNum(_maxNumCrawlies);
  tempNode = doc->createTextNode(tempStr);
  tempElem = doc->createElement("max_num_crawlies");
  tempElem.appendChild(tempNode);
  retVal.appendChild(tempElem);

  // spawn chance
  tempStr.setNum(_spawnChance);
  tempNode = doc->createTextNode(tempStr);
  tempElem = doc->createElement("spawn_chance");
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

  QDomElement styleElem = doc->createElement("crawly_style");
  QDomElement entryElem;
  int i;
  for(i=0;i<_numStyles; i++) {
    entryElem = doc->createElement("crawly_style_entry");

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
    tempStr.setNum(_styles[i].minSpriteSpeed);
    tempNode = doc->createTextNode(tempStr);
    tempElem = doc->createElement("min_sprite_speed");
    tempElem.appendChild(tempNode);
    entryElem.appendChild(tempElem);

    // max sprite speed
    tempStr.setNum(_styles[i].maxSpriteSpeed);
    tempNode = doc->createTextNode(tempStr);
    tempElem = doc->createElement("max_sprite_speed");
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
			     _styles[i].maxLength, 
			     _styles[i].thickness,
			     _styles[i].minSpriteSpeed,
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

QString CrawliesBackgroundProfile::getPaletteName(int styleIdx) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return "";

  return _styles[styleIdx].pal;
}
void CrawliesBackgroundProfile::setPaletteName(int styleIdx, QString palName) {
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

int CrawliesBackgroundProfile::getThickness(int styleIdx) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return 0;

  return _styles[styleIdx].thickness;
}
void CrawliesBackgroundProfile::setThickness(int styleIdx, int thickness) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return;
  
  _styles[styleIdx].thickness = thickness;
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

crawly_profile_style CrawliesBackgroundProfile::getStyle(int styleIdx) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles) {
    crawly_profile_style empty;
    return empty;
  }

  return _styles[styleIdx];
}

void CrawliesBackgroundProfile::setStyle(int styleIdx, 
					 crawly_profile_style& style) {
  if(_styles == NULL || styleIdx < 0 || styleIdx >= _numStyles)
    return;

  // to sanity check data, we'll use all other set routines instead
  // of just copying.
  setPaletteName(styleIdx, style.pal);
  setMinLength(styleIdx, style.minLength);
  setMaxLength(styleIdx, style.maxLength);
  setThickness(styleIdx, style.thickness);
  setMinSpriteSpeed(styleIdx, style.minSpriteSpeed);
  setMaxSpriteSpeed(styleIdx, style.maxSpriteSpeed);
  setPaletteSpeed(styleIdx, style.palSpeed);
  setHeadConstantColor(styleIdx, style.bHeadConstantColor);
  setHeadRandomColor(styleIdx, style.bHeadRandomColor);
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

