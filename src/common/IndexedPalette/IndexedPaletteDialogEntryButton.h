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
 * This is a subclass of QPushButton that is used by the IndexedPaletteDialog to draw the grid
 * of entries.  Simply remembers the location given to it and will emit a signal when the
 * color has changed.
 */

/** NOTES:
	look at QColorDialog, specifically getRgba()
	look at QRgba
*/

#ifndef __INDEXEDPALETTEDIALOGENTRYBUTTON_H__
#define __INDEXEDPALETTEDIALOGENTRYBUTTON_H__

#include <QPushButton>
#include <QColor>

class IndexedPaletteDialogEntryButton : public QPushButton {

	Q_OBJECT

public:
	IndexedPaletteDialogEntryButton(int x=0, int y=0, QRgb* color=NULL, QWidget* parent = 0, Qt::WindowFlags f = 0);

	QRgb getColor();

public slots:
	void buttonClicked(bool checked = false);

signals:
	void colorChanged(int x, int y, QRgb color);

private:
	int _x;
	int _y;
	QRgb _color;

};

#endif