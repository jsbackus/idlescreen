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
 * This class is the GPL dialog.
 */

#ifndef __GPLDIALOG_H__
#define __GPLDIALOG_H__

#include <QDialog>

class GPLDialog : public QDialog {

	Q_OBJECT

public:
	GPLDialog(bool bAccepted = false, QWidget* parent = 0, Qt::WindowFlags f = 0);

	bool isAccepted();

public slots:
	void gplAcceptDown(bool checked = false);
	void accept(void);
	void done(int r = 0);
	void reject(void);

private:
	bool _bAccepted;

};

#endif