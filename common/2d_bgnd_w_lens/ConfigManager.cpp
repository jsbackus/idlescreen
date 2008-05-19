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

#include <QCoreApplication>
#include <QApplication>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

// debug
//#include <QMessageBox>

#include "../utility/misc_funcs.h"
#include "../core/win_misc_funcs.h"
#include "project_specific_extern_defs.h"

#include "ConfigManager.h"

//Constructor and deconstructor
ConfigManager::ConfigManager() {

	//specify the plugins path.  plugins should have been installed
	//by the lib package.  the libs need to be added to the system path.
	//first check in HKEY_CURRENT_USER.  If not there, check HKEY_LOCAL_MACHINE.
/*
	QString getExternalDefaultConfigFile();
	QString getExternalDefaultConfigPath();
	QString rootPath = readRegString(QString("Software\\Phej"),
										QString("Lib Install Base Path"), false);
	if(rootPath == "") {
		rootPath = readRegString(QString("Software\\Phej"),
										QString("Lib Install Base Path"), true);
	}

	QString libPath = rootPath + QString("\\Qt-4.2.2\\plugins\\");
	QCoreApplication::addLibraryPath(libPath);
*/

	//set up data structures
	_currentProfile = NULL;
	_currentLensProfile = NULL;
	_backgroundProfileTypes = NULL;
	_lensProfileTypes = NULL;

	//set up list of background types
	_numBackgroundProfileTypes = 0;
	_backgroundProfileTypes = getBackgroundTypes(&_numBackgroundProfileTypes);

	//set up list of lens types
	_numLensProfileTypes = 0;
	_lensProfileTypes = getLensTypes(&_numLensProfileTypes);

	_bGPLAccepted = false;
}

ConfigManager::~ConfigManager() {
	_currentProfile = NULL;
	_currentLensProfile = NULL;

	//delete background profile types data structure
	if(_backgroundProfileTypes != NULL) {
		for(int i=0; i<_numBackgroundProfileTypes; i++) {
			if(_backgroundProfileTypes[i] != NULL) {
				delete _backgroundProfileTypes[i];
				_backgroundProfileTypes[i] = NULL;
			}
		}
		delete [] _backgroundProfileTypes;
		_backgroundProfileTypes = NULL;
	}

	//delete lens profile types data structure
	if(_lensProfileTypes != NULL) {
		for(int i=0; i<_numLensProfileTypes; i++) {
			if(_lensProfileTypes[i] != NULL) {
				delete _lensProfileTypes[i];
				_lensProfileTypes[i] = NULL;
			}
		}
		delete [] _lensProfileTypes;
		_lensProfileTypes = NULL;
	}

	//clean out hashes
	deleteAllSettings();
}

void ConfigManager::load() {
	deleteAllSettings();

	//try to load from the primary config file.
	//if unsuccessful, try to load defaults.
	if(!importFromFile(getMainConfigFile(), true)) {
		setDefaults();
	}
}

QString ConfigManager::getExternalDefaultConfigPath() {
	QString retVal;

#ifdef _M_IX86
	QString installPath = readRegString(QString("Software\\IdleScreen"), QString("InstallPath"), false);
	if(installPath == "") {
		installPath = readRegString(QString("Software\\IdleScreen"), QString("InstallPath"), true);
	}
	retVal += installPath + "/defaults/";
#else
	retVal += "/etc/idlescreen/defaults/";
#endif

	return retVal;
}
QString ConfigManager::getExternalDefaultConfigFile() {
	QString retVal = getExternalDefaultConfigPath() + getAppConfigName() + ".xml";
	return retVal;
}

QString ConfigManager::getMainConfigPath() {
	QString retVal = QDir::homePath() + "/";

#ifdef _M_IX86
	retVal += "Application Data/idlescreen/";
#else
	retVal += ".idlescreen/";
#endif

	retVal += getAppConfigName();

	return retVal;
}

QString ConfigManager::getMainConfigFile() {

	QString retVal = getMainConfigPath();
	retVal += "/settings.xml";

	return retVal;
}

