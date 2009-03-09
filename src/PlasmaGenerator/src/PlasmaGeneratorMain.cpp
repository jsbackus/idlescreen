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
 * This is the main entry point for the PlasmaGenerator.
 */

#include <iostream>
using namespace std;

#include <QApplication>
#include <QStringList>

#include "ConfigManager.h"
#include "project_specific_extern_defs.h"
#include "PlasmaGeneratorDialog.h"

int doHelp(QString* appName, QApplication *app) {
  cout<<getAppFullName().toStdString()<<" v. "<<
    getAppVersion().toStdString()<<endl;
  cout<<"Part of the IdleScreen Project"<<endl;
  cout<<"http://sites.google.com/site/idlescreenproject/"<<endl<<endl;
  cout<<"Usage: "<<appName->toStdString()<<" [options]"<<endl;
  cout<<endl<<"Options:"<<endl;
  cout<<"\t-h\t\tDisplay help and version information"<<endl;
  cout<<"\t-c\t\tUse command-line mode."<<endl;
  cout<<"\t-p [filename]\tUse the specified profile"<<endl;
  cout<<"\t-o [filename]\tOutput Plasma to specified filename"<<endl;
  cout<<"\t-n\t\tForce never overwrite target file"<<endl;
  cout<<"\t-a\t\tForce always overwrite target file"<<endl;
  cout<<endl<<"Note that if command-line mode isn't specified the default is GUI mode.  In"<<endl;
  cout<<"GUI mode, all options are ignored.  Also note that any Qt-specific options will"<<endl;
  cout<<"be passed on to Qt regardless of other options specified."<<endl<<endl;
  return 0;
}

int doCommandLine(QStringList* args, QApplication *app) {
  if(app == NULL) {
    return -1;
  }

  //load configuration
  ConfigManager manager;
  manager.load();

  QString profileName;
  QString outfileName;
  bool bOverwrite = false;
  bool bSpecifyOverwrite = false;

  // process arguments
  int i = 0;
  while(i<args->size()) {
    if(args->at(i).compare(QString("-o"), Qt::CaseInsensitive) == 0) {
      // specify output file name
      if(i+1 < args->size()) {
	outfileName = args->at(i+1);
      }
      i+=2;
    } else if(args->at(i).compare(QString("-p"), Qt::CaseInsensitive) == 0) {
      // specify profile name
      if(i+1 < args->size()) {
	profileName = args->at(i+1);
      }
      i+=2;
    } else if(args->at(i).compare(QString("-n"), Qt::CaseInsensitive) == 0) {
      // never overwrite
      bSpecifyOverwrite = true;
      bOverwrite = false;
      i++;
    } else if(args->at(i).compare(QString("-a"), Qt::CaseInsensitive) == 0) {
      // always overwrite
      bSpecifyOverwrite = true;
      bOverwrite = true;
      i++;
    } else {
      i++;
    }
  }
  
  // have ConfigManager generate and save the plasma
  if(!manager.GeneratePlasmaFile(profileName, outfileName, bSpecifyOverwrite,
				 bOverwrite))
    return -1;

  return 0;
}

int doDialog(QStringList* args, QApplication *app) {
  if(app == NULL) {
    return -1;
  }

  //load configuration
  ConfigManager manager;
  manager.load();

  //set up the dialog
  PlasmaGeneratorDialog* dlg = new PlasmaGeneratorDialog(&manager);
  if(dlg == NULL) {
    return -1;
  }

  //make sure that the GPL has been accepted.
  if(!manager.getGPLAccepted()) {
    dlg->showGPLDialog();
    if(!manager.getGPLAccepted()) {
      return -1;
    }
  }

  //make the dialog visible
  dlg->show();

  //run the dialog box
  app->exec();

  return 0;
}

int main(int argv, char** argc) {

  QString appName = argc[0];

  //start up the QApp
  QApplication app(argv, argc);

  bool bComMode = false;
  QStringList argList;
  // convert arguments to QStringList
  for(int i=0; i<argv; i++) {
    QString arg(argc[i]);
    if(arg.compare(QString("-h"), Qt::CaseInsensitive) == 0) {
      doHelp(&appName,&app);
      return 0;
    } else if(arg.compare(QString("-c"), Qt::CaseInsensitive) == 0) {
      bComMode = true;
    } else {
      argList.append(arg);
    }
  }

  if(bComMode) {
    return doCommandLine(&argList, &app);
  } else {
    return doDialog(&argList, &app);
  }
}
