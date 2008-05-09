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

#include <QApplication>
#include <QPoint>
#include <QRect>
#include <QPixmap>
#include <QColorDialog>

#include <QApplication>
#include <QAction>

#include "IndexedPaletteEditorTableWidget.h"
#include "IndexedPaletteEditorTableWidgetItem.h"
#include "IndexedPaletteEditorTableWidgetSplitDlg.h"

IndexedPaletteEditorTableWidget::IndexedPaletteEditorTableWidget(int width, int height) {
	_widget = NULL;
	_emptyEditAction = NULL;
	_definedPasteAction = NULL;
	_emptyPasteAction = NULL;
	_bClipboardValid = false;
	_defaultColor = QColor(0,0,0,255);
	_bWasRightClick = false;

	// create widget object
	_widget = new QTableWidget(1,1);
	connect(_widget, SIGNAL(cellClicked(int, int)), this, SLOT(cellClicked(int, int)));
	connect(_widget, SIGNAL(cellDoubleClicked(int, int)), this, SLOT(cellDoubleClicked(int, int)));
	connect(_widget, SIGNAL(cellPressed(int, int)), this, SLOT(cellPressed(int, int)));

	QAction* tmpAction = NULL;
	_definedMenu.clear();
	_emptyMenu.clear();

	// insert cell
	_emptyInsertAction = _emptyMenu.addAction("Insert");
	connect(_emptyInsertAction, SIGNAL(triggered()), this, SLOT(insertTriggered()));

	// edit color
	tmpAction = _definedMenu.addAction("Edit");
	_definedMenu.setDefaultAction(tmpAction);
	connect(tmpAction, SIGNAL(triggered()), this, SLOT(editTriggered()));
	_emptyEditAction = _emptyMenu.addAction("Edit");
	connect(_emptyEditAction, SIGNAL(triggered()), this, SLOT(editTriggered()));
	_emptyEditAction->setEnabled(false);
	// clear color
	tmpAction = _definedMenu.addAction("Delete");
	connect(tmpAction, SIGNAL(triggered()), this, SLOT(deleteTriggered()));
	// copy color
	tmpAction = _definedMenu.addAction("Copy");
	connect(tmpAction, SIGNAL(triggered()), this, SLOT(copyTriggered()));
	// paste color
	_definedPasteAction = _definedMenu.addAction("Paste");
	connect(_definedPasteAction, SIGNAL(triggered()), this, SLOT(pasteTriggered()));
	_definedPasteAction->setEnabled(false);
	_emptyPasteAction = _emptyMenu.addAction("Paste");
	connect(_emptyPasteAction, SIGNAL(triggered()), this, SLOT(pasteTriggered()));
	_emptyPasteAction->setEnabled(false);

	changeSize(width, height);
}

IndexedPaletteEditorTableWidget::~IndexedPaletteEditorTableWidget() {
	
	if(_widget != NULL) {
		delete _widget;
		_widget = NULL;
	}
}

QRgb IndexedPaletteEditorTableWidget::getColor(int x, int y) {
	// if we can't find the color, or the list isn't initialized, return black.
	if(_widget == NULL)
		return _defaultColor.rgb();

	// first search down the columns to find one that matches X
	IndexedPaletteEditorTableWidgetItem* listItem;
	bool bDone = false;
	int c = 0;
	int r = 0;
	int count = _widget->columnCount();
	while(!bDone) {
		listItem = (IndexedPaletteEditorTableWidgetItem*)_widget->item(r,c);
		if(listItem == NULL)
			return _defaultColor.rgb();

		if(listItem->getX() == x)
			bDone = true;
		else if(listItem->getX() > x)
			return _defaultColor.rgb();
		else
			c++;
		if(c >= count)
			bDone = true;
	}

	bDone = false;
	count = _widget->rowCount();
	while(!bDone) {
		listItem = (IndexedPaletteEditorTableWidgetItem*)_widget->item(r,c);
		if(listItem == NULL)
			return _defaultColor.rgb();

		if(listItem->getY() == y) 
			return listItem->getColor();
		else if(listItem->getY() > y)
			return _defaultColor.rgb();
		else
			r++;
		if(r >= count)
			bDone = true;
	}

	// item not found.  return default.
	return _defaultColor.rgb();
}

