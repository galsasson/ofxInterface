#include "Label.h"


namespace ofxInterface {
	Label::Label() :Node()
	{

	}


	Label::~Label()
	{
	}

	void Label::draw()
	{
		auto bounds = font->getTextBounds(text, style, 0, 0);
		float dy = bounds.height*0.5;

		bounds.y += dy;

		// since fontstash uses its own clip functions, we need to manually disable the draw
		// masks out the text completely, when out of clip
		// need to find a more appropriate function
		ofRectangle boundsGlobal( toGlobal(ofVec3f(bounds.x,bounds.y)),toGlobal(ofVec3f(bounds.width,bounds.height)) );
		boundsGlobal.width -= boundsGlobal.x;
		boundsGlobal.height -= boundsGlobal.y;

		if (!getGlobalRenderClip() || getRenderClipRect().intersects(boundsGlobal)) {
			if (isDropshadow) {
				font->drawColumn(text, shadow, shadowPos.x, shadowPos.y + dy, getWidth(), alignment);
			}
			font->drawColumn(text, style, 0, dy, getWidth(), alignment);
		}

		
	}

	void ofxInterface::Label::setup(LabelSettings s)
	{
		Node::setup(s);
		font = s.font;
		style = s.style;
		text = s.text;
		alignment = s.alignment;
		setShadow(s.isDropshadow, s.shadowSize, s.shadowPos.x, s.shadowPos.y, s.shadowColor);

	}

	void Label::setText(string text_)
	{
		text = text_;
	}

	void Label::setAlignment(ofAlignHorz alignment_)
	{
		alignment = alignment_;
	}
	void Label::setShadow(bool isEnabled, float w, float x, float y, ofColor color) {
		isDropshadow = isEnabled;
		shadow = style;
		shadow.blur = w;
		shadow.color = color;
		shadowPos = ofVec2f(x, y);
	}
	void Label::setFontSize(float size) {
		style.fontSize = size;
		shadow.fontSize = size;
	}
	float Label::getFontSize() {
		return style.fontSize;
	}
}