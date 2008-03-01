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
 * This class represents a full configuration.  It contains the name of the background
 * profile, the lens profile (or collection), screen dimensions, and the timer rate.
 *
 * @author jeff backus
 */

#ifndef __MASTERPROFILE_H__
#define __MASTERPROFILE_H__

#include <QString>
#include <QtXml/QDomNode>
#include <QtXml/QDomDocument>

class MasterProfile {

public:

	MasterProfile();
	~MasterProfile();

	/*
	 * Attempts to load this background profile object from the
	 * specified QDomNode.
	 */
	static MasterProfile* load(QDomNode &node);

	/*
	 * Returns a QDomNode object that represents this profile.
	 */
	QDomNode save(QDomDocument* doc);

	/*
	 * Get/set the name of this profile.
	 */
	void setName(QString &name);
	QString getName();

	/*
	 * Returns the tag name used by Dom elements for all
	 * subclasses of BackgroundProfile.
	 */
	static QString getXMLTagName();

	//*** Begin Screen Related ***
	int getScreenWidth();
	int getScreenHeight();
	int getTextureWidth();
	int getTextureHeight();

	void setScreenWidth(int width);
	void setScreenHeight(int height);
	//*** End Screen Related

	QString getBackgroundProfileName();
	void setBackgroundProfileName(QString name);

	QString getLensProfileName();
	void setLensProfileName(QString name);

	int getTimerMillis();
	void setTimerMillis(int millis);

	/*
	 * Overloaded assignment operator
	 */
	MasterProfile& operator=(MasterProfile& other);

private:

	QString _name;

	int _screenWidth;
	int _screenHeight;

	QString _backgroundProfileName;
	QString _lensProfileName;

	int _timerMillis;
};

#endif