void IndexedPaletteEditorTableWidget::setColor(int x, int y, QRgb color) {
	if(_widget == NULL)
		return;
	// check coordinates
	if(x >= _width || y >= _height)
		return;

	// first, search in the X direction
	bool bDone = false;
	IndexedPaletteEditorTableWidgetItem* listItem = NULL;
	int ix = 0;
	int iy = 0;
	int count = _widget->columnCount();
	while(!bDone) {
		listItem = (IndexedPaletteEditorTableWidgetItem*)_widget->item(iy,ix);
		int tmpX = listItem->getX();
		int width = listItem->getWidth();
		if(tmpX == x) {
			// we've found the correct column.  Check the width
			// to see if we need to add a new column after this one
			if(width > 1) {
				changeColumnWidth(ix,1);
				insertEmptyColumn(ix+1, tmpX+1, width-1);
				_xHeader.replace(ix, formatHeaderStr(tmpX));
			}
			bDone = true;
		} else if(tmpX < x && tmpX + width > x) {
			int leftWidth = x - tmpX;
			int rightWidth = tmpX + width - x - 1;

			if(leftWidth > 0) {
				// if we still have a column to the left,
				// adjust the widths of all items in the column
				// and insert a new one.
				changeColumnWidth(ix,leftWidth);
				_xHeader.replace(ix, formatHeaderStr(tmpX, leftWidth));
				tmpX=x;
				ix++;
				insertEmptyColumn(ix, tmpX, 1);
			}

			if(rightWidth > 0) {
				// if we have a column to the right, insert it.
				insertEmptyColumn(ix+1, tmpX+1, rightWidth);
			}

			bDone = true;
		} else {
			ix++;
			if(ix >= count)
				bDone = true;
		}
	}

	// now, go down the row looking for the right Y value
	count = _widget->rowCount();
	bDone = false;
	while(!bDone) {
		listItem = (IndexedPaletteEditorTableWidgetItem*)_widget->item(iy,ix);
		int tmpY = listItem->getY();
		int height = listItem->getHeight();
		if(tmpY == y) {
			// we've found the correct row.

			// check the height.  If it is > 1, insert an empty row
			if(height > 1) {
				changeRowHeight(iy,1);
				insertEmptyRow(iy+1, tmpY+1, height-1);
				_yHeader.replace(iy, formatHeaderStr(tmpY));
			}
			
			// Update the color of this cell with the new color
			listItem->setColor(color);
			listItem->setEmpty(false);
			bDone = true;
		} else if(tmpY < y && tmpY + height > y) {
			int topHeight = y - tmpY;
			int bottomHeight = tmpY + height - y - 1;

			if(topHeight > 0) {
				// if we still have a row to the left,
				// adjust the heights of all items in the row
				// insert a new one, and set the color of the
				// correct cell.
				changeRowHeight(iy,topHeight);
				_yHeader.replace(iy, formatHeaderStr(tmpY, topHeight));
				tmpY=y;
				iy++;
				insertEmptyRow(iy, tmpY, 1);
				listItem = (IndexedPaletteEditorTableWidgetItem*)_widget->item(iy,ix);
				listItem->setColor(color);
				listItem->setEmpty(false);
			}

			if(bottomHeight > 0) {
				// if we have a row underneath, insert it.
				insertEmptyRow(iy+1, tmpY+1, bottomHeight);
			}
			
			bDone = true;
		} else {
			iy++;
			if(iy >= count)
				bDone = true;
		}
	}
	_widget->setHorizontalHeaderLabels(_xHeader);
	_widget->setVerticalHeaderLabels(_yHeader);	
	listItem = NULL;
}

