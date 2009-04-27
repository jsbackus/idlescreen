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

/**
 * This is the base class for all spiral background profiles.
 *
 * @author jeff backus
 * @date 03/23/2009
 */

#ifndef __SPIRALBACKGROUNDPROFILE_H__
#define __SPIRALBACKGROUNDPROFILE_H__

#include <QString>
#include <QtXml/QDomNode>
#include <QtXml/QDomDocument>
#include <QLayout>

#include "2d_bgnd_w_lens/BackgroundProfile.h"

class SpiralBackgroundProfile : public BackgroundProfile {

 public:
  SpiralBackgroundProfile();
  virtual ~SpiralBackgroundProfile();

  /**
   * Attempts to load this background profile object from the
   * specified QDomNode.
   */
  virtual BackgroundProfile* load(QDomNode &node);

  /**
   * Returns a QDomNode object that represents this profile.
   */
  virtual QDomNode save(QDomDocument* doc);

  /**
   * Creates and returns a new background object.
   */
  virtual Background* getNewBackgroundObj(int height, int width,
					  QHash<QString, IndexedPaletteProfile*>* palHash
					  );

  /**
   * get/set palette name.
   */
  QString getPaletteName();
  void setPaletteName(QString paletteName);

  /**
   * get/set steps per tick
   */
  int getGenStepsPerTick();
  void setGenStepsPerTick(int genStepsPerTick);

  /**
   * Get/set animate palette.
   */
  bool getAnimatePalette();
  void setAnimatePalette(bool bAnimatePalette);

  /**
   * get/set palette x speed.
   */
  float getPaletteXSpeed();
  void setPaletteXSpeed(float speed);

  /**
   * get/set palette y speed.
   */
  float getPaletteYSpeed();
  void setPaletteYSpeed(float speed);

  /**
   * Overloaded assignment operator.
   */
  virtual SpiralBackgroundProfile& operator=(SpiralBackgroundProfile& other);

  /**
   * Creates a new object with this object's settings.
   */
  virtual BackgroundProfile* clone();

  /**
   * Called whenever palette names change
   */
  virtual void paletteNameChanged(QString oldName, QString newName);
	
  /**
   * Called whenever a palette is removed.
   */
  virtual void paletteRemoved(QString palName);

  /**
   * Retrieves the configuration QLayout.  The receiver is
   * responsible for deleting the QLayout, which in turn will delete
   * all of the components it contains.  Does not contain the base
   * class information!
   *
   * @see SpiralBackgroundProfileWidget
   *
   * @param bCreateNewWidget If false, the last widget created will be returned.
   * If true or the first time this function is called, it will create a new
   * layout.
   */
  //virtual QLayout* getConfigPanel(bool bCreateNewPanel);

  /**
   * Transfers all of the data in the panel to the internal data structures.
   */
  //virtual void saveGUIData();

 protected:
  /**
   * Attempts to load the base profile object from the
   * specified QDomNode.
   */
  void loadBase(QDomNode &node);

  /**
   * Returns a QDomNode object that represents the base profile.
   */
  void saveBase(QDomElement* node, QDomDocument* doc);

  /**
   * Copies the base values of the specified profile to this profile.
   */
  void baseCopy(SpiralBackgroundProfile& other);

  QString _palName;
  int _genStepsPerTick;
  bool _bAnimatePalette;
  float _paletteXSpeed;
  float _paletteYSpeed;

  // begin GUI related
  QLayout* _configLayout;

};
#endif
