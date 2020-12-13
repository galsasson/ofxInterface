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
	textureDown = NULL;
	setPadding(0, 0, 0, 0);
	bDrawBackground = false;
	bDrawBorder = false;
	bgColor = ofColor(255);;
	tintColor = ofColor(255);
	borderColor = ofColor(0);
	touchDownFunc = []() {};
	touchUpFunc = []() {};
	ofAddListener(eventTouchDown, this, &TextureButton::onTouchDown);
	ofAddListener(eventTouchUp, this, &TextureButton::onTouchUp);
}

void TextureButton::setTextures(ofTexture* up, ofTexture* down)
{
	if (!up)
		return;

	texture = up;
	textureDown = down ? down : up;
	float w = fmax(texture->getWidth(), textureDown->getWidth());
	float h = fmax(texture->getHeight(), textureDown->getHeight());
	setSize(w, h);
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
	if (texture && textureDown) {
		if (Node::isTouched()) {
			textureDown->draw(padLeft, padBottom, getWidth() - padLeft - padRight, getHeight() - padTop - padBottom);
		}
		else {
			texture->draw(padLeft, padBottom, getWidth() - padLeft - padRight, getHeight() - padTop - padBottom);
		}
	}

	if (bDrawBorder) {
		ofSetColor(borderColor);
		ofNoFill();
		ofDrawRectangle(0, 0, getWidth(), getHeight());
	}
}

void TextureButton::onTouchDown(TouchEvent& event)
{
	touchDownFunc();
}

void TextureButton::onTouchUp(TouchEvent& event)
{
	touchUpFunc();
}

}	// namespace
