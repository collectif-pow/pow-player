#pragma once

#include "ofMain.h"
#include "ofxNetwork.h"
#ifdef TARGET_OPENGLES
#include "wiringPi.h"
#include "ofRPIVideoPlayer.h"
#endif

#define INTERVAL 300

class ofApp : public ofBaseApp {
public:
	void setup();
#ifdef TARGET_OPENGLES
	void setupGPIOs();
#endif
	void update();
	void draw();
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

	const static int RELAY_PIN = 7;
};
