#include "ofMain.h"
#include "LightBulb.h"
#include <algorithm>    // std::max
#include <math.h>

// Member functions definitions including constructor
LightBulb::LightBulb(int x, int y, int size) {
    _x = x;
    _y = y;
    _size = size;
    _centerX = x+size/2;
    _centerY = y+size/2;
    _rMax = size/2;
    _rMin = _rMax/3;
    _rStep = max(1,(_rMax-_rMin)/4);

    _continue = false;
    setVal(0);
    setAnim(LB_ANIM_OFF, 1000, 255, false);
    resetAnim();
}

void LightBulb::setDmx(ofxOscSender& dmx, int dmxchannel) {
  _dmx = dmx;
  _chan = dmxchannel;
  ofLog()<<"set channel: "<<dmxchannel;
}

void LightBulb::setVal(int value) {
	_val = value;
	_alpha = _val*4/(_rStep*3);

  if (_chan ) {
    ofxOscMessage m;
    m.setAddress("/dmx/buffer");
    m.addIntArg(_chan);
    m.addIntArg(_val);
    //if (_chan == 1) ofLog()<<"set value: "<<_val;
    _dmx.sendMessage(m);
  }
}

void LightBulb::setValUnscalled(float value) {
	setVal( max(0, min(255, int(round(value*_cur_intensity)) )) );
}

void LightBulb::setAnim(int type) {
  setAnim(type, _anim_period, _anim_intensity, _anim_continue);
}

void LightBulb::setAnim(int type, int period, int intensity, bool loop) {
  _anim_type = type;
  _anim_period = period;
  _anim_intensity = intensity;
  _anim_continue = loop;
  if (!isRunning()) resetAnim();
}

void LightBulb::resetAnim(){
  _cur_type = _anim_type;
  _cur_period = _anim_period;
  _cur_intensity = _anim_intensity;
  _continue = _anim_continue;
  _cur_offset = ofGetElapsedTimeMillis();
  _cur_progress = 0.0;
  if (_cur_type == LB_ANIM_OFF) setVal(0);
}

void LightBulb::animate() {
  if (_cur_type == LB_ANIM_OFF) return;

  // PROGRESS
  _cur_progress = ((ofGetElapsedTimeMillis() - _cur_offset)) * 1.0 / _cur_period;
  if (_cur_progress > 1.0) {
    if (!_continue) {setAnim(LB_ANIM_OFF); setVal(0);}
    resetAnim();
  }

  // RAMP
  if (_cur_type == LB_ANIM_RAMP) setValUnscalled( _cur_progress );

  // SIN
  else if (_cur_type == LB_ANIM_SIN) setValUnscalled( (1-cos(_cur_progress*6.283))/2 );

  // TRI
  else if (_cur_type == LB_ANIM_TRI) {
    if (_cur_progress <= 0.5) setValUnscalled( _cur_progress*2 );
    else setValUnscalled( (1-_cur_progress)*2 );
  }

  // STEP
  else if (_cur_type == LB_ANIM_STEP) {
    if (_cur_progress <= 0.5) setValUnscalled( 0.0 );
    else setValUnscalled( 1.0 );
  }
}

bool LightBulb::isRunning() {
  return (_cur_type != LB_ANIM_OFF);
}

void LightBulb::draw() {
	for (int radius=_rMax; radius>_rMin; radius-=_rStep) {
		ofSetColor(255, _alpha);
		ofDrawCircle(_centerX, _centerY, radius);
	}
}
