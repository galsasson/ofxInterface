//
//  ButtonExample.h
//  example-basic
//
//  Created by Gal Sasson on 2/4/15.
//
//

#ifndef __example_basic__ButtonExample__
#define __example_basic__ButtonExample__

#include <stdio.h>
#include "ofMain.h"

#include "ofxInterface.h"

using namespace ofxInterface;

class ButtonExample : public ofxInterface::Node
{

public:
	void setup(float x, float y, float w, float h);

	void update();
	void draw();


	void onTouchDown(TouchEvent& event);
	void onTouchMove(TouchEvent& event);
	void onTouchUp(TouchEvent& event);


	ofColor color;
	ofVec2f touchAnchor;

};

#endif /* defined(__example_basic__ButtonExample__) */
