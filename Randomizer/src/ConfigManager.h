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
 * This class is responsible for maintaining all configuration information,
 * including all communication with the registry.
 */

#ifndef __CONFIGMANAGER_H__
#define __CONFIGMANAGER_H__

#include<QStringList>

#include "HistoryList.h"

class ConfigManager {
public:

	//constructor loads values from registry
	ConfigManager(void);

	void load();
	void save();

	bool importFromFile(QString filename);
	bool exportToFile(QString filename);

	void deleteAllSettings();

	//sets whether or not to randomly pick the next scr
	void enableRandom(bool bEnable = false);

	//Returns whether or not random mode is enabled.
	bool isRandom();

	//Retrieves the size of the history list
	int getHistorySize();

	//sets the size of the history list
	void setHistorySize(int size);

	//Search Path List
	void setSearchPathList(QStringList pathList);
	QStringList getSearchPathList();

	//Gets and sets the screen savers to choose from
	void setScrList(QStringList scrList);
	QStringList getScrList();

	//set to enable the timer
	void enableTimer(bool bEnable = false);

	//Use the timer?
	bool useTimer();

	//get the number of minutes to set the timer for
	int getTimerMinutes();

	//set the number of minutes
	void setTimerMinutes(int minutes = 10);

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
	QString getNextScr();

	void setDefaults();

	//These are used to get/set the program's call name
	void setProgramName(QString name);
	QString getProgramName();

	//whether the GPL is accepted or not
	void setGPLAccepted(bool bAccepted = false);
	bool getGPLAccepted();

private:
	// used to get the name and path of the config files
	QString getMainConfigFile();
	QString getMainConfigPath();
	QString getExternalDefaultConfigFile();
	QString getExternalDefaultConfigPath();

	// returns the string used in as the Dom root.
	QString getDomNodeQString();

	bool _bGPLAccepted;
	bool _bRandom;

	HistoryList* _historyList;

	QStringList _pathList;
	QStringList _scrList;
	QString _prgmName;

	bool _bEnableTimer;
	int _timerMinutes;
	bool _bBlankDesktop;
};

#endif