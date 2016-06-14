#include "ofMain.h"
#include "LightGrid.h"
#include <algorithm>    // std::max

// Member functions definitions including constructor
LightGrid::LightGrid(int nX, int nY) {

	_nX = min(16,nX);
	_nY = min(16,nY);
	_mode = LG_MODE_OFF;
	resetParams();

	int size = min( (ofGetWidth()/_nX), (ofGetHeight()/_nY) );

	for (int x=0; x<_nX; x++)
		for (int y=0; y<_nY; y++)
			bulbs[x][y] = new LightBulb(size*x, size*y, size);

}

void LightGrid::setDmx(ofxOscSender& dmx) {
	for (int x=0; x<_nX; x++)
		for (int y=0; y<_nY; y++)
			bulbs[x][y]->setDmx(dmx, y*16+x+1);
}

void LightGrid::setVal(int x, int y, int value) {
	if (x < _nX && y < _nY && x >= 0 && y >= 0)	bulbs[x][y]->setVal(value);
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
					bulbs[x][y]->setAnim(LB_ANIM_SIN,
								ran(getParam(0), getParam(1)),
								ran(getParam(2), getParam(3)), false);
	}

	// MODE CHOEUR
	else if (_mode == LG_MODE_CHOEUR) {
		// Center not running => cycle done: restart
		if (!bulbs[1][1]->isRunning()) {
			bool animBulbs[16][16] = {false};

			// Cross
			if (getParam(0) == 0) {
				animBulbs[1][0] = true;
				animBulbs[4][0] = true;
				animBulbs[0][1] = true;
				animBulbs[1][1] = true;
				animBulbs[2][1] = true;
				animBulbs[3][1] = true;
				animBulbs[4][1] = true;
				animBulbs[5][1] = true;
				animBulbs[1][2] = true;
				animBulbs[4][2] = true;
			}
			// Plus
			else {
				animBulbs[0][0] = true;
				animBulbs[2][0] = true;
				animBulbs[3][0] = true;
				animBulbs[5][0] = true;
				animBulbs[1][1] = true;
				animBulbs[4][1] = true;
				animBulbs[0][2] = true;
				animBulbs[2][2] = true;
				animBulbs[3][2] = true;
				animBulbs[5][2] = true;
			}
			for (int x=0; x<_nX; x++)
				for (int y=0; y<_nY; y++)
					if (animBulbs[x][y]) bulbs[x][y]->setAnim(LB_ANIM_TRI, getParam(1), getParam(2), false);
					else bulbs[x][y]->setAnim(LB_ANIM_OFF);

			setParam(0, (getParam(0) + 1)%2);
		}

	}

	for (int x=0; x<_nX; x++)
		for (int y=0; y<_nY; y++)
			bulbs[x][y]->animate();
}

void LightGrid::draw() {
	//ofLog()<<"draw grid "<<_mode;
	for (int x=0; x<_nX; x++)
		for (int y=0; y<_nY; y++)
			bulbs[x][y]->draw();
}

int LightGrid::ran(int min, int max) {
	int randNum = rand()%(max-min + 1) + min;
	return randNum;
}

void LightGrid::resetParams() {
	for (int i=0; i<16; i++)
	 	for (int j=0; j<16; j++)
			_param[i][j] = 0;
}

void LightGrid::setParam(int i, int v) {
	_param[_mode][i] = v;
}

int LightGrid::getParam(int i) {
	return _param[_mode][i];
}

void LightGrid::stopAll() {
	_mode = LG_MODE_OFF;
	for (int x=0; x<_nX; x++)
		for (int y=0; y<_nY; y++) {
			bulbs[x][y]->setAnim(LB_ANIM_OFF);
			bulbs[x][y]->resetAnim();
		}
	setVal(0);
}

void LightGrid::modeJingle(int lowFreq, int highFreq, int lowIntens, int highIntens) {
	stopAll();
	_mode = LG_MODE_JINGLE;

	setParam(0, lowFreq);
	setParam(1, highFreq);
	setParam(2, lowIntens);
	setParam(3, highIntens);
}

void LightGrid::modeChoeur(int rate, int intensity) {
	stopAll();
	_mode = LG_MODE_CHOEUR;

	setParam(0, 0);
	setParam(1, rate);
	setParam(2, intensity);
}
