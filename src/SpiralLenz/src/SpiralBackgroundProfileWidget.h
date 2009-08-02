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
 * This class represents the configuration widget panels for the different
 * spiral styles.  Each profile is responsible for generating an appropriate
 * layout for this panel.
 *
 * @author jeff backus
 * @date 04/03/2009
 */

#ifndef __SPIRALBACKGROUNDPROFILEWIDGET_H__
#define __SPIRALBACKGROUNDPROFILEWIDGET_H__

#include <QString>
#include <QtXml/QDomNode>
#include <QtXml/QDomDocument>
#include <QWidget>

#include "SpiralBackgroundProfile.h"

class SpiralBackgroundProfileWidget : public QWidget {
	Q_OBJECT

public:
	//constructor & destructor
	ConfigWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
	~ConfigWidget();
 private:

};
