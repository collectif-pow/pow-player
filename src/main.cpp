#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main() {
#ifdef TARGET_OPENGLES
	ofGLESWindowSettings settings;
	settings.setGLESVersion(2);
#else
	ofGLWindowSettings settings;
	settings.setGLVersion(3,2);
#endif
	// settings.setSize(800, 600);
	settings.windowMode = OF_FULLSCREEN;
	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}
