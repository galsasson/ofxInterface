//
//  InfoWindow.hpp
//  interfaceExample
//
//  Created by Nicole Messier on 7/17/17.
//
//

#pragma once

#include "ofMain.h"
#include "ofxInterface.h"
#include "CloseButton.h"
#include "ScreenObject.h"
#include "ofEvent.h"
#include "ofxAnimatableFloat.h"
#include "Globals.h"

class WindowManager; 

class InfoWindow : public ofxInterface::Node{
public:
    
    struct InfowWindowData{
        string id; 
    };
    
    InfoWindow();
    ~InfoWindow();
    void setup(ofVec2f size, ofVec2f position, string _id);
    void update(float dt);
    void draw();
    
    //touch
    void onTouchDown(ofxInterface::TouchEvent& event);
    void onTouchMove(ofxInterface::TouchEvent& event);
    void onTouchUp(ofxInterface::TouchEvent& event);
    ofVec2f touchAnchor;
    bool IW_touched;
    
    //attributes
    ofColor color;
    ofColor highlightColor;
    string id;
    
    //animation
    ofVec2f startPosition;
    ofVec2f endPosition;
    ofVec2f currentPosition;
    ofVec2f animLength;
    float animationDuration;
    ofxAnimatableFloat animVal;
    void animateOpen();
    void animateClose();
    void finishedAnim(ofxAnimatable::AnimationEvent & event);
    float animation_dt;
    float rectWidth;
    float rectHeight;
    float minSize; 
    
    //additional nodes
    CloseButton* cbObject;
    
    //on close events
    void onEventClose(ofxInterface::TouchEvent& event);
    ofEvent<InfoWindowData>eventClose;
    
protected:
    int state;
    
    enum States{
        OPENING,
        INTERACT,
        CLOSING,
        DELETING
    };
};

