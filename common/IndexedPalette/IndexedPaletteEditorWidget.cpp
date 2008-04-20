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

IndexedPaletteEditorWidget::IndexedPaletteEditorWidget() {
	_prgDlg = NULL;
}

IndexedPaletteEditorWidget::~IndexedPaletteEditorWidget() {
	_prgDlg = NULL;
}

QRgb IndexedPaletteEditorWidget::getColor(int x, int y) {
	QColor retVal(0,0,0,255);

	return retVal.rgb();
}

void IndexedPaletteEditorWidget::setColor(int x, int y, QRgb color) {
	// nada
}

void IndexedPaletteEditorWidget::changeSize(int width, int height) {
	// nada
}

void IndexedPaletteEditorWidget::clearColor(int x, int y) {
	// nada
}

//steps the progress dialog
void IndexedPaletteEditorWidget::stepProgress(int numSteps) {
	if(_prgDlg != NULL) {
		int val = _prgDlg->value();
		_prgDlg->setValue(val+numSteps);
		QApplication::processEvents();		
	}
}

void IndexedPaletteEditorWidget::setProgressDialog(QProgressDialog* prgDlg) {
	_prgDlg = prgDlg;
}

// returns the actual widget.  Calling class should NOT destroy widget.
QWidget* IndexedPaletteEditorWidget::getWidget() {
	return NULL;
}
