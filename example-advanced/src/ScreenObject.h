//
//  ScreenObject.hpp
//  interfaceExample
//
//  Created by Nicole Messier on 7/17/17.
//
//

#pragma once

#include "ofMain.h"
#include "ofxInterface.h"
#include "Globals.h"

class ScreenObject : public ofxInterface::Node{
public:
    
    ScreenObject();
    ~ScreenObject();
    
    void setup(ofVec2f rectSize, ofVec2f position);
    void update(float dt);
    void draw();
    
    //touch
    void onTouchDown(ofxInterface::TouchEvent& event);
    void onTouchUp(ofxInterface::TouchEvent& event);
    void onClick(ofxInterface::TouchEvent& event);
    
    ofEvent<umoData> eventUmoClicked; 
    
    ofVec2f touchAnchor;
    bool UO_touched;
    
    //attributes
    ofColor color;
    ofColor highlightColor;
    ofVec2f size;
    ofVec2f startPosition;
    string objectId;
};