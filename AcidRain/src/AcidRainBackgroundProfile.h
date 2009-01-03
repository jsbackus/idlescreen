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
 * This background profile is responsible for setting up and returning a new 
 * CrawliesManager background object.
 *
 * @author jeff backus
 * @date 12/28/2008
 */

#ifndef __ACIDRAINBACKGROUNDPROFILE_H__
#define __ACIDRAINBACKGROUNDPROFILE_H__

#include <QString>
#include <QtXml/QDomNode>
#include <QtXml/QDomDocument>

#include "../../common/2d_bgnd_w_lens/BackgroundProfile.h"
#include "AcidRainManager.h"

struct rain_profile_style {
  QString pal;
  int minLength;
  int maxLength;
  int thickness;
  float minInitialV;
  float maxInitialV;
  float palSpeed;
  bool bHeadConstantColor;
  bool bHeadRandomColor;
};

class AcidRainBackgroundProfile : public BackgroundProfile {

public:
	AcidRainBackgroundProfile();
	~AcidRainBackgroundProfile();

	/** 
	 * Attempts to load this background profile object from the
	 * specified QDomNode.
	 */
	BackgroundProfile* load(QDomNode &node);

	/**
	 * Returns a QDomNode object that represents this profile.
	 */
	QDomNode save(QDomDocument* doc);

	/**
	 * Creates and returns a new background object.
	 */
	Background* getNewBackgroundObj(int height, int width, QHash<QString, IndexedPaletteProfile*>* palHash);

	/**
	 * get/set max rain density as an integer number between 0-100.
	 */
	int getMaxRainDensity();
	void setMaxRainDensity(int density);

	/**
	 * Get/set the acceleration "downward".  Must be a positive number!
	 */
	float getGravity();
	void setGravity(int gravity);

	/**
	 * Get/set the amount of rain "bounce".  Must be a positive number!
	 */
	float getRecoilElasticity();
	void setRecoilElasticity(int recoil);

	/**
	 * Get/set the max horizontal acceleration and delta acceleration.
	 */
	float getMaxHorizontalAcceleration();
	void setMaxHorizontalAcceleration(float accel);
	float getMaxHorizontalAcclerationDelta();
	void setMaxHorizontalAccelerationDelta(float delta);

	/**
	 * Get/set functions related for style subprofiles.
	 */
	int getNumStyles();
	void clearStyleList();
	void addStyle();
	void deleteStyle(int styleIdx);

	QString getPaletteName(int styleIdx);
	void setPaletteName(int styleIdx, QString palName);

	int getMinLength(int styleIdx);
	void setMinLength(int styleIdx, int minLength);
	int getMaxLength(int styleIdx);
	void setMaxLength(int styleIdx, int maxLength);

	int getThickness(int styleIdx);
	void setThickness(int styleIdx, int thickness);

	float getMinInitialV(int styleIdx);
	void setMinInitialV(int styleIdx, float velocity);
	float getMaxInitialV(int styleIdx);
	void setMaxInitialV(int styleIdx, float velocity);

	float getPaletteSpeed(int styleIdx);
	void setPaletteSpeed(int styleIdx, float palSpeed);

	bool isHeadConstantColor(int styleIdx);
	void setHeadConstantColor(int styleIdx, bool bConstant);

	bool isHeadRandomColor(int styleIdx);
	void setHeadRandomColor(int styleIdx, bool bRandom);

	rain_profile_style getStyle(int styleIdx);
	void setStyle(int styleIdx, rain_profile_style& style);

	/**
	 * Overloaded assignment operator.
	 */
	AcidRainBackgroundProfile& operator=(AcidRainBackgroundProfile& other);

	/**
	 * Creates a new object with this object's settings.
	 */
	BackgroundProfile* clone();

private:
  /**
   * Grows the list of crawly styles.
   */
  void growStyleList(int size=RAIN_STYLE_CHUNK_SIZE);

  rain_profile_style* _styles;
  int _numStyles;
  int _maxNumStyles;

  int _maxRainDensity;
  float _gravity;
  float _maxHorizAccel;
  float _maxHorizAccelDelta;
  float _recoilElasticity;
};
#endif
