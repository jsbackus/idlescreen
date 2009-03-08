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

/**
 * This class generates a new plasma based on the dimensions and palette
 * information provided.  It can produce QImage objects of the plasma as
 * well as save the plasma to a file.
 */

#ifndef __PLASMAGENERATOR_H__
#define __PLASMAGENERATOR_H__

#include <QObject>
#include <QImage>
#include <QString>

#include "IndexedPalette/IndexedPalette.h"
#include "plasma/PlasmaAlgorithm.h"

#define PLASMAGENERATOR_NUM_ITERS_PER_CALC 2000

class PlasmaGenerator : public QObject {

  Q_OBJECT

 public:
  /**
   * Public constructor.
   */
  PlasmaGenerator();

  /**
   * Public destructor.
   */
  ~PlasmaGenerator();
  
  /**
   * Retrieves the plasma's height.
   */
  int getHeight();

  /**
   * Sets the plasma's height.
   */
  void setHeight(int height);

  /**
   * Retrieves the plasma's width.
   */
  int getWidth();

  /**
   * Sets the plasma's width.
   */
  void setWidth(int width);

  /**
   * Gets the palette.
   */
  IndexedPalette& getPalette();

  /**
   * Sets the palette.
   */
  void setPalette(IndexedPalette& pal);

  /**
   * Gets whether to clamp the color (as opposed to wrapping).  See 
   * PlasmaAlgorithm.h for more information.
   */
  bool getClampColor();
  
  /**
   * Sets whether to clamp the color or not.  See PlasmaAlgorithm.h for
   * more informatin.
   */
  void setClampColor(bool bClampColor);

  /**
   * Gets whether to set the plasma up for horizontal tiling or not.
   */
  bool getTileHorizontal();

  /**
   * Sets whether to set the plasma up for horizontal tiling or not.
   */
  void setTileHorizontal(bool bTileHorizontal);

  /**
   * Gets whether to set the plasma up for vertical tiling or not.
   */
  bool getTileVertical();

  /**
   * Sets whether to set the plasma up for vertical tiling or not.
   */
  void setTileVertical(bool bTileVertical);

  /**
   * Gets the coarseness factor to use.
   */
  float getCoarseness();
 
  /**
   * Sets the coarseness factor to use.
   */
  void setCoarseness(float coarseness);

  /**
   * Generates a new plasma.  Returns true if successful.
   */
  bool genPlasma();

  /**
   * Retrieves the plasma as a QImage object.
   */
  QImage& getPlasma();

  /**
   * Saves the plasma to the specified path.  Returns true on success.
   */
  bool savePlasma(QString filename, bool bOverwrite=false);

 signals:
  /**
   * This signal is emitted every so often with an indicator as
   * to the percentage of completed calculations.
   */
  void progressUpdated(int percentComplete);
  
 private:
  int _height;
  int _width;
  IndexedPalette _pal;
  bool _bClampColor;
  bool _bTileHorizontal;
  bool _bTileVertical;
  float _coarseness;

  QImage _img;

};

#endif
