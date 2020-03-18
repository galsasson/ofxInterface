#pragma once
#include "Node.h"
#include "ofxFontStash2.h"

namespace ofxInterface {
	struct LabelSettings :NodeSettings{
		shared_ptr<ofxFontStash2::Fonts> font;
		ofxFontStash2::Style style;
		string text;
		ofAlignHorz alignment = OF_ALIGN_HORZ_LEFT;

		bool isDropshadow = false;
		ofxFontStash2::Style shadow;
		ofVec2f shadowPos;
		float shadowSize;
		ofColor shadowColor;
	};

	class Label :
		public Node
	{
	public:
		Label();
		~Label();

        virtual void draw();
		virtual void setup(LabelSettings s);
		void setText(string text);
		void setAlignment(ofAlignHorz alignment);
		void setShadow(bool isEnabled, float w = 0, float x= 0, float y = 0, ofColor color = ofColor(0));
		void setFontSize(float size);
		float getFontSize();


    private:
		shared_ptr<ofxFontStash2::Fonts> font;
		ofxFontStash2::Style style;
		string text;
		ofAlignHorz alignment = OF_ALIGN_HORZ_LEFT;

		bool isDropshadow = false;
		ofxFontStash2::Style shadow;
		ofVec2f shadowPos;
	};
}

