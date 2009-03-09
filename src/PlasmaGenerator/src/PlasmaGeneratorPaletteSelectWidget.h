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
 * This widget contains a selection box as well as buttons to add, edit, duplicate, and delete palettes.
 */

#ifndef __PLASMAGENERATORPALETTESELECTWIDGET_H__
#define __PLASMAGENERATORPALETTESELECTWIDGET_H__

#include <QWidget>
#include <QString>
#include <QListWidget>

#include "ConfigManager.h"
#include "IndexedPalette/IndexedPaletteProfile.h"
#include "IndexedPalette/IndexedPaletteDialog.h"

class PlasmaGeneratorPaletteSelectWidget : public QWidget {

  Q_OBJECT

    public:
  PlasmaGeneratorPaletteSelectWidget(ConfigManager* _confMgr = NULL, QWidget* parent = 0, Qt::WindowFlags f = 0);
  ~PlasmaGeneratorPaletteSelectWidget();

  /**
   * Retrieves the name of the currently selected palette.
   */
  QString getCurrentPaletteName();

  /**
   * Sets the currently selected palette.
   */
  void setCurrentPalette(QString name);

  public slots:

  void addClicked(bool checked = false);
  void copyClicked(bool checked = false);
  void editClicked(bool checked = false);
  void removeClicked(bool checked = false);

 signals:
  void selectionChanged(QString newName);

 private:
  //populates the list box
  void populateBox(void);

  //searches the list for the specified string, returning its index, or -1.
  int findItem(QString name);

  //attempts to edit the specified palette.
  void editPalette(const QString& palName, bool bReplace = false);

  ConfigManager* _confMgr;

  QListWidget* _list;
	
  IndexedPaletteDialog* _palDlg;
  QString _palOldName;
};
#endif
