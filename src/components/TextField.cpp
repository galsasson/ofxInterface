#include "TextField.h"


namespace ofxInterface {
	TextField::TextField() :Node()
	{

	}


	TextField::~TextField()
	{
	}

	void TextField::draw()
	{
		vector<ofxFontStash2::StyledText> blocks;
		ofxFontStash2::Parser::parseText(text, font->getStyles(), "text", blocks);
		int y = font->getTextBounds(blocks[0].text, blocks[0].style, 0, 0).height * 0.5;

		alignmentRenderFbo.begin();
		auto bounds = font->drawFormattedColumn(text, 0, y, getWidth(), horzAlignment);
		alignmentRenderFbo.end();
		//float dy = bounds.height * 0.5;

		// since fontstash uses its own clip functions, we need to manually disable the draw
		// masks out the text completely, when out of clip
		// need to find a more appropriate function
		ofRectangle boundsGlobal(toGlobal(ofVec3f(bounds.x, bounds.y)), bounds.width, bounds.height);
		//boundsGlobal.y -= bounds.height;
		// 
		//boundsGlobal.width -= boundsGlobal.x;
		//boundsGlobal.height -= boundsGlobal.y;


		auto cl = getGlobalRenderClip();
		auto in = getRenderClipRect().intersects(boundsGlobal);
		if (!cl || in) {
			font->drawFormattedColumn(text, 0, y, getWidth(), horzAlignment);
		}
		

		
	}

	void TextField::setup(TextFieldSettings s)
	{
		Node::setup(s);

		font = s.font;
		text = s.text;
		horzAlignment = s.horzAlignment;

		alignmentRenderFbo.allocate(1, 1);
	}

	void TextField::setText(string text_)
	{
		text = text_;
	}

	void TextField::setAlignment(ofAlignHorz alignment_) {
		horzAlignment = alignment_;
	}
}