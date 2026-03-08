#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	ofSetFullscreen(false);
	ofSetFrameRate(FPS);
	ofBackground(0);

}

//--------------------------------------------------------------
void ofApp::update(){
	gameManager.update();
}

//--------------------------------------------------------------
void ofApp::draw(){
	gameManager.draw();
}

//--------------------------------------------------------------
void ofApp::exit() {

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	gameManager.player.keyPressed(key);
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
	gameManager.player.keyReleased(key);
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
