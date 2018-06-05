#pragma once

#include "ofMain.h"
#include "ofxNetwork.h"
#include "ofxSerial.h"
#ifdef TARGET_OPENGLES
#include "wiringPi.h"
#include "ofRPIVideoPlayer.h"
#endif

class ofApp : public ofBaseApp {
public:
	void setup();
#ifdef TARGET_OPENGLES
	void setupGPIOs();
#endif
	void update();
	void draw();
	void keyPressed(int key);
	void updateOffline();
	void updateOnline();
	void drawOffline();
	void drawOnline();
	#ifdef TARGET_OPENGLES
	ofRPIVideoPlayer player;
	#else
	ofVideoPlayer player;
	#endif
	ofPath path;
	ofxUDPManager client;
	ofXml conf;
	bool online;
	bool shouldPlay;

	ofx::IO::SerialDevice device;
	string onMessage;
	string offMessage;

	const static int RELAY_PIN = 7;
};
