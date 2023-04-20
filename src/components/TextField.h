#pragma once
#include "Node.h"
#include "ofxFontStash2.h"
#include "ofxFontStashStyle.h"

namespace ofxInterface {
	struct TextFieldSettings : NodeSettings{
		shared_ptr<ofxFontStash2::Fonts> font;
		string text;
		ofAlignHorz horzAlignment = OF_ALIGN_HORZ_LEFT;
	};

	class TextField :
		public Node
	{
	public:
		TextField();
		~TextField();

        virtual void draw();
		virtual void setup(TextFieldSettings s);
		void setText(string text);
		void setAlignment(ofAlignHorz horzAlignment);

    private:
		shared_ptr<ofxFontStash2::Fonts> font;
		string text;
		ofAlignHorz horzAlignment = OF_ALIGN_HORZ_LEFT;
		
		ofFbo alignmentRenderFbo;
	};
}

