//
//  AppManager.h
//  interfaceExample
//
//  Created by Nicole Messier on 7/20/17.
//
//

#pragma once
#include "ofMain.h"
#include "ofxInterface.h"
#include "WindowManager.h"
#include "ScreenObjectManager.h"
#include "Globals.h"

class AppManager : public ofxInterface::Node{
public:
    AppManager();
    ~AppManager();
    
    void setup(ofVec2f size, ofVec2f position);
    void update(float dt);
    void draw();
    void setupManagers(); 
    
    WindowManager *windowMan;
    ScreenObjectManager *objectMan;
    
    void createInfoWindow(umoData & data);
};
