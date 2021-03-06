#pragma once

#include "ofMain.h"
#include "TFTtouch.h"
#include "LightGrid.h"
#include "ofxOsc.h"

class ofApp : public ofBaseApp {

public:
	void setup();
  void process();
	void update();
	void draw();

	void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	TFTTouch touch;

	LightGrid *grid;
	ofxOscSender dmx;
	ofxOscReceiver com;

};
