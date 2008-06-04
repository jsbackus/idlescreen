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
 * This class represents a dialog box that allows the user to add/edit/delete
 * all available palettes.  Utilizes IndexedPaletteDialog for actual
 * modification of the palettes.  Does not have the ability to undo!
 */

#ifndef __PALETTEEDITOR_H__
#define __PALETTEEDITOR_H__

#include <QDialog>
//#include <QListWidget>

#include "PaletteSelectWidget.h"
#include "ConfigManager.h"
#include "../IndexedPalette/IndexedPaletteDialog.h"

class PaletteEditor : public QDialog {

	Q_OBJECT

public:
	PaletteEditor(ConfigManager* confMgr = NULL, QWidget* parent = 0, Qt::WindowFlags f = 0);
	~PaletteEditor();

public slots:
	void closeClicked(bool checked = false);

//signals:
	//void paletteUpdated(void);

private:

	//QHash<QString, IndexedPaletteProfile*>* _paletteHash;
	//QListWidget* _listWidget;
	PaletteSelectWidget* _listWidget;

	IndexedPaletteDialog* _palDlg;

	QString _palOldName;

	ConfigManager* _confMgr;
	QString _windowTitle;
};

#endif
