#include "ColorPanel.h"

namespace ofxInterface {


void ColorPanel::setup(ofColor color, float w, float h)
{
	ColorPanelSettings s;
	s.size = ofVec2f(w, h);
	s.colors.front() = color;
	s.bDrawBackground = true;
	setup(s);
}

void ColorPanel::setup(ColorPanelSettings settings) {
	Node::setup(settings);
	borderWidth = settings.strokeWidth;
	bDrawBackground = settings.bDrawBackground;
	borderRadius = settings.borderRadius;
	strokeColor = settings.strokeColor;

	if (settings.colors.size() > 0) {
		colors.clear();
	}
	for (size_t i = 0; i < settings.colors.size();++i) {
		colors.push_back(settings.colors[i]);
		if (i < settings.gradientColorPositions.size()) {
			gradientColorPositions.push_back(settings.gradientColorPositions[i]);
		}
		else {
			if (i == 0) {
				gradientColorPositions.push_back(0.0);
			}
			else if (i == 1) {
				gradientColorPositions.push_back(1.0);
			}
			
		}
	}
	gradientDirection = settings.gradientDirection;

	shader.push_back(getGradientShader2Colors());
	shader.push_back(getGradientShader3Colors());


	initShape();

	ofAddListener(eventNodeSizeChanged,this, &ColorPanel::onNodeSizeChanged);

}

Node* ColorPanel::clone()
{
	auto ret = new ColorPanel(*this);
	return ret;
}

ofColor ColorPanel::getColor() {
	return colors.front();
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
	return borderRadius;
}

void ColorPanel::setRoundedEdge(float ang) {
	borderRadius = ang;
}

void ColorPanel::onNodeSizeChanged(Node& n)
{
	initShape();
}

void ColorPanel::initShape()
{
	shape.clear();
	outline.clear();
	shape.setFilled(colors.front().a > 0);
	outline.setStrokeWidth(borderWidth);
	shape.setFillColor(colors.front());
	outline.setStrokeColor(strokeColor);
	shape.setUseShapeColor(true);
	outline.setUseShapeColor(true);

	if (borderRadius > 0) {
		shape.rectRounded(borderWidth, borderWidth, getWidth() - getWidth() * borderWidth, getHeight() - 2 * borderWidth, borderRadius);
		outline.rectRounded(0, 0, getWidth(), getHeight(), borderRadius);
		outline.rectRounded(borderWidth, borderWidth, getWidth() - getWidth() * borderWidth, getHeight() - 2 * borderWidth, borderRadius);
	}
	else {
		shape.rectangle(borderWidth, borderWidth, getWidth() - 2 * borderWidth, getHeight() - 2 * borderWidth);
		outline.rectangle(0, 0, getWidth(), getHeight());
		outline.rectangle(borderWidth, borderWidth, getWidth() - 2 * borderWidth, getHeight() - 2 * borderWidth);
	}

	// shader
	if (colors.size() > 1) {
		fbo.allocate(getWidth(), getHeight());

		ofEnableBlendMode(OF_BLENDMODE_DISABLED);
		fbo.begin();

		ofClear(0, 0);
		int nShader = 0;
		if (colors.size() == 3) {
			nShader = 1;
		}
			shader[nShader].begin();

		
			
			ofVec2f pEnd = angleToBorderPoint(gradientDirection);
			ofVec2f pStart = angleToBorderPoint(fmod(gradientDirection+180,360));
			ofVec2f dir = pEnd - pStart;
			pStart += dir * gradientColorPositions.front();
			pEnd = pStart + dir * gradientColorPositions.back();

		
			

		shader[nShader].setUniform2f("begin", pStart);
		shader[nShader].setUniform2f("end", pEnd);
		
		

		shader[nShader].setUniform2f("size", getSize());
		shader[nShader].setUniform4f("color1", colorToVec4(colors[0]));
		shader[nShader].setUniform4f("color2", colorToVec4(colors[1]));
		if (nShader == 2) {
			shader[nShader].setUniform1f("mid", 0.66f);
			shader[nShader].setUniform4f("color3", colorToVec4(colors[2]));
		}
		

		/*if (roundAngle > 0) {
			ofDrawRectRounded(0, 0, getWidth(), getHeight(), roundAngle);
		}
		else {*/
		ofDrawRectangle(0, 0, getWidth(), getHeight());
		//}

		shader[nShader].end();

		fbo.end();
		ofEnableBlendMode(OF_BLENDMODE_ALPHA);
		ofSetColor(255);
		fbo.draw(0, 0);
	}
	
}

ofVec2f ColorPanel::angleToBorderPoint(float angle)
{
	ofVec2f d = angleToDirVector(angle);
	ofVec2f p = ofVec2f(0.5, 0.5);

	if (angle < 45 || angle >315) {
		float r = (1.0 - p.x) / d.x;
		return ofVec2f(1.0, p.y + r * d.y);
	}
	else if (angle < 135) {
		float r = (1.0 - p.y) / d.y;
		return ofVec2f(p.x + r * d.x,1.0);
	}
	else if (angle < 225) {
		float r = (0 - p.x) / d.x;
		return ofVec2f(0, p.y + r * d.y);
	}
	else {
		float r = (0 - p.y) / d.y;
		return ofVec2f(p.x + r * d.x, 0);
	}
}

ofVec2f ColorPanel::angleToDirVector(float angle)
{
	ofVec2f vDirE = ofVec2f(1, 0);
	vDirE.rotate(angle);
	return vDirE;
}

void ColorPanel::draw()
{
	outline.draw();
	if (colors.size() == 1) {
		shape.draw();
	}
	else {
		fbo.draw(borderWidth, borderWidth, getWidth() - getWidth() * borderWidth, getHeight() - 2 * borderWidth);
	}
	
}



}   // namespace