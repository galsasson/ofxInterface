//
//  BitmapTextButton.cpp
//  ofxInterface
//
//  Created by Gal Sasson on 1/19/15.
//
//

#include "BitmapTextButton.h"

namespace ofxInterface
{

void BitmapTextButton::setup(const string &_label)
{
	label = _label;
	setSize(label.length()*8 + 10, 20);
    setName(label);

	bDrawBorder = true;
	bDrawBackground = true;

	borderColor = ofColor(0);
	bgColor = ofColor(255);
	labelColor = ofColor(0);
}

void BitmapTextButton::draw()
{
	if (bDrawBackground) {
		ofSetColor(bgColor * (isTouched()?0.5:1));
		ofFill();
		ofDrawRectangle(0, 0, getWidth(), getHeight());
	}

	ofSetColor(labelColor);
	ofDrawBitmapString(label, 5, getHeight()-5);

	if (bDrawBorder) {
		ofSetColor(borderColor);
		ofNoFill();
		ofSetLineWidth(1);
		ofDrawRectangle(0, 0, getWidth(), getHeight());
	}
}


} 	// namespace
