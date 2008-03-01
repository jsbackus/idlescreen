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

#include "../../misc_funcs.h"
#include "DiamondLensProfile.h"

DiamondLensProfile::DiamondLensProfile() {
	_xmlTypeValue = "DiamondLensProfile";

	_minSizeX = float(0.15);
	_maxSizeX = float(0.30);
	_minSizeY = float(0.15);
	_maxSizeY = float(0.30);
	_bRandSize = true;
	_maxNumLenses = 5;
}

DiamondLensProfile::~DiamondLensProfile() {
}

/* Attempts to load this background profile object from the
 * specified QDomNode.
 */
LensProfile* DiamondLensProfile::load(QDomNode &node) {
	float tmpF;
	int tmpI;

	DiamondLensProfile* retVal = new DiamondLensProfile();
	retVal->loadBase(node);

	QDomElement tempElem;

	tempElem = node.firstChildElement("min_width");
	if(!tempElem.isNull()) {
		tmpF = tempElem.text().toFloat();
		if(tmpF > 0.00 && tmpF < 1.00) {
			retVal->_minSizeX = tmpF;
		}
	}

	tempElem = node.firstChildElement("max_width");
	if(!tempElem.isNull()) {
		tmpF = tempElem.text().toFloat();
		if(tmpF > 0.00 && tmpF < 1.00) {
			retVal->_maxSizeX = tmpF;
		}
	}

	tempElem = node.firstChildElement("min_height");
	if(!tempElem.isNull()) {
		tmpF = tempElem.text().toFloat();
		if(tmpF > 0.00 && tmpF < 1.00) {
			retVal->_minSizeY = tmpF;
		}
	}

	tempElem = node.firstChildElement("max_height");
	if(!tempElem.isNull()) {
		tmpF = tempElem.text().toFloat();
		if(tmpF > 0.00 && tmpF < 1.00) {
			retVal->_maxSizeY = tmpF;
		}
	}

	tempElem = node.firstChildElement("random_size");
	if(!tempElem.isNull()) {
		retVal->_bRandSize = stringToBool(tempElem.text());
	}

	tempElem = node.firstChildElement("instances_max");
	if(!tempElem.isNull()) {
		tmpI = tempElem.text().toInt();
		if(tmpI > 0 && tmpI < 10000) {
			retVal->_maxNumLenses = tmpI;
		}
	}

	return retVal;
}

/*
 * Returns a QDomNode object that represents this profile.
 */
QDomNode DiamondLensProfile::save(QDomDocument* doc) {
	QString tempStr;
	QDomText tempNode;
	QDomElement tempElem;
	QDomElement retVal = doc->createElement(getXMLTagName());

	saveBase(doc, retVal);

	tempStr.setNum(_minSizeX);
	tempNode = doc->createTextNode(tempStr);
	tempElem = doc->createElement("min_width");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	tempStr.setNum(_maxSizeX);
	tempNode = doc->createTextNode(tempStr);
	tempElem = doc->createElement("max_width");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	tempStr.setNum(_minSizeY);
	tempNode = doc->createTextNode(tempStr);
	tempElem = doc->createElement("min_height");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	tempStr.setNum(_maxSizeY);
	tempNode = doc->createTextNode(tempStr);
	tempElem = doc->createElement("max_height");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	tempNode = doc->createTextNode(boolToString(_bRandSize));
	tempElem = doc->createElement("random_size");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	tempStr.setNum(_maxNumLenses);
	tempNode = doc->createTextNode(tempStr);
	tempElem = doc->createElement("instances_max");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	return retVal;
}

/*
 * Creates and returns a new LensObject object.
 */
