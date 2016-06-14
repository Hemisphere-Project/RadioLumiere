#include "ofMain.h"
#include "LightGrid.h"
#include <algorithm>    // std::max

// Member functions definitions including constructor
LightGrid::LightGrid(int nX, int nY) {

	_nX = min(16,nX);
	_nY = min(16,nY);
	_mode = LG_MODE_OFF;
	_param1 = 0;

	int size = min( (ofGetWidth()/_nX), (ofGetHeight()/_nY) );

	for (int x=0; x<_nX; x++)
		for (int y=0; y<_nY; y++)
			bulbs[x][y] = new LightBulb(size*x, size*y, size);

}

void LightGrid::setVal(int x, int y, int value) {
	if (x < _nX && y < _nY)	bulbs[x][y]->setVal(value);
}

void LightGrid::setVal(int value) {
	for (int x=0; x<_nX; x++)
		for (int y=0; y<_nY; y++)
			bulbs[x][y]->setVal(value);
}

void LightGrid::animate() {

	// MODE JINGLE
	if (_mode == LG_MODE_JINGLE) {
		for (int x=0; x<_nX; x++)
			for (int y=0; y<_nY; y++)
				if (!bulbs[x][y]->isRunning())
					bulbs[x][y]->setAnim(LB_ANIM_SIN, ran(100,2000), ran(20,255), false);
	}

	// MODE CHOEUR
	else if (_mode == LG_MODE_CHOEUR) {
		// Center not running => cycle done
		if (!bulbs[1][1]->isRunning()) {
			int rate = 1000;
			// Cross
			if (_param1 == 0) {
				bulbs[1][1]->setAnim(LB_ANIM_SIN, rate, 255, false);
			}
			// Plus
			else {

			}
			_param1 = (_param1 + 1)%2;
		}

	}

	for (int x=0; x<_nX; x++)
		for (int y=0; y<_nY; y++)
			bulbs[x][y]->animate();
}

void LightGrid::draw() {
	for (int x=0; x<_nX; x++)
		for (int y=0; y<_nY; y++)
			bulbs[x][y]->draw();
}

int LightGrid::ran(int min, int max) {
	int randNum = rand()%(max-min + 1) + min;
	return randNum;
}

void LightGrid::stopAll() {
	for (int x=0; x<_nX; x++)
		for (int y=0; y<_nY; y++)
			bulbs[x][y]->setAnim(LB_ANIM_OFF);
	setVal(0);
}

void LightGrid::modeJingle() {
	stopAll();
	_mode = LG_MODE_JINGLE;
}

void LightGrid::modeChoeur() {
	stopAll();
	_mode = LG_MODE_CHOEUR;
	_param1 = 0;
}
