#pragma once
#include "LightBulb.h"

#define LG_MODE_OFF				0
#define LG_MODE_JINGLE		1
#define LG_MODE_CHOEUR		2
#define LG_MODE_PAROLES		3
#define LG_MODE_INTERACT	4

class LightGrid {

	public:
		LightGrid(int nX, int nY);
		void setVal(int x, int y, int value);
		void setVal(int value);
		void animate();
		void draw();

		void stopAll();
		void modeJingle();
		void modeChoeur();

	private:
		LightBulb* bulbs[16][16];
		int _nX;
		int _nY;
		int _mode;
		int _param1;

		int ran(int min, int max);


};
