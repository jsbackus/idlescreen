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
 * This code is used to execute a program in a different directory.
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
 *
 * Revision history:
 * 080308	jsbackus	Initial version.
 */

#include <windows.h>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <conio.h>
#include <process.h>

using namespace std;

//#ifndef IDS_REDIRECTOR_ID_STRING
//	#define IDS_REDIRECTOR_ID_STRING        2112
//#endif
#include "redirector_resource.h"

//#include "../../../common/utility/file_crc.h"

const float VERSION_NUMBER = 0.5;
const char PARENT_KEY[] = "Software\\IdleScreen\\Redirector";

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
 * This method tries to verify that the target is not another instance of
 * redirector.  Does so by looking for a string with the resource
 * number IDS_REDIRECTOR_ID_STRING that matches the string of this
 * executable.  Not fool-proof, but better than nothing.  Returns
 * true if the target is okay to run.
 */
bool verifyTargetNotRedirector(const char* path) {

	// first attempt to load the target as a data module
	HMODULE targetFile = LoadLibraryEx(path, NULL, LOAD_LIBRARY_AS_DATAFILE);
	HMODULE thisFile = GetModuleHandle(NULL);

	// target file is bad.  Return
	// false so that the caller will exit.
	if(targetFile == NULL)
		return false;

	int bob = IDS_REDIRECTOR_ID_STRING;

	char targetBuf[1024];
	int targetLen = LoadString(targetFile, IDS_REDIRECTOR_ID_STRING, &targetBuf[0], sizeof(targetBuf)/sizeof(TCHAR));
	char thisBuf[1024];
	int thisLen = LoadString(thisFile, IDS_REDIRECTOR_ID_STRING, &thisBuf[0], sizeof(thisBuf)/sizeof(TCHAR));

	// compare
	return !( (thisLen == targetLen) && !strcmp(&targetBuf[0], &thisBuf[0]));
}

/**
 * This method takes an LPSTR and converts it to the usual argc & argv format.
 * Takes as arguments a reference to argc and returns a pointer to an array
 * of pointers.  The first pointer will be NULL as well as the entry after
 * the last argument.  The argument string is divided up on spaces, with
 * multiple spaces ignored.  Arguments enclosed in "" or '' are grouped
 * together.
 */
char** convArgs(LPSTR lpCmdLine, int& argc) {
	int i;
	char** retVal = NULL;
	bool bOpenQuote = false;
	bool bOpenDoubleQuote = false;
	bool bPreviousWasSpace = true;
	bool bDone = false;

	// first, count the number of arguments, account for the quotes.
	argc = 0;
	i = 0;
	while(!bDone) {
		switch(lpCmdLine[i]) {
			case 0x00:
				bDone = true;
			case ' ':
				if(!bOpenQuote && !bOpenDoubleQuote && !bPreviousWasSpace) {
					argc++;
					bPreviousWasSpace = true;
				}
				break;
			case '"':
				bOpenDoubleQuote = !bOpenDoubleQuote;
				bPreviousWasSpace = false;
				break;
			case '\'':
				bOpenQuote = !bOpenQuote;
				bPreviousWasSpace = false;
				break;
			default:
				bPreviousWasSpace = false;
				break;
		}
		i++;
	}

	if(bOpenQuote) {
		cout<<"Missing matching \'!"<<endl;
		exit(1);
	}
	
	if(bOpenDoubleQuote) {
		cout<<"Missing matching \"!"<<endl;
		exit(1);
	}

	// create array of char*
	retVal = new char*[argc+2];
	retVal[0] = NULL;
	retVal[argc+1] = NULL;

	if(argc == 0) {
		// if there weren't any arguments, return here.
		return retVal;
	}

	// now populate the list of pointers
	bDone = false;
	bOpenQuote = false;
	bOpenDoubleQuote = false;
	bPreviousWasSpace = true;
	i = 0;
	int cur = 1;
	while(!bDone) {
		switch(lpCmdLine[i]) {
			case 0x00:
				bDone = true;
			case ' ':
				if(!bOpenQuote && !bOpenDoubleQuote) {
					bPreviousWasSpace = true;
					// insert NULL termination
					lpCmdLine[i] = 0x00;
				}
				break;
			case '"':
				if(!bOpenQuote && !bOpenDoubleQuote && bPreviousWasSpace) {
					retVal[cur++] = &lpCmdLine[i];
				}
				bPreviousWasSpace = false;
				bOpenDoubleQuote = !bOpenDoubleQuote;
				break;
			case '\'':
				if(!bOpenQuote && !bOpenDoubleQuote && bPreviousWasSpace) {
					retVal[cur++] = &lpCmdLine[i];
				}
				bPreviousWasSpace = false;
				bOpenQuote = !bOpenQuote;
				break;
			default:
				if(bPreviousWasSpace) {
					retVal[cur++] = &lpCmdLine[i];
				}
				bPreviousWasSpace = false;
				break;
		}
		i++;
	}
	return retVal;
}

/**
 * Main entry point.  It takes the arguments that are
 * passed to int winmain() and returns the success/error code.
 * Also takes the keyname to look up as an argument.
 */
int redirector(LPSTR lpCmdLine, const char* keyName) {
	int i;

	bool bIsThumbnail = false;

	int argc = 0;
	char** argv = NULL;

	argv = convArgs(lpCmdLine, argc);

	// process arguments, if any.
	for(i=1;i<argc+1;i++) {
		char* tmp = argv[i];
		if( (*argv[i] == '-' || *argv[i] == '/') && (*(argv[i]+2) == 0)) {
			switch(*(argv[i]+1)) {
				case 'v':
				case 'V':
					displayAbout();
					break;
				case 'p':
				case 'P':
					bIsThumbnail = true;
					break;
			}
		}
	}

	// attempt to locate target executable
	char path[1024];
	char key[1024];

	// make a copy to process
	strcpy(&key[0], keyName);
	
	// first as non-priviledged user
	DWORD dsize = sizeof(path)-2;
	if(!getPathFromRegistry(&key[0], &path[0], dsize, false)) {
		// if not, check as admin
		if(!getPathFromRegistry(&key[0], &path[0], dsize, true)) {
			cout<<"Unable to locate registry key for '"<<&key[0]<<"'!"<<endl;
			return 1;
		}
	}

	// make sure it is okay to run the target
	if(!verifyTargetNotRedirector(&path[0])) {
		exit(1);
	}

	argv[0] = &path[0];

	// if this is a thumbnail, use exec otherwise the setup box
	// will appear to hang.  Use spawn for all other cases.
	if(bIsThumbnail) {
		_execv(&path[0], argv);
		// this is only reached if exec fails.
		cout<<"Execution failed!  Is '"<<path<<"' correct?"<<endl;
		return 1;
	} else {
		_spawnv(_P_WAIT, &path[0], argv);
		return 0;
	}
}