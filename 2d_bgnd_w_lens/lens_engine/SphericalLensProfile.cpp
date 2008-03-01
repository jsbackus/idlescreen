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
#include "SphericalLensProfile.h"

SphericalLensProfile::SphericalLensProfile() {
	_xmlTypeValue = "SphericalLensProfile";

	_minRadius = float(0.075);
	_maxRadius = float(0.15);
	_distortionFactor = float(0.875);
	_bRandSize = true;
	_maxNumLenses = 5;
}

SphericalLensProfile::~SphericalLensProfile() {
}

/* Attempts to load this background profile object from the
 * specified QDomNode.
 */
LensProfile* SphericalLensProfile::load(QDomNode &node) {
	int tmpI;
	float tmpF;

	SphericalLensProfile* retVal = new SphericalLensProfile();
	retVal->loadBase(node);

	QDomElement tempElem;

	tempElem = node.firstChildElement("min_radius");
	if(!tempElem.isNull()) {
		tmpF = tempElem.text().toFloat();
		if(tmpF > 0.00 && tmpF < 1.00) {
			retVal->_minRadius = tmpF;
		}
	}

	tempElem = node.firstChildElement("max_radius");
	if(!tempElem.isNull()) {
		tmpF = tempElem.text().toFloat();
		if(tmpF > 0.00 && tmpF < 1.00) {
			retVal->_maxRadius = tmpF;
		}
	}

	tempElem = node.firstChildElement("distortion_factor");
	if(!tempElem.isNull()) {
		tmpF = tempElem.text().toFloat();
		if(tmpF > 0.00) {
			retVal->_distortionFactor = tmpF;
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
QDomNode SphericalLensProfile::save(QDomDocument* doc) {
	QString tempStr;
	QDomText tempNode;
	QDomElement tempElem;
	QDomElement retVal = doc->createElement(getXMLTagName());

	saveBase(doc, retVal);

	tempStr.setNum(_minRadius);
	tempNode = doc->createTextNode(tempStr);
	tempElem = doc->createElement("min_radius");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	tempStr.setNum(_maxRadius);
	tempNode = doc->createTextNode(tempStr);
	tempElem = doc->createElement("max_radius");
	tempElem.appendChild(tempNode);
	retVal.appendChild(tempElem);

	tempStr.setNum(_distortionFactor);
	tempNode = doc->createTextNode(tempStr);
	tempElem = doc->createElement("distortion_factor");
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
LensObject* SphericalLensProfile::getNewLensObject() {
	if(_screenObj == NULL)
		return NULL;

	int sizeX = _screenObj->_sizeX;
	int sizeY = _screenObj->_sizeY;
	float sizePercentX;
	float sizePercentY;

	if(sizeX < sizeY) {
		sizeY = sizeX;
	} else {
		sizeX = sizeY;
	}

	if(_bRandSize) {
		int minI, maxI;
		minI = int(float(sizeX)*_minRadius*2);
		maxI = int(float(sizeX)*_maxRadius*2);
		sizeY = sizeX = (rand() % (maxI - minI)) + minI;

		sizePercentX = float(sizeX) / float(_screenObj->_sizeX);
		sizePercentY = float(sizeY) / float(_screenObj->_sizeY);
	} else {
		sizeX = int(float(sizeX)*_maxRadius*2);
		sizePercentX = _maxRadius*2;

		sizeY = int(float(sizeY)*_maxRadius*2);
		sizePercentY = _maxRadius*2;
	}

	GenericLensObject* retVal = getNewGenericLens(sizePercentX, sizePercentY);

	int* field = NULL;
	bool* bitmap = NULL;

	retVal->getDataStructures(&bitmap, &field);

	//avoid div by zero!
	_distortionFactor = float(1.0) - _distortionFactor;
	if(_distortionFactor < 0.0001) {
		_distortionFactor = float(0.0001);
	}

	int radius = 0;

	//for simplicity..
	int screenX = _screenObj->_sizeX;
	float rho = _distortionFactor;  

	int xmod = sizeX - 4*_outlineWidth;
	int ymod = sizeY - 4*_outlineWidth;

	float rmag = float(( (xmod < ymod) ? (xmod*xmod) : (ymod*ymod) ) / (4.0 * (1.0 - rho*rho)));
	float d = rho * sqrt(rmag);
	double px, py, u, iz;
	float midX = float(float(sizeX)/2.0) - float(1.0);
	float midY = float(float(sizeY)/2.0) - float(1.0);

	if(field != NULL && bitmap != NULL)
	{
		for(int y=0;y<sizeY;y++)
		{
			int yLoc = y*sizeX;
			int yVal = y*screenX;
			for(int x=0;x<sizeX;x++)
			{
				float ix = float(x) - midX;
				float iy = float(y) - midY;
				iz = sqrt(rmag - ix*ix - iy*iy) - d;
				if(iz > 0.0)
				{
					if(radius <= 0)
					{
						radius = int(sqrt(ix*ix+iy*iy));
					}
					u = sqrt(rmag);
					u = d / sqrt (rmag - ix*ix - iy*iy);

					//calculate transformed pixel
					px = midX + u*ix;//(x-midX);
					py = midY + u*iy;//(y-midY);

					field[yLoc+x] = (int(py)*screenX + int(px))*4;
					bitmap[yLoc+x] = true;
				}
				else
				{
					//just copy straight
					field[yLoc+x] = (yVal+x)*4;
					bitmap[yLoc+x] = false;
				}
			}
		}

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
int SphericalLensProfile::getMaxNumLenses() {
	return _maxNumLenses;
}
