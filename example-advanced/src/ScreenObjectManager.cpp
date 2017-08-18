//
//  ScreenObjectManager.cpp
//  interfaceExample
//
//  Created by Nicole Messier on 7/19/17.
//
//

#include "ScreenObjectManager.h"

ScreenObjectManager::ScreenObjectManager(){
    //--- grid ---
    numColumns_grid = 4;
    numRows_grid = 3;
    maxColumnWidth_grid = ofGetWidth()/numColumns_grid;
    
    //--- umo ---
    numColumns_umo = 12;
    numRows_umo = 15;
    spacing = 7;
    maxWidth_umo = (float)maxColumnWidth_grid/(float)numColumns_umo - (float)spacing;
    minWidth_umo = 2.0f;
    height_umo = ofGetHeight()/(float)numRows_umo - (float)spacing;
    num_umo = numRows_umo*numColumns_umo*numColumns_grid;
    
}

ScreenObjectManager::~ScreenObjectManager(){
    
}

void ScreenObjectManager::setup(ofVec2f size, ofVec2f position){
    setPosition(position);
    setSize(size.x, size.y);
}

void ScreenObjectManager::update(float dt){
    
}

void ScreenObjectManager::draw(){
    
}

void ScreenObjectManager::setupUmoObjects(){
    //set up grid of umass objects
    int counter = 0;
        for(int j=0; j<numColumns_grid; j++){
            for(int x=0; x<numColumns_umo; x++){
                for(int y=0; y<numRows_umo; y++){
                    counter ++;
                    int xPos = maxColumnWidth_grid*j + maxColumnWidth_grid/numColumns_umo*(x + 1/2) + spacing;
                    int yPos = ofGetHeight()/numRows_umo*y + spacing + height_umo/2;
                    
                    ofVec2f umo_size = ofVec2f(ofRandom(minWidth_umo, maxWidth_umo), height_umo);
                    ScreenObject *thisObject = new ScreenObject();
                    thisObject->setup(umo_size, ofVec2f((float)xPos, (float)yPos));
                    thisObject->objectId = "umo" + ofToString(counter);
                    thisObject->setName(thisObject->objectId);
                    umassObjects.push_back(thisObject);
                }
            }
        }
    
    //add listener and add as child
    for(int i=0; i<num_umo; i++){
        ofAddListener(umassObjects[i]->eventUmoClicked, this, &ScreenObjectManager::onUmoObjectClicked);
        addChild(umassObjects[i]);
    }
}

void ScreenObjectManager::onUmoObjectClicked(umoData & data){
    ofLogNotice() << "button " << data.umoId << " clicked";
    ofNotifyEvent(eventUmoObjectClicked, data, this);
}