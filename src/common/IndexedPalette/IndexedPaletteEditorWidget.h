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

#ifndef __INDEXEDPALETTEEDITORWIDGET_H__
#define __INDEXEDPALETTEEDITORWIDGET_H__

#include <QObject>
#include <QWidget>
#include <QColor>
#include <QMouseEvent>
#include <QProgressDialog>

class IndexedPaletteEditorWidget : public QObject {

	Q_OBJECT

public:
	IndexedPaletteEditorWidget();
	~IndexedPaletteEditorWidget();

	virtual QRgb getColor(int x, int y);
	virtual void setColor(int x, int y, QRgb color);
	virtual void clearColor(int x, int y);

	// resizes the image.  Steps the progress dialog height+2 times.
	// clears out all data held within the widget.
	virtual void changeSize(int width=0, int height=0);

	// returns the actual widget.  Calling class should NOT destroy widget.
	virtual QWidget* getWidget();

	void setProgressDialog(QProgressDialog* prgDlg);
	
public slots:
	//void buttonClicked(bool checked = false);

signals:
	void colorChanged(int x, int y, QRgb* color);

protected:

	//steps the progress dialog
	void stepProgress(int numSteps=1);

	QProgressDialog* _prgDlg;

	int _width;
	int _height;
};

#endif
