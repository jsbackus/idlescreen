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
#include <QMessageBox>

#include <QRegExp>
#include <QFileInfo>
#include <QTextStream>
#include <QDir>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

#include <windows.h>
#include <stdlib.h>
#include <ctime>
using namespace std;

#include "globaldefs.h"
#include "core/win_misc_funcs.h"
#include "utility/misc_funcs.h"
#include "ConfigManager.h"

ConfigManager::ConfigManager(void) {
	//seed
	srand((unsigned)time( NULL ));

	_historyList = NULL;
	setDefaults();
}

//sets whether or not to randomly pick the next scr
void ConfigManager::enableRandom(bool bEnable) {
	_bRandom = bEnable;
}

//Returns whether or not random mode is enabled.
bool ConfigManager::isRandom() {
	return _bRandom;
}

//Retrieves the size of the history list
int ConfigManager::getHistorySize() {
	return _historyList->getHistoryMaxSize();
}

//sets the size of the history list
void ConfigManager::setHistorySize(int size) {
	_historyList->setHistoryMaxSize(size);
}

void ConfigManager::setSearchPathList(QStringList pathList) {
	_pathList = pathList;
}
QStringList ConfigManager::getSearchPathList() {
	return _pathList;
}

//Gets and sets the screen savers to choose from
void ConfigManager::setScrList(QStringList scrList) {
	_scrList = scrList;
}
QStringList ConfigManager::getScrList() {
	return _scrList;
}

//Sets defaults
void ConfigManager::setDefaults() {
	deleteAllSettings();

	// attempt to load from machine defaults file.
	if(!importFromFile(getExternalDefaultConfigFile())) {
		if(!importFromFile(":/defaults.xml")) {
			_bGPLAccepted = false;
			_bRandom = true;
			_bEnableTimer = false;
			_timerMinutes = 10;
			_bBlankDesktop = true;
			_historyList = new HistoryList(&_scrList, NULL, 5);
			_pathList << "c:\\windows\\system32";
		}
	}
}

/**
 * Retrieves the path of the next screen saver to run.
 * If random is enabled, the path will be randomly selected
 * from _scrList, making sure that path is not in the history
 * list.  If random is not enabled, the path after the first
 * path in the history list will be returned.  In both cases,
 * the path will be checked for validity.  Will return ""
 * if it can't find a valid screen saver.  Calling method
 * should call save() afterward to commit changes to the
 * history list.
 */
QString ConfigManager::getNextScr() {
	QString retVal("");

	if(_scrList.empty()) {
		return retVal;
	}

	int pick = -1;
	int numScrs = _scrList.count();
	if(_bRandom && numScrs > 1) {
		// make sure that there are more scrs than the size of the history.
		if(numScrs < _historyList->getHistoryMaxSize()) {
			_historyList->setHistoryMaxSize(numScrs-1);
		}
		while(pick == -1) {
			pick = rand()%numScrs;
			if(_historyList->isInHistory(pick)) {
				pick = -1;
			}
		}
	} else {
		if(_historyList->getHistorySize() == 0) {
			pick = 0;
		} else {
			pick = _historyList->getAt(0)+1;
			if(pick >= _scrList.count()) {
				pick = 0;
			}
		}
	}

	//insert at the head of the history list and check to make sure it is valid.
	//If the path is invalid, returns "".
	retVal = _scrList.at(pick);
	_historyList->addScrToHistory(pick);
	QFileInfo file(retVal);
	if(!file.exists()) {
		retVal = QString("");
	}
	
	return retVal;
}

//These are used to get/set the program's call name
void ConfigManager::setProgramName(QString name) {
	_prgmName = name;
}

QString ConfigManager::getProgramName() {
	//!!! NOTE !!! Needs Fixed!
	//return _prgmName;
	return "Randomizer.scr";
}

//whether the GPL is accepted or not
void ConfigManager::setGPLAccepted(bool bAccepted) {
	_bGPLAccepted = bAccepted;
}

bool ConfigManager::getGPLAccepted() {
	return _bGPLAccepted;
}

//Use the timer?
bool ConfigManager::useTimer() {
	return _bEnableTimer;
}

//get the number of minutes to set the timer for
int ConfigManager::getTimerMinutes() {
	return _timerMinutes;
}

//set to enable the timer
void ConfigManager::enableTimer(bool bEnable) {
	_bEnableTimer = bEnable;
}

//set the number of minutes
void ConfigManager::setTimerMinutes(int minutes) {
	_timerMinutes = minutes;
}

