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


//check to see if in windows
#ifdef _M_IX86

#include <QString>

#include <windows.h>
#include <stdlib.h>
#include <ctime>
using namespace std;

//if bLocalMachine is true, looks for key in HKEY_LOCAL_MACHINE, otherwise looks in HKEY_CURRENT_USER
QString readRegString(QString parentKey, QString targetKey, bool bLocalMachine) {
	QString retVal = "";

	HKEY key;
	HKEY regFolder = (bLocalMachine)? HKEY_LOCAL_MACHINE : HKEY_CURRENT_USER;

	wchar_t keyName[255];
	parentKey.toWCharArray(&keyName[0]);
	keyName[parentKey.size()] = 0x00;

	if (RegOpenKeyEx( regFolder,
		&keyName[0], //lpctstr
		0,			//reserved
		KEY_READ,
		&key) == ERROR_SUCCESS)	
	{
		DWORD dsize;
		wchar_t cTemp[1024];

		dsize = sizeof(cTemp)-2;
		targetKey.toWCharArray(&keyName[0]);
		keyName[targetKey.size()] = 0x00;
		if(RegQueryValueEx(key, &keyName[0], NULL, NULL,(BYTE*)&cTemp[0], &dsize)!= ERROR_SUCCESS) {
			return QString("");
		}

		retVal = QString::fromWCharArray(cTemp);
	}

	return retVal;
}
#else

#include <QString>
QString readRegString(QString parentKey, QString targetKey) {
	return QString("");
}

#endif