void ConfigManager::deleteAllSettings() {
	_currentProfile = NULL;
	_currentLensProfile = NULL;

	//clean out hashes
	QStringList keys = _backgroundHash.keys();
	for(int i=0; i<keys.size();i++) {
		BackgroundProfile* tmpPtr = _backgroundHash.take(keys.at(i));
		delete tmpPtr;
		tmpPtr = NULL;
	}
	keys = _paletteHash.keys();
	for(int i=0; i<keys.size();i++) {
		IndexedPaletteProfile* tmpPtr = _paletteHash.take(keys.at(i));
		delete tmpPtr;
		tmpPtr = NULL;
	}
	keys = _lensHash.keys();
	for(int i=0; i<keys.size();i++) {
		LensProfile* tmpPtr = _lensHash.take(keys.at(i));
		delete tmpPtr;
		tmpPtr = NULL;
	}
	keys = _lensHash.keys();
	for(int i=0; i<keys.size();i++) {
		MasterProfile* tmpPtr = _profiles.take(keys.at(i));
		delete tmpPtr;
		tmpPtr = NULL;
	}

	//clean out lists
	_selectedProfileList.clear();
	_profileHistoryList.clear();
	_profileHistorySize = 0;

	_bRandomProfiles = false;
	_bGPLAccepted = false;
}


bool ConfigManager::importFromFile(QString filename, bool bLoadMiscInfo) {

	QDomDocument doc(getDomNodeQString());
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly))
		return false;
	if (!doc.setContent(&file)) {
		file.close();
		return false;
	}
	file.close();

	QDomElement root = doc.documentElement();

	//load background profiles
	QDomElement tempNode = doc.firstChildElement().firstChildElement(BackgroundProfile::getXMLTagName());
	while(!tempNode.isNull()) {
		QDomElement tempElem = tempNode.firstChildElement("type");
		if(!tempElem.isNull()) {
			//search list of known types to find the appropriate profile
			int i=0;
			while(i<_numBackgroundProfileTypes && tempElem.text() != _backgroundProfileTypes[i]->getBackgroundType())
				i++;
			if(i<_numBackgroundProfileTypes && tempElem.text() == _backgroundProfileTypes[i]->getBackgroundType()){
				BackgroundProfile* tempProfile = _backgroundProfileTypes[i]->load(tempNode);
				//if non-NULL, add it to the hash of background profiles
				if(tempProfile != NULL) {
					_backgroundHash.insert(tempProfile->getName(), tempProfile);
				}
			}

		}
		tempNode = tempNode.nextSiblingElement(BackgroundProfile::getXMLTagName());
	}

	//load palette profiles
	tempNode = doc.firstChildElement().firstChildElement(IndexedPaletteProfile::getXMLTagName());
	while(!tempNode.isNull()) {
		IndexedPaletteProfile* tempProfile = IndexedPaletteProfile::load(tempNode);
		//if non-NULL, add it to the hash of palette profiles
		if(tempProfile != NULL) {
			_paletteHash.insert(tempProfile->getName(), tempProfile);
		}
		tempNode = tempNode.nextSiblingElement(IndexedPaletteProfile::getXMLTagName());
	}

	//load lens profiles
	tempNode = doc.firstChildElement().firstChildElement(LensProfile::getXMLTagName());
	while(!tempNode.isNull()) {
		QDomElement tempElem = tempNode.firstChildElement("type");
		if(!tempElem.isNull()) {
			//search list of known types to find the appropriate profile
			int i=0;
			while(i<_numLensProfileTypes && tempElem.text() != _lensProfileTypes[i]->getLensType())
				i++;
			if(i<_numLensProfileTypes && tempElem.text() == _lensProfileTypes[i]->getLensType()){
				LensProfile* tempProfile = _lensProfileTypes[i]->load(tempNode);
				//if non-NULL, add it to the hash of background profiles
				if(tempProfile != NULL) {
					_lensHash.insert(tempProfile->getName(), tempProfile);
				}
			}

		}
		tempNode = tempNode.nextSiblingElement(LensProfile::getXMLTagName());
	}

	//load master profiles
	tempNode = doc.firstChildElement().firstChildElement(MasterProfile::getXMLTagName());
	while(!tempNode.isNull()) {
		MasterProfile* tempProfile = MasterProfile::load(tempNode);
		//if non-NULL, add it to the hash of palette profiles
		if(tempProfile != NULL) {
			_profiles.insert(tempProfile->getName(), tempProfile);
		}
		tempNode = tempNode.nextSiblingElement(MasterProfile::getXMLTagName());
	}

	//load misc info
	if(bLoadMiscInfo) {
		loadMiscInfo(root);
	}

	return true;
}

