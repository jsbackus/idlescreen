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
 * This dialog box is used to configure an IndexedPaletteProfile object.
 */

#ifndef __INDEXEDPALETTEDIALOG_H__
#define __INDEXEDPALETTEDIALOG_H__

#include <QDialog>
#include <QLineEdit>
#include <QScrollArea>
#include <QLabel>
#include <QSpinBox>

#include "IndexedPaletteProfile.h"
#include "IndexedPaletteEditorWidget.h"

class IndexedPaletteDialog : public QDialog {

	Q_OBJECT

public:
	IndexedPaletteDialog(IndexedPaletteProfile* palProfile = NULL, QWidget* parent = 0, Qt::WindowFlags f = 0);
	~IndexedPaletteDialog();

	//returns the modified palette.
	IndexedPaletteProfile getPalette(void);

public slots:
	void okClicked(bool checked = false);
	void cancelClicked(bool checked = false);
	void revertClicked(bool checked = false);
	void helpClicked(bool checked = false);

	void colorChanged(int x, int y, QRgb* color);
	void defaultColorClicked(bool checked = false);
	void resizeClicked(bool checked = false);
	void interpClicked(bool checked = false);
	void linearClicked(bool checked = false);
	void sineClicked(bool checked = false);
	void wrapClicked(bool checked = false);

signals:
	void paletteUpdated(void);

private:

	//populates the widget based on _palProfile.
	//Steps the dialog box _palProfile.getHeight()*2+2 number of times.
	void populateColorWidget(void);

	//updates the preview icon
	//Steps the dialog box _palProfile.getHeight() number of times.
	void updatePreviewIcon(void);

	//steps the progress dialog
	void stepProgress();

	IndexedPaletteProfile _palProfile;
	IndexedPaletteProfile _origPalProfile;

	QScrollArea* _colorScrollWidget;
	IndexedPaletteEditorWidget* _colorWidget;

	QLineEdit* _nameEdit;

	QLabel* _previewLabel;
	QPixmap _previewPixmap;
	int _previewZoom;

	QProgressDialog* _prgDlg;
	int _prgCount;

	QSpinBox* _xDimBox;
	QSpinBox* _yDimBox;

	QRadioButton* _linearButton;
	QRadioButton* _sineButton;
	InterpType _lastInterpType;
	QString _windowTitle;
};


#endif