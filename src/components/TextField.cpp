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
		int y = font->getTextBounds(blocks[0].text,blocks[0].style, 0, 0).height*0.5;

		font->drawFormattedColumn(text, 0, y, getWidth(), alignment);
	}

	void TextField::setup(TextFieldSettings s)
	{
		Node::setup(s);

		font = s.font;
		text = s.text;
		alignment = s.alignment;
	}

	void TextField::setText(string text_)
	{
		text = text_;
	}

	void TextField::setAlignment(ofAlignHorz alignment_) {
		alignment = alignment_;
	}
}