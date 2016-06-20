#include "ofMain.h"
#include "LightGrid.h"
#include <algorithm>    // std::max

// Member functions definitions including constructor
LightGrid::LightGrid(int nX, int nY) {

	_nX = min(16,nX);
	_nY = min(16,nY);
	_mode = LG_MODE_OFF;

	resetParams();

	// Default JINGLE params
	setParam(LG_MODE_JINGLE, 0, 50);		// flick rate min
	setParam(LG_MODE_JINGLE, 1, 1000);	// flick rate max
	setParam(LG_MODE_JINGLE, 2, 10);		// intens min
	setParam(LG_MODE_JINGLE, 3, 255);		// intens max

	// Default CHOEUR params
	setParam(LG_MODE_CHOEUR, 1, 600);	// flick rate
	setParam(LG_MODE_CHOEUR, 2, 255);	// intens max

	// Default CHOEUR params
	setParam(LG_MODE_PAROLES, 1, 300);	// flick rate
	setParam(LG_MODE_PAROLES, 2, 255);	// intens max

	// Default MANU params
	setParam(LG_MODE_MANU, 0, 300); 	// flick rate
	setParam(LG_MODE_MANU, 1, 255);		// intens max
	setParam(LG_MODE_MANU, 2, 100);		// intens %
	setParam(LG_MODE_MANU, 3, 3);			// height

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

	// MODE MANU
	else if (_mode == LG_MODE_MANU) {
		int modBright = getParam(1)*getParam(2)/100;

		// Interval
		int x1 = getParam(10);
		int x2;
		if (getParam(11) > 0) x2 = getParam(11);
		else x2 = x1;
		int ANIM = LB_ANIM_OFF;
		bool restart = (x1 > 0 && !bulbs[x1-1][(_nY-1)]->isRunning());

		for (int x=0; x<_nX; x++)
			if (x1 > 0 && x >= (x1-1) && x<x2) {
				if (restart)
					for (int y=0; y<_nY; y++) {
						if (y>=(_nY-getParam(3))) ANIM = LB_ANIM_TRI;
						else ANIM = LB_ANIM_OFF;
						
						bulbs[x][y]->setAnim(ANIM, getParam(0), modBright, false);
						bulbs[x][y]->resetAnim();
						bulbs[x+3][y]->setAnim(ANIM, getParam(0), modBright, false);
						bulbs[x+3][y]->resetAnim();
					}
			}
			else
				for (int y=0; y<_nY; y++)
					bulbs[x][y]->setAnim(LB_ANIM_SIN,
											ran(getParam(LG_MODE_JINGLE,0), getParam(LG_MODE_JINGLE,1)),
											ran(getParam(LG_MODE_JINGLE,2), modBright/3), false);

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

int LightGrid::getMode() {
	return _mode;
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

void LightGrid::modeJingle() {
	if (_mode == LG_MODE_JINGLE) return;
	stopAll();
	_mode = LG_MODE_JINGLE;
}

void LightGrid::modeChoeur() {
	if (_mode == LG_MODE_CHOEUR) return;
	stopAll();
	_mode = LG_MODE_CHOEUR;
	resetTrigs();

	//CROSS
	_animTrig[0][0][0] = true;
	_animTrig[0][2][0] = true;
	_animTrig[0][3][0] = true;
	_animTrig[0][5][0] = true;
	_animTrig[0][1][1] = true;
	_animTrig[0][4][1] = true;
	_animTrig[0][0][2] = true;
	_animTrig[0][2][2] = true;
	_animTrig[0][3][2] = true;
	_animTrig[0][5][2] = true;

	setParam(0, 0);
}

void LightGrid::modeParoles() {
	if (_mode == LG_MODE_PAROLES) return;
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
}

void LightGrid::modeManu() {
	if (_mode == LG_MODE_MANU) return;
	stopAll();
	_mode = LG_MODE_MANU;

	/*setParam(10, 0);
	setParam(11, 0);*/
}
