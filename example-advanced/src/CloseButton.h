//
//  CloseButton.hpp
//  interfaceExample
//
//  Created by Nicole Messier on 7/17/17.
//
//

#pragma once

#include "ofMain.h"
#include "ofxInterface.h"
#include "ofEvent.h"

class InfoWindow;
class ScreenObject;

class CloseButton : public ofxInterface::Node{
public:
    CloseButton();
    ~CloseButton();
    void setup(ofVec2f size, ofVec2f position);
    void update(float dt);
    void draw();
    
    void onTouchDown(ofxInterface::TouchEvent &event);
    void onTouchUp(ofxInterface::TouchEvent &event);
    void onClick(ofxInterface::TouchEvent &event);
    
    ofColor color;
    ofColor highlightColor;
    
    bool CB_draw; 
    bool CB_touched;
    ofVec2f touchAnchor;
    
protected:
 
};