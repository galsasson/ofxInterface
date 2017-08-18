//
//  ScreenObjectManager.h
//  interfaceExample
//
//  Created by Nicole Messier on 7/19/17.
//
//

#pragma once
#include "ofMain.h"
#include "ofxInterface.h"
#include "ScreenObject.h"
#include "Globals.h"

class ScreenObjectManager : public ofxInterface::Node{
public:
    ScreenObjectManager();
    ~ScreenObjectManager();
    
    void setup(ofVec2f size, ofVec2f position);
    void update(float dt);
    void draw();
    void setupUmoObjects();
    
    vector<ScreenObject*> umassObjects;
    
    //--- grid ---
    int numColumns_grid;
    int numRows_grid;
    int maxColumnWidth_grid;
    
    //--- umo ---
    int numColumns_umo;
    int numRows_umo;
    int spacing;
    float maxWidth_umo;
    float minWidth_umo;
    float height_umo;
    int num_umo;
    
    //events
    ofEvent<umoData> eventUmoObjectClicked;
    void onUmoObjectClicked(umoData & data);
    
private:
    
    
};
