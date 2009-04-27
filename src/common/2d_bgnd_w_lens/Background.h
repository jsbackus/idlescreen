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
 * This class represents a generic background
 * object.
 */

#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#include "globaldefs.h"

class Background {

 public:
  /**
   * Draws the whole Background.
   */
  virtual void drawBackground(screen_struct* screenObj);

  /**
   * Whether or not the background should animate
   * itself (if it supports it)
   */
  void enableAnimation(bool bEnableAnimation);

  /**
   * Call this every tick to animate.
   * Must be overloaded by derived class.
   */
  virtual void clocktick();

  /**
   * Used to set whether to show the fractal generation or not.
   * If so, the number of steps are shown per render pass.
   */
  //void setShowGeneration(bool bShowGen, int numPasses);

  /**
   * Returns true if the background has finished whatever
   * setup it needs to do, animated or otherwise.
   */
  bool isSetupFinished();

  /**
   * Constructor takes the background dimensions as
   * arguments.
   */
  Background(int sizeX, int sizeY);

  ~Background(void);

  //*** Begin Screen Related ***
  int getScreenWidth();
  int getScreenHeight();
  int getTextureWidth();
  int getTextureHeight();
  //*** End Screen Related

 protected:
  /**
   * Default constructor...
   */
  Background();

  /**
   * Called from the constructor and does all of the work,
   * so that derived classes can call from their constructors.
   * sizeX and sizeY are the dimensions of the background, not
   * necessarily the size of the screen object, which has to
   * be a power of 2.
   */
  void initBackground(int sizeX, int sizeY);

  int _sizeX;
  int _sizeY;

  bool _bEnableAnimation;

  bool _bSetupFinished;
};

#endif
