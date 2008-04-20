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
 * This class is the base class for the indexed palette editor
 * widgets.  Implementing classes aren't actually widgets, but
 * must return a suitable widget.
 */

#ifndef __INDEXEDPALETTEEDITORTABLEWIDGET_H__
#define __INDEXEDPALETTEEDITORTABLEWIDGET_H__

#include <QTableWidget>
#include <QMenu>
#include <QColor>
#include <QStringList>
#include <QAction>

#include "IndexedPaletteEditorWidget.h"

class IndexedPaletteEditorTableWidget : public IndexedPaletteEditorWidget {

	Q_OBJECT

public:
	IndexedPaletteEditorTableWidget(int width=0, int height=0);
	~IndexedPaletteEditorTableWidget();

	QRgb getColor(int x, int y);
	void setColor(int x, int y, QRgb color);
	void clearColor(int column, int row);

	// resizes the image.  Steps the progress dialog height+2 times.
	// clears out all data held within the widget.
	void changeSize(int width=0, int height=0);

	// returns the actual widget.  Calling class should NOT destroy widget.
	QWidget* getWidget();

public slots:
	void deleteTriggered();
	void insertTriggered();
	void editTriggered();
	void copyTriggered();
	void pasteTriggered();

	void cellClicked(int row, int column);

protected:
	// inserts a column of empty cells of the specified width at the specified X coords.
	void insertEmptyColumn(int tableX, int palX, int width);
	// inserts a row of empty cells of the specified height at the specified Y coord.
	void insertEmptyRow(int tableY, int palY, int height);
	// changes the width of the specified column
	void changeColumnWidth(int tableX, int newWidth);
	// changes the height of the specified row
	void changeRowHeight(int tableY, int newHeight);

	// returns a string formatted for use in the headers
	QString formatHeaderStr(int start, int length=1);

	QTableWidget* _widget;

	QColor _defaultColor;

	bool _bClipboardValid;
	QRgb _clipboard;
	QAction* _definedPasteAction;
	QAction* _emptyPasteAction;

	QAction* _emptyEditAction;

	QStringList _xHeader;
	QStringList _yHeader;

	QMenu _definedMenu;
	QMenu _emptyMenu;

	int _selRow;
	int _selCol;
};

#endif
