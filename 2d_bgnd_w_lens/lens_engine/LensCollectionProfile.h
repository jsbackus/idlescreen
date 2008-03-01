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
 * This profile is a container class that holds a collection of LensProfile objects.
 * Whenever a new lens is requested, it is randomly picked from the profiles in
 * the collection.  Collections can be contained within collections.  Note that
 * the root collection determines the maximum number of lenses!
 */

#ifndef __LENSCOLLECTIONPROFILE_H__
#define __LENSCOLLECTIONPROFILE_H__

#include <QHash>
#include <QStringList>

#include "LensProfile.h"

class LensCollectionProfile : public LensProfile {
public:
	LensCollectionProfile(QHash<QString, LensProfile*>* lensHash);
	~LensCollectionProfile();

	/* Attempts to load this background profile object from the
	 * specified QDomNode.
	 */
	LensProfile* load(QDomNode &node);

	/*
	 * Returns a QDomNode object that represents this profile.
	 */
	QDomNode save(QDomDocument* doc);

	/*
	 * Creates and returns a new LensObject object.
	 */
	LensObject* getNewLensObject();

	/*
	 * Returns the maximum number of lenses this profile will allow.
	 */
	int getMaxNumLenses();

	/*
	 * Initializes the profile so that it can generate new lenses with getNewLensObject().
	 */
	void initialize(QHash<QString, IndexedPaletteProfile*>* palHash, screen_struct* screenObj); 

private:
	QHash<QString, LensProfile*>* _lensHash;
	QStringList _profiles;
	int _maxNumLenses;
	bool _bRandProfile;
	int _lastProfile;
};

#endif