LensObject* DiamondLensProfile::getNewLensObject() {
	if(_screenObj == NULL)
		return NULL;

	int sizeX;
	int sizeY;
	float sizePercentX;
	float sizePercentY;

	if(_bRandSize) {
		int minI, maxI;
		minI = int(float(_screenObj->_sizeX)*_minSizeX);
		maxI = int(float(_screenObj->_sizeX)*_maxSizeX);
		sizeX = (rand() % (maxI - minI)) + minI;
		minI = int(float(_screenObj->_sizeY)*_minSizeY);
		maxI = int(float(_screenObj->_sizeY)*_maxSizeY);
		sizeY = (rand() % (maxI - minI)) + minI;

		sizePercentX = float(sizeX) / float(_screenObj->_sizeX);
		sizePercentY = float(sizeY) / float(_screenObj->_sizeY);
	} else {
		sizeX = int(float(_screenObj->_sizeX)*_maxSizeX);
		sizePercentX = _maxSizeX;

		sizeY = int(float(_screenObj->_sizeY)*_maxSizeY);
		sizePercentY = _maxSizeY;
	}

	GenericLensObject* retVal = getNewGenericLens(sizePercentX, sizePercentY);

	int* field = NULL;
	bool* bitmap = NULL;

	retVal->getDataStructures(&bitmap, &field);

	int screenX = _screenObj->_sizeX;

	if(field != NULL && bitmap != NULL) {
		//set up vertices
		//top
		int topX = sizeX / 2;
		int topY = sizeY - 1 - _outlineWidth;

		//left
		int leftX = _outlineWidth;
		int leftY = sizeY / 2;

		//right
		int rightX = sizeX - 1 - _outlineWidth;
		int rightY = sizeY / 2;

		//bottom
		int bottomX = sizeX / 2;
		int bottomY = _outlineWidth;

		//center
		int centerX = sizeX / 2;
		int centerY = sizeY / 2; 

		/*
		 * V = P - (point on the line)
		 * N = (normal to the line)
		 * V.N = |V|*|N|*cos(theta)
		 * cos(theta) = V.N / |V|*|N|
		 * if V.N/|V|*|N| is:
		 *	= 0: P on line
		 *	> 0: P on same side as normal "inside"
		 *	< 0: P on diff side as normal "outside"
		 */

		/* Since we are using the midpoints of all four sides as our
		 * vertices, the top left and bottom right sides are parallel
		 * and are perpendicular to the bottom left and top right sides.
		 * Thus we only need to calculate two normals, n1, the normal
		 * to the bottom left and top right sides (1st quadrant), and
		 * n2, the normal to the top left and bottom right sides
		 * (2nd quadrant).  We can then figure out which side the
		 * center is on for each line.  When we go through all of the
		 * points in the bounding region, we can compare with the
		 * results for the center point.
		 *
		 * As for the normals, since we are restricted to 2D,
		 * we can use: N of v(x,y) = (Vx,Vy) * [[0 1] [-1 0]] = (-Vy, Vx)
		 */

		float n1x = -1.0*(leftY - bottomY);
		float n1y = leftX - bottomX;

		float n2x = -1.0*(topY - leftY);
		float n2y = topX - leftX;

		/* Because we are only interested in the sign of the result,
		 * we don't need to divide by |V|*|N|.
		 */
		//top left
		int vX = centerX - leftX;
		int vY = centerY - leftY;
		float uTL = dotProduct(vX, vY, n2x, n2y);
		
		//bottom left
		float uBL = dotProduct(vX, vY, n1x, n1y);

		//top right
		vX = centerX - rightX;
		vY = centerY - rightY;
		float uTR = dotProduct(vX, vY, n1x, n1y);

		//bottom right
		float uBR = dotProduct(vX, vY, n2x, n2y);
		
		//process every point in the bounding region
		for(int y=0; y<sizeY; y++) {
			int yLoc = y*sizeX;
			int yVal = y*screenX;
			int yTL = y - leftY;
			int yBL = y - leftY;
			int yTR = y - rightY;
			int yBR = y - rightY;

			for(int x=0;x<sizeX;x++) {
				int xTL = x - leftX;
				int xBL = x - leftX;
				int xTR = x - rightX;
				int xBR = x - rightX;

				field[yLoc+x] = (yVal+x)*4;

				float vTL = dotProduct(xTL, yTL, n2x, n2y);
				float vBL = dotProduct(xBL, yBL, n1x, n1y);
				float vTR = dotProduct(xTR, yTR, n1x, n1y);
				float vBR = dotProduct(xBR, yBR, n2x, n2y);

				/* we'll multiply the u * v vectors together.
				 * if the result is positive (indicating like signs) or 0.00,
				 * then that point is part of the lens, otherwise it is
				 * outside.
				 */
				if(vTL*uTL >= 0.00 && vBL*uBL >= 0.00
					&& vTR*uTR >= 0.00 && vBR*uBR >= 0.00) {
						bitmap[yLoc+x] = true;
					} else {
						bitmap[yLoc+x] = false;
					}
			}
		}
/*
		//fill buffers
		int x1 = v2x - v1x;
		int y1 = v2y - v1y;
		int x2 = v3x - v1x;
		int y2 = v3y - v1y;
		float dot11 = dotProduct(x1,y1,x1,y1);
		float dot12 = dotProduct(x1,y1,x2,y2);
		float dot22 = dotProduct(x2,y2,x2,y2);
		float divisor = 1.0 / (dot11*dot22 - dot12*dot12);
		for(int y=0; y < sizeY; y++) {
			int yLoc = y*sizeX;
			int yVal = y*screenX;
			int y3 = y - v1y;
			for(int x=0; x<sizeX; x++) {

				field[yLoc+x] = (yVal+x)*4;

				int x3 = x - v1x;
				float dot13 = dotProduct(x1,y1,x3,y3);
				float dot23 = dotProduct(x2,y2,x3,y3);
				float u = (dot22*dot13 - dot12*dot23) * divisor;
				float v = (dot11*dot23 - dot12*dot13) * divisor;
				if(u > 0.0 && v > 0.0 && u + v < 1) {
					bitmap[yLoc+x] = true;
				} else {
					bitmap[yLoc+x] = false;
				}
			}
		}
*/
		//lastly, calculate outline
		retVal->calculateOutline();

		field = NULL;
		bitmap = NULL;
	}
	return retVal;
}

/*
 * Returns the maximum number of lenses this profile will allow.
 */
int DiamondLensProfile::getMaxNumLenses() {
	return _maxNumLenses;
}
