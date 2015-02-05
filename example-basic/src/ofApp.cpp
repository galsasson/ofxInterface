#include "ofApp.h"


using namespace ofxInterface;


//--------------------------------------------------------------
void ofApp::setup(){

	ofEnableAlphaBlending();
	ofEnableSmoothing();
	ofBackground(0);

	/******
	 * setup the scene with width and height
	 */
	scene = new Node();
	scene->setSize(ofGetWidth(), ofGetHeight());
	scene->setName("Scene");

	/******
	 * this is the touch manager, give him the scene to set it up
	 */
	TouchManager::one().setup(scene);


	/******
	 * create custom gui components in a grid and add them to the scene
	 */
	float size = 40;
	float pad = 40;
	int perRow = 10;
	for (int i=0; i<perRow*4; i++) {

		// setup them in a grid
		float x = 150 + (i%perRow)*(size+pad);
		float y = 250 + (i/perRow)*(size+pad);

		// create a ButtonExample node
		ButtonExample *btn = new ButtonExample();
		btn->setup(x, y, 40+size-ofRandom(20), size+ofRandom(30));
		btn->setName("btn" + ofToString(i));

		// add it to the scene
		scene->addChild(btn);

		if (i%perRow>0) {
			// this can be called to place nodes next to each other
			btn->placeNextTo(*buttons[i-1], Node::RIGHT);
		}

		// keep reference (we need it to update the nodes)
		buttons.push_back(btn);
	}
}

//--------------------------------------------------------------
void ofApp::update(){

	/******
	 * update the touch manager,
	 */
	TouchManager::one().update();


	/******
	 * update the nodes (optional, only if you have an update)
	 */
	for (int i=0; i<buttons.size(); i++)
	{
		buttons[i]->update();
	}


}

//--------------------------------------------------------------
void ofApp::draw(){
	/******
	 * this will render the scene
	 * 1. this function takes all the visible nodes
	 * 2. sort them by 'plane' float
	 * 3. transform into the local space of each node
	 * 4. calls the 'draw' function on each node
	 */
	scene->render();

	/******
	 * if you want debug rendering
	 */
	if (bShowDebug) {
		scene->renderDebug();
	}



	ofSetColor(255);
	ofDrawBitmapString("hit 'd' to toggle debug rendering", 5, ofGetHeight()-8);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	/******
	 * toggle debug render
	 */

	switch (key) {
		case 'd':
			bShowDebug = !bShowDebug;
			break;
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

	/******
	 * pass touch/mouse events to the touch manager
	 */
	TouchManager::one().touchMove(button, ofVec2f(x, y));
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

	/******
	 * pass touch/mouse events to the touch manager
	 */
	TouchManager::one().touchDown(button, ofVec2f(x, y));

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

	/******
	 * pass touch/mouse events to the touch manager
	 */
	TouchManager::one().touchUp(button, ofVec2f(x, y));
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
