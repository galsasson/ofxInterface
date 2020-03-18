#pragma once
#include "Node.h"
#include "ofxFontStash2.h"
#include "ofxFontStashStyle.h"

namespace ofxInterface {
	struct TextFieldSettings : NodeSettings{
		shared_ptr<ofxFontStash2::Fonts> font;
		string text;
		ofAlignHorz alignment = OF_ALIGN_HORZ_LEFT;
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
		void setAlignment(ofAlignHorz alignment);

    private:
		shared_ptr<ofxFontStash2::Fonts> font;
		string text;
		ofAlignHorz alignment = OF_ALIGN_HORZ_LEFT;
	};
}

