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
#include <iostream>
using namespace std;
// end debug

#include <QtXml/QDomElement>
#include <QtXml/QDomText>

#include "utility/misc_funcs.h"
#include "spirals/RectangularSpiralAlgorithm.h"
#include "RectangularSpiralBackgroundProfile.h"
#include "SpiralBackground.h"

// begin tmp
// modified 02/06/10 to get back on track..
//#include "spirals/NgonSpiralAlgorithm.h"
#include "spirals/RectangularSpiralAlgorithm.h"
// end mod
#include "spirals/PolarSpiralAlgorithm.h"
// end tmp

RectangularSpiralBackgroundProfile::RectangularSpiralBackgroundProfile() {
  _xmlTypeValue = "RectangularSpiralBackground";

  //defaults
  _colorWidth = 2;
  _emptyWidth = 1;
  _bConstantColor = false;
  _bRandomColor = false;
  _bIncrementColor = false;
}

RectangularSpiralBackgroundProfile::~RectangularSpiralBackgroundProfile() {
  // no es nada
}

/**
 * Attempts to load this background profile object from the
 * specified QDomNode.
 */
BackgroundProfile* RectangularSpiralBackgroundProfile::load(QDomNode &node) {
  int tmpI;
  QDomElement tempElem;

  //sanity check on the input
  if(node.nodeName() != getXMLTagName())
    return NULL;

  //make sure that the type value matches
  tempElem = node.firstChildElement("type");
  if(tempElem.isNull() || tempElem.text() != _xmlTypeValue)
    return NULL;

  RectangularSpiralBackgroundProfile* retVal = new RectangularSpiralBackgroundProfile();
  if(retVal == NULL)
    return NULL;

  // load base
  retVal->loadBase(node);

  // load extended data
  tempElem = node.firstChildElement("color_width");
  if(!tempElem.isNull()) {
    tmpI = tempElem.text().toInt();
    retVal->setColorWidth(tmpI);
  }

  tempElem = node.firstChildElement("empty_width");
  if(!tempElem.isNull()) {
    tmpI = tempElem.text().toInt();
    retVal->setEmptyWidth(tmpI);
  }

  tempElem = node.firstChildElement("constant_color");
  if(!tempElem.isNull()) {
    QString param = tempElem.text();
    retVal->setConstantColor(stringToBool(param));
  }

  tempElem = node.firstChildElement("random_color");
  if(!tempElem.isNull()) {
    QString param = tempElem.text();
    retVal->setRandomColor(stringToBool(param));
  }

  tempElem = node.firstChildElement("increment_color");
  if(!tempElem.isNull()) {
    QString param = tempElem.text();
    retVal->setColorIncrement(stringToBool(param));
  }

  // return new profile
  return retVal;
}

/**
 * Returns a QDomNode object that represents this profile.
 */
QDomNode RectangularSpiralBackgroundProfile::save(QDomDocument* doc) {
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

  // save extended data
  //color width
  tempStr.setNum(_colorWidth);
  tempNode = doc->createTextNode(tempStr);
  tempElem = doc->createElement("color_width");
  tempElem.appendChild(tempNode);
  retVal.appendChild(tempElem);

  //empty width
  tempStr.setNum(_emptyWidth);
  tempNode = doc->createTextNode(tempStr);
  tempElem = doc->createElement("empty_width");
  tempElem.appendChild(tempNode);
  retVal.appendChild(tempElem);

  //constant color?
  tempNode = doc->createTextNode(boolToString(_bConstantColor));
  tempElem = doc->createElement("constant_color");
  tempElem.appendChild(tempNode);
  retVal.appendChild(tempElem);

  //random color?
  tempNode = doc->createTextNode(boolToString(_bRandomColor));
  tempElem = doc->createElement("random_color");
  tempElem.appendChild(tempNode);
  retVal.appendChild(tempElem);

  //increment color?
  tempNode = doc->createTextNode(boolToString(_bIncrementColor));
  tempElem = doc->createElement("increment_color");
  tempElem.appendChild(tempNode);
  retVal.appendChild(tempElem);

  // return node.
  return retVal;
}

  /**
   * Gets the width of the line to be filled with color
   */
