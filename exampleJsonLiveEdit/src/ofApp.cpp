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

	// add the file watcher to recreate scene when file changed 
	ofAddListener(watcher.events.onItemModified, this, &ofApp::onGuiJsonChanged);
	watcher.addPath(ofToDataPath("", true), true, &fileFilter);
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

void ofApp::onGuiJsonChanged(const ofxIO::DirectoryWatcherManager::DirectoryEvent & evt)
{
	// delete all children from base node
	while (scene->getChildren().size() >0)
	{
		scene->removeChild(0);
	}

	// create new elements from definition
	GuiFactory factory;
	factory.setup(ofLoadJson("guiElements.json"), assets);
	factory.createElements(scene, ofLoadJson("gui.json"));
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
