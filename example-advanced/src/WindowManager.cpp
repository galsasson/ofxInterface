//
//  WindowManager.cpp
//  interfaceExample
//
//  Created by Nicole Messier on 7/18/17.
//
//

#include "WindowManager.h"

WindowManager::WindowManager(){
    setPosition(0,0);
    setSize(0,0); 
}

WindowManager::~WindowManager(){
    
}

void WindowManager::setup(){
    
}

void WindowManager::update(float dt){
    
    //remove objects that are in the deletequeue
    //and delete object from queue
    for(int i=deleteQueue.size() - 1; i>=0; i--){
        if(deleteQueue[i].deleteFlag){
            ofxInterface::Node* deleteWindow = getChildWithName(deleteQueue[i].windowId);
            removeChild(deleteWindow);
            delete deleteWindow;
            deleteQueue.erase(deleteQueue.begin() + i);
        }
    }
    
}

void WindowManager::draw(){
    
}

void WindowManager::createInfoWindow(umoData & data){
    //create info window on click
    infoWindowCouner++;
    InfoWindow *newWindow = new InfoWindow();
    nodeName = "InfoWindow" + ofToString(infoWindowCouner);
    newWindow->setName(nodeName);
    newWindow->setup(ofVec2f(200, 100), ofVec2f(300, 500), nodeName);
    addChild(newWindow);
    newWindow->animateOpen(); 
    newWindow->setPlane(1.0f);
    ofLogNotice() << newWindow->getPlane();
    
    //add listener
    ofAddListener(newWindow->eventClose, this, &WindowManager::deleteInfoWindow);
}


void WindowManager::deleteInfoWindow(InfoWindowData & args){
    ofLogNotice() << "deleteInfoWindow";
    //add to delete queue
    deleteQueue.push_back(args); 
}
