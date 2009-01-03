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
 * This class represents the dialog box used to edit the Crawly styles
 * associated with each CrawliesBackgroundProfile object.
 */

#ifndef __STYLEEDITDIALOG_H__
#define __STYLEEDITDIALOG_H__

#include <QDialog>
#include <QListWidget>
#include <QComboBox>
#include <QCheckBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QSlider>

#include "IndexedPalette/IndexedPaletteDialog.h"
#include "2d_bgnd_w_lens/ConfigManager.h"
#include "AcidRainBackgroundProfile.h"

class StyleEditDialog : public QDialog {

	Q_OBJECT

public:
  StyleEditDialog(rain_profile_style* style, ConfigManager* confMgr = NULL, QWidget* parent = 0, Qt::WindowFlags f = 0);
  ~StyleEditDialog();

  /**
   * Returns the settings.  If cancel was clicked,
   * this routine will replace data with initial values.
   */
  rain_profile_style getStyleData();

public slots:
	//ok is responsible for updating the ConfigManager
	void okClicked();
	void cancelClicked();
	void helpClicked();

	void addPalClicked();
	void editPalClicked();

	void minUsePalSizeClicked(bool checked = false);
	void maxUsePalSizeClicked(bool checked = false);
	void headConstantColorClicked(bool checked = false);
	void headRandomColorClicked(bool checked = false);

private:
	//attempts to edit the specified palette.
	void editPalette(const QString& palName, bool bReplace = false);

	// populates the palette name selection combo box
	void populatePalList(QString selected);

	//CrawliesBackgroundProfile Style related
	QComboBox* _paletteBox;
	QSpinBox* _minSizeBox;
	QSpinBox* _maxSizeBox;
	QSpinBox* _thicknessBox;
	QDoubleSpinBox* _minSpeedBox;
	QDoubleSpinBox* _maxSpeedBox;
	QDoubleSpinBox* _palSpeedBox;

	IndexedPaletteDialog* _palDlg;
	QString _palOldName;

	bool _bMinUsePalSize;
	bool _bMaxUsePalSize;
	bool _bHeadConstant;
	bool _bHeadRandom;

	ConfigManager* _confMgr;

	QString _windowTitle;

	rain_profile_style _dataVals;
};
#endif
