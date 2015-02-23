//
//  SolidColorPanel.cpp
//  ofxInterface
//
//  Created by Gal Sasson on 8/13/14.
//
//

#include "SolidColorPanel.h"

namespace ofxInterface {


void SolidColorPanel::setup(float w, float h)
{
    setSize(w, h);

	bDrawBackground = true;
	bDrawBorder = false;
	bRounded = false;
	roundAngle = 5;
}

void SolidColorPanel::draw()
{
	if (bDrawBackground) {
		ofFill();
		ofSetColor(bgColor);

		if (bRounded) {
			ofDrawRectRounded(0, 0, getWidth(), getHeight(), roundAngle);
		}
		else {
			ofDrawRectangle(0, 0, getWidth(), getHeight());
		}
	}

	if (bDrawBorder) {
		ofNoFill();
		ofSetLineWidth(2);
		ofSetColor(strokeColor);
		if (bRounded) {
			ofDrawRectRounded(0, 0, getWidth(), getHeight(), roundAngle);
		}
		else {
			ofDrawRectangle(0, 0, getWidth(), getHeight());
		}
	}
}



}   // namespace