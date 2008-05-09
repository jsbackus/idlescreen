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
#include <stream.h>
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
	int getScreenWidth();
	int getScreenHeight();
	int getTextureWidth();
	int getTextureHeight();
	//*** End Screen Related

	//*** Begin Lens Related ***
	/*
	 * Initializes lens related code
	 */
	void initializeLenses(screen_struct* screenObj);

	/*
	 * Returns the maximum number of lenses that the current profile allows.
	 */
	int getMaxNumLenses();

	/*
	 * Returns a new lens object that conforms to the currently selected profile.
	 */
	LensObject* getNewLens();

	/**
	 * Gets the list of all LensProfiles.
	 */
	QStringList getAvailableLensProfiles();

	/**
	 * Removes the selected lens profile.  Returns whether successful.
	 */
	bool removeLensProfile(QString name);

	/**
	 * Adds a new lens profile to the hash.
	 */
	void addLensProfile(LensProfile& lensProfile);

	/**
	 * Retrieves a copy of the specified lens profile.
	 */
	LensProfile getLensProfile(QString name);

	/**
	 * Replaces the profile with the specified name with the
	 * specified lens profile.
	 */
	bool replaceLensProfile(QString oldLensProfileName, LensProfile& newLensProfile);

	/**
	 * Checks to see if a lens profile with the specified name is already
	 * in the hash.
	 */
	bool doesLensProfileExist(QString name);

	//*** End Lens Related ***

	//*** Begin Background Related ***

	/**
	 * Creates and returns a new background object based on the settings of 
	 * the current profile.
	 */
	Background* getNewBackground();

	/**
	 * Gets the list of all BackgroundProfiles.
	 */
	QStringList getAvailableBackgroundProfiles();

	/**
	 * Removes the selected background profile.  Returns whether successful.
	 */
	bool removeBackgroundProfile(QString name);

	/**
	 * Adds a new background profile to the hash.
	 */
	void addBackgroundProfile(BackgroundProfile& backgroundProfile);

	/**
	 * Retrieves a copy of the specified background profile.
	 */
	BackgroundProfile* getBackgroundProfile(QString name);

	/**
	 * Replaces the profile with the specified name with the
	 * specified background profile.
	 */
	bool replaceBackgroundProfile(QString oldBackgroundProfileName, BackgroundProfile& newBackgroundProfile);

	/**
	 * Checks to see if a background profile with the specified name is already
	 * in the hash.
	 */
	bool doesBackgroundProfileExist(QString name);

	//*** End Background Related ***

	//*** Begin MasterProfile Related ***

	/**
	 * Called when in scr mode to choose the active profile.
	 */
	void chooseActiveProfile();

	/**
	 * Gets the list of all MasterProfiles.
	 */
	QStringList getAvailableProfiles();

	/**
	 * Gets the list of currently selected MasterProfiles.
	 * NOTE: Order is important!
	 */
	QStringList getSelectedProfiles();

	/**
	 * Sets the list of currently selected MasterProfiles.
	 * NOTE: Order is important!
	 */
	void setSelectedProfiles(QStringList selProfList);

	/**
	 * Removes the selected profile.  Returns whether successful.
	 */
	bool removeProfile(QString name);

	/**
	 * Adds a new profile to the hash.
	 */
	void addProfile(MasterProfile& profile);

	/**
	 * Retrieves a copy of the specified profile.
	 */
	MasterProfile getProfile(QString name);

	/**
	 * Replaces the profile with the specified name with the
	 * specified profile.
	 */
	bool replaceProfile(QString oldProfileName, MasterProfile& newProfile);

	/**
	 * Checks to see if a profile with the specified name is already
	 * in the hash.
	 */
	bool doesProfileExist(QString name);

	/**
	 * Gets whether the master profile is selected randomly.
	 */
	bool isRandomProfile(void);

	/**
	 * Gets whether the master profile is selected randomly.
	 */
	void setRandomProfile(bool bIsRandom);

	/**
	 * Gets the history size
	 */
	int getProfileHistorySize();

	/**
	 * Gets the history size
	 */
	void setProfileHistorySize(int size);

	//*** End MasterProfile Related ***

	//*** Begin Palette Related ***

	/**
	 * Returns a list of the names of the palettes currently in the palette hash.
	 */
	QStringList getPaletteNames();

	/**
	 * Removes the palette with the specified name from the hash.
	 */
	bool removePalette(QString name);

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
	IndexedPaletteProfile getPaletteProfile(QString name);
	//*** End Palette Related ***

	//*** Begin Misc ***

	//whether or not the GPL has been accepted
	bool getGPLAccepted();
	void setGPLAccepted(bool bAccepted);

	//creates a new ConfigManager object that only contains the specified configuration.
	//it is intended for to be used in generating the preview widget.
	ConfigManager* createPreview(int width = 64, int height = 64, LensProfile* lensProfile = NULL, BackgroundProfile* backgroundProfile = NULL, int timerMillis = 50);

	//*** End Misc ***

protected:
	QString getMainConfigFile();
	QString getMainConfigPath();

	//sets the active profile
	void setActiveProfile(QString profileName);

	void loadMiscInfo(QDomNode &node);
	void saveMiscInfo(QDomDocument* doc, QDomElement* root);

	// returns the string used in as the Dom root.
	QString getDomNodeQString();

	//int _numProfiles;
	//profile_struct* _profiles;
	QHash<QString, MasterProfile*> _profiles;
	QStringList _selectedProfileList;
	QStringList _profileHistoryList;
	int _profileHistorySize;
	bool _bRandomProfiles;
	MasterProfile* _currentProfile;

	QHash<QString, IndexedPaletteProfile*> _paletteHash;

	QHash<QString, BackgroundProfile*> _backgroundHash;
	int _numBackgroundProfileTypes;
	BackgroundProfile** _backgroundProfileTypes;

	QHash<QString, LensProfile*> _lensHash;
	int _numLensProfileTypes;
	LensProfile** _lensProfileTypes;
	LensProfile* _currentLensProfile;

	bool _bGPLAccepted;

};

#endif