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
 * This is the base class for all Lens related profiles, including the LensProfileCollection.
 * LensProfiles are responsible for loading/saving from QDomNodes, and creating a new lens
 * when requested.
 *
 * A quick note about Lens Tinting:  The profile stores the name of a palette from which
 * tint colors are chosen.  The lens tint is chosen from the colors where y=0.  If
 * _bRandTint is true, the color is randomly chosen, otherwise colors are chosen in order.
 * If the palette has more than 1 row, the outline tints are chosen from the colors where
 * y=1.  When choosing colors, when x is decided for the main tint, that same x is used
 * for the outline.
 * 
 * @author jeff backus
 * @date 9/15/2007
 */

#ifndef __LENSPROFILE_H__
#define __LENSPROFILE_H__

#include <QString>
#include <QHash>
#include <QtXml/QDomNode>
#include <QtXml/QDomDocument>

#include "../globaldefs.h"
#include "LensObject.h"
#include "GenericLensObject.h"
#include "../../IndexedPalette/IndexedPaletteProfile.h"

class LensProfile {
public:

	LensProfile();
	~LensProfile();

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

	/*
	 * Initializes the profile so that it can generate new lenses with getNewLensObject().
	 */
	virtual void initialize(QHash<QString, IndexedPaletteProfile*>* palHash, screen_struct* screenObj);

	/*
	 * Whether this profile has been initialized or not.
	 */
	bool isInitialized();

	/*
	 * Get/set the name of this profile.
	 */
	void setName(const QString &name);
	QString getName();

	/*
	 * Returns the tag name used by Dom elements for all
	 * subclasses of BackgroundProfile.
	 */
	static QString getXMLTagName();

	/*
	 * Returns the type of BackgroundProfile.
	 */
	QString getLensType();

	/*
	 * Creates a new object with this object's settings.
	 */
	virtual LensProfile* clone();

	/*
	 * Overloaded assignment operator.
	 */
	virtual LensProfile& operator=(LensProfile& other);

protected:
	//load and save base class values.  The only values not saved or loaded are _sizeX & _sizeY
	void loadBase(QDomNode &node);
	void saveBase(QDomDocument* doc, QDomNode &target);
	void initializeBase(QHash<QString, IndexedPaletteProfile*>* palHash, screen_struct* screenObj);

	// deep copies the specified target
	void baseCopy(LensProfile& other);

	/*
	 * Sets the following bits of data:
	 * sizeX, sizeY
	 * outlineWidth
	 * screenObj
	 * primary and outline tints (including complement coloring)
	 * size
	 * starting position
	 * tragectory
	 * speed
	 */
	GenericLensObject* getNewGenericLens(float sizePercentX, float sizePercentY);

	//convert between string and side_type
	static QString sideToString(side_type side);
	static side_type stringToSide(QString side);

	QString _xmlTypeValue; /* Needs set by subclasses!*/
	screen_struct* _screenObj;

	IndexedPalette* _pal;

	QString _name;
	QString _palName;
	int _outlineWidth;

	bool _bRandTint;
	bool _bUseComplement;
	bool _bUseTint;
	bool _bUseOutlineComplement;
	bool _bUseOutlineTint;
	int _lastColorIndex;

	float _minSpeed;
	float _maxSpeed;
	bool _bRandSpeed;

	float _minAngle;
	float _maxAngle;
	bool _bRandAngle;

	side_type _sides[4];
	int _lastSide;
	int _numSides;
	bool _bRandSide;

	bool _bInitialized;
};

#endif
