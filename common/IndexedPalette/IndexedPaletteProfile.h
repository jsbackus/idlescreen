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
 * This object is used to load/store palettes as well as create IndexedPalette objects
 * when needed.
 *
 * @author jeff backus
 * @date 9/15/2007
 */

#ifndef __INDEXEDPALETTEPROFILE_H__
#define __INDEXEDPALETTEPROFILE_H__

#ifdef _M_IX86
#include <windows.h>
#else
#include <stream.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#include <QString>
#include <QList>
#include <QtXml/QDomNode>
#include <QtXml/QDomDocument>
#include <QColor>

#include "IndexedPalette.h"

struct color_data_struct {

	GLubyte r, g, b, a;
	int x, y;
};

class IndexedPaletteProfile {

public:
	IndexedPaletteProfile();

	/* Attempts to load this IndexedPaletteProfile object from the
	 * specified QDomNode.
	 */
	static IndexedPaletteProfile* load(QDomNode &node);

	/*
	 * Returns a QDomNode object that represents this palette.
	 */
	QDomNode save(QDomDocument* doc);

	/*
	 * Creates and returns a new IndexedPalette based on this palette.
	 */
	IndexedPalette* createPalette();

	/*
	 * Get/set the name of this profile.
	 */
	void setName(QString &name);
	QString getName();

	/*
	 * Get/set the specified color.  If the color has been defined,
	 * the default color will be returned.
	 */
	void setColor(int x, int y, int r, int g, int b, int a);
	void setColor(int x, int y, QRgb color);
	void getColor(int x, int y, int* r, int* g, int* b, int* a);
	QRgb getColor(int x, int y);

	/*
	 * Used to delete/undefine a color.
	 */
	void deleteColor(int x, int y);

	/*
	 * Whether the specified color has been previously defined.
	 */
	bool isColorDefined(int x, int y);

	/*
	 * Get/set default color.
	 */
	void setDefaultColor(int r, int g, int b);
	void setDefaultColor(QRgb color);
	void getDefaultColor(int* r, int* g, int* b);
	QRgb getDefaultColor(void);

	/*
	 * Get/set width and height
	 */
	void setWidth(int width);
	int getWidth();

	void setHeight(int height);
	int getHeight();

	/*
	 * Get/set interpolate colors
	 */
	void setInterpolateColors(bool bInterpolateColors);
	bool getInterpolateColors();

	/*
	 * get/set wrap colors
	 */
	void setWrapColors(bool bWrapColors);
	bool getWrapColors();

	/*
	 * Returns the tag name used by Dom elements.
	 */
	static QString getXMLTagName();

	/*
	 * Overloaded assignment operator
	 */
	IndexedPaletteProfile& operator=(IndexedPaletteProfile& other);

private:
	QList<color_data_struct> _colorList;
	QString _name;
	bool _bInterpolateColors;
	bool _bWrapColors;
	int _width;
	int _height;
	GLubyte _defaultColor[3];
};

#endif