void ConfigManager::load() {
	deleteAllSettings();

	//try to load from the primary config file.
	//if unsuccessful, try to load defaults.
	if(!importFromFile(getMainConfigFile())) {
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

// returns the string used in as the Dom root.
QString ConfigManager::getDomNodeQString() {
	return getAppConfigName() + QString("_Configuration");
}

void ConfigManager::deleteAllSettings() {
	if(_historyList != NULL) {
		delete _historyList;
		_historyList = NULL;
	}
	_pathList.clear();
	_scrList.clear();
}

bool ConfigManager::importFromFile(QString filename) {
	QDomDocument doc(getDomNodeQString());
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly))
		return false;
	if (!doc.setContent(&file)) {
		file.close();
		return false;
	}
	file.close();

	QDomElement tempNode;
	QDomElement tempElem;

	QDomElement root = doc.documentElement();

	// load whether the GPL has been accepted
	tempElem = root.firstChildElement("gpl_accepted");
	if(!tempElem.isNull()) {
		QString param = tempElem.text();
		_bGPLAccepted = stringToBool(param);
	}

	// load the list of selected screen savers
	tempElem = doc.firstChildElement().firstChildElement("selected_scr");
	while(!tempElem.isNull()) {
		_scrList.append(tempElem.text());
		tempElem = tempElem.nextSiblingElement("selected_scr");
	}

	// load the list of screen saver paths
	tempElem = doc.firstChildElement().firstChildElement("search_path");
	while(!tempElem.isNull()) {
		_pathList.append(tempElem.text());
		tempElem = tempElem.nextSiblingElement("search_path");
	}

	// load the number of minutes for the timer
	tempElem = root.firstChildElement("timer_minutes");
	if(!tempElem.isNull()) {
		_timerMinutes = tempElem.text().toInt();
	}

	// load whether to enable the timer or not
	tempElem = root.firstChildElement("timer_enabled");
	if(!tempElem.isNull()) {
		QString param = tempElem.text();
		_bEnableTimer = stringToBool(param);
	}

	// load whether to enable random or not
	tempElem = root.firstChildElement("random_enabled");
	if(!tempElem.isNull()) {
		QString param = tempElem.text();
		_bRandom = stringToBool(param);
	}

	// load whether to blank the desktop or not
	tempElem = root.firstChildElement("blank_desktop");
	if(!tempElem.isNull()) {
		QString param = tempElem.text();
		_bBlankDesktop = stringToBool(param);
	}

	// import the history list
	if(_historyList == NULL) {
		_historyList = new HistoryList(&_scrList);
	}
	if(_historyList != NULL) {
		_historyList->load(root);
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

	exportToFile(targetFile);
}

bool ConfigManager::exportToFile(QString filename) {
	QDomDocument doc(getDomNodeQString());
	QDomElement root = doc.createElement(getDomNodeQString());
	doc.appendChild(root);

	QString tempStr;
	QDomText tempNode;
	QDomElement tempElem;
	int i;

	//save gpl accepted
	tempNode = doc.createTextNode(boolToString(_bGPLAccepted));
	tempElem = doc.createElement("gpl_accepted");
	tempElem.appendChild(tempNode);
	root.appendChild(tempElem);

	// save the list of selected screen savers
	for(i=0; i<_scrList.size();i++) {
		tempNode = doc.createTextNode(_scrList.at(i));
		tempElem = doc.createElement("selected_scr");
		tempElem.appendChild(tempNode);
		root.appendChild(tempElem);
	}

	// save the list of screen saver paths
	for(i=0; i<_pathList.size();i++) {
		tempNode = doc.createTextNode(_pathList.at(i));
		tempElem = doc.createElement("search_path");
		tempElem.appendChild(tempNode);
		root.appendChild(tempElem);
	}

	// save the number of minutes for the timer
	tempStr.setNum(_timerMinutes);
	tempNode = doc.createTextNode(tempStr);
	tempElem = doc.createElement("timer_minutes");
	tempElem.appendChild(tempNode);
	root.appendChild(tempElem);

	// load whether to enable the timer or not
	tempNode = doc.createTextNode(boolToString(_bEnableTimer));
	tempElem = doc.createElement("timer_enabled");
	tempElem.appendChild(tempNode);
	root.appendChild(tempElem);

	// load whether to enable random or not
	tempNode = doc.createTextNode(boolToString(_bRandom));
	tempElem = doc.createElement("random_enabled");
	tempElem.appendChild(tempNode);
	root.appendChild(tempElem);

	// load whether to blank the desktop or not
	tempNode = doc.createTextNode(boolToString(_bBlankDesktop));
	tempElem = doc.createElement("blank_desktop");
	tempElem.appendChild(tempNode);
	root.appendChild(tempElem);

	// save history
	if(_historyList != NULL) {
		_historyList->save(doc,root);
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

// get/set whether to hide the desktop or not
void ConfigManager::setBlankDesktop(bool bBlank) {
	_bBlankDesktop = bBlank;
}

bool ConfigManager::getBlankDesktop() {
	return _bBlankDesktop;
}
