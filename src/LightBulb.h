#pragma once
#include "ofxOsc.h"

#define LB_ANIM_OFF		0
#define LB_ANIM_RAMP	1
#define LB_ANIM_SIN		2
#define LB_ANIM_TRI		3
#define LB_ANIM_STEP	4


class LightBulb {

	public:
		void setDmx(ofxOscSender& dmx, int dmxchannel);
		void setVal(int value);
		void setAnim(int type, int period, int intensity, bool loop);
		void setAnim(int type);
		bool isRunning();
		void resetAnim();
		void animate();
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
		int _alpha;

		ofxOscSender _dmx;
		int _chan;

		int _anim_type;
		int _anim_period;
		int _anim_intensity;
		bool _anim_continue;

		int _cur_type;
		int _cur_period;
		int _cur_intensity;
	  uint64_t _cur_offset;
		float _cur_progress;

		void setValUnscalled(float value);

		bool _continue;

};