// resizes the image.  Steps the progress dialog height+2 times.
// clears out all data held within the widget.
void IndexedPaletteEditorTableWidget::changeSize(int width, int height) {

	if(width < 1 || height < 1)
		return;

	_width = width;
	_height = height;

	if(_widget == NULL) {
		if(_widget == NULL)
			return;
	} else {
		// clear the widget otherwise
		_widget->clear();
	}

	_xHeader.clear();
	_xHeader.append(formatHeaderStr(0,_width));
	_widget->setHorizontalHeaderLabels(_xHeader);
	_yHeader.clear();
	_yHeader.append(formatHeaderStr(0,_height));
	_widget->setVerticalHeaderLabels(_yHeader);

	IndexedPaletteEditorTableWidgetItem* tmpItem = new IndexedPaletteEditorTableWidgetItem();
	if(tmpItem != NULL) {
		tmpItem->setValue(_defaultColor.rgb(), 0, 0, _width, _height, true);
		_widget->setItem(0,0,tmpItem);
		tmpItem = NULL;
	}

	// this doesn't take that long...
	stepProgress(height+2);
}

void IndexedPaletteEditorTableWidget::clearColor(int column, int row) {
	if(_widget == NULL)
		return;

	IndexedPaletteEditorTableWidgetItem* listItem = (IndexedPaletteEditorTableWidgetItem*)_widget->item(row, column);
	if(listItem == NULL)
		return;

	int x = listItem->getX();
	int y = listItem->getY();
	listItem->setEmpty(true);
	emit colorChanged(x, y, NULL);
}

// returns the actual widget.  Calling class should NOT destroy widget.
QWidget* IndexedPaletteEditorTableWidget::getWidget() {
	return 	_widget;
}

// inserts a column of empty cells of the specified width at the specified X coord.
void IndexedPaletteEditorTableWidget::insertEmptyColumn(int tableX, int palX, int width) {
	if(_widget == NULL)
		return;
	if(width < 1)
		return;
	_widget->insertColumn(tableX);
	_xHeader.insert(tableX, formatHeaderStr(palX, width));
	for(int y = 0; y<_widget->rowCount(); y++) {
		IndexedPaletteEditorTableWidgetItem* listItem = (IndexedPaletteEditorTableWidgetItem*)_widget->item(y,0);
		if(listItem != NULL) {
			int palY = listItem->getY();
			int height = listItem->getHeight();
			listItem = new IndexedPaletteEditorTableWidgetItem();
			if(listItem != NULL) {
				listItem->setValue(_defaultColor.rgb(), palX, palY, width, height, true);
				_widget->setItem(y,tableX,listItem);
			}
		}
	}
}

// inserts a row of empty cells of the specified height at the specified Y coord.
void IndexedPaletteEditorTableWidget::insertEmptyRow(int tableY, int palY, int height) {
	if(_widget == NULL)
		return;
	if(height < 1) 
		return;
	_widget->insertRow(tableY);
	_yHeader.insert(tableY, formatHeaderStr(palY, height));
	for(int x = 0; x<_widget->columnCount(); x++) {
		IndexedPaletteEditorTableWidgetItem* listItem = (IndexedPaletteEditorTableWidgetItem*)_widget->item(0,x);
		if(listItem != NULL) {
			int palX = listItem->getX();
			int width = listItem->getWidth();
			listItem = new IndexedPaletteEditorTableWidgetItem();
			if(listItem != NULL) {
				listItem->setValue(_defaultColor.rgb(), palX, palY, width, height, true);
				_widget->setItem(tableY,x,listItem);
			}
		}
	}
}

