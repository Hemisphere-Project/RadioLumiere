#pragma once
#include "LightBulb.h"

class LightGrid {

	public:
		LightGrid(int nX, int nY);
		void set(int x, int y, int value);
		void set(int value);
		void draw();
	
	private:
		LightBulb* bulbs[16][16];
		int _nX;
		int _nY;

};
