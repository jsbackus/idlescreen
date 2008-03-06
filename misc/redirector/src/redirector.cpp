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
 */

/**
 * This program is used to execute a program in a different directory.
 * This is necessary because Windows requires screen savers and their
 * libraries to be installed in WINDOWS\SYSTEM32.  Unfortunately,
 * it isn't sufficient to modify the search path, because the OS doesn't
 * use it.
 *
 * Redirector figures out which program by looking for a key with the
 * same name as its call name in the registry.  In an effort to prevent
 * an infinite loop, redirector calculates a CRC value for itself and
 * the program specified in the registry.  If the values match, it will
 * not run.  There is the potential for false positives, but it is
 * low.
 */

#include <windows.h>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <conio.h>
#include <process.h>

using namespace std;

#include "../../../common/utility/file_crc.h"

const float VERSION_NUMBER = 0.5;
const char PARENT_KEY[] = "Software\\IdleScreen";
/**
 * This method attempts to load the path for the specified key.  If
 * bSearchAdmin is true, the method will look in HKEY_LOCAL_MACHINE,
 * otherwise it will look in HKEY_CURRENT_USER.  Returns true
 * if successful.  The specified buffer that will store the path
 * must be large enough to hold the path!
 */
bool getPathFromRegistry(char* keyName, char* path, DWORD buffSize, bool bSearchAdmin) {
	//get the install path and the PATH environmental variables from the registry
	HKEY key;

	HKEY regRoot;

	if(bSearchAdmin) {
		regRoot = HKEY_LOCAL_MACHINE;
	} else {
		regRoot = HKEY_CURRENT_USER;
	}

	//install path
	if (RegOpenKeyEx( regRoot,
		&PARENT_KEY[0], //lpctstr
		0,			//reserved
		KEY_READ,
		&key) == ERROR_SUCCESS)	
	{

		//install path
		path[0] = 0x00;
		RegQueryValueEx(key, keyName, NULL, NULL,(BYTE*)path, &buffSize);

		if(buffSize <= 1 || path[0] == 0x00)
			return false;

		RegCloseKey(key);

	} else {
		RegCloseKey(key);
		return false;
	}

	return true;
}

/**
 * This method outputs information about redirector and exits.
 */
void displayAbout() {
	cout<<endl<<"Executable Redirector version "<<VERSION_NUMBER<<" is part of the Idle Screen Project."<<endl;
	cout<<"Please visit http://idlescreen.googlepages.com for more information."<<endl;
	exit(0);
}

/**
 * Main entry point.
 */
int main(int argc, char** argv) {
	int i;

	// process arguments, if any.
	for(i=1;i<argc;i++) {
		if(strcmp(argv[i], "-v")==0 || strcmp(argv[i], "-V")==0) {
			displayAbout();
		}
	}

	// attempt to locate target executable
	char path[1024];
	char key[1024];

	// make a copy to process
	strcpy(&key[0], argv[0]);
	
	// strip out the path part of the filename
	int startIdx = 0;
	while(key[startIdx++] != 0x00);
	while(key[startIdx] != '\\' && startIdx >= 0)
		startIdx--;
	startIdx++;

	// first as non-priviledged user
	DWORD dsize = sizeof(path)-2;
	if(!getPathFromRegistry(&key[startIdx], &path[0], dsize, false)) {
		// if not, check as admin
		if(!getPathFromRegistry(&key[startIdx], &path[0], dsize, true)) {
			cout<<"Unable to locate registry key for '"<<&key[startIdx]<<"'!"<<endl;
			return 1;
		}
	}

	// calculate CRC and compare to the CRC of this executable
	unsigned long crcThis = 0;
	if(!genCRC(argv[0], &crcThis)) {
		cout<<"Failed CRC generation of "<<argv[0]<<endl;
	}
	unsigned long crcTarget = 0;
	if(!genCRC(&path[0], &crcTarget)) {
		cout<<"Failed CRC generation of "<<path<<endl;
	}
	if(crcThis == crcTarget) {
		cout<<"CRC values match!  Could you be pointing to another copy of redirector?"<<endl;
		exit(1);
	} else {
		cout<<"No match!"<<endl;
	}

	// repackage arguments
	char** newArgs = new char*[argc+1];
	if(newArgs == NULL) {
		cout<<"Error allocating memory!"<<endl;
	}

	newArgs[0] = &path[0];

	for(i=1;i<argc;i++) {
		newArgs[i] = argv[i];
	}

	newArgs[argc] = NULL;

	// execute!
	_execv(&path[0], newArgs);

	// this is only reached if exec fails.
	cout<<"Execution failed!  Is '"<<path<<"' correct?"<<endl;
	return 1;
}