void ConfigManager::save() {
	QString targetFile = getMainConfigFile();

	//make sure target directory exists.  If not, create it.
	QDir targetDir(getMainConfigPath());
	if(!targetDir.exists()) {
		//attempt to make the path.  If failure,
		//just return.
		if(!targetDir.mkpath(getMainConfigPath()))
			return;
	}

	exportToFile(targetFile, true);
}

bool ConfigManager::exportToFile(QString filename, bool bSaveMiscInfo) {
	QDomDocument doc(getDomNodeQString());
	QDomElement root = doc.createElement(getDomNodeQString());
	doc.appendChild(root);

	//store background profiles..
	QList<QString> keys = _backgroundHash.keys();
	for(int i=0; i<keys.size();i++) {
		root.appendChild(_backgroundHash.value(keys.at(i))->save(&doc));
	}

	//store palette profiles..
	keys = _paletteHash.keys();
	for(int i=0; i<keys.size();i++) {
		root.appendChild(_paletteHash.value(keys.at(i))->save(&doc));
	}

	//store lens profiles..
	keys = _lensHash.keys();
	for(int i=0; i<keys.size();i++) {
		root.appendChild(_lensHash.value(keys.at(i))->save(&doc));
	}

	//store master profiles..
	keys = _profiles.keys();
	for(int i=0; i<keys.size();i++) {
		root.appendChild(_profiles.value(keys.at(i))->save(&doc));
	}

	if(bSaveMiscInfo) {
		saveMiscInfo(&doc, &root);
	}

	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		return false;

	QTextStream out(&file);
	QString payload = doc.toString();
	out << payload << "\n";
	file.close();

	return true;
}

void ConfigManager::loadMiscInfo(QDomNode &node) {
	QDomElement tempElem;

	tempElem = node.firstChildElement("random_master_profiles");
	if(!tempElem.isNull()) {
		QString param = tempElem.text();
		_bRandomProfiles = stringToBool(param);
	}

	tempElem = node.firstChildElement("gpl_accepted");
	if(!tempElem.isNull()) {
		QString param = tempElem.text();
		_bGPLAccepted = stringToBool(param);
	}

	tempElem = node.firstChildElement("master_profile_history_size");
	if(!tempElem.isNull()) {
		_profileHistorySize = tempElem.text().toInt();
	}

	//Read in the history list.  We want to preserve the order of this list, indicated by the
	//stored location.  We will start with the last node in the hopes that the order is preserved
	//in the XML file and during read back.  That way, we only have to grow the list once.  We
	//don't want to just use the _imageHistorySize, because there may be fewer entries and we
	//don't want empty strings at the end of the list.
	QDomElement tempNode = node.firstChildElement("master_profile_history").lastChildElement("history_entry");
	while(!tempNode.isNull()) {	
		QString name;
		int index;

		tempElem = tempNode.firstChildElement("name");
		if(!tempElem.isNull()) {
			name = tempElem.text();
		}

		tempElem = tempNode.firstChildElement("location");
		if(!tempElem.isNull()) {
			index = tempElem.text().toInt();
		}

		//grow the list to the appropriate size.  Should only be executed once - assuming
		//the order of entries is preserved within the XML file and when reading back.
		while(index >= _profileHistoryList.size()) {
			_profileHistoryList.append("");
		}

		//insert the string at the appropriate location
		_profileHistoryList.replace(index, name);

		tempNode = tempNode.previousSiblingElement("history_entry");
	}

	//read in the list of selected profiles.  same method as for the history list.
	tempNode = node.firstChildElement("selected_master_profile_list").lastChildElement("selected_entry");
	while(!tempNode.isNull()) {	
		QString name;
		int index;

		tempElem = tempNode.firstChildElement("name");
		if(!tempElem.isNull()) {
			name = tempElem.text();
		}

		tempElem = tempNode.firstChildElement("location");
		if(!tempElem.isNull()) {
			index = tempElem.text().toInt();
		}

		//grow the list to the appropriate size.  Should only be executed once - assuming
		//the order of entries is preserved within the XML file and when reading back.
		while(index >= _selectedProfileList.size()) {
			_selectedProfileList.append("");
		}

		//insert the string at the appropriate location
		_selectedProfileList.replace(index, name);

		tempNode = tempNode.previousSiblingElement("selected_entry");
	}

	// truncate profile history to no more than 1 less than the number of profiles or 0, whichever is greater.
	int numProfiles = _selectedProfileList.size();
	if(_profileHistorySize >= numProfiles) {
		_profileHistorySize = numProfiles - 1;
	}
	// check profile history size.  If less than 1, disable it.
	if(_profileHistorySize < 1) {
		_profileHistorySize = 0;
		_bRandomProfiles = false;
	}
}

