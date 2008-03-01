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

#include <QPixmap>
#include <QColorDialog>

#include <QApplication>

#include "IndexedPaletteEditorWidget.h"

IndexedPaletteEditorWidget::IndexedPaletteEditorWidget(int width, int height, QWidget* parent, Qt::WindowFlags f) {

	_prgDlg = NULL;

	//define "constants"
	_colorWidth = 8;
	_colorHeight = 8;
	_innerBorderThickness = 1;
	_outerBorderThickness = 2;
	_innerBorderColor = qRgba(0, 0, 0, 255);
	_outerBorderColor = qRgba(213, 208, 193, 255);
	_undefinedColor = qRgba(213, 208, 193, 0);
	_cellHeight = _outerBorderThickness*2+_innerBorderThickness*2+_colorHeight;
	_cellWidth = _outerBorderThickness*2+_innerBorderThickness*2+_colorWidth;

	_clickedX = -1;
	_clickedY = -1;

	changeSize(width, height);
}

IndexedPaletteEditorWidget::~IndexedPaletteEditorWidget() {
	_prgDlg = NULL;
}

QRgb IndexedPaletteEditorWidget::getColor(int x, int y) {
	return _image.pixel(x*_cellWidth+_outerBorderThickness*1+_innerBorderThickness*+1,
						y*_cellHeight+_outerBorderThickness*1+_innerBorderThickness*+1);
}

void IndexedPaletteEditorWidget::setColor(int x, int y, QRgb color) {
	drawCell(x, y, color);
	updateImage();
}

void IndexedPaletteEditorWidget::drawCell(int x, int y, QRgb color) {
	int i,j;
	int yOffset = 0;
	int xOffset;

	//outer border - top
	for(i=0;i<_outerBorderThickness;i++) {
		for(j=0;j<_cellWidth;j++) {
			_image.setPixel(x*_cellWidth+j, y*_cellHeight+yOffset+i, _outerBorderColor);
		}
	}
	
	yOffset = _outerBorderThickness;

	//inner border - top
	for(i=0;i<_innerBorderThickness;i++) {
		xOffset = 0;
		for(j=0;j<_outerBorderThickness;j++) {
			_image.setPixel(x*_cellWidth+xOffset+j, y*_cellHeight+yOffset+i, _outerBorderColor);
		}
		xOffset += _outerBorderThickness;
		for(j=0;j<_colorWidth+_innerBorderThickness*2;j++) {
			_image.setPixel(x*_cellWidth+xOffset+j, y*_cellHeight+yOffset+i, _innerBorderColor);
		}
		xOffset += _colorWidth+_innerBorderThickness*2;
		for(j=0;j<_outerBorderThickness;j++) {
			_image.setPixel(x*_cellWidth+xOffset+j, y*_cellHeight+yOffset+i, _outerBorderColor);
		}
	}

	yOffset += _innerBorderThickness;
			
	//data
	for(i=0;i<_colorHeight;i++) {
		xOffset = 0;
		for(j=0;j<_outerBorderThickness;j++) {
			_image.setPixel(x*_cellWidth+xOffset+j, y*_cellHeight+yOffset+i, _outerBorderColor);
		}
		xOffset += _outerBorderThickness;
		for(j=0;j<_innerBorderThickness;j++) {
			_image.setPixel(x*_cellWidth+xOffset+j, y*_cellHeight+yOffset+i, _innerBorderColor);
		}
		xOffset += _innerBorderThickness;
		for(j=0;j<_colorWidth;j++) {
			_image.setPixel(x*_cellWidth+xOffset+j, y*_cellHeight+yOffset+i, color);
		}
		xOffset += _colorWidth;
		for(j=0;j<_innerBorderThickness;j++) {
			_image.setPixel(x*_cellWidth+xOffset+j, y*_cellHeight+yOffset+i, _innerBorderColor);
		}
		xOffset += _innerBorderThickness;
		for(j=0;j<_outerBorderThickness;j++) {
			_image.setPixel(x*_cellWidth+xOffset+j, y*_cellHeight+yOffset+i, _outerBorderColor);
		}
	}

	yOffset += _colorHeight;

	//inner border - bottom
	for(i=0;i<_innerBorderThickness;i++) {
		xOffset = 0;
		for(j=0;j<_outerBorderThickness;j++) {
			_image.setPixel(x*_cellWidth+xOffset+j, y*_cellHeight+yOffset+i, _outerBorderColor);
		}
		xOffset += _outerBorderThickness;
		for(j=0;j<_colorWidth+_innerBorderThickness*2;j++) {
			_image.setPixel(x*_cellWidth+xOffset+j, y*_cellHeight+yOffset+i, _innerBorderColor);
		}
		xOffset += _colorWidth+_innerBorderThickness*2;
		for(j=0;j<_outerBorderThickness;j++) {
			_image.setPixel(x*_cellWidth+xOffset+j, y*_cellHeight+yOffset+i, _outerBorderColor);
		}
	}

	yOffset += _innerBorderThickness;
	
	//outer border - bottom
	for(i=0;i<_outerBorderThickness;i++) {
		for(j=0;j<_cellWidth;j++) {
			_image.setPixel(x*_cellWidth+j, y*_cellHeight+yOffset+i, _outerBorderColor);
		}
	}
}

