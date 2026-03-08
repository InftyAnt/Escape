#define _CRTDBG_MAP_ALLOC

#include "ofMain.h"
#include "ofApp.h"
#include "Map.h"
//#include <crtdbg.h>
//========================================================================
int main( ){
	//_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	//_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	//_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	//_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
	//_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
	//_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);

	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	srand(time(0));

	ofGLFWWindowSettings settings;
	settings.setSize(1920, 1080);
	settings.windowMode = OF_WINDOW;
	ofLog() << "Creating window...";
	auto window = ofCreateWindow(settings);
	ofLog() << "Window created.";


	ofRunApp(window, make_shared<ofApp>());
	ofRunMainLoop();

	//_CrtDumpMemoryLeaks();

	return 0;

}
