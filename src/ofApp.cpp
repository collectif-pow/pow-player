#include "ofApp.h"

using namespace std;

string settingsPath = ofToDataPath("/Users/bigx/Downloads/test/settings.xml", true);

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	ofHideCursor();
	// get conf
	conf.load(settingsPath);
	online = conf.getChild("online").getBoolValue();
	string videoName = conf.getChild("file").getValue();
	// video settings
	player.load("/Users/bigx/Downloads/test/movies/" + videoName);
	if (online) player.setPaused(true);
	else {
		player.setPaused(false);
		player.setLoopState(OF_LOOP_NORMAL);
	}
	shouldPlay = false;
	// udp settings
	if (online) {
		client.Create();
		client.Bind(8888);
		client.SetNonBlocking(true);
	}
	// rpi stuff
	#ifdef TARGET_OPENGLES
	setupGPIOs();
	#endif
}

//--------------------------------------------------------------
#ifdef TARGET_OPENGLES
void ofApp::setupGPIOs(){
	// setup wiring pi
	if(wiringPiSetup() == -1) {
	ofLogNotice(__func__) << "Error on wiringPi setup";
	}
	// relay pin
	pinMode(RELAY_PIN, OUTPUT);
	digitalWrite(RELAY_PIN, HIGH);
}
#endif

//--------------------------------------------------------------
void ofApp::updateOffline() {
	#ifndef TARGET_OPENGLES
	player.update();
	#endif
}

//--------------------------------------------------------------
void ofApp::updateOnline() {
	#ifndef TARGET_OPENGLES
	if (shouldPlay) player.update();
	#endif
	char data[10];
	client.Receive(data, 10);
	string msg = data;
	// start
	if (msg == "start") {
		shouldPlay = true;
		player.stop();
		player.setPaused(false);
		player.setLoopState(OF_LOOP_NONE);
	}
	// stop
	else if (msg == "stop") {
		shouldPlay = false;
		player.stop();
		player.setPaused(true);
	}
	// play
	else if (msg == "play") {
		shouldPlay = true;
		player.setPaused(false);
	}
	// pause
	else if (msg == "pause") {
		shouldPlay = true;
		player.setPaused(true);
	}
	else if (msg == "loop") {
		shouldPlay = true;
		player.stop();
		player.setPaused(false);
		player.setLoopState(OF_LOOP_NORMAL);
	}
}

//--------------------------------------------------------------
void ofApp::drawOffline() {
	player.draw(0, 0, ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::drawOnline() {
	if (shouldPlay) player.draw(0, 0, ofGetWidth(), ofGetHeight());
	else ofClear(0);
}

//--------------------------------------------------------------
void ofApp::update() {
	if (online) updateOnline();
	else updateOffline();
}

//--------------------------------------------------------------
void ofApp::draw() {
	if (online) drawOnline();
	else drawOffline();
}