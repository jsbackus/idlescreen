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
 * This background profile is responsible for setting up and returning a new 
 * PlasmaFractal background object.
 *
 * @author jeff backus
 * @date 09/01/2007
 */

#ifndef __PLASMAFRACTALBACKGROUNDPROFILE_H__
#define __PLASMAFRACTALBACKGROUNDPROFILE_H__

#include <QString>
#include <QtXml/QDomNode>
#include <QtXml/QDomDocument>

#include "../../common/2d_bgnd_w_lens/BackgroundProfile.h"

class PlasmaFractalBackgroundProfile : public BackgroundProfile {

public:
	PlasmaFractalBackgroundProfile();
	~PlasmaFractalBackgroundProfile();

	/* Attempts to load this background profile object from the
	 * specified QDomNode.
	 */
	BackgroundProfile* load(QDomNode &node);

	/*
	 * Returns a QDomNode object that represents this profile.
	 */
	QDomNode save(QDomDocument* doc);

	/*
	 * Creates and returns a new background object.
	 */
	Background* getNewBackgroundObj(int height, int width, QHash<QString, IndexedPaletteProfile*>* palHash);

	/*
	 * get/set palette name.
	 */
	QString getPaletteName();
	void setPaletteName(QString paletteName);

	/*
	 * get/set coarseness.
	 */
	float getCoarseness();
	void setCoarseness(float coarseness);

	/*
	 * get/set steps per tick
	 */
	int getGenStepsPerTick();
	void setGenStepsPerTick(int genStepsPerTick);

	/*
	 * Get/set animate palette.
	 */
	bool getAnimatePalette();
	void setAnimatePalette(bool bAnimatePalette);

	/*
	 * get/set palette x speed.
	 */
	float getPaletteXSpeed();
	void setPaletteXSpeed(float speed);

	/*
	 * get/set palette y speed.
	 */
	float getPaletteYSpeed();
	void setPaletteYSpeed(float speed);

	/*
	 * Get/set clamp color index.
	 */
	bool getClampColorIndex();
	void setClampColorIndex(bool bClampColorIndex);

	/*
	 * Overloaded assignment operator.
	 */
	PlasmaFractalBackgroundProfile& operator=(PlasmaFractalBackgroundProfile& other);

	/*
	 * Creates a new object with this object's settings.
	 */
	BackgroundProfile* clone();

	/**
	 * Called whenever palette names change
	 */
	virtual void paletteNameChanged(QString oldName, QString newName);
	
	/**
	 * Called whenever a palette is removed.
	 */
	virtual void paletteRemoved(QString palName);

private:
	QString _palName;
	float _coarseness;
	int _genStepsPerTick;
	bool _bAnimatePalette;
	float _paletteXSpeed;
	float _paletteYSpeed;
	bool _bClampColorIndex;
};
#endif
