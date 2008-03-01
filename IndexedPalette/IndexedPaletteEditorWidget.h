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
 * This widget draws a grid with data points that represent
 * the colors in the palette.  It also intercepts mouse events
 * to look for clicking on the data points.  Clicking on a
 * datapoint will bring up the color chooser.
 */

#ifndef __INDEXEDPALETTEEDITORWIDGET_H__
#define __INDEXEDPALETTEEDITORWIDGET_H__

#include <QLabel>
#include <QColor>
#include <QMouseEvent>
#include <QProgressDialog>

class IndexedPaletteEditorWidget : public QLabel {

	Q_OBJECT

public:
	IndexedPaletteEditorWidget(int width=0, int height=0, QWidget* parent = 0, Qt::WindowFlags f = 0);
	~IndexedPaletteEditorWidget();

	QRgb getColor(int x, int y);
	void setColor(int x, int y, QRgb color);
	void clearColor(int x, int y);

	//resizes the image.  Steps the progress dialog height+2 times.
	void changeSize(int width=0, int height=0);

	void setProgressDialog(QProgressDialog* prgDlg);
	
public slots:
	//void buttonClicked(bool checked = false);

signals:
	void colorChanged(int x, int y, QRgb color);

protected:
	void mousePressEvent(QMouseEvent* event);
	void mouseReleaseEvent(QMouseEvent* event);

private:

	//draws the specified cell, including borders and color swatch
	void drawCell(int x, int y, QRgb color);

	//updates the widget image
	void updateImage();

	//steps the progress dialog
	void stepProgress();

	int _width;
	int _height;

	QImage _image;

	//"constants"
	int _colorWidth;
	int _colorHeight;
	int _innerBorderThickness;
	int _outerBorderThickness;
	int _cellHeight;
	int _cellWidth;
	QRgb _innerBorderColor;
	QRgb _outerBorderColor;
	QRgb _undefinedColor;

	int _clickedX;
	int _clickedY;

	QProgressDialog* _prgDlg;
};

#endif