#define _CRTDBG_MAP_ALLOC

#include "ofMain.h"
#include "ofApp.h"

int main(){

	ofGLFWWindowSettings settings;
	settings.setSize(1920, 1080);
	settings.windowMode = OF_WINDOW;
	ofLog() << "Creating window...";
	auto window = ofCreateWindow(settings);
	ofLog() << "Window created.";


	ofRunApp(window, make_shared<ofApp>());
	ofRunMainLoop();

	return 0;

}
