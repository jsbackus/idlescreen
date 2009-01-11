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

// debug
#include <iostream>
using namespace std;
//#include <QMessageBox>
//#include "lens_engine/SphericalLensProfile.h"

#include <QCoreApplication>
#include <QApplication>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>

#include "utility/misc_funcs.h"
#include "core/win_misc_funcs.h"
#include "project_specific_extern_defs.h"

#include "ConfigManager.h"

//Constructor and deconstructor
ConfigManager::ConfigManager() {

  //specify the plugins path.  plugins should have been installed
  //by the lib package.  the libs need to be added to the system path.
  //first check in HKEY_CURRENT_USER.  If not there, check HKEY_LOCAL_MACHINE.
  /*
    QString getExternalDefaultConfigFile();
    QString getExternalDefaultConfigPath();
    QString rootPath = readRegString(QString("Software\\Phej"),
    QString("Lib Install Base Path"), false);
    if(rootPath == "") {
    rootPath = readRegString(QString("Software\\Phej"),
    QString("Lib Install Base Path"), true);
    }

    QString libPath = rootPath + QString("\\Qt-4.2.2\\plugins\\");
    QCoreApplication::addLibraryPath(libPath);
  */


  _bGPLAccepted = false;
}

ConfigManager::~ConfigManager() {

  //clean out hashes
  deleteAllSettings();
}

void ConfigManager::load() {
  deleteAllSettings();

  //try to load from the primary config file.
  //if unsuccessful, try to load defaults.
  if(!importFromFile(getMainConfigFile(), true)) {
    setDefaults();
  }
}

QString ConfigManager::getExternalDefaultConfigPath() {
  QString retVal;

#ifdef _M_IX86
  QString installPath = readRegString(QString("Software\\IdleScreen"), QString("InstallPath"), false);
  if(installPath == "") {
    installPath = readRegString(QString("Software\\IdleScreen"), QString("InstallPath"), true);
  }
  retVal += installPath + "/defaults/";
#else
  retVal += "/etc/idlescreen/";
#endif

  return retVal;
}
QString ConfigManager::getExternalDefaultConfigFile() {
  QString retVal = getExternalDefaultConfigPath() + getAppConfigName() + ".xml";
  return retVal;
}

QString ConfigManager::getMainConfigPath() {
  QString retVal = QDir::homePath() + "/";

#ifdef _M_IX86
  retVal += "Application Data/idlescreen/";
#else
  retVal += ".idlescreen/";
#endif

  retVal += getAppConfigName();

  return retVal;
}

QString ConfigManager::getMainConfigFile() {

  QString retVal = getMainConfigPath();
  retVal += "/settings.xml";

  return retVal;
}

void ConfigManager::deleteAllSettings() {

  //clean out hashes
  keys = _paletteHash.keys();
  for(int i=0; i<keys.size();i++) {
    IndexedPaletteProfile* tmpPtr = _paletteHash.take(keys.at(i));
    delete tmpPtr;
    tmpPtr = NULL;
  }

  //clean out lists
  _bGPLAccepted = false;
}


bool ConfigManager::importFromFile(QString filename, bool bLoadMiscInfo) {
  QDomDocument doc(getDomNodeQString());
  QFile file(filename);
  if (!file.open(QIODevice::ReadOnly)) {
    cout<<"Unable to open file "<<filename.toStdString()
	<<" for reading!"<<endl;
    return false;
  }
  if (!doc.setContent(&file)) {
    file.close();
    cout<<"File "<<filename.toStdString()
	<<" not a proper configuration file!"
	<<endl;
    return false;
  }
  file.close();
  QDomElement root = doc.documentElement();

  //load palette profiles
  tempNode = doc.firstChildElement().firstChildElement(IndexedPaletteProfile::getXMLTagName());
  while(!tempNode.isNull()) {
    IndexedPaletteProfile* tempProfile = IndexedPaletteProfile::load(tempNode);
    //if non-NULL, add it to the hash of palette profiles
    if(tempProfile != NULL) {
      _paletteHash.insert(tempProfile->getName(), tempProfile);
    }
    tempNode = tempNode.nextSiblingElement(IndexedPaletteProfile::getXMLTagName());
  }

  //load misc info
  if(bLoadMiscInfo) {
    loadMiscInfo(root);
  }

  return true;
}

void ConfigManager::save() {
  QString targetFile = getMainConfigFile();

  //make sure target directory exists.  If not, create it.
  QDir targetDir(getMainConfigPath());
  if(!targetDir.exists()) {
    //attempt to make the path.  If failure,
    //just return.
    if(!targetDir.mkpath(getMainConfigPath()))
      return;
  }

  exportToFile(targetFile, true);
}

bool ConfigManager::exportToFile(QString filename, bool bSaveMiscInfo) {
  QDomDocument doc(getDomNodeQString());
  QDomElement root = doc.createElement(getDomNodeQString());
  doc.appendChild(root);

  //store palette profiles..
  keys = _paletteHash.keys();
  for(int i=0; i<keys.size();i++) {
    root.appendChild(_paletteHash.value(keys.at(i))->save(&doc));
  }

  if(bSaveMiscInfo) {
    saveMiscInfo(&doc, &root);
  }

  QFile file(filename);
  if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    return false;

  QTextStream out(&file);
  QString payload = doc.toString();
  out << payload << "\n";
  file.close();

  return true;
}

