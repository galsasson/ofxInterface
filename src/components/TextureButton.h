//
//  TextureButton.h
//  ofxUINode
//
//  Created by Gal Sasson on 7/24/14.
//
//

#ifndef __ofxUINode__TextureButton__
#define __ofxUINode__TextureButton__

#include <iostream>
#include "ofMain.h"
#include "ofxUINode.h"

class TextureButton : public ofxUINode
{
public:
	TextureButton();
	void setup(ofTexture *tex, float x=0, float y=0);

	void setBorder(bool draw) { bDrawBorder = draw; }
	void setBackground(bool draw) { bDrawBackground = draw; }

	void setTintColor(const ofColor& c) { tintColor = c; }
	void setBGColor(const ofColor& c) { bgColor = c; }
	void setBorderColor(const ofColor& c) { borderColor = c; }

	void setPadding(float top, float right, float bottom, float left);

	void update(float dt);
	void draw();



private:

	float padTop, padRight, padBottom, padLeft;

	bool bDrawBorder;
	bool bDrawBackground;

	ofColor tintColor;
	ofColor bgColor;
	ofColor borderColor;

	ofTexture* texture;
};

#endif /* defined(__BaseApp__TextureButton__) */
