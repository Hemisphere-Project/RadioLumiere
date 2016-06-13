#include "ofMain.h"
#include "LightGrid.h"
#include <algorithm>    // std::max

// Member functions definitions including constructor
LightGrid::LightGrid(int nX, int nY) { 

	_nX = min(16,nX);
	_nY = min(16,nY);
	
	int size = min( (ofGetWidth()/_nX), (ofGetHeight()/_nY) );

	for (int x=0; x<_nX; x++) 
		for (int y=0; y<_nY; y++)
			bulbs[x][y] = new LightBulb(size*x, size*y, size);

}

void LightGrid::set(int x, int y, int value) {
	if (x < _nX && y < _nY)	bulbs[x][y]->set(value);
}

void LightGrid::set(int value) {
	for (int x=0; x<_nX; x++) 
		for (int y=0; y<_nY; y++)
			bulbs[x][y]->set(value);
}

void LightGrid::draw() {
	for (int x=0; x<_nX; x++) 
		for (int y=0; y<_nY; y++)
			bulbs[x][y]->draw();
}