void ConfigManager::loadMiscInfo(QDomNode &node) {
  QDomElement tempElem;

  tempElem = node.firstChildElement("gpl_accepted");
  if(!tempElem.isNull()) {
    QString param = tempElem.text();
    _bGPLAccepted = stringToBool(param);
  }

  tempElem = node.firstChildElement("default_texture_width");
  if(!tempElem.isNull()) {
    _defWidth = tempElem.text().toInt();
  }

  tempElem = node.firstChildElement("default_texture_height");
  if(!tempElem.isNull()) {
    _defHeight = tempElem.text().toInt();
  }

}

void ConfigManager::saveMiscInfo(QDomDocument* doc, QDomElement* root) {
  QString tempStr;
  QDomText tempNode;
  QDomElement tempElem;

  // gpl accepted?
  tempNode = doc->createTextNode(boolToString(_bGPLAccepted));
  tempElem = doc->createElement("gpl_accepted");
  tempElem.appendChild(tempNode);
  root->appendChild(tempElem);

  // default texture width
  tempStr.setNum(_defWidth);
  tempNode = doc->createTextNode(tempStr);
  tempElem = doc->createElement("default_texture_width");
  tempElem.appendChild(tempNode);
  root->appendChild(tempElem);

  // default texture height
  tempStr.setNum(_defHeight);
  tempNode = doc->createTextNode(tempStr);
  tempElem = doc->createElement("default_texture_height");
  tempElem.appendChild(tempNode);
  root->appendChild(tempElem);

}

//Sets defaults
void ConfigManager::setDefaults() {
  deleteAllSettings();

  // attempt to load from machine defaults file.
  if(!importFromFile(getExternalDefaultConfigFile(), true)) {
    if(!importFromFile(":/defaults.xml", true)) {
      cout<<"Unable to load any settings!"<<endl;
      exit(1);
    }
  }
}

int ConfigManager::getTextureWidth() {
  return _defWidth;
}

int ConfigManager::getTextureHeight() {
  return _defHeight;
}

//whether or not the GPL has been accepted
bool ConfigManager::getGPLAccepted() {
  return _bGPLAccepted;
}

void ConfigManager::setGPLAccepted(bool bAccepted) {
  _bGPLAccepted = bAccepted;
}

/**
 * Returns a list of the names of the palettes currently in the palette hash.
 */
QStringList ConfigManager::getPaletteNames() {
  return _paletteHash.keys();
}

/**
 * Removes the palette with the specified name from the hash.
 * If bUpdateProfiles is true, it will iterate through all
 * background profiles alerting them to the removal.  When
 * replacing a palette, this should be false!
 */
bool ConfigManager::removePalette(QString name, bool bUpdateProfiles) {
  // first, alert all background profiles that a palette is being
  // removed if bUpdateProfiles is true.
  if(bUpdateProfiles) {
    QList<QString> keys = _backgroundHash.keys();
    for(int i=0; i<keys.size();i++) {
      _backgroundHash.value(keys.at(i))->paletteRemoved(name);
    }
  }

  // now attempt to remove the palette and return success status.
  return (_paletteHash.remove(name) > 0);
}

/**
 * Adds the specified palette to the hash if it isn't already
 * in the hash.
 */
void ConfigManager::addPalette(IndexedPaletteProfile& pal) {
  //check for preexisting palette name.
  if(!doesPaletteExist(pal.getName())) {
    //if not, create a new palette profile off of the heap to add to the hash.
    IndexedPaletteProfile* newPal = new IndexedPaletteProfile();
    *newPal = pal;
    _paletteHash.insert(pal.getName(), newPal);
  }
}

/**
 * Checks to see if a palette with the specified name is already
 * in the hash.
 */
bool ConfigManager::doesPaletteExist(QString name) {
  return _paletteHash.contains(name);
}

/**
 * Replaces the palette with the specified name with the
 * specified palette if the new name isn't already
 * in the list.
 */
bool ConfigManager::replacePalette(QString oldPaletteName, IndexedPaletteProfile& newPalette) {
  //first, check to see if the new palette name is already in the list and that
  //it isn't the same as the old name.  If so, fail.
  QString newPalName = newPalette.getName();
  if(oldPaletteName != newPalName && doesPaletteExist(newPalName)) {
    return false;
  }

  //now check to see if the old palette name is in the list.  if so, replace, otherwise add.
  if(doesPaletteExist(oldPaletteName)) {
    removePalette(oldPaletteName, false);
    addPalette(newPalette);
    
    // alert all background profiles that a palette is being
    // replaced
    QList<QString> keys = _backgroundHash.keys();
    for(int i=0; i<keys.size();i++) {
      _backgroundHash.value(keys.at(i))->paletteNameChanged(oldPaletteName,
							    newPalName);
    }
  } else {
    addPalette(newPalette);
  }

  return true;
}

/**
 * Returns a copy of the specified palette profile.  Caller should
 * check for existence first with doesPaletteExist();
 */
IndexedPaletteProfile ConfigManager::getPaletteProfile(const QString& name) {
  IndexedPaletteProfile* retVal = _paletteHash.value(name);

  if(retVal != NULL)
    return *retVal;

  //if the specified palette isn't in the hash, return a new one.
  IndexedPaletteProfile newPalette;
  return newPalette;
}

// returns the string used in as the Dom root.
QString ConfigManager::getDomNodeQString() {
  return getAppConfigName() + QString("_Configuration");
}

/**
 * Gets the human-readable name of the program.
 */
QString ConfigManager::getProgName() {
  return getAppConfigName();
}

