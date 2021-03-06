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

#include "BackgroundProfile.h"

/* Attempts to load this background profile object from the
 * specified QDomNode.
 */
BackgroundProfile* BackgroundProfile::load(QDomNode &node) {
	//nada
	return NULL;
}

/*
 * Returns a QDomNode object that represents this profile.
 */
QDomNode BackgroundProfile::save(QDomDocument* doc) {
	//nada
	QDomNode nada;
	return nada;
}

/*
 * Creates and returns a new background object.
 */
Background* BackgroundProfile::getNewBackgroundObj(int height, int width, QHash<QString, IndexedPaletteProfile*>* palHash) {
	//nada
	return NULL;
}

/*
 * Get/set the name of this profile.
 */
void BackgroundProfile::setName(QString &name) {
	_name = name;
}

QString BackgroundProfile::getName() {
	return _name;
}

/*
 * Returns the tag name used by Dom elements for all
 * subclasses of BackgroundProfile.
 */
QString BackgroundProfile::getXMLTagName() {
	return QString("BackgroundProfile");
}

/*
 * Returns the type of BackgroundProfile.
 */
QString BackgroundProfile::getBackgroundType() {
	return _xmlTypeValue;
}

/*
 * Creates a new object with this object's settings.
 */
BackgroundProfile* BackgroundProfile::clone() {
	return new BackgroundProfile();
}

	/**
	 * Called whenever palette names change
	 */
void BackgroundProfile::paletteNameChanged(QString oldName, QString newName) {
  // nada
}
	
	/**
	 * Called whenever a palette is removed.
	 */
void BackgroundProfile::paletteRemoved(QString palName) {
  // nada
}

