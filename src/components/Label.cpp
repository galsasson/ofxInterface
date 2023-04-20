#include "Label.h"


namespace ofxInterface {
	Label::Label() :Node()
	{

	}

	Label::Label(const Label & mom):Node(mom)
	{
		font = mom.font;
		style = mom.style;
		text = mom.text;
		horzAlignment = mom.horzAlignment;
		isDropshadow = mom.isDropshadow;
		shadow = mom.shadow;
		shadowPos = mom.shadowPos;
	}


	Label::~Label()
	{
	}

	Node * Label::clone()
	{
		return new Label(*this);
	}

	void Label::draw()
	{
		auto bounds = font->getTextBounds(text, style, 0, 0);
		float dy = bounds.height*0.5;

		bounds.y += dy;

		// since fontstash uses its own clip functions, we need to manually disable the draw
		// masks out the text completely, when out of clip
		// need to find a more appropriate function
		//ofRectangle boundsGlobal( toGlobal(ofVec3f(bounds.x,bounds.y)),toGlobal(ofVec3f(bounds.width,bounds.height)) );
		ofRectangle boundsGlobal( toGlobal(ofVec3f(bounds.x,bounds.y)),bounds.width,bounds.height );
		boundsGlobal.y -= getHeight() * 0.5;
		//boundsGlobal.width -= boundsGlobal.x;
		//boundsGlobal.height -= boundsGlobal.y;

		auto cl = getGlobalRenderClip();
		auto in = getRenderClipRect().intersects(boundsGlobal);
		if (!getGlobalRenderClip() || getRenderClipRect().intersects(boundsGlobal)) {
			if (isDropshadow) {
				font->drawColumn(text, shadow, shadowPos.x, shadowPos.y + dy, getWidth(), horzAlignment);
			}
			font->drawColumn(text, style, 0, dy, getWidth(), horzAlignment);
		}

		
	}

	void ofxInterface::Label::setup(LabelSettings s)
	{
		Node::setup(s);
		font = s.font;
		style = s.style;
		text = s.text;
		horzAlignment = s.horzAlignment;
		setShadow(s.isDropshadow, s.shadowSize, s.shadowPos.x, s.shadowPos.y, s.shadowColor);

	}

	void Label::setText(string text_)
	{
		text = text_;
	}

	string Label::getText()
	{
		return text;
	}

	void Label::setAlignment(ofAlignHorz alignment_)
	{
		horzAlignment = alignment_;
	}
	void Label::setShadowEnabled(bool isEnabled)
	{
		isDropshadow = isEnabled;
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
	ofxFontStash2::Style Label::getFontStyle()
	{
		return style;
	}

	void Label::setColor(ofColor color)
	{
		style.color = color;
	}
	ofColor Label::getColor()
	{
		return style.color;
	}
}