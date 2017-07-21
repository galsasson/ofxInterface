//
//  AppManager.cpp
//  interfaceExample
//
//  Created by Nicole Messier on 7/20/17.
//
//

#include "AppManager.h"

AppManager::AppManager(){
    
}

AppManager::~AppManager(){
    
}

void AppManager::setup(ofVec2f size, ofVec2f position){
    setPosition(position);
    setSize(size.x, size.y);
}

void AppManager::setupManagers(){
    //set up window manager
    windowMan = new WindowManager();
    windowMan->setup();
    windowMan->setName("WindowManager");
    addChild(windowMan);
    
    //set up umass window manager;
    objectMan = new ScreenObjectManager();
    objectMan->setName("ScreenObjectManager");
    objectMan->setup(ofVec2f(ofGetWidth(), ofGetHeight()), ofVec2f(0,0));
    addChild(objectMan);
    objectMan->setupUmoObjects();
    ofAddListener(objectMan->eventUmoObjectClicked, this, &AppManager::createInfoWindow);
}

void AppManager::update(float dt){
    
}

void AppManager::draw(){
    
}

void AppManager::createInfoWindow(umoData & data){
    windowMan->createInfoWindow(data);
}