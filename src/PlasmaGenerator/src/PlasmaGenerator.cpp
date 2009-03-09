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
//end debug

#include <QFile>

#include "PlasmaGenerator.h"

/**
 * Public constructor.
 */
PlasmaGenerator::PlasmaGenerator() {
  _height = 0;
  _width = 0;
  _bClampColor = true;
  _bTileHorizontal = false;
  _bTileVertical = false;
  _coarseness = 1.0;
}

/**
 * Public destructor.
 */
PlasmaGenerator::~PlasmaGenerator() {
  // nada
}
  
/**
 * Retrieves the plasma's height.
 */
int PlasmaGenerator::getHeight() {
  return _height;
}

/**
 * Sets the plasma's height.
 */
void PlasmaGenerator::setHeight(int height) {
  _height = height;
}

/**
 * Retrieves the plasma's width.
 */
int PlasmaGenerator::getWidth() {
  return _width;
}

/**
 * Sets the plasma's width.
 */
void PlasmaGenerator::setWidth(int width) {
  _width = width;
}

/**
 * Gets the palette.
 */
IndexedPalette& PlasmaGenerator::getPalette() {
  return _pal;
}

/**
 * Sets the palette.
 */
void PlasmaGenerator::setPalette(IndexedPalette& pal) {
  _pal = pal;
}

/**
 * Gets whether to clamp the color (as opposed to wrapping).  See 
 * PlasmaAlgorithm.h for more information.
 */
bool PlasmaGenerator::getClampColor() {
  return _bClampColor;
}
  
/**
 * Sets whether to clamp the color or not.  See PlasmaAlgorithm.h for
 * more informatin.
 */
void PlasmaGenerator::setClampColor(bool bClampColor) {
  _bClampColor = bClampColor;
}

/**
 * Gets whether to set the plasma up for horizontal tiling or not.
 */
bool PlasmaGenerator::getTileHorizontal() {
  return _bTileHorizontal;
}

/**
 * Sets whether to set the plasma up for horizontal tiling or not.
 */
void PlasmaGenerator::setTileHorizontal(bool bTileHorizontal) {
  _bTileHorizontal = bTileHorizontal;
}

/**
 * Gets whether to set the plasma up for vertical tiling or not.
 */
bool PlasmaGenerator::getTileVertical() {
  return _bTileVertical;
}

/**
 * Sets whether to set the plasma up for vertical tiling or not.
 */
void PlasmaGenerator::setTileVertical(bool bTileVertical) {
  _bTileVertical = bTileVertical;
}

/**
 * Gets the coarseness factor to use.
 */
float PlasmaGenerator::getCoarseness() {
  return _coarseness;
}
 
/**
 * Sets the coarseness factor to use.
 */
void PlasmaGenerator::setCoarseness(float coarseness) {
  _coarseness = coarseness;
}

/**
 * Generates a new plasma.  Returns true if successful.
 */
bool PlasmaGenerator::genPlasma() {

  int numColors = _pal.getWidth();

  if(_height < 1 || _width < 1 || numColors < 1 || _coarseness < 0.0) {
    return false;
  }
  
  // calculate the number of steps
  float totalNumSteps = (_height*_width /
			 (3*PLASMAGENERATOR_NUM_ITERS_PER_CALC)) + _height;
  float currStep = 0;

  // initialize the PlasmaAlgorithm, then step through each calc
  // step, emitting a progress update each time.
  int* field = new int[_height*_width];
  if(field == NULL) {
    return false;
  }

  PlasmaAlgorithm plasma;

  if(!plasma.initialize(field, _width, _height, numColors, _coarseness, 
			_bClampColor, _bTileHorizontal, _bTileVertical)) {
    return false;
  }

  while(!plasma.calc(PLASMAGENERATOR_NUM_ITERS_PER_CALC)) {
    currStep+=1.0;
    emit progressUpdated((int)(currStep*100.0/totalNumSteps));
  }

  // generate a new plasma then stuff into a QImage object.
  _img = QImage(_width, _height, QImage::Format_ARGB32);
  int buff[4];

  for(int y = 0; y<_height; y++) {
    for(int x=0; x<_width; x++) {
      _pal.getColor(field[y*_width+x],0,&buff[0], 4);
      QRgb color = qRgba(buff[0], buff[1], buff[2], buff[3]);
      _img.setPixel(x, y, color);
    }
    currStep+=1.0;
    emit progressUpdated((int)(currStep*100.0/totalNumSteps));
  }

  // emit 100%
  emit progressUpdated(100);

  // clean up
  delete [] field;
  field = NULL;

  return true;
}

/**
 * Retrieves the plasma as a QImage object.
 */
QImage& PlasmaGenerator::getPlasma() {
  return _img;
}

/**
 * Saves the plasma to the specified path.  Returns true on success.
 */
bool PlasmaGenerator::savePlasma(QString filename, bool bOverwrite) {
  QString targFileName = filename;

  if(!bOverwrite) {
    int num = 1;
    QString ext = filename.right(filename.size() - filename.lastIndexOf("."));
    QString prefix = filename.left(filename.lastIndexOf("."));

    // check to see if a file by that name already exists.  If so,
    // then append a number increment until a file with the combined
    // name doesn't exist.
    while(QFile(targFileName).exists()) {
      targFileName = prefix + QString("_")+
	QString::number(num++) + ext;
    }
  }

  // save
  return _img.save(targFileName);
}
