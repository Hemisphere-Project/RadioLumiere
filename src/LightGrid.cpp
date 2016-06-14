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
		if (!bulbs[1][1]->isRunning())
		{
			for (int x=0; x<_nX; x++)
				for (int y=0; y<_nY; y++)
					if (_animTrig[getParam(0)][x][y]) bulbs[x][y]->setAnim(LB_ANIM_TRI, getParam(1), getParam(2), false);
					else bulbs[x][y]->setAnim(LB_ANIM_OFF);

			// Select next trig bank
			setParam(0, (getParam(0) + 1)%2);
		}
	}

	// MODE PAROLES
	else if (_mode == LG_MODE_PAROLES) {
		bool isRunning = false;
		for (int x=0; x<_nX; x++)
			for (int y=0; y<_nY; y++)
				if (_animTrig[getParam(0)][x][y] && bulbs[x][y]->isRunning()) {
					isRunning = true;
					break;
					break;
				}
		if (!isRunning) {
			setParam(0, (getParam(0) + 1)%12);
			for (int x=0; x<_nX; x++)
				for (int y=0; y<_nY; y++)
					if (_animTrig[getParam(0)][x][y]) bulbs[x][y]->setAnim(LB_ANIM_TRI, getParam(1), getParam(2), false);
					else bulbs[x][y]->setAnim(LB_ANIM_OFF);
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

void LightGrid::resetTrigs() {
	for (int i=0; i<16; i++)
	 	for (int j=0; j<16; j++)
			for (int k=0; k<MAX_BANKS; k++)
				_animTrig[i][j][k] = false;
}

void LightGrid::resetParams() {
	for (int i=0; i<16; i++)
	 	for (int j=0; j<16; j++)
			_param[i][j] = 0;
}

void LightGrid::setParam(int mode, int i, int v) {
	_param[mode][i] = v;
}

void LightGrid::setParam(int i, int v) {
	_param[_mode][i] = v;
}

int LightGrid::getParam(int i) {
	return _param[_mode][i];
}

int LightGrid::getParam(int mode, int i) {
	return _param[mode][i];
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

	resetTrigs();
	//CROSS
	_animTrig[0][1][0] = true;
	_animTrig[0][4][0] = true;
	_animTrig[0][0][1] = true;
	_animTrig[0][1][1] = true;
	_animTrig[0][2][1] = true;
	_animTrig[0][3][1] = true;
	_animTrig[0][4][1] = true;
	_animTrig[0][5][1] = true;
	_animTrig[0][1][2] = true;
	_animTrig[0][4][2] = true;

	//PLUS
	_animTrig[1][0][0] = true;
	_animTrig[1][2][0] = true;
	_animTrig[1][3][0] = true;
	_animTrig[1][5][0] = true;
	_animTrig[1][1][1] = true;
	_animTrig[1][4][1] = true;
	_animTrig[1][0][2] = true;
	_animTrig[1][2][2] = true;
	_animTrig[1][3][2] = true;
	_animTrig[1][5][2] = true;

	setParam(0, 0);
	setParam(1, rate);
	setParam(2, intensity);
}

void LightGrid::modeParoles(int rate, int intensity) {
	stopAll();
	_mode = LG_MODE_PAROLES;

	resetTrigs();
	_animTrig[0][0][0] = true;
	_animTrig[1][1][1] = true;
	_animTrig[2][2][2] = true;
	_animTrig[3][3][2] = true;
	_animTrig[4][4][1] = true;
	_animTrig[5][5][0] = true;

	_animTrig[6][0][2] = true;
	_animTrig[7][1][1] = true;
	_animTrig[8][2][0] = true;
	_animTrig[9][3][0] = true;
	_animTrig[10][4][1] = true;
	_animTrig[11][5][2] = true;

	setParam(0, 0);
	setParam(1, rate);
	setParam(2, intensity);
}
