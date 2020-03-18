#include "ofApp.h"

using namespace ofxInterface;

//--------------------------------------------------------------
void ofApp::setup(){

	// create asset manager and load assets from "assets.json"
	assets = make_shared<ofxAssetManager>(ofxAssetManager());
	assets->setup();

	// create a base Node
	scene = new Node();
	scene->setSize(ofGetWidth(), ofGetHeight());
	ofxInterface::TouchManager::one().setup(scene);

	// create the scene from scene definition
	GuiFactory factory;
	factory.setup(ofLoadJson("guiElements.json"), assets);
	factory.createElements(scene, ofLoadJson("gui.json"));

	// register key events from soft keyboard to input
	// use static_cast to access special element functions
	static_cast<TextInput*>(scene->getChildWithName("input"))->registerKeyInput(static_cast<SoftKeyboard*>(scene->getChildWithName("keyboard"))->keyPressed);

	// you can use lambda functions to create a callback for each node
	// touchUp, touchDown, touchMoved and clicked are callbacks for every node 
	// we add a callback for the button changing the button description
	scene->getChildWithName("button")->setTouchDownFunction([this](ofxInterface::TouchEvent& t) {
		auto label = static_cast<Label*>(scene->getChildWithName("button")->getChildWithName("description"));
		label->setText("clicked");
	});
	scene->getChildWithName("button")->setTouchUpFunction([this](ofxInterface::TouchEvent& t) {
		auto label = static_cast<Label*>(scene->getChildWithName("button")->getChildWithName("description"));
		label->setText("a button");
	});
}

//--------------------------------------------------------------
void ofApp::update(){
	scene->update(ofGetLastFrameTime());
}

//--------------------------------------------------------------
void ofApp::draw(){
	scene->render();
	
	if (isDebug) {
		scene->renderDebug();
	}
	
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'd') {
		isDebug = !isDebug;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
	ofxInterface::TouchManager::one().touchMove(0, ofVec2f(x, y));
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	ofxInterface::TouchManager::one().touchDown(0, ofVec2f(x, y));
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
	ofxInterface::TouchManager::one().touchUp(0, ofVec2f(x, y));
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