void ConfigManager::saveMiscInfo(QDomDocument* doc, QDomElement* root) {
	QString tempStr;
	QDomText tempNode;
	QDomElement tempElem;

	//random profile order?
	tempNode = doc->createTextNode(boolToString(_bRandomProfiles));
	tempElem = doc->createElement("random_master_profiles");
	tempElem.appendChild(tempNode);
	root->appendChild(tempElem);

	//gpl accepted?
	tempNode = doc->createTextNode(boolToString(_bGPLAccepted));
	tempElem = doc->createElement("gpl_accepted");
	tempElem.appendChild(tempNode);
	root->appendChild(tempElem);

	//profile history size
	tempStr.setNum(_profileHistorySize);
	tempNode = doc->createTextNode(tempStr);
	tempElem = doc->createElement("master_profile_history_size");
	tempElem.appendChild(tempNode);
	root->appendChild(tempElem);

	//master profile history list.  Only store up to _profileHistorySize if the size of the
	//list is larger than _profileHistorySize.
	QDomElement historyElem = doc->createElement("master_profile_history");
	QDomElement entryElem;
	int i;
	int curHisSize = _profileHistoryList.size();
	int count = (curHisSize < _profileHistorySize) ? curHisSize : _profileHistorySize;
	for(i=0;i<_profileHistoryList.size(); i++) {
		entryElem = doc->createElement("history_entry");
		//path
		tempNode = doc->createTextNode(_profileHistoryList.at(i));
		tempElem = doc->createElement("name");
		tempElem.appendChild(tempNode);
		entryElem.appendChild(tempElem);

		//history location
		tempStr.setNum(i);
		tempNode = doc->createTextNode(tempStr);
		tempElem = doc->createElement("location");
		tempElem.appendChild(tempNode);
		entryElem.appendChild(tempElem);

		historyElem.appendChild(entryElem);
	}
	root->appendChild(historyElem);

	//master profile history list
	historyElem = doc->createElement("selected_master_profile_list");
	for(i=0;i<_selectedProfileList.size(); i++) {
		entryElem = doc->createElement("selected_entry");
		//path
		tempNode = doc->createTextNode(_selectedProfileList.at(i));
		tempElem = doc->createElement("name");
		tempElem.appendChild(tempNode);
		entryElem.appendChild(tempElem);

		//history location
		tempStr.setNum(i);
		tempNode = doc->createTextNode(tempStr);
		tempElem = doc->createElement("location");
		tempElem.appendChild(tempNode);
		entryElem.appendChild(tempElem);

		historyElem.appendChild(entryElem);
	}
	root->appendChild(historyElem);
}

//Sets defaults
void ConfigManager::setDefaults() {
	deleteAllSettings();

	// attempt to load from machine defaults file.
	if(!importFromFile(getExternalDefaultConfigFile(), true)) {
		if(!importFromFile(":/defaults.xml", true))
			exit(1);
	}
}

