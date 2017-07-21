//
//  InfoWindow.cpp
//  interfaceExample
//
//  Created by Nicole Messier on 7/17/17.
//
//

#include "InfoWindow.h"
#include "WindowManager.h"

InfoWindow::InfoWindow(){

}

InfoWindow::~InfoWindow(){
    ofRemoveListener(eventTouchDown, this, &InfoWindow::onTouchDown);
    ofRemoveListener(eventTouchUp, this, &InfoWindow::onTouchUp);
    ofRemoveListener(eventTouchMove, this, &InfoWindow::onTouchMove);
    ofRemoveListener(cbObject->eventClick, this, &InfoWindow::onEventClose);
    
    removeChild(cbObject);
    delete cbObject; 
}

void InfoWindow::setup(ofVec2f size, ofVec2f position, string _id){
    //set size + position
    setSize(size.x, size.y);
    setPosition(position.x, position.y);
    minSize = 30.0f;
    
    //attributes
    color = ofColor(140, 214, 244);
    highlightColor = ofColor(255, 0, 0);
    id = _id;
    
    //register touch events
    ofAddListener(eventTouchDown, this, &InfoWindow::onTouchDown);
    ofAddListener(eventTouchUp, this, &InfoWindow::onTouchUp);
    ofAddListener(eventTouchMove, this, &InfoWindow::onTouchMove);
    IW_touched = false;
    
    
    //close button
    cbObject = new CloseButton();
    cbObject->setup(ofVec2f(20, 20), ofVec2f(0, 0));
    cbObject->setName("CloseButton");
    addChild(cbObject);
    
    //add close button listener
    ofAddListener(cbObject->eventClick, this, &InfoWindow::onEventClose);
    
    //animations
    animation_dt = 1.0f / 60.0f; ;
    animationDuration = 3.0f;
    animVal.reset(0.0f);
    animVal.setRepeatType(PLAY_ONCE);
    animVal.setDuration(animationDuration);
    animVal.setCurve(EASE_IN);
    animVal.animateFromTo(0.0f, 1.0f);
    endPosition = ofVec2f(ofRandom(ofGetWidth()), 10);
    startPosition = position;
    currentPosition = endPosition;
    animLength = endPosition - startPosition;
    state = OPENING;
    
    ofAddListener(animVal.animFinished, this, &InfoWindow::finishedAnim);
}

void InfoWindow::update(float dt){
    switch (state){
        case OPENING:{
            animateOpen();
        }
            break;
        case INTERACT:{
            
        }
            break;
        case CLOSING:{
            animateClose();
        }
            break;
    }
}

void InfoWindow::draw(){
    //draw the frame
    ofSetColor(color);
    ofDrawRectangle(0, 0, rectWidth, rectHeight);
    
    // draw the touch anchor
    if (IW_touched) {
        ofSetColor(highlightColor);
        ofFill();
        ofDrawEllipse(touchAnchor, 10, 10);
    }
}

#pragma mark - ANIMATIONS

void InfoWindow::animateOpen(){
    if(getEnabled()){
        setEnabled(false);
    }
    
    animVal.update(animation_dt);
    float xPos = startPosition.x + animLength.x*animVal.val();
    float yPos = startPosition.y + animLength.y*animVal.val();
    setPosition(xPos, yPos);
    
    rectWidth = minSize + (getWidth()-minSize)*animVal.val();
    rectHeight = minSize + (getHeight()-minSize)*animVal.val();
}

void InfoWindow::animateClose(){
    if(getEnabled()){
        setEnabled(false);
        animVal.reset();
        animVal.animateToIfFinished(1.0f);
    }
    
    animVal.update(animation_dt);
    float xPos = currentPosition.x + animLength.x*animVal.val();
    float yPos = currentPosition.y + animLength.y*animVal.val();
    setPosition(xPos, yPos);
    
    rectWidth = getWidth() - (getWidth()-minSize)*animVal.val();
    rectHeight = getHeight() - (getHeight()-minSize)*animVal.val();
}

void InfoWindow::finishedAnim(ofxAnimatable::AnimationEvent & event){
    switch(state){
        case OPENING: {
            state = INTERACT;
            setEnabled(true);
        }
            break;
        case INTERACT: {
            
        }
            break;
        case CLOSING: {
            state = DELETING;
            InfoWindowData args;
            args.windowId = id;
            args.deleteFlag = true;
            ofNotifyEvent(eventClose, args, this);
        }
            break;
        case DELETING: break;
    }
}


#pragma mark - TOUCH EVENTS

void InfoWindow::onTouchDown(ofxInterface::TouchEvent &event){
    ofVec2f local = toLocal(event.position);
    IW_touched = true;
    touchAnchor = local;
}

void InfoWindow::onTouchUp(ofxInterface::TouchEvent &event){
    ofVec2f local = toLocal(event.position);
    IW_touched = false;
}

void InfoWindow::onTouchMove(ofxInterface::TouchEvent &event){
    ofVec2f parentPos = ((Node*)parent)->toLocal(event.position);
    setPosition(parentPos - touchAnchor);
    currentPosition = parentPos - touchAnchor;
}

void InfoWindow::onEventClose(ofxInterface::TouchEvent& event){
    animLength = startPosition - currentPosition;
    state = CLOSING;
}