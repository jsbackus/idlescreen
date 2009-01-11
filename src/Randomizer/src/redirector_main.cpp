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
 */

/**
 * This is the main entry point for the redirector exec.  The majority
 * of the code is in redirector.cpp, located in the common/utility.
 *
 * Revision history:
 * 080308	jsbackus	Initial version.
 */
#include <windows.h>

#include "../resource_files/resource.h"

#include "../../common/utility/redirector.h"

const char KEY_NAME[] = "Randomizer";

/**
 * Main entry point.
 */
//int winmain(int argc, char** argv) {
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	return redirector(lpCmdLine, &KEY_NAME[0]);
}