//sets the active profile
void ConfigManager::setActiveProfile(QString profileName) {
	_currentProfile = _profiles.value(profileName);
}

/**
 * Returns the number of milliseconds between each timer tick.
 */
long ConfigManager::getTimerMillis() {
	if(_currentProfile == NULL)
		return 50;

	return _currentProfile->getTimerMillis();
}

int ConfigManager::getScreenWidth() {
	if(_currentProfile == NULL)
		return -1;

	return _currentProfile->getScreenWidth();
}

int ConfigManager::getScreenHeight() {
	if(_currentProfile == NULL)
		return -1;

	return _currentProfile->getScreenHeight();
}

int ConfigManager::getTextureWidth() {
	if(_currentProfile == NULL)
		return -1;

	return _currentProfile->getTextureWidth();
}

int ConfigManager::getTextureHeight() {
	if(_currentProfile == NULL)
		return -1;

	return _currentProfile->getTextureHeight();
}

/*
 * Initializes lens related code
 */
void ConfigManager::initializeLenses(screen_struct* screenObj) {
	if(_currentProfile != NULL) {
		_currentLensProfile = _lensHash.value(_currentProfile->getLensProfileName());
		if(_currentLensProfile != NULL) {
			_currentLensProfile->initialize(&_paletteHash, screenObj);
		}
	}
}

/*
 * Returns the maximum number of lenses that the current profile allows.
 */
int ConfigManager::getMaxNumLenses() {
	if(_currentProfile == NULL)
		return 0;

	LensProfile* lensProfile = _lensHash.value(_currentProfile->getLensProfileName());

	if(lensProfile != NULL) {
		return _lensHash.value(_currentProfile->getLensProfileName())->getMaxNumLenses();
	} else {
		return NULL;
	}
}

/*
 * Returns a new lens object that conforms to the currently selected profile.
 */
LensObject* ConfigManager::getNewLens() {
	if(_currentProfile == NULL)
		return NULL;

	LensProfile* lensProfile = _lensHash.value(_currentProfile->getLensProfileName());

	if(lensProfile != NULL) {
		return _lensHash.value(_currentProfile->getLensProfileName())->getNewLensObject();
	} else {
		return NULL;
	}
}

/**
 * Creates and returns a new background object based on the settings of 
 * the current profile.
 */
Background* ConfigManager::getNewBackground() {
	if(_currentProfile == NULL)
		return NULL;

	return _backgroundHash.value(_currentProfile->getBackgroundProfileName())->getNewBackgroundObj(
													_currentProfile->getScreenHeight(),
													_currentProfile->getScreenWidth(),
													&_paletteHash);
}

/**
 * Called when in scr mode to choose the active profile.
 */
