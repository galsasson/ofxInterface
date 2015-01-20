//
//  BitmapTextButton.h
//  AnimationBreaker
//
//  Created by Gal Sasson on 1/19/15.
//
//

#ifndef __ofxUINode__BitmapTextButton__
#define __ofxUINode__BitmapTextButton__

#include <stdio.h>
#include "ofMain.h"

#include "ofxUINode.h"

class BitmapTextButton : public ofxUINode
{
public:

	void setup(const string& label);

	void setBackground(bool set) { bDrawBackground = set; }
	void setBorder(bool set) { bDrawBorder = set; }

	void setLabelColor(const ofColor& c) { labelColor = c; }
	void setBGColor(const ofColor& c) { bgColor = c; }
	void setBorderColor(const ofColor& c) { borderColor = c; }

	void draw();

private:

	bool bDrawBackground;
	bool bDrawBorder;

	ofColor borderColor;
	ofColor bgColor;
	ofColor labelColor;

	string label;
};
#endif /* defined(__ofxUINode__BitmapTextButton__) */
