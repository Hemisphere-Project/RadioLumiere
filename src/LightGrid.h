#pragma once
#include "LightBulb.h"

class LightGrid {

	public:
		LightGrid(int nX, int nY);
		void setVal(int x, int y, int value);
		void setVal(int value);
		void animate();
		void draw();

	private:
		LightBulb* bulbs[16][16];
		int _nX;
		int _nY;

};
