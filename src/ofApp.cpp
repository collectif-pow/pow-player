#include "ofApp.h"

using namespace std;


//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(30);
	ofHideCursor();
	// get conf
	#ifdef TARGET_OPENGLES
	string settingsPath = ofToDataPath("/media/movies/settings.xml", true);
	#else
	string settingsPath = ofToDataPath("/Users/bigx/Downloads/test/settings.xml", true);
	#endif
	conf.load(settingsPath);
	online = conf.getChild("online").getBoolValue();
	string videoName = conf.getChild("file").getValue();
	// rs232 settings
	string port = conf.getChild("port").getValue();
	int baudRate = conf.getChild("baud").getIntValue();
	onMessage = conf.getChild("on").getValue();
	offMessage = conf.getChild("off").getValue();
	if (port.length() > 0 && onMessage.length() > 0 && offMessage.length() > 0 && baudRate > 0) {
		if (!device.setup(port, baudRate)) {
			ofLogNotice(__func__) << "Error on serial setup";
		}
	}
	// video settings
	#ifdef TARGET_OPENGLES
	player.omxPlayer.disableLooping();
	player.load("/media/movies/movies/" + videoName);
	#else
	player.load("/Users/bigx/Downloads/test/movies/" + videoName);
	#endif
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
}

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
		#ifdef TARGET_OPENGLES
		player.omxPlayer.restartMovie();
		#else
		player.stop();
		#endif
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
		player.play();
		player.setPaused(false);
	}
	// pause
	else if (msg == "pause") {
		shouldPlay = true;
		player.setPaused(true);
	}
	else if (msg == "loop") {
		shouldPlay = true;
		#ifdef TARGET_OPENGLES
		player.omxPlayer.restartMovie();
		#else
		player.stop();
		#endif
		player.play();
		player.setPaused(false);
		player.setLoopState(OF_LOOP_NORMAL);
	}
	else if (msg == "on") {
		ofx::IO::ByteBuffer textBuffer(onMessage);
		device.writeBytes(textBuffer);
		device.writeByte('\r');
	}
	else if (msg == "off") {
		ofx::IO::ByteBuffer textBuffer(offMessage);
		device.writeBytes(textBuffer);
		device.writeByte('\r');
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