void ConfigManager::chooseActiveProfile() {
	//setActiveProfile("Image SCR");

	//set the current choice to the last choice made
	int current;
	if(_profileHistoryList.size() > 0) {
		current = _selectedProfileList.indexOf(_profileHistoryList.at(0));
	} else {
		current = -1;
	}

	//get the max size of the history list.  If it is equal to or larger than the
	//size of the list of selected profiles, truncate to the size of the
	//selected list - 1.  This *shouldn't* happen.  If not using random
	//profiles, truncate the list to size 1.
	int historySize = _profileHistorySize;
	if(historySize >= _selectedProfileList.size() || !_bRandomProfiles) {
		if(!_bRandomProfiles) {
			historySize = 1;
		} else if(historySize >= _selectedProfileList.size()) {
			historySize = _selectedProfileList.size()-1;
		} 
		while(_profileHistoryList.size() > historySize) {
			_profileHistoryList.removeLast();
		}
	}

	if(_bRandomProfiles) {
		//if so, continue picking one until it isn't the current one or in the history.
		bool bInHistory = true;
		while(bInHistory) {
			int newProfile = rand()%_selectedProfileList.size();
			//this is in case there isn't a history list.
			if(current < 0) {
				bInHistory = false;
				current = newProfile;
			}
			if(newProfile != current) {
				QString newProfileName = _selectedProfileList.at(newProfile);
				//search history
				bInHistory = false;
				int i=0;
				while(i<_profileHistoryList.size() && !bInHistory) {
					if(newProfileName == _profileHistoryList.at(i)) {
						bInHistory = true;
					}
					i++;
				}
			}
			current = newProfile;
		}	
	} else {
		current++;
		if(current >= _selectedProfileList.size()) {
			current = 0;
		}
	}

	//update history
	//first shift down
	int tmpIndex;
	int tmpSize = _profileHistoryList.size();
	if(tmpSize < historySize) {
		tmpIndex = tmpSize-1;

		//if the list is shorter than the max size and nonzero, move
		//the last item with append instead of replace
		if(tmpSize > 0) {
			_profileHistoryList.append(_profileHistoryList.at(tmpIndex));
		}
	} else {
		tmpIndex = historySize-1;
	}
	while(tmpIndex > 0) {
		_profileHistoryList.replace(tmpIndex, _profileHistoryList.at(tmpIndex-1));
		tmpIndex--;
	}

	//insert at the top, or append if the list is empty
	if(historySize == 0) {
		_profileHistoryList.append(_selectedProfileList.at(current));
	} else {
		_profileHistoryList.replace(0,_selectedProfileList.at(current));
	}

	//set active profile
	setActiveProfile(_selectedProfileList.at(current));
}

//whether or not the GPL has been accepted
bool ConfigManager::getGPLAccepted() {
	return _bGPLAccepted;
}

void ConfigManager::setGPLAccepted(bool bAccepted) {
	_bGPLAccepted = bAccepted;
}

/**
 * Returns a list of the names of the palettes currently in the palette hash.
 */
QStringList ConfigManager::getPaletteNames() {
	return _paletteHash.keys();
}

/**
 * Removes the palette with the specified name from the hash.  Returns
 * whether successfull.
 */
bool ConfigManager::removePalette(QString name) {
	return (_paletteHash.remove(name) > 0);
}

/**
 * Adds the specified palette to the hash if it isn't already
 * in the hash.
 */
void ConfigManager::addPalette(IndexedPaletteProfile& pal) {
	//check for preexisting palette name.
	if(!doesPaletteExist(pal.getName())) {
		//if not, create a new palette profile off of the heap to add to the hash.
		IndexedPaletteProfile* newPal = new IndexedPaletteProfile();
		*newPal = pal;
		_paletteHash.insert(pal.getName(), newPal);
	}
}

/**
 * Checks to see if a palette with the specified name is already
 * in the hash.
 */
bool ConfigManager::doesPaletteExist(QString name) {
	return _paletteHash.contains(name);
}

/**
 * Replaces the palette with the specified name with the
 * specified palette if the new name isn't already
 * in the list.
 */
bool ConfigManager::replacePalette(QString oldPaletteName, IndexedPaletteProfile& newPalette) {
	//first, check to see if the new palette name is already in the list and that
	//it isn't the same as the old name.  If so, fail.
	QString newPalName = newPalette.getName();
	if(oldPaletteName != newPalName && doesPaletteExist(newPalName)) {
		return false;
	}

	//now check to see if the old palette name is in the list.  if so, replace, otherwise add.
	if(doesPaletteExist(oldPaletteName)) {
		removePalette(oldPaletteName);
		addPalette(newPalette);
	} else {
		addPalette(newPalette);
	}

	return true;
}

/**
 * Returns a copy of the specified palette profile.  Caller should
 * check for existence first with doesPaletteExist();
 */
IndexedPaletteProfile ConfigManager::getPaletteProfile(QString name) {
	IndexedPaletteProfile* retVal = _paletteHash.value(name);

	if(retVal != NULL)
		return *retVal;

	//if the specified palette isn't in the hash, return a new one.
	IndexedPaletteProfile newPalette;
	return newPalette;
}

/**
 * Gets the list of all MasterProfiles.
 */
QStringList ConfigManager::getAvailableProfiles() {
	return _profiles.keys();
}

