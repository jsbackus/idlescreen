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
 * This is the base class for all spiral background profiles.
 *
 * @author jeff backus
 * @date 03/23/2009
 */

#ifndef __RECTANGULARSPIRALBACKGROUNDPROFILE_H__
#define __RECTANGULARSPIRALBACKGROUNDPROFILE_H__

#include <QString>
#include <QtXml/QDomNode>
#include <QtXml/QDomDocument>

#include "SpiralBackgroundProfile.h"

class RectangularSpiralBackgroundProfile : public SpiralBackgroundProfile {

 public:
  RectangularSpiralBackgroundProfile();
  ~RectangularSpiralBackgroundProfile();

  /**
   * Attempts to load this background profile object from the
   * specified QDomNode.
   */
  BackgroundProfile* load(QDomNode &node);

  /**
   * Returns a QDomNode object that represents this profile.
   */
  QDomNode save(QDomDocument* doc);

  /**
   * Creates and returns a new background object.
   */
  Background* getNewBackgroundObj(int height, int width,
				  QHash<QString, IndexedPaletteProfile*>* palHash
					  );

  /**
   * Gets the width of the line to be filled with color
   */
  int getColorWidth();

  /**
   * Sets the width of the line to be filled with color.
   */
  void setColorWidth(int colorWidth);

  /**
   * Gets the width of the line to be filled with "empty space" (palette's
   * default color).
   */
  int getEmptyWidth();

  /**
   * Gets the width of the line to be filled with "empty space" (palette's
   * default color).
   */
  void setEmptyWidth(int emptyWidth);

  /**
   * Gets whether or not the color should remain constant.
   */
  bool isConstantColor();

  /**
   * Sets whether or not the color should remain constant.
   */
  void setConstantColor(bool bConstantColor);

  /**
   * Gets whether or not the initial color is random or index 0.
   */
  bool isRandomColor();
  
  /**
   * Sets whether or not the initial color is random or index 0.
   */
  void setRandomColor(bool bRandomColor);

  /**
   * Gets whether or not the color index increases as it spirals out.
   */
  bool doesColorIncrement();
  
  /**
   * Sets whether or not the color index increases as it spirals out.
   */
  void setColorIncrement(bool bIncrementColor);

  /**
   * Overloaded assignment operator.
   */
  RectangularSpiralBackgroundProfile& operator=(RectangularSpiralBackgroundProfile& other);

  /**
   * Creates a new object with this object's settings.
   */
  BackgroundProfile* clone();

 private:

  int _colorWidth;
  int _emptyWidth;
  bool _bConstantColor;
  bool _bRandomColor;
  bool _bIncrementColor;
};
#endif
