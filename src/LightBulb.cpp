#include "ofMain.h"
#include "LightBulb.h"
#include <algorithm>    // std::max

// Member functions definitions including constructor
LightBulb::LightBulb(int x, int y, int size) { 
    _x = x;
    _y = y;
    _size = size;
    _centerX = x+size/2;
    _centerY = y+size/2;
    _rMax = size/2;
    _rMin = _rMax/3;
    _rStep = max(1,(_rMax-_rMin)/6);
    set(0);
}

void LightBulb::set(int value) {
	_val = value;
	_alpha = _val*4/(_rStep*3);
}

void LightBulb::draw() {
	for (int radius=_rMax; radius>_rMin; radius-=_rStep) {
		ofSetColor(255, _alpha);
		ofDrawCircle(_centerX, _centerY, radius);
	}
}
