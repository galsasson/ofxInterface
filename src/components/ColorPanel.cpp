#include "ColorPanel.h"

namespace ofxInterface {


void ColorPanel::setup(ofColor color, float w, float h)
{
    setSize(w, h);
	setColor(color);
	bDrawBackground = true;
}

void ColorPanel::setup(ColorPanelSettings settings) {
	Node::setup(settings);
	borderWidth = settings.strokeWidth;
	bDrawBackground = settings.bDrawBackground;
	roundAngle = settings.borderRadius;
	bgColor = settings.bgColor;
	strokeColor = settings.strokeColor;
}

ofColor ColorPanel::getColor() {
	return bgColor;
}

ofColor ColorPanel::getStrokeColor() {
	return strokeColor;
}

void ColorPanel::setStrokeColor(const ofColor & c) {
	strokeColor = c;
}

bool ColorPanel::isDrawBackground() {
	return bDrawBackground;
}

void ColorPanel::setDrawBackground(bool set) {
	bDrawBackground = set;
}

float ColorPanel::getBorderWidth() {
	return borderWidth;
}

void ColorPanel::setBorderWidth(float set) {
	borderWidth = set;
}

float ColorPanel::getRoundedEdge() {
	return roundAngle;
}

void ColorPanel::setRoundedEdge(float ang) {
	roundAngle = ang;
}

void ColorPanel::draw()
{

	if (bDrawBackground) {
		ofFill();
		ofSetColor(bgColor);

		if (roundAngle > 0) {
			ofDrawRectRounded(0, 0, getWidth(), getHeight(), roundAngle);
		}
		else {
			ofDrawRectangle(0, 0, getWidth(), getHeight());
		}
	}

	if (borderWidth > 0) {
		ofNoFill();
		ofSetLineWidth(borderWidth);
		ofSetColor(strokeColor);
		if (roundAngle > 0) {
			ofDrawRectRounded(0, 0, getWidth(), getHeight(), roundAngle);
		} else {
			ofDrawRectangle(0, 0, getWidth(), getHeight());
		}
	}
}



}   // namespace