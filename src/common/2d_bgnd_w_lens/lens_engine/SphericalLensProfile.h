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
 * This profile creates a spherical lens.
 */

#ifndef __SphericalLENSPROFILE_H__
#define __SphericalLENSPROFILE_H__

#include "LensProfile.h"

class SphericalLensProfile : public LensProfile {
public:
	SphericalLensProfile();
	~SphericalLensProfile();

	/* Attempts to load this background profile object from the
	 * specified QDomNode.
	 */
	virtual LensProfile* load(QDomNode &node);

	/*
	 * Returns a QDomNode object that represents this profile.
	 */
	virtual QDomNode save(QDomDocument* doc);

	/*
	 * Creates and returns a new LensObject object.
	 */
	virtual LensObject* getNewLensObject();

	/*
	 * Returns the maximum number of lenses this profile will allow.
	 */
	virtual int getMaxNumLenses();
	void setMaxNumLenses(int num);

	/*
	 * Creates a new object with this object's settings.
	 */
	LensProfile* clone();

	/*
	 * Overloaded assignment operator.
	 */
	SphericalLensProfile& operator=(SphericalLensProfile& other);

	/**
	 * Gets/sets the radius (specified as a percentage
	 * of the screen).
	 */
	float getMinRadius();
	float getMaxRadius();
	void setMinRadius(float r);
	void setMaxRadius(float r);
	void setRadius(float min, float max, bool bRand);
	bool getRandRadius();
	void setRandRadius(bool bRand);

private:
	float _minRadius;
	float _maxRadius;
	bool _bRandSize;
	int _maxNumLenses;
	float _distortionFactor;
};

#endif
