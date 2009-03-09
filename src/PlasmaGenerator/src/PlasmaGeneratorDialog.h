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

#ifndef __PLASMAGENERATORDIALOG_H__
#define __PLASMAGENERATORDIALOG_H__

#include <QString>
#include <QDialog>
#include <QLineEdit>
#include <QScrollArea>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QLabel>
#include <QPixmap>
#include <QSlider>
#include <QProgressDialog>

#include "gpl_related/gpldialog.h"

#include "ConfigManager.h"
#include "PlasmaGenerator.h"

class PlasmaGeneratorDialog : public QDialog {

  Q_OBJECT
    
public:
  PlasmaGeneratorDialog(ConfigManager* manager, QWidget* parent = 0, Qt::WindowFlags f = 0);
  ~PlasmaGeneratorDialog();

  /**
   * Displays the GPL license dialog.
   */
  void showGPLDialog(void);

public slots:
  void applyClicked(bool checked = false);
  void okClicked(bool checked = false);
  void cancelClicked(bool checked = false);
  void helpClicked(bool checked = false);

  void managePalettesClicked(bool checked = false);

  void setDefaultClicked(bool checked = false);
  void newProfileClicked(bool checked = false);
  void copyProfileClicked(bool checked = false);
  void renameProfileClicked(bool checked = false);
  void deleteProfileClicked(bool checked = false);
  void profileChanged(const QString& newProfile);

  void generateClicked(bool checked = false);
  void saveClicked(bool checked = false);
  void browseClicked(bool checked = false);

  void importClicked(bool checked = false);
  void exportClicked(bool checked = false);

private:

  /**
   * Updates the profile info in the config manager.
   */
  void updateProfile();

  /**
   * Updates all widgets with the currently selected profile.
   */
  void updateDisplay();

  QString _windowTitle;

  QLineEdit* _fileEdit;
  QComboBox* _nameBox;
  QComboBox* _palName;

  QSpinBox* _xDimBox;
  QSpinBox* _yDimBox;

  QCheckBox* _tileVertBox;
  QCheckBox* _tileHorizBox;
 
  QRadioButton* _alwaysBox;
  QRadioButton* _askBox;
  QRadioButton* _neverBox;

  QPushButton* _defaultButton;

  QSlider* _coarsenessBox;
  float _coarsenessAdjust;
  QCheckBox* _clampColorBox;

  QScrollArea* _previewScrollWidget;
  QLabel* _previewLabel;
  QPixmap _previewPixmap;

  QProgressDialog* _prgDialog;

  ConfigManager* _manager;

  bool _bDefault;
  bool _bDontUpdateDisplay;
  QString _currentProfile;

  QString _fileFormatFilter;
  QString _selFilter;

  PlasmaGenerator* _plasma;

  GPLDialog* _gplDialog;
  
};


#endif