/**
 * Gets the list of currently selected MasterProfiles.
 * NOTE: Order is important!
 */
QStringList ConfigManager::getSelectedProfiles() {
	return _selectedProfileList;
}

/**
 * Sets the list of currently selected MasterProfiles.
 * NOTE: Order is important!
 */
void ConfigManager::setSelectedProfiles(QStringList selProfList) {
	_selectedProfileList.clear();
	_selectedProfileList = selProfList;
}

/**
 * Removes the selected profile.  Returns whether successful.
 */
bool ConfigManager::removeProfile(QString name) {
	return (_profiles.remove(name) > 0);
}

/**
 * Adds a new profile to the hash.
 */
void ConfigManager::addProfile(MasterProfile& profile) {
	//check for preexisting profile name.
	if(!doesProfileExist(profile.getName())) {
		//if not, create a new profile off of the heap to add to the hash.
		MasterProfile* newProfile = new MasterProfile();
		*newProfile = profile;
		_profiles.insert(profile.getName(), newProfile);
	}
}

/**
 * Retrieves a copy of the specified profile.
 */
MasterProfile ConfigManager::getProfile(QString name) {
	MasterProfile* retVal = _profiles.value(name);

	if(retVal != NULL)
		return *retVal;

	//if the specified profile isn't in the hash, return a new one.
	MasterProfile newProfile;
	return newProfile;
}

/**
 * Replaces the profile with the specified name with the
 * specified profile.
 */
bool ConfigManager::replaceProfile(QString oldProfileName, MasterProfile& newProfile) {
	//first, check to see if the new profile name is already in the list and that
	//it isn't the same as the old name.  If so, fail.
	QString newProfileName = newProfile.getName();
	if(oldProfileName != newProfileName && doesProfileExist(newProfileName)) {
		return false;
	}

	//now check to see if the old profile name is in the list.  if so, replace, otherwise add.
	if(doesProfileExist(oldProfileName)) {
		removeProfile(oldProfileName);
		addProfile(newProfile);
	} else {
		addProfile(newProfile);
	}

	return true;
}

/**
 * Checks to see if a palette with the specified name is already
 * in the hash.
 */
bool ConfigManager::doesProfileExist(QString name) {
	return _profiles.contains(name);
}

/**
 * Gets whether the master profile is selected randomly.
 */
bool ConfigManager::isRandomProfile(void) {
	return _bRandomProfiles;
}

/**
 * Gets whether the master profile is selected randomly.
 */
void ConfigManager::setRandomProfile(bool bIsRandom) {
	_bRandomProfiles = bIsRandom;
}

/**
 * Gets the history size
 */
int ConfigManager::getProfileHistorySize() {
	return _profileHistorySize;
}

/**
 * Gets the history size
 */
void ConfigManager::setProfileHistorySize(int size) {
	if(size > 0) {
		_profileHistorySize = size;
	} else {
		_profileHistorySize = 1;
	}
}

/**
 * Gets the list of all BackgroundProfiles.
 */
QStringList ConfigManager::getAvailableBackgroundProfiles() {
	return _backgroundHash.keys();
}

/**
 * Removes the selected background profile.  Returns whether successful.
 */
bool ConfigManager::removeBackgroundProfile(QString name) {
	return (_backgroundHash.remove(name) > 0);
}

/**
 * Adds a new background profile to the hash.
 */
void ConfigManager::addBackgroundProfile(BackgroundProfile& backgroundProfile) {
	//check for preexisting profile name.
	if(!doesProfileExist(backgroundProfile.getName())) {
		//if not, create a new profile off of the heap to add to the hash.
		BackgroundProfile* newProfile = backgroundProfile.clone();
		_backgroundHash.insert(backgroundProfile.getName(), newProfile);
	}
}

/**
 * Retrieves a copy of the specified background profile.
 */
BackgroundProfile* ConfigManager::getBackgroundProfile(QString name) {
	BackgroundProfile* retVal = _backgroundHash.value(name);

	if(retVal != NULL)
		return retVal->clone();

	//if the specified background profile isn't in the hash, return a new one.
	BackgroundProfile newProfile;
	return newProfile.clone();
}

