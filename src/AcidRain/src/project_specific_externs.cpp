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
 * This file contains project specific definitions, including version number.
 */

#include <QString>

#include "../../common/2d_bgnd_w_lens/BackgroundProfile.h"
#include "../../common/2d_bgnd_w_lens/lens_engine/LensProfile.h"
#include "AcidRainBackgroundProfile.h"

/**
 * This function will create an array that contains the profiles
 * for allowable backgrounds and sets numProfiles to indicate
 * the number of profiles in the array.  If there is a problem,
 * numProfiles will be set to 0 and NULL will be returned.
 */
BackgroundProfile** getBackgroundTypes(int* numProfiles) {
	BackgroundProfile** retVal = NULL;

	*numProfiles = 1;
	retVal = new BackgroundProfile*[*numProfiles];

	if(retVal == NULL) {
		*numProfiles = 0;
		return NULL;
	}

	retVal[0] = (BackgroundProfile*) (new AcidRainBackgroundProfile());

	if(retVal[0] == NULL) {
		*numProfiles = 0;
		return NULL;
	}

	return retVal;
}

/**
 * This function will create an array that contains the profiles
 * for allowable lenses and sets numProfiles to indicate
 * the number of profiles in the array.  If there is a problem,
 * numProfiles will be set to 0 and NULL will be returned.
 */
LensProfile** getLensTypes(int* numProfiles) {
	LensProfile** retVal = NULL;
	/*
	*numProfiles = 1;
	retVal = new LensProfile*[*numProfiles];

	if(retVal == NULL) {
		*numProfiles = 0;
		return NULL;
	}

	retVal[0] = (LensProfile*) (new SphericalLensProfile());

	if(retVal[0] == NULL) {
		*numProfiles = 0;
		return NULL;
	}
	*/
	return retVal;
}

/**
 * This function returns the string used to create the path for
 * the settings file.  It is also used to define the XML
 * document root.
 */
QString getAppConfigName(){
	return QString("AcidRain");
}

/**
 * Returns the full name of the screen saver to be used in
 * places such as title bars of dialog boxes and the About
 * dialog box.
 */
QString getAppFullName() {
	return QString("AcidRain");
}

/**
 * Returns the version string for the screen saver.
 */
QString getAppVersion() {
	return QString("0.1");
}


