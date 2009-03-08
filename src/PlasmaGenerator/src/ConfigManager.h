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
 * This class maintains all of the options used to
 * configure this screen saver.  All configurations
 * are actually stored in profiles.
 */

#ifndef __CONFIGMANAGER_H__
#define __CONFIGMANAGER_H__

#ifdef _M_IX86
#include <windows.h>
#else
#include <stdlib.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#include <QString>
#include <QList>
#include <QHash>
#include <QStringList>

#include "2d_bgnd_w_lens/globaldefs.h"
#include "IndexedPalette/IndexedPalette.h"
#include "IndexedPalette/IndexedPaletteProfile.h"

#include "PlasmaGeneratorProfile.h"

class ConfigManager {

 public:
  //Constructor and deconstructor
  ConfigManager();
  ~ConfigManager();

  void load();
  void save();

  bool importFromFile(QString filename, bool bLoadMiscInfo);
  bool exportToFile(QString filename, bool bSaveMiscInfo);

  void deleteAllSettings();

  //Sets defaults
  void setDefaults();

  //*** Begin Timer Related ***

  /**
   * Returns the number of milliseconds between each timer tick.
   */
  long getTimerMillis();

  //*** End Timer Related ***

  //*** Begin Screen Related ***
  int getTextureWidth();
  int getTextureHeight();
  //*** End Screen Related

  //*** Begin PlasmaGeneratorProfile Related ***

  /**
   * Gets the default profile name.
   */
  QString getDefaultProfile();

  /**
   * Sets the default profile name.
   */
  void setDefaultProfile(QString profileName);

  /**
   * Gets the list of all PlasmaGeneratorProfiles.
   */
  QStringList getAvailableProfiles();

  /**
   * Removes the selected profile.  Returns whether successful.
   */
  bool removeProfile(QString name);

  /**
   * Adds a new profile to the hash.
   */
  void addProfile(PlasmaGeneratorProfile& profile);

  /**
   * Retrieves a copy of the specified profile.
   */
  PlasmaGeneratorProfile& getProfile(QString name);

  /**
   * Replaces the profile with the specified name with the
   * specified profile.
   */
  bool replaceProfile(QString oldProfileName, 
		      PlasmaGeneratorProfile& newProfile);

  /**
   * Checks to see if a profile with the specified name is already
   * in the hash.
   */
  bool doesProfileExist(QString name);

  //*** End PlasmaGeneratorProfile Related ***

  //*** Begin Palette Related ***

  /**
   * Returns a list of the names of the palettes currently 
   * in the palette hash.
   */
  QStringList getPaletteNames();

  /**
   * Removes the palette with the specified name from the hash.
   * If bUpdateProfiles is true, it will iterate through all
   * background profiles alerting them to the removal.  When
   * replacing a palette, this should be false!
   */
  bool removePalette(QString name, bool bUpdateProfiles=true);

  /**
   * Adds the specified palette to the hash if it isn't already
   * in the hash.  Returns whether successful.
   */
  void addPalette(IndexedPaletteProfile& pal);

  /**
   * Checks to see if a palette with the specified name is already
   * in the hash.
   */
  bool doesPaletteExist(QString name);

  /**
   * Replaces the palette with the specified name with the
   * specified palette if the new name isn't already
   * in the list.
   */
  bool replacePalette(QString oldPaletteName, IndexedPaletteProfile& newPalette);

  /**
   * Returns a copy of the specified palette profile.  Caller should
   * check for existence first with doesPaletteExist();
   */
  IndexedPaletteProfile getPaletteProfile(const QString& name);
  //*** End Palette Related ***

  //*** Begin Misc ***

  //whether or not the GPL has been accepted
  bool getGPLAccepted();
  void setGPLAccepted(bool bAccepted);

  //creates a new ConfigManager object that only contains the specified configuration.
  //it is intended for to be used in generating the preview widget.
  //  ConfigManager* createPreview(int width = 64, int height = 64, LensProfile* lensProfile = NULL, BackgroundProfile* backgroundProfile = NULL, int timerMillis = 50);

  /**
   * Gets the human-readable name of the program.
   */
  QString getProgName();

  /**
   * This is an uber function for command-line mode that will
   * generate a plasma based on the arguments provided.  If any
   * of them are empty, defaults will be used.
   */
  bool GeneratePlasmaFile(QString profileName, QString outFileName, 
			  bool bOverwriteOverride, bool bOverwrite);

  //*** End Misc ***

 protected:
  QString getMainConfigFile();
  QString getMainConfigPath();
  QString getExternalDefaultConfigFile();
  QString getExternalDefaultConfigPath();

  void loadMiscInfo(QDomNode &node);
  void saveMiscInfo(QDomDocument* doc, QDomElement* root);

  // returns the string used in as the Dom root.
  QString getDomNodeQString();

  QHash<QString, PlasmaGeneratorProfile*> _profiles;
  QString _defaultProfile;

  QHash<QString, IndexedPaletteProfile*> _paletteHash;

  bool _bGPLAccepted;

  int _defWidth;
  int _defHeight;
};


#endif