// returns a string formatted for use in the headers
QString IndexedPaletteEditorTableWidget::formatHeaderStr(int start, int length) {
	QString retVal(QString::number(start));
	if(length > 1) {
		retVal.append("..");
		retVal.append(QString::number(start+length-1));
	}
	return retVal;
}
void IndexedPaletteEditorTableWidget::deleteTriggered() {
	clearColor(_selCol, _selRow);
}
void IndexedPaletteEditorTableWidget::editTriggered() {
	if(_widget == NULL)
		return;
	IndexedPaletteEditorTableWidgetItem* listItem = (IndexedPaletteEditorTableWidgetItem*)_widget->item(_selRow, _selCol);
	if(listItem == NULL)
		return;

	bool bOk = false;
	int x = listItem->getX();
	int y = listItem->getY();
	QRgb oldColor = listItem->getColor();
	QRgb newColor = QColorDialog::getRgba(oldColor, &bOk, NULL);
	if(bOk) {
		setColor(x, y, newColor);
		emit colorChanged(x, y, &newColor);
	}
}
void IndexedPaletteEditorTableWidget::insertTriggered() {
	if(_widget == NULL)
		return;
	IndexedPaletteEditorTableWidgetItem* listItem = (IndexedPaletteEditorTableWidgetItem*)_widget->item(_selRow, _selCol);
	if(listItem == NULL)
		return;

	int startX = listItem->getX();
	int width = listItem->getWidth();
	int startY = listItem->getY();
	int height = listItem->getHeight();
	IndexedPaletteEditorTableWidgetSplitDlg dlg(startX, startX+width-1, startY, startY+height-1);
	if(dlg.exec() == QDialog::Rejected)
		return;

	int retX = dlg.getX();
	int retY = dlg.getY();

	// attempt to insert new rows and columns where necessary as well
	// as adjust the currently selected column and row dimensions
	if(retX >= startX && retY >= startY) {
		if(retX == startX) {
			// inserting at beginning of consolidated column.
			// adjust width to 1 and insert a new column afterward
			changeColumnWidth(_selCol,1);
			_xHeader.replace(_selCol, formatHeaderStr(startX, 1));
			insertEmptyColumn(_selCol+1, retX+1, width-1);
		} else {
			// otherwise, adjust original column width,
			// insert a new column of width 1 for the selected column,
			// then check to see if a new column needs added to account
			// for the rest of the width.
			changeColumnWidth(_selCol,retX-startX);
			_xHeader.replace(_selCol, formatHeaderStr(startX, retX-startX));
			insertEmptyColumn(_selCol+1, retX, 1);
			width = width - (retX-startX) - 1;
			if(width > 0)
				insertEmptyColumn(_selCol+2, retX+1, width);
		}

		if(retY == startY) {
			// inserting at beginning of consolidated column.
			// adjust width to 1 and insert a new column afterward
			changeRowHeight(_selRow,1);
			_yHeader.replace(_selRow, formatHeaderStr(startY, 1));
			insertEmptyRow(_selRow+1, retY+1, height-1);
		} else {
			// otherwise, adjust original column width,
			// insert a new column of width 1 for the selected column,
			// then check to see if a new column needs added to account
			// for the rest of the width.
			changeRowHeight(_selRow,retY-startY);
			_yHeader.replace(_selRow, formatHeaderStr(startY, retY-startY));
			insertEmptyRow(_selRow+1, retY, 1);
			height = height - (retY-startY) - 1;
			if(height > 0)
				insertEmptyRow(_selRow+2, retY+1, height);
		}
		_widget->setHorizontalHeaderLabels(_xHeader);
		_widget->setVerticalHeaderLabels(_yHeader);
	}
}
void IndexedPaletteEditorTableWidget::cellPressed(int row, int column) {
	// check to see if this was a right-click or not and store it.
	_bWasRightClick = (QApplication::mouseButtons() == Qt::RightButton);
}
void IndexedPaletteEditorTableWidget::cellDoubleClicked(int row, int column) {
	if(_widget == NULL)
		return;

	// double-clicking will perform the default action

	// examine the cell that was clicked.  If it is empty, insert,
	// otherwise change color.
	IndexedPaletteEditorTableWidgetItem* listItem = (IndexedPaletteEditorTableWidgetItem*)_widget->item(row, column);
	if(listItem == NULL)
		return;

	_selRow = row;
	_selCol = column;
	// get center of cell to pop up window
	QRect rect = _widget->visualItemRect(listItem);
	QPoint p = _widget->mapToGlobal(rect.center());
	if(listItem->getEmpty() && !(listItem->getWidth() == 1 && listItem->getHeight() == 1)) {
		insertTriggered();
	} else {
		editTriggered();
	}
}
void IndexedPaletteEditorTableWidget::cellClicked(int row, int column) {
	if(_widget == NULL)
		return;

	// if not a right-click, return.
	if(!_bWasRightClick)
		return;

	// otherwise, display the appropriate menu.

	// examine the cell that was clicked.  If it is empty, display the empty menu,
	// otherwise display the defined menu.
	IndexedPaletteEditorTableWidgetItem* listItem = (IndexedPaletteEditorTableWidgetItem*)_widget->item(row, column);
	if(listItem == NULL)
		return;

	_selRow = row;
	_selCol = column;
	// get center of cell to pop up window
	QRect rect = _widget->visualItemRect(listItem);
	QPoint p = _widget->mapToGlobal(rect.center());
	if(listItem->getEmpty()) {
		if(listItem->getWidth() == 1 && listItem->getHeight() == 1) {
			_emptyEditAction->setEnabled(true);
			_emptyInsertAction->setEnabled(false);
			_emptyMenu.setDefaultAction(_emptyEditAction);
		} else {
			_emptyEditAction->setEnabled(false);
			_emptyInsertAction->setEnabled(true);
			_emptyMenu.setDefaultAction(_emptyInsertAction);
		}
		_emptyPasteAction->setEnabled(_bClipboardValid && _emptyEditAction->isEnabled());
		_emptyMenu.popup(p);
	} else {
		_definedMenu.popup(p);
	}

	// go ahead and clear right click
	_bWasRightClick = false;
}

