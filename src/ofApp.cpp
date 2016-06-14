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
    //grid->modeJingle(10, 2000, 30, 255);
    grid->modeChoeur(1000, 255);

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

  // MODE 1
	else if(m.getAddress() == "/mode/jingle") {
    grid->modeJingle(10, 2000, 30, 255);
  }

  // MODE 2
	else if(m.getAddress() == "/mode/choeur") {
    grid->modeChoeur(1000, 255);
  }

  // MODE 3
	else if(m.getAddress() == "/mode/paroles") {

	}
  // MODE 4
	else if(m.getAddress() == "/mode/interactif") {

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
