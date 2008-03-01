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

#include <stdlib.h>
#include <ctime>

using namespace std;

#include "PlasmaAlgorithm.h"

PlasmaAlgorithm::PlasmaAlgorithm() {
	_coarseness = 1.0;
	_bClampColor = false;
	_field = NULL;
	_genHead = NULL;
	_genTail = NULL;

	//seed the random number generator.
	srand((unsigned)time( NULL ));
}

PlasmaAlgorithm::~PlasmaAlgorithm() {
	_field = NULL;

	while(_genHead != NULL) {
		_genTail = _genHead;
		_genHead = _genHead->next;
		delete _genTail;
		_genTail = NULL;
	}
}

/**
 * This method initializes the algorithm.  It takes a pointer to the field to be filled,
 * the dimensions of the field, the size of the palette, the coarseness factor, whether
 * to clamp colors (as opposed to wrapping around), whether the plasma should be tiled
 * in the horizontal direction, and whether the plasma should be tiled in the vertical
 * direction.  Returns true if the algorithm was successfully initialized.
 * NOTE: Expects field to already be allocated with sufficient space!
 */
bool PlasmaAlgorithm::initialize(int* field, int width, int height, int numColors, float coarseness,
								 bool bClampColor, bool bTileHorizontal, bool bTileVertical) 
{
	if(field == NULL)
		return false;
	if(width <= 0)
		return false;
	if(height <= 0)
		return false;
	if(numColors <= 0)
		return false;

	_field = field;
	_width = width;
	_height = height;
	_coarseness = coarseness;
	_bClampColor = bClampColor;
	_numColors = numColors;

	// initialize field to -1 (no color)
	for(int i=0;i<_height*_width;i++)
	{
		_field[i] = -1;
	}

	// !!! ADD IN CODE TO SUPPORT TILING !!!
	int tmpWidth = _width-1;
	int tmpHeight = _height-1;

	if(bTileHorizontal)
		tmpWidth = tmpWidth/2;

	if(bTileVertical)
		tmpHeight = tmpHeight/2;

	// generate the four random colors
	int color;
	//(0,0)
	color = rand()%(_numColors);
	_field[0] = color;
	//(tmpWidth, 0)
	color = rand()%(_numColors);
	_field[tmpWidth] = color;
	//(0, tmpHeight)
	color = rand()%(_numColors);
	_field[(tmpHeight-1)*_width] = color;
	//(tmpWidth, tmpHeight)
	color = rand()%(_numColors);
	_field[tmpWidth+(_width*(tmpHeight-1))] = color;

	// add the primary region to the queue for processing
	_genHead = new plasma_coord_node;
	_genTail = _genHead;
	_genHead->next = NULL;
	_genHead->x1 = 0;
	_genHead->y1 = 0;
	_genHead->x2 = tmpWidth;
	_genHead->y2 = tmpHeight;

	// check to see if we are going to mirror and act accordingly
	if(bTileHorizontal) {
		// mirror about the Y axis and add the corresponding region
		// C[_width-1, 0] = C[0,0]
		_field[_width-1] = _field[0];
		// C[_width-1, tmpHeight] = C[0, tmpHeight]
		_field[(_width-1)+(tmpHeight)*_width] = _field[(tmpHeight)*_width];

		_genTail->next = new plasma_coord_node;
		_genTail = _genTail->next;
		_genTail->x1 = tmpWidth;
		_genTail->y1 = 0;
		_genTail->x2 = _width-1;
		_genTail->y2 = tmpHeight;
		_genTail->next = NULL;
	}

	if(bTileVertical) {
		// mirror about the X axis and add the corresponding region
		// C[0, _height-1] = C[0,0]
		_field[(_height-1)*_width] = _field[0];
		// C[tmpWidth, _height-1] = C[tmpWidth, 0]
		_field[tmpWidth+(_height-1)*_width] = _field[tmpWidth];

		_genTail->next = new plasma_coord_node;
		_genTail = _genTail->next;
		_genTail->x1 = 0;
		_genTail->y1 = tmpHeight;
		_genTail->x2 = tmpWidth;
		_genTail->y2 = _height-1;
		_genTail->next = NULL;
	}

	if(bTileHorizontal && bTileVertical) {
		// mirror about the X & Y axes and add the corresponding region
		// C[_width-1, _height-1] = C[0,0]
		_field[_height*_width-1] = _field[0];

		_genTail->next = new plasma_coord_node;
		_genTail = _genTail->next;
		_genTail->x1 = tmpWidth;
		_genTail->y1 = tmpHeight;
		_genTail->x2 = _width-1;
		_genTail->y2 = _height-1;
		_genTail->next = NULL;
	}

	return true;
}

/**
 * This method will complete one iteration of the algorithm.
 */
