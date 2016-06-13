#pragma once

class LightBulb {

	public:
		void set(int value);
		void draw();
		LightBulb(int x, int y, int size);
	
	private:
		int _x;
		int _y;
		int _size;
		int _centerX;
		int _centerY;
		int _val;
		int _rMax;
		int _rMin;
		int _rStep;
		int _aFact100;
		int _alpha;
};