void IndexedPaletteEditorTableWidget::copyTriggered() {
	if(_widget == NULL || _emptyPasteAction == NULL || _definedPasteAction == NULL)
		return;
	IndexedPaletteEditorTableWidgetItem* listItem = (IndexedPaletteEditorTableWidgetItem*)_widget->item(_selRow, _selCol);
	if(listItem == NULL)
		return;

	_bClipboardValid = true;
	_clipboard = listItem->getColor();
	_definedPasteAction->setEnabled(true);
	_emptyPasteAction->setEnabled(_emptyEditAction->isEnabled());
}

void IndexedPaletteEditorTableWidget::pasteTriggered() {
	if(_widget == NULL)
		return;
	IndexedPaletteEditorTableWidgetItem* listItem = (IndexedPaletteEditorTableWidgetItem*)_widget->item(_selRow, _selCol);
	if(listItem == NULL)
		return;

	if(_bClipboardValid) {
		int x = listItem->getX();
		int y = listItem->getY();
		listItem->setColor(_clipboard);
		listItem->setEmpty(false);
		emit colorChanged(x, y, &_clipboard);
	}
}

// changes the width of the specified column
void IndexedPaletteEditorTableWidget::changeColumnWidth(int tableX, int newWidth) {
	if(_widget == NULL)
		return;
	for(int k = 0; k < _widget->rowCount(); k++) {
		IndexedPaletteEditorTableWidgetItem* tmpItem = 
			(IndexedPaletteEditorTableWidgetItem*)_widget->item(k,tableX);
		if(tmpItem != NULL)
			tmpItem->setWidth(newWidth);
	}
}

// changes the height of the specified row
void IndexedPaletteEditorTableWidget::changeRowHeight(int tableY, int newHeight) {
	if(_widget == NULL)
		return;
	for(int k = 0; k < _widget->columnCount(); k++) {
		IndexedPaletteEditorTableWidgetItem* tmpItem = 
			(IndexedPaletteEditorTableWidgetItem*)_widget->item(tableY,k);
		if(tmpItem != NULL)
			tmpItem->setHeight(newHeight);
	}
}
