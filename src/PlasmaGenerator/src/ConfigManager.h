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
//#include <stream.h>
#include <stdlib.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#include <QString>
#include <QList>
#include <QHash>
#include <QStringList>

#include "globaldefs.h"
#include "lens_engine/LensObject.h"
#include "lens_engine/LensProfile.h"
#include "../IndexedPalette/IndexedPalette.h"
#include "../IndexedPalette/IndexedPaletteProfile.h"
#include "BackgroundProfile.h"
#include "MasterProfile.h"

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
	ConfigManager* createPreview(int width = 64, int height = 64, LensProfile* lensProfile = NULL, BackgroundProfile* backgroundProfile = NULL, int timerMillis = 50);

	/**
	 * Gets the human-readable name of the program.
	 */
	QString getProgName();

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

	QHash<QString, IndexedPaletteProfile*> _paletteHash;

	bool _bGPLAccepted;

	int _defWidth;
	int _defHeight;
};


#endif
