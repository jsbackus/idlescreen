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
#include <QColor>
#include <QColorDialog>

#include "IndexedPaletteDialogEntryButton.h"

IndexedPaletteDialogEntryButton::IndexedPaletteDialogEntryButton(int x, int y, QRgb* color, QWidget* parent, Qt::WindowFlags f) {
	_x = x;
	_y = y;

	//set up color
	if(color != NULL) {
		QPixmap pixmap(32,32);
		pixmap.fill(QColor(*color));
		QIcon icon(pixmap);
		setIcon(icon);
	}

	QObject::connect(this, SIGNAL(clicked(bool)), this, SLOT(buttonClicked(bool)));

	//setMinimumSize(36, 36);
	//setMaximumSize(36, 36);
	setFixedSize(36,36);
}

QRgb IndexedPaletteDialogEntryButton::getColor() {
	return _color;
}

void IndexedPaletteDialogEntryButton::buttonClicked(bool checked) {
	bool bOk = false;
	QRgb newColor = QColorDialog::getRgba(_color, &bOk, this);
	if(bOk) {
		_color = newColor;

		QPixmap pixmap(32,32);
		pixmap.fill(QColor(_color));
		QIcon icon(pixmap);
		setIcon(icon);

		emit colorChanged(_x, _y, _color);
	}
}