int RectangularSpiralBackgroundProfile::getColorWidth() {
  return _colorWidth;
}

  /**
   * Sets the width of the line to be filled with color.
   */
void RectangularSpiralBackgroundProfile::setColorWidth(int colorWidth) {
  cout<<"Attempting to set color width to "<<colorWidth<<"...";
  if(0 < colorWidth && colorWidth < 30000) {
    _colorWidth = colorWidth;
    cout<<"Successful.";
  }
  cout<<endl;
}

  /**
   * Gets the width of the line to be filled with "empty space" (palette's
   * default color).
   */
int RectangularSpiralBackgroundProfile::getEmptyWidth() {
  return _emptyWidth;
}

  /**
   * Gets the width of the line to be filled with "empty space" (palette's
   * default color).
   */
void RectangularSpiralBackgroundProfile::setEmptyWidth(int emptyWidth) {
  if(0 <= emptyWidth && emptyWidth < 30000) {
    _emptyWidth = emptyWidth;
  }
}

  /**
   * Gets whether or not the color should remain constant.
   */
bool RectangularSpiralBackgroundProfile::isConstantColor() {
  return _bConstantColor;
}

  /**
   * Sets whether or not the color should remain constant.
   */
void RectangularSpiralBackgroundProfile::setConstantColor(bool bConstantColor) {
  _bConstantColor = bConstantColor;
}

  /**
   * Gets whether or not the initial color is random or index 0.
   */
bool RectangularSpiralBackgroundProfile::isRandomColor() {
  return _bRandomColor;
}
  
  /**
   * Sets whether or not the initial color is random or index 0.
   */
void RectangularSpiralBackgroundProfile::setRandomColor(bool bRandomColor) {
  _bRandomColor = bRandomColor;
}

/**
 * Gets whether or not the color index increases as it spirals out.
 */
bool RectangularSpiralBackgroundProfile::doesColorIncrement() {
  return _bIncrementColor;
}
  
  /**
   * Sets whether or not the color index increases as it spirals out.
   */
void RectangularSpiralBackgroundProfile::setColorIncrement(bool bIncrementColor) {
  _bIncrementColor = bIncrementColor;
}

/**
 * Creates and returns a new background object.
 */
Background* RectangularSpiralBackgroundProfile::getNewBackgroundObj(int height,
								    int width,
							 QHash<QString, IndexedPaletteProfile*>* palHash) {

  /*
  PolarSpiralAlgorithm* alg = 
    new PolarSpiralAlgorithm(_colorWidth,_emptyWidth,_bConstantColor,
			    _bRandomColor, _bIncrementColor);
  */
  /*
  NgonSpiralAlgorithm* alg = 
    new NgonSpiralAlgorithm(0,_colorWidth,_emptyWidth,_bConstantColor,
			    _bRandomColor, _bIncrementColor);
  */

  RectangularSpiralAlgorithm* alg = 
    new RectangularSpiralAlgorithm(_colorWidth,_emptyWidth,_bConstantColor,
				   _bRandomColor, _bIncrementColor);

  if(alg == NULL)
    return NULL;

  return new SpiralBackground(width, height, alg,
			      palHash->value(_palName)->createPalette(),
			      _bAnimatePalette, _paletteXSpeed, _paletteYSpeed,
			      _genStepsPerTick);
}

/**
 * Overloaded assignment operator
 */
RectangularSpiralBackgroundProfile& RectangularSpiralBackgroundProfile::operator=(RectangularSpiralBackgroundProfile& other) {
  
  baseCopy(other);

  _colorWidth = other._colorWidth;
  _emptyWidth = other._emptyWidth;
  _bConstantColor = other._bConstantColor;
  _bRandomColor = other._bRandomColor;
  _bIncrementColor = other._bIncrementColor;

  return *this;
}

/**
 * Creates a new object with this object's settings.
 */
BackgroundProfile* RectangularSpiralBackgroundProfile::clone() {
  RectangularSpiralBackgroundProfile* retVal = 
    new RectangularSpiralBackgroundProfile();

  *retVal = *this;

  return (BackgroundProfile*)retVal;
}



