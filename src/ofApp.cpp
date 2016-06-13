#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofBackground(0,0,0);

    //rpiLogo.loadImage("Raspi_Colour_R.png");
    alpha = 0;

    touch.init(90);
    ofLog()<<touch.getName();

    grid = new LightGrid(6,3);

    // DMX
    dmx.setup("127.0.0.1", 3000);
    system("fuser -k 3000/udp; ~/HDmx/HDmx &");
}

//--------------------------------------------------------------
void ofApp::exit(){
    ofLogVerbose("ofApp") << "exit()";
}

//--------------------------------------------------------------
void ofApp::update(){

    alpha += 3;
    alpha = alpha % 512;

    grid->animate();

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

    /*if (alpha < 256) ofSetColor(255,255,255, alpha);
    else ofSetColor(255,255,255, 511-alpha);

    ofPushMatrix();
    ofTranslate(ofGetWidth()/2.0f,ofGetHeight()/2.0f);
    ofRotateZ(angle);
    ofEnableAlphaBlending();
    rpiLogo.draw(-rpiLogo.getWidth()/2.0f,-rpiLogo.getHeight()/2.0f);
    ofDisableAlphaBlending();
    ofPopMatrix();
	*/

	grid->draw();

	/*int maxRadius = 30;
	int minRadius = 10;
	int radiusStepSize = 5;
	int a = alpha * radiusStepSize / (maxRadius-minRadius);  // Increase for a more opaque brush
	// draw smaller and smaller circles and layering (increasing) opaqueness
	for (int radius=maxRadius; radius>minRadius; radius-=radiusStepSize) {
		ofSetColor(255, a);
		ofDrawCircle(150, 150, radius);
	}*/


}

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
