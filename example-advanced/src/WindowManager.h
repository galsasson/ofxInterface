//
//  WindowManager.hpp
//  interfaceExample
//
//  Created by Nicole Messier on 7/18/17.
//
//

#pragma once
#include "ofMain.h"
#include "ofxInterface.h"
#include "InfoWindow.h"
#include "ofxAnimatableFloat.h"

class WindowManager : public ofxInterface::Node{
public:
    WindowManager();
    ~WindowManager();
    
    void setup();
    void update(float dt);
    void draw();
    
    void createInfoWindow(umoData & data);
    void deleteInfoWindow(InfoWindowData & args);
    
    vector<InfoWindowData> deleteQueue;
    string nodeName;
    
    
protected:
    int infoWindowCouner = 0; 
    
};