void IndexedPaletteEditorWidget::updateImage() {
	setPixmap(QPixmap::fromImage(_image));
}

void IndexedPaletteEditorWidget::mousePressEvent(QMouseEvent* event) {
	//if this was a left click, record where the button was clicked.
	if(event->button() == Qt::LeftButton) {
		//determine if the user clicked on a color.
		_clickedX = event->x();
		_clickedY = event->y();

		int tmpX = _clickedX;
		int tmpY = _clickedY;

		tmpX = (tmpX % _cellWidth) - _outerBorderThickness+_innerBorderThickness;
		tmpY = (tmpY % _cellHeight) - _outerBorderThickness+_innerBorderThickness;

		if( (0<tmpX && tmpX<_colorWidth) && (0<tmpY && tmpY<_colorHeight) ) {
			_clickedX = _clickedX / _cellWidth;
			_clickedY = _clickedY / _cellHeight;
			event->accept();
		} else {
			_clickedX = -1;
			_clickedY = -1;
			event->ignore();
		}

	} else {
		event->ignore();
	}
}

void IndexedPaletteEditorWidget::mouseReleaseEvent(QMouseEvent* event) {
	//if the left button was released and the press event was on a color
	//change that color.
	if(_clickedX >= 0 && _clickedY >= 0 && event->button() == Qt::LeftButton) {
		QRgb oldColor = getColor(_clickedX, _clickedY);
		//if the color in the clicked-on section is the undefined color, pass in black.
		if(oldColor == _undefinedColor) {
			oldColor = qRgba(0, 0, 0, 255);
		}
		bool bOk = false;
		QRgb newColor = QColorDialog::getRgba(oldColor, &bOk, this);
		if(bOk) {
			setColor(_clickedX, _clickedY, newColor);
			emit colorChanged(_clickedX, _clickedY, newColor);
		}

		_clickedX = -1;
		_clickedY = -1;

		event->accept();
	} else {
		event->ignore();
	}
}

void IndexedPaletteEditorWidget::changeSize(int width, int height) {

	if(width <= 0 || height <= 0)
		return;
	int count = 0;
	QProgressDialog progress("Please Wait...", 0, 0, height+2, this);
	progress.setWindowModality(Qt::WindowModal);

	clear();

	_width = width;
	_height = height;

	resize(width*_cellWidth, height*_cellHeight);
	_image = QImage(width*_cellWidth, height*_cellHeight, QImage::Format_ARGB32);
	stepProgress();

	//draw widget
	for(int y=0;y<_height;y++) {
		for(int x=0;x<_width;x++) {
			drawCell(x, y, _undefinedColor);
		}
		stepProgress();
	}

	updateImage();
	stepProgress();
}

void IndexedPaletteEditorWidget::clearColor(int x, int y) {
	setColor(x, y, _undefinedColor);
}

//steps the progress dialog
void IndexedPaletteEditorWidget::stepProgress() {
	if(_prgDlg != NULL) {
		int val = _prgDlg->value();
		_prgDlg->setValue(val+1);
		QApplication::processEvents();		
	}
}

void IndexedPaletteEditorWidget::setProgressDialog(QProgressDialog* prgDlg) {
	_prgDlg = prgDlg;
}
