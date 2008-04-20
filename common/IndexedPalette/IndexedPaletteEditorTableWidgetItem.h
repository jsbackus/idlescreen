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
 * This class extends TableWidgetItem to add functionality
 * used by IndexedPaletteTableWidget.
 */

#ifndef __INDEXEDPALETTEEDITORTableWidgetITEM_H__
#define __INDEXEDPALETTEEDITORTableWidgetITEM_H__

#include <QTableWidgetItem>
#include <QColor>

class IndexedPaletteEditorTableWidgetItem : public QTableWidgetItem {

public:
	IndexedPaletteEditorTableWidgetItem(const QRgb* color=NULL, const int x=0, const int y=0);
	~IndexedPaletteEditorTableWidgetItem();

	// Get/set the color
	QRgb getColor();
	void setColor(const QRgb& color);

	// get/set x/y value
	int getX();
	int getY();
	void setX(const int x);
	void setY(const int y);
	void setCoords(const int x, const int y);

	// height, width, empty?
	int getWidth();
	void setWidth(int width);
	int getHeight();
	void setHeight(int height);
	bool getEmpty();
	void setEmpty(bool bEmpty);

	// set everything
	void setValue(const QRgb& color, const int x, const int y, const int width, const int height, const bool bEmpty);

	/*
	 * Overloaded operators.  Comparison operators only compare
	 * coordinate values, such that items can be ordered by Y:X
	 */
	IndexedPaletteEditorTableWidgetItem& operator=(const IndexedPaletteEditorTableWidgetItem& other);
	bool operator<(const IndexedPaletteEditorTableWidgetItem& right);
	bool operator>(const IndexedPaletteEditorTableWidgetItem& right);
	bool operator<=(const IndexedPaletteEditorTableWidgetItem& right);
	bool operator>=(const IndexedPaletteEditorTableWidgetItem& right);
	bool operator==(const IndexedPaletteEditorTableWidgetItem& right);
	bool operator!=(const IndexedPaletteEditorTableWidgetItem& right);

protected:
	// Populates base class data structures
	void convData();

	QRgb _color;
	int _x;
	int _y;

	// used for consolidated empty entries
	bool _bEmpty;
	int _width;
	int _height;
	
	// constants
	static const ICON_WIDTH = 32;
	static const ICON_HEIGHT = 32;

	QColor _definedBGColor;
	QColor _undefinedBGColor;

};
#endif
