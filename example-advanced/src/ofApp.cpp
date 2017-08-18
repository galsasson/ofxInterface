#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofEnableSmoothing();
    ofEnableAlphaBlending();
    ofBackground(0);
    
    //set up the scene with width + height
    scene = new ofxInterface::Node();
    scene->setSize(ofGetWidth(), ofGetHeight());
    scene->setName("Scene");
    
    //pass the scene to the touch manager
    ofxInterface::TouchManager::one().setup(scene);
    
    //set up AppManager
    appMan = new AppManager();
    appMan->setup(ofVec2f(ofGetWidth(), ofGetHeight()), ofVec2f(0, 0));
    appMan->setName("AppManager");
    scene->addChild(appMan);
    appMan->setupManagers();
    
}

//--------------------------------------------------------------
void ofApp::update(){
    //update touch manager
    ofxInterface::TouchManager::one().update();
    scene->updateSubtree(1/60.0f, true);
}

//--------------------------------------------------------------
void ofApp::draw(){
    //draw scene
    scene->render();
    
    if(bShowDebug){
        scene->renderDebug();
    }

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key){
            case 'd':
                bShowDebug = !bShowDebug;
            break;
            case 'c':
                ofLogNotice() << scene->getNumChildren();
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
    ofxInterface::TouchManager::one().touchMove(button, ofVec2f(x,y));
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    ofxInterface::TouchManager::one().touchDown(button, ofVec2f(x,y));
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    ofxInterface::TouchManager::one().touchUp(button, ofVec2f(x,y));
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
