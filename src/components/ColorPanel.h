#pragma once

#include "ofMain.h"

#include "Node.h"

namespace ofxInterface {
	struct ColorPanelSettings : NodeSettings{
		float strokeWidth = 0;
		bool bDrawBackground = true;
		float borderRadius = 0;

		ofColor bgColor = ofColor(0);
		ofColor strokeColor = ofColor(0);
	};

	class ColorPanel : public ofxInterface::Node
	{
	public:
		void setup(ofColor color, float w, float h);
		void setup(ColorPanelSettings settings);

		void draw();

		ofColor getColor();
		void setColor(const ofColor& c) { bgColor = c; }

		ofColor getStrokeColor();
		void setStrokeColor(const ofColor& c);

		bool isDrawBackground();
		void setDrawBackground(bool set);

		float getBorderWidth();
		void setBorderWidth(float set);

		float getRoundedEdge();
		void setRoundedEdge(float ang);

	private:
		float borderWidth;
		bool bDrawBackground = true;
		float roundAngle;

		ofColor bgColor;
		ofColor strokeColor;
	};

}   

