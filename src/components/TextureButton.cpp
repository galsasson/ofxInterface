//
//  TextureButton.cpp
//  ofxInterface
//
//  Created by Gal Sasson on 7/24/14.
//
//

#include "TextureButton.h"

namespace ofxInterface
{


TextureButton::TextureButton()
{
	texture = NULL;
}

void TextureButton::setup(ofTexture *tex, float x, float y)
{
	texture = tex;
	setSize(tex->getWidth(), tex->getHeight());

	setPosition(x, y, 0);

	setPadding(0, 0, 0, 0);

	bDrawBackground = false;
	bDrawBorder = false;

	bgColor = ofColor(255);;
	tintColor = ofColor(255);
	borderColor = ofColor(0);
}

void TextureButton::setTexture(ofTexture* tex)
{
	texture = tex;
	setSize(tex->getWidth(), tex->getHeight());
}

void TextureButton::setPadding(float top, float right, float bottom, float left)
{
	padTop = top;
	padRight = right;
	padBottom = bottom;
	padLeft = left;
}

void TextureButton::update(float dt)
{

}

void TextureButton::draw()
{
	if (bDrawBackground) {
		ofSetColor(bgColor);
		ofFill();
		ofDrawRectangle(0, 0, getWidth(), getHeight());
	}

	ofSetColor(tintColor);
	if (texture) {
		texture->draw(padLeft, padBottom, getWidth()-padLeft-padRight, getHeight()-padTop-padBottom);
	}

	if (bDrawBorder) {
		ofSetColor(borderColor);
		ofNoFill();
		ofDrawRectangle(0, 0, getWidth(), getHeight());
	}
}

}	// namespace