bool PlasmaAlgorithm::calc() {
	if(_genHead == NULL)
		return true;

	plasma_coord_node* current = _genHead;
		
	//LERP the four sides

	//top
	int xCoord = int(float(current->x2-current->x1)/2.0 + 0.5) + current->x1;
	int topColor = int(float(_field[current->y1*_width+current->x1] + _field[current->y1*_width+current->x2]) / 2.0 + 0.5);
	_field[current->y1*_width + xCoord] = topColor;

	//bottom
	xCoord = int(float(current->x2-current->x1)/2.0 + 0.5) + current->x1;
	int botColor = int(float(_field[current->y2*_width+current->x1] + _field[current->y2*_width+current->x2]) / 2.0 + 0.5);
	_field[current->y2*_width + xCoord] = botColor;

	//left
	int yCoord = int(float(current->y2-current->y1)/2.0 + 0.5) + current->y1;
	int leftColor = int(float(_field[current->y1*_width+current->x1] + _field[current->y2*_width+current->x1]) / 2.0 + 0.5);
	_field[current->x1 + _width*yCoord] = leftColor;

	//right
	yCoord = int(float(current->y2-current->y1)/2.0+0.5)+current->y1;
	int rightColor = int(float(_field[current->y1*_width+current->x2] + _field[current->y2*_width+current->x2]) / 2.0 + 0.5);
	_field[current->x2 + _width*yCoord] = rightColor;

	//center 
	xCoord = int(float(current->x2-current->x1)/2.0 + 0.5)+current->x1;
	yCoord = int(float(current->y2-current->y1)/2.0 + 0.5)+current->y1;

	//right now, implement as a simple LERP, then add in
	//the deviation
	//int fudgeFactor = int( float(rand() % (2*_numColors) - (_numColors)) * float(current->x2 - current->x1)/float(_width) * _coarseness);
    int fudgeFactor;
	//first, set the fudge factor to be a random number x:  -palWidth/2 <= x <= palWidth/2
	fudgeFactor = rand() % _numColors - (_numColors/2);
	//multiply in the coarseness factor and scale by the distance between two points divided by the size of the screen
	fudgeFactor = int( float(fudgeFactor) * float(current->x2 - current->x1)/float(_width) * _coarseness);

	_field[yCoord*_width + xCoord] = (topColor + botColor + leftColor + rightColor) / 4 + fudgeFactor;
	if(_field[yCoord*_width + xCoord] < 0)
	{
		if(_bClampColor)
		{
			_field[yCoord*_width + xCoord] = 0;
		}
		else
		{
			_field[yCoord*_width + xCoord] += _numColors;
		}
	}
	if(_field[yCoord*_width + xCoord] > _numColors-1)
	{
		if(_bClampColor)
		{
			_field[yCoord*_width + xCoord] = _numColors-1;
		}
		else
		{
			_field[yCoord*_width + xCoord] -= _numColors;
		}
	}

	//subdivide iff the x coords aren't adjacent or equal and the y coords
	//aren't adjacent or equal
	if((current->x1 < current->x2 - 2) || (current->y1 < current->y2-2))
	{
		//top left
		_genTail->next = new plasma_coord_node;
		_genTail = _genTail->next;
		_genTail->x1 = current->x1;
		_genTail->y1 = current->y1;
		_genTail->x2 = xCoord;
		_genTail->y2 = yCoord;

		//top right
		_genTail->next = new plasma_coord_node;
		_genTail = _genTail->next;
		_genTail->x1 = xCoord;
		_genTail->y1 = current->y1;
		_genTail->x2 = current->x2;
		_genTail->y2 = yCoord;

		//bottom left
		_genTail->next = new plasma_coord_node;
		_genTail = _genTail->next;
		_genTail->x1 = current->x1;
		_genTail->y1 = yCoord;
		_genTail->x2 = xCoord;
		_genTail->y2 = current->y2;

		//bottom right
		_genTail->next = new plasma_coord_node;
		_genTail = _genTail->next;
		_genTail->x1 = xCoord;
		_genTail->y1 = yCoord;
		_genTail->x2 = current->x2;
		_genTail->y2 = current->y2;

		//done adding
		_genTail->next = NULL;
	}

	//delete this one
	_genHead = _genHead->next;
	delete current;
	current = NULL;

	// check to see if we are at the end of the list.
	if(_genHead == NULL)
	{
		// yep, we're done!
		_genTail = NULL;
		return true;
	}

	// not done yet.
	return false;
}

/**
 * Calculates up to numIterations iterations of the algorithm.  If N <= 0,
 * algorithm continues until finished.
 */
bool PlasmaAlgorithm::calc(int numIterations) {
	for(int i=0; i<numIterations; i++) {
		if(calc())
			return true;
	}
	return false;
}

/**
 * This is an overloaded version of calc() that will block until the algorithm
 * has completed the generation of the plasma fractal.
 */
bool PlasmaAlgorithm::calcAll() {
	return calc(-1);
}

