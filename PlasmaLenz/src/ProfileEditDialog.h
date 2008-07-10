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
 * This class represents the dialog box used to edit the screen saver profiles.
 * Manages both the MasterProfile and BackgroundProfile objects.
 * 
 * Revision History:
 * 080301	jsbackus	Initial Revision.
 */

#ifndef __PROFILEEDITDIALOG_H__
#define __PROFILEEDITDIALOG_H__

#include <QDialog>
#include <QListWidget>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QSlider>

//#include "../../common/2d_bgnd_w_lens/ConfigManager.h"
//#include "../../common/2d_bgnd_w_lens/MasterProfile.h"
//#include "../../common/2d_bgnd_w_lens/BackgroundProfile.h"
//#include "PlasmaFractalBackgroundProfile.h"
//#include "../../../common/IndexedPalette/IndexedPaletteDialog.h"

#include "2d_bgnd_w_lens/ConfigManager.h"
#include "2d_bgnd_w_lens/MasterProfile.h"
#include "2d_bgnd_w_lens/BackgroundProfile.h"
#include "PlasmaFractalBackgroundProfile.h"
#include "IndexedPalette/IndexedPaletteDialog.h"

class ProfileEditDialog : public QDialog {

	Q_OBJECT

public:
	ProfileEditDialog(QString targetName = "", ConfigManager* confMgr = NULL, QWidget* parent = 0, Qt::WindowFlags f = 0);
	~ProfileEditDialog();

public slots:
	//ok is responsible for updating the ConfigManager
	void okClicked(bool checked = false);
	void cancelClicked(bool checked = false);

	void addPalClicked(bool checked = false);
	void editPalClicked(bool checked = false);
	void animatePalClicked(bool checked = false);

	void addAccepted(void);
	void editAccepted(void);

signals:
	void profileNameChange(QString oldName, QString newName);

private:
	//attempts to edit the specified palette.
	void editPalette(IndexedPaletteProfile* pal, bool bWasAddRename = false);

	// populates the palette name selection combo box
	void populatePalList(QString selected);

	//MasterProfile related
	QLineEdit* _nameBox;
	QSpinBox* _timerBox;
	QComboBox* _screenXBox;
	QComboBox* _screenYBox;

	MasterProfile _mp;

	//PlasmaFractalBackgroundProfile related
	QSlider* _coarsenessBox;
	float _coarsenessAdjust;
	QComboBox* _paletteBox;
	bool _bAnimatePalette;
	QDoubleSpinBox* _paletteXSpeedBox;
	QDoubleSpinBox* _paletteYSpeedBox;
	QCheckBox* _clampColorBox;
//	PlasmaFractalBackgroundProfile _plasma;

	IndexedPaletteDialog* _palDlg;
	QString _palOldName;
	bool _bFinishedAddEdit;

	// lens related
	QSlider* _lensSizeSlider;
	QSlider* _lensVarSlider;
	QSpinBox* _numLenses;

	ConfigManager* _confMgr;

	QString _oldName;
	QString _windowTitle;
};
#endif
