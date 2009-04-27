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
 * This is the base class for all background profiles.  The background profiles
 * are responsible for correctly constructing and returning an appropriate
 * background object.
 *
 * @author jeff backus
 * @date 09/01/2007
 */

#ifndef __BackgroundProfile_H__
#define __BackgroundProfile_H__

#include <QString>
#include <QHash>
#include <QtXml/QDomNode>
#include <QtXml/QDomDocument>

#include "Background.h"
#include "../IndexedPalette/IndexedPaletteProfile.h"

class BackgroundProfile {

 public:
  /**
   * Default constructor/destructor
   */
  BackgroundProfile();
  virtual ~BackgroundProfile();

  /* Attempts to load this background profile object from the
   * specified QDomNode.
   */
  virtual BackgroundProfile* load(QDomNode &node);

  /*
   * Returns a QDomNode object that represents this profile.
   */
  virtual QDomNode save(QDomDocument* doc);

  /*
   * Creates and returns a new background object.
   */
  virtual Background* getNewBackgroundObj(int height, int width, QHash<QString, IndexedPaletteProfile*>* palHash);

  /**
   * Called whenever palette names change
   */
  virtual void paletteNameChanged(QString oldName, QString newName);
	
  /**
   * Called whenever a palette is removed.
   */
  virtual void paletteRemoved(QString palName);

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

  /*
   * Returns the type of BackgroundProfile.
   */
  QString getBackgroundType();

  /*
   * Creates a new object with this object's settings.
   */
  virtual BackgroundProfile* clone();

 protected:

  QString _name;
  QString _xmlTypeValue; /* Needs set by subclasses!*/
};

#endif
