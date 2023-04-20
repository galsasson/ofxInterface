#pragma once

#include "ofMain.h"

#include "Node.h"
#include "GradientShader.h"

namespace ofxInterface {
	struct ColorPanelSettings : NodeSettings{
		float strokeWidth = 0;
		bool bDrawBackground = true;
		float borderRadius = 0;

		vector<ofColor> colors = {ofColor(0)};
		ofColor strokeColor = ofColor(0);
		float gradientDirection = 0;
		vector<float> gradientColorPositions;
	};

	class ColorPanel : public ofxInterface::Node
	{
	public:
		void setup(ofColor color, float w, float h);
		void setup(ColorPanelSettings settings);
		virtual Node* clone() override;

		void draw();

		ofColor getColor();
		void setColor(const ofColor& c) { colors[0] = c; shape.setFillColor(c); }

		ofColor getStrokeColor();
		void setStrokeColor(const ofColor& c);

		bool isDrawBackground();
		void setDrawBackground(bool set);

		float getBorderWidth();
		void setBorderWidth(float set);

		float getRoundedEdge();
		void setRoundedEdge(float ang);

		void onNodeSizeChanged(Node& n);

	protected:
		void initShape();
		ofVec2f angleToBorderPoint(float angle);
		ofVec2f angleToDirVector(float angle);

		float borderWidth;
		bool bDrawBackground = true;
		float borderRadius;

		vector<ofColor> colors;
		ofColor strokeColor;
		float gradientDirection;
		vector<float> gradientColorPositions;

		ofPath shape;
		ofPath outline;

		vector<ofShader> shader;
		ofFbo fbo;
	};

}   