/**
 * Replaces the profile with the specified name with the
 * specified background profile.
 */
bool ConfigManager::replaceBackgroundProfile(QString oldBackgroundProfileName, BackgroundProfile& newBackgroundProfile) {
	//first, check to see if the new profile name is already in the list and that
	//it isn't the same as the old name.  If so, fail.
	QString newBackgroundProfileName = newBackgroundProfile.getName();
	if(oldBackgroundProfileName != newBackgroundProfileName && doesBackgroundProfileExist(newBackgroundProfileName)) {
		return false;
	}

	//now check to see if the old profile name is in the list.  if so, replace, otherwise add.
	if(doesBackgroundProfileExist(oldBackgroundProfileName)) {
		removeBackgroundProfile(oldBackgroundProfileName);
		addBackgroundProfile(newBackgroundProfile);
	} else {
		addBackgroundProfile(newBackgroundProfile);
	}

	return true;
}

/**
 * Checks to see if a background profile with the specified name is already
 * in the hash.
 */
bool ConfigManager::doesBackgroundProfileExist(QString name) {
	return _backgroundHash.contains(name);
}

/**
 * Gets the list of all LensProfiles.
 */
QStringList ConfigManager::getAvailableLensProfiles() {
	return _lensHash.keys();
}

/**
 * Removes the selected lens profile.  Returns whether successful.
 */
bool ConfigManager::removeLensProfile(QString name) {
	return (_lensHash.remove(name) > 0);
}

/**
 * Adds a new lens profile to the hash.
 */
void ConfigManager::addLensProfile(LensProfile& lensProfile) {
	//check for preexisting profile name.
	if(!doesLensProfileExist(lensProfile.getName())) {
		//if not, create a new profile off of the heap to add to the hash.
// !!! NOTE: There is a bug here such that only the base class gets cloned and added !!!
//QMessageBox::information(NULL, lensProfile.getLensType(), lensProfile.getName(), QMessageBox::Ok);
		LensProfile* newProfile = lensProfile.clone();
//QMessageBox::information(NULL, newProfile->getLensType(), newProfile->getName(), QMessageBox::Ok);
		_lensHash.insert(lensProfile.getName(), newProfile);
	}
}

/**
 * Retrieves a copy of the specified lens profile.
 */
LensProfile ConfigManager::getLensProfile(QString name) {
	LensProfile* retVal = _lensHash.value(name);

	if(retVal != NULL)
		return *retVal;

	//if the specified lens profile isn't in the hash, return a new one.
	LensProfile newProfile;
	return newProfile;
}

/**
 * Replaces the lens profile with the specified name with the
 * specified lens profile.
 */
bool ConfigManager::replaceLensProfile(QString oldLensProfileName, LensProfile& newLensProfile) {
	//first, check to see if the new profile name is already in the list and that
	//it isn't the same as the old name.  If so, fail.
	QString newLensProfileName = newLensProfile.getName();
	if(oldLensProfileName != newLensProfileName && doesLensProfileExist(newLensProfileName)) {
		return false;
	}

	//now check to see if the old profile name is in the list.  if so, replace, otherwise add.
	if(doesLensProfileExist(oldLensProfileName)) {
		removeLensProfile(oldLensProfileName);
		addLensProfile(newLensProfile);
	} else {
		addLensProfile(newLensProfile);
	}

	return true;
}

/**
 * Checks to see if a lens profile with the specified name is already
 * in the hash.
 */
bool ConfigManager::doesLensProfileExist(QString name) {
	return _lensHash.contains(name);
}

//creates a new ConfigManager object that only contains the specified configuration.
//it is intended for to be used in generating the preview widget.
ConfigManager* ConfigManager::createPreview(int width, int height, LensProfile* lensProfile, BackgroundProfile* backgroundProfile, int timerMillis) {
	return NULL;
}

// returns the string used in as the Dom root.
QString ConfigManager::getDomNodeQString() {
	return getAppConfigName() + QString("_Configuration");
}
