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
 * This is the configuration profile for the PlasmaGenerator program.
 *
 * @author jeff backus
 * @date 03/07/2009
 */

#ifndef __PLASMAGENERATORPROFILE_H__
#define __PLASMAGENERATORPROFILE_H__

#include <QString>
#include <QHash>
#include <QtXml/QDomNode>
#include <QtXml/QDomDocument>

#include "PlasmaGenerator.h"
#include "IndexedPalette/IndexedPaletteProfile.h"

enum PlasmaGeneratorOverwritePolicy{ALWAYS, NEVER, ASK};

class PlasmaGeneratorProfile {

 public:
  PlasmaGeneratorProfile();
  ~PlasmaGeneratorProfile();

  /* Attempts to load this profile object from the
   * specified QDomNode.
   */
  static PlasmaGeneratorProfile* load(QDomNode &node);

  /*
   * Returns a QDomNode object that represents this profile.
   */
  QDomNode save(QDomDocument* doc);

  /*
   * Creates and returns a new PlasmaGenerator object.  Does not
   * generate the plasma.
   */
  PlasmaGenerator* getGenerator(QHash<QString, IndexedPaletteProfile*>* palHash);

  /*
   * get/set palette name.
   */
  QString getPaletteName();
  void setPaletteName(QString paletteName);

  /*
   * get/set coarseness.
   */
  float getCoarseness();
  void setCoarseness(float coarseness);

  /*
   * Get/set clamp color index.
   */
  bool getClampColorIndex();
  void setClampColorIndex(bool bClampColorIndex);

  /*
   * Overloaded assignment operator.
   */
  PlasmaGeneratorProfile& operator=(PlasmaGeneratorProfile& other);

  /*
   * Creates a new object with this object's settings.
   */
  PlasmaGeneratorProfile* clone();

  /**
   * Called whenever palette names change
   */
  void paletteNameChanged(QString oldName, QString newName);
	
  /**
   * Called whenever a palette is removed.
   */
  void paletteRemoved(QString palName);

  /**
   * Gets the current filename target.
   */
  QString getFileTarget();

  /**
   * Sets the current filename target.
   */
  void setFileTarget(QString filename);

  /**
   * Gets file overwrite policy.
   */
  PlasmaGeneratorOverwritePolicy getOverwritePolicy();

  /**
   * Sets the file overwrite policy.
   */
  void setOverwritePolicy(PlasmaGeneratorOverwritePolicy policy);

  /*
   * Get/set the name of this profile.
   */
  void setName(QString &name);
  QString getName();

  /*
   * Returns the tag name used by Dom elements for all
   * subclasses of BackgroundProfile.
   */
  static QString getXMLTagName();

 private:
  QString _palName;
  QString _name;
  float _coarseness;
  bool _bClampColorIndex;
  bool _bTileHorizontal;
  bool _bTileVertical;
  int _height;
  int _width;
  QString _targetFileName;
  PlasmaGeneratorOverwritePolicy _policy;
};
#endif
