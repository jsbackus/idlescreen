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

#include <QIcon>
#include <QPixmap>

#include "IndexedPaletteEditorTableWidgetItem.h"

IndexedPaletteEditorTableWidgetItem::IndexedPaletteEditorTableWidgetItem(const QRgb* color, const int x, const int y){
	QTableWidgetItem(UserType);

	// see Qt::ItemFlags for more info
	setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

	_definedBGColor = QColor(226,225,209,255);
	_undefinedBGColor = QColor(213, 208, 193,255);

	_bEmpty = true;
	_width = 1;
	_height = 1;
	_x = 0;
	_y = 0;

	if(color != NULL) {
		_color = *color;
		_x = x;
		_y = y;
	}

	convData();
}

IndexedPaletteEditorTableWidgetItem::~IndexedPaletteEditorTableWidgetItem() {
	// nada
}

// Get/set the color
QRgb IndexedPaletteEditorTableWidgetItem::getColor() {
	return _color;
}

void IndexedPaletteEditorTableWidgetItem::setColor(const QRgb& color) {
	setValue(color, _x, _y, _width, _height, _bEmpty);
}

// get/set x/y value
int IndexedPaletteEditorTableWidgetItem::getX() {
	return _x;
}
int IndexedPaletteEditorTableWidgetItem::getY() {
	return _y;
}
void IndexedPaletteEditorTableWidgetItem::setX(const int x) {
	setValue(_color, x, _y, _width, _height, _bEmpty);
}
void IndexedPaletteEditorTableWidgetItem::setY(const int y) {
	setValue(_color, _x, y, _width, _height, _bEmpty);
}
void IndexedPaletteEditorTableWidgetItem::setCoords(const int x, const int y) {
	setValue(_color, x, y, _width, _height, _bEmpty);
}

int IndexedPaletteEditorTableWidgetItem::getWidth() {
	return _width;
}

int IndexedPaletteEditorTableWidgetItem::getHeight() {
	return _height;
}

bool IndexedPaletteEditorTableWidgetItem::getEmpty() {
	return _bEmpty;
}
void IndexedPaletteEditorTableWidgetItem::setWidth(int width) {
	setValue(_color, _x, _y, width, _height, _bEmpty);
}
void IndexedPaletteEditorTableWidgetItem::setHeight(int height) {
	setValue(_color, _x, _y, _width, height, _bEmpty);
}
void IndexedPaletteEditorTableWidgetItem::setEmpty(bool bEmpty) {
	setValue(_color, _x, _y, _width, _height, bEmpty);
}

// set everything
void IndexedPaletteEditorTableWidgetItem::setValue(const QRgb& color, const int x, const int y, const int width, const int height, const bool bEmpty) {
	_color = color;
	_x = x;
	_y = y;
	_width = width;
	_height = height;
	_bEmpty = bEmpty;
	convData();
}

/*
 * Overloaded operators.  Comparison operators only compare
 * coordinate values, such that items can be ordered by Y:X
 */
IndexedPaletteEditorTableWidgetItem& IndexedPaletteEditorTableWidgetItem::operator=(const IndexedPaletteEditorTableWidgetItem& other) {
	_color = other._color;
	_x = other._x;
	_y = other._y;
	convData();

	return *this;
}
bool IndexedPaletteEditorTableWidgetItem::operator<(const IndexedPaletteEditorTableWidgetItem& right) {
	return (_y > right._y || ( _y == right._y && _x < right._x));
}
bool IndexedPaletteEditorTableWidgetItem::operator>(const IndexedPaletteEditorTableWidgetItem& right) {
	return (_y > right._y || ( _y == right._y && _x > right._x));
}
bool IndexedPaletteEditorTableWidgetItem::operator<=(const IndexedPaletteEditorTableWidgetItem& right) {
	return (_y < right._y || ( _y == right._y && _x <= right._x));
}
bool IndexedPaletteEditorTableWidgetItem::operator>=(const IndexedPaletteEditorTableWidgetItem& right) {
	return (_y > right._y || ( _y == right._y && _x >= right._x));
}

bool IndexedPaletteEditorTableWidgetItem::operator==(const IndexedPaletteEditorTableWidgetItem& right) {
	return (_x == right._x && _y == right._y);
}
bool IndexedPaletteEditorTableWidgetItem::operator!=(const IndexedPaletteEditorTableWidgetItem& right) {
	return !(*this == right);
}

// Populates base class data structures
void IndexedPaletteEditorTableWidgetItem::convData() {
	QString text;
	if(_bEmpty) {
		text.append("Undefined (");
		text.append(QString::number(_x));
		if(_width > 1) {
			text.append("..");
			text.append(QString::number(_x+_width-1));
		}
		text.append(",");
		text.append(QString::number(_y));
		if(_height > 1) {
			text.append("..");
			text.append(QString::number(_y+_height-1));
		}
		QPixmap pixmap(ICON_WIDTH,ICON_HEIGHT);
		pixmap.fill(_undefinedBGColor);
		QIcon icon(pixmap);
		setIcon(icon);
		setBackground(QBrush(_undefinedBGColor));
	} else {
		QPixmap pixmap(ICON_WIDTH,ICON_HEIGHT);
		pixmap.fill(_color);
		QIcon icon(pixmap);
		setIcon(icon);
		setBackground(QBrush(_definedBGColor));

		text.append("(");
		text.append(QString::number(_x));
		text.append(",");
		text.append(QString::number(_y));		
	}
	text.append(")");
	setText(text);
}
