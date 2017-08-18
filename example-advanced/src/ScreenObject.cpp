//
//  ScreenObject.cpp
//  interfaceExample
//
//  Created by Nicole Messier on 7/17/17.
//
//

#include "ScreenObject.h"

ScreenObject::ScreenObject(){
    
}

ScreenObject::~ScreenObject(){
    ofRemoveListener(eventTouchDown, this, &ScreenObject::onTouchDown);
    ofRemoveListener(eventTouchUp, this, &ScreenObject::onTouchUp);
    ofRemoveListener(eventClick, this, &ScreenObject::onClick);
}

void ScreenObject::setup(ofVec2f rectSize, ofVec2f position){
    //these come from ofxInterface::Node
    setPosition(position);
    setSize(rectSize.x, rectSize.y);
    
    color = ofColor(255, 0, 0);
    highlightColor = ofColor(0, 0, 255);
    UO_touched = false;
   
    //register touch events
    ofAddListener(eventTouchDown, this, &ScreenObject::onTouchDown);
    ofAddListener(eventTouchUp, this, &ScreenObject::onTouchUp);
    ofAddListener(eventClick, this, &ScreenObject::onClick);
}

void ScreenObject::update(float dt){
    
}

void ScreenObject::draw(){
    //draw the frame
    ofSetColor(color);
    ofDrawRectangle(0, 0, getWidth(), getHeight());
    
    //draw the touch anchor
    if(UO_touched){
        ofSetColor(highlightColor);
        ofDrawEllipse(touchAnchor, 10, 10);
    }
}

void ScreenObject::onTouchDown(ofxInterface::TouchEvent& event){
    //event.position is always in global space, use toLocal
    ofVec2f local = toLocal(event.position);
    UO_touched = true;
    touchAnchor = local;
}


void ScreenObject::onTouchUp(ofxInterface::TouchEvent& event){
    ofVec2f local = toLocal(event.position);
    UO_touched = false;
}

void ScreenObject::onClick(ofxInterface::TouchEvent& event){
    umoData thisData;
    thisData.umoId = objectId;
    ofNotifyEvent(eventUmoClicked, thisData, this);
}