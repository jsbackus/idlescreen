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

#include<QDialog>
#include<QListWidget>
#include<QLineEdit>
#include<QStringList>
#include<QWidget>

/**
 * This is the search path dialog
 */
#ifndef __SEARCHPATHDIALOG_H__
#define __SEARCHPATHDIALOG_H__

#include<QDialog>


class SearchPathDialog : public QDialog {
	Q_OBJECT

public:
	SearchPathDialog(QWidget* parent = 0, Qt::WindowFlags f = 0);

	void setPathList(QStringList pathList);
	QStringList retrievePathList(void);

public slots:
	void accept(void);
	void done(int r);
	void reject(void);
	void addClicked(bool checked = false);
	void removeClicked(bool checked = false);
	void browseClicked(bool checked = false);

private:
	//transfers from the list widget to the string list.
	void getListData();

	QStringList _pathList;
	QListWidget* _listWidget;
	QLineEdit* _editWidget;
};

#endif