#pragma once
#include "LightBulb.h"

#define LG_MODE_OFF				0
#define LG_MODE_JINGLE		1
#define LG_MODE_CHOEUR		2
#define LG_MODE_PAROLES		3
#define LG_MODE_MANU			4

#define MAX_BANKS	16

class LightGrid {

	public:
		LightGrid(int nX, int nY);
		void setDmx(ofxOscSender& dmx);
		void setVal(int x, int y, int value);
		void setVal(int value);
		void animate();
		void draw();

		int getMode();
		void stopAll();
		void modeJingle();
		void modeChoeur();
		void modeParoles();
		void modeManu();
		void resetTrigs();

		int getParam(int i);
		void setParam(int i, int v);

		int getParam(int mode, int i);
		void setParam(int mode, int i, int v);


	private:
		LightBulb* bulbs[16][16];
		int _nX;
		int _nY;
		int _mode;
		int _param[16][16];

		bool _animTrig[MAX_BANKS][16][16];

		int ran(int min, int max);

		void resetParams();

};
