#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofBackground(0,0,0);
    ofSetFrameRate(20);

    // TOUCH SCREEN
    touch.init(90);
    ofLog()<<touch.getName();

    // GRID
    grid = new LightGrid(6,3);
    grid->modeManu();
    //grid->modeChoeur(600, 255);

    // OSC COM
    com.setup(4000);

    // DMX
    dmx.setup("127.0.0.1", 3000);
    system("fuser -k 3000/udp; ~/HDmx/HDmx &");
    grid->setDmx(dmx);
}

//--------------------------------------------------------------
void ofApp::exit(){
    ofLogVerbose("ofApp") << "exit()";
}

//--------------------------------------------------------------
void ofApp::update(){

    while(com.hasWaitingMessages()) process();
    grid->animate();

    // Render DMX
    ofxOscMessage m;
    m.setAddress("/dmx/render");
    dmx.sendMessage(m);

    /*if (alpha < 256) grid->set(alpha);
    else grid->set(511-alpha);*/

    /*ofxOscMessage m;
    m.setAddress("/dmx/set");
    m.addIntArg(1);
    m.addIntArg(5);
    dmx.sendMessage(m);*/

	/*int Tx=touch.getCoordTouch().x;
	int Ty=touch.getCoordTouch().y;
	int Tp=touch.getCoordTouch().z;
	string p ="X:"+ofToString(Tx)+" Y:"+ofToString(Ty)+" Pressure:"+ofToString(Tp)+" FPS:"+ofToString(ofGetFrameRate());
	ofLog()<<p;	*/
}

//--------------------------------------------------------------
void ofApp::draw() {
	grid->draw();
}

void ofApp::process() {

	// get the next message
	ofxOscMessage m;
	com.getNextMessage(m);

  // MODE 0
	if(m.getAddress() == "/mode/off") {
    grid->stopAll();
  }

  /////////
  // MODE JINGLE
	else if(m.getAddress() == "/mode/jingle") {
    if (m.getNumArgs() >= 4) {
      grid->setParam(LG_MODE_JINGLE, 0, m.getArgAsInt32(0));
      grid->setParam(LG_MODE_JINGLE, 1, m.getArgAsInt32(1));
      grid->setParam(LG_MODE_JINGLE, 2, m.getArgAsInt32(2));
      grid->setParam(LG_MODE_JINGLE, 3, m.getArgAsInt32(3));
    }
    grid->modeJingle();  //50, 1000, 10, 255
  }

  // PARAMS JINGLE
  else if(m.getAddress() == "/params/jingle/lowfreq")
    {if (m.getNumArgs() >= 1) grid->setParam(LG_MODE_JINGLE, 0, m.getArgAsInt32(0));}

  else if(m.getAddress() == "/params/jingle/highfreq")
    {if (m.getNumArgs() >= 1) grid->setParam(LG_MODE_JINGLE, 1, m.getArgAsInt32(0));}

  else if(m.getAddress() == "/params/jingle/lowintens")
    {if (m.getNumArgs() >= 1) grid->setParam(LG_MODE_JINGLE, 2, m.getArgAsInt32(0));}

  else if(m.getAddress() == "/params/jingle/highintens")
    {if (m.getNumArgs() >= 1) grid->setParam(LG_MODE_JINGLE, 3, m.getArgAsInt32(0));}

  /////////
  // MODE CHOEUR
	else if(m.getAddress() == "/mode/choeur") {
    if (m.getNumArgs() >= 2) {
      grid->setParam(LG_MODE_CHOEUR, 1, m.getArgAsInt32(0));
      grid->setParam(LG_MODE_CHOEUR, 2, m.getArgAsInt32(1));
    }
    grid->modeChoeur();  //600, 255
  }

  // PARAMS CHOEUR
  else if(m.getAddress() == "/params/choeur/rate")
    {if (m.getNumArgs() >= 1) grid->setParam(LG_MODE_CHOEUR, 1, m.getArgAsInt32(0));}

  else if(m.getAddress() == "/params/choeur/intensity")
    {if (m.getNumArgs() >= 1) grid->setParam(LG_MODE_CHOEUR, 2, m.getArgAsInt32(0));}

  /////////
  // MODE PAROLES
	else if(m.getAddress() == "/mode/paroles") {
    if (m.getNumArgs() >= 2) {
      grid->setParam(LG_MODE_PAROLES, 1, m.getArgAsInt32(0));
      grid->setParam(LG_MODE_PAROLES, 2, m.getArgAsInt32(1));
    }
    grid->modeParoles();  // 300, 255
	}

  // PARAMS PAROLES
  else if(m.getAddress() == "/params/paroles/rate")
    {if (m.getNumArgs() >= 1) grid->setParam(LG_MODE_PAROLES, 1, m.getArgAsInt32(0));}

  else if(m.getAddress() == "/params/paroles/intensity")
    {if (m.getNumArgs() >= 1) grid->setParam(LG_MODE_PAROLES, 2, m.getArgAsInt32(0));}

  /////////
  // MODE MANU
	else if(m.getAddress() == "/mode/manu") {
    if (m.getNumArgs() >= 2) {
      grid->setParam(LG_MODE_MANU, 0, m.getArgAsInt32(0));
      grid->setParam(LG_MODE_MANU, 1, m.getArgAsInt32(1));
    }
    grid->modeManu();
	}

  // PARAMS MANU
  else if(m.getAddress() == "/params/manu/rate")
    {if (m.getNumArgs() >= 1) grid->setParam(LG_MODE_MANU, 0, m.getArgAsInt32(0));}

  else if(m.getAddress() == "/params/manu/intensity")
    {if (m.getNumArgs() >= 1) grid->setParam(LG_MODE_MANU, 1, m.getArgAsInt32(0));}

  else if(m.getAddress() == "/manu/dim")
    {if (m.getNumArgs() >= 1) grid->setParam(LG_MODE_MANU, 2, m.getArgAsInt32(0));}

  else if(m.getAddress() == "/manu/height")
    {if (m.getNumArgs() >= 1) grid->setParam(LG_MODE_MANU, 3, m.getArgAsInt32(0));}

  else if(m.getAddress() == "/manu/select") {
    if (m.getNumArgs() >= 1) grid->setParam(LG_MODE_MANU, 10, m.getArgAsInt32(0));
    else grid->setParam(LG_MODE_MANU, 10, 0);
    if (m.getNumArgs() >= 2) grid->setParam(LG_MODE_MANU, 11, m.getArgAsInt32(1));
    else grid->setParam(LG_MODE_MANU, 11, 0);
  }




}



/*
DEFAULTS OF STUFF
*/

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    ofLogVerbose("ofApp") << "keyPressed() key = " << key;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    ofLogVerbose("ofApp") << "keyReleased() key = " << key;
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    ofLogVerbose("ofApp") << "mouseMoved() x = " << x << "/" << y;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    ofLogVerbose("ofApp") << "mouseDragged() x = " << x << "/" << y << " button=" << button;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    ofLogVerbose("ofApp") << "mousePressed() x = " << x << "/" << y << " button=" << button;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    ofLogVerbose("ofApp") << "mouseReleased() x = " << x << "/" << y << " button=" << button;
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    ofLogVerbose("ofApp") << "windowResized() w = " << w  << " h = " << h;
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    ofLogVerbose("ofApp") << "gotMessage() msg.message = " << msg.message;
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    ofLogVerbose("ofApp") << "dragEvent()";
}
