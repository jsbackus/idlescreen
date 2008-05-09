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
 * This class represents the dialog box used to select what
 * to import/export.
 * 
 * Revision History:
 * 080301	jsbackus	Initial Revision.
 */

#ifndef __IMPORTEXPORTDIALOG_H__
#define __IMPORTEXPORTDIALOG_H__

#include <QDialog>
#include <QString>
#include <QStringList>
#include <QTreeWidget>

class ImportExportDialog : public QDialog {

	Q_OBJECT

public:
	ImportExportDialog(bool bImport = true, QWidget* parent = 0, Qt::WindowFlags f = 0);
	~ImportExportDialog();

	// delete items in the list
	void clearAll();
	void clearSubItems(const QString itemName);

	// adds subitems under the specified item.  If the
	// item doesn't exist, it is created.
	void addSubItems(const QString itemName, const QStringList subItemNames, const bool bChecked=true);

	// returns a QStringList of all of the subitems under the specified
	// item with the specified state.
	QStringList getSubItems(const QString itemName, const bool bChecked=true);

public slots:
	void okClicked(bool checked = false);
	void cancelClicked(bool checked = false);
	void itemChanged(QTreeWidgetItem* item, int column);

private:
	bool _bImport;
	QTreeWidget* _tree;
};

#endif
