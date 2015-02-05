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
	 * this is the touch manager, give it the scene to set it up
	 */
	TouchManager::one().setup(scene);


	/******
	 * create custom gui components in a grid and add them to the scene
	 */
	float size = 40;
	float pad = 40;
	int perRow = 10;
	for (int i=0; i<perRow*4; i++) {

		float x = 150 + (i%perRow)*(size+pad);
		float y = 250 + (i/perRow)*(size+pad);
		ButtonExample *btn = new ButtonExample();
		btn->setup(x, y, 40+size-ofRandom(20), size+ofRandom(30));
		btn->setName("btn" + ofToString(i));
		btn->setPlane(ofRandom(1));

		// add to the scene
		scene->addChild(btn);

		if (i%perRow>0) {
			// this can be called to place nodes next to their edge
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
	 * this will draw the scene
	 * 1. this function takes all the visible nodes
	 * 2. sort them by 'plane' float
	 * 3. translate to the position of each node
	 * 4. calls the 'draw' function on each node
	 */
	scene->render();

	/******
	 * if you want debug rendering
	 */
	if (bShowDebug) {
		scene->renderDebug();
	}
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
