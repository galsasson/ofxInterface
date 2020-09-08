#include "TextInput.h"

void ofxInterface::TextInput::setup(TextInputSettings settings)
{
	settings.type = RADIO;
	ModalElement::setup(settings);
	descriptionText = settings.descriptionText;
	maxChars = settings.maxChars;

	font = settings.font;
	style = settings.style;
	styleInactive = style;
	styleInactive.color = colorInactive;
	enableNewline = settings.enableNewline;
	alignment = settings.alignment;
	autoResize = settings.autoResize;

	if (autoResize) {
		resizeField();
	}

	// listener
	ofAddListener(eventTouchDown, this, &TextInput::onTouchDown);
	ofAddListener(eventTouchMove, this, &TextInput::onTouchMove);

}

void ofxInterface::TextInput::draw()
{
	auto baseRect = font->getTextBounds("X", style, 0, 0);
	float dy = -baseRect.y + (getHeight() - baseRect.height)*0.75;

	if (enableNewline) {
		dy = -baseRect.y;
	}
	// draw content
	if (content.get().size() == 0) {
		font->drawColumn(descriptionText, styleInactive, 0, dy, getWidth(), alignment);
		//font->draw(descriptionText, styleInactive, 0, dy);
	}else {
		//font->draw(content, style, 0, dy);
		font->drawColumn(content, style, 0, dy, getWidth(), alignment);
	}

	// draw index pointer
	if (isSelected) {
		ofSetColor(colorActive);
		if (enableNewline) {
			vector<ofxFontStash2::StyledText> blocks;
			blocks.emplace_back(ofxFontStash2::StyledText({ content.get().substr(0, indexPointer), style }));
			vector<ofxFontStash2::StyledLine> lines;
			font->layoutLines(blocks, getWidth(), lines, alignment, 0);

			int wLine = 0;
			if (lines.size() > 0) {
				wLine = lines.back().lineW;
				if (lines.back().elements.size() > 0) {
					wLine += lines.back().elements.front().x;
				}
				else if (alignment == OF_ALIGN_HORZ_CENTER) {
					wLine = getWidth()*0.5;
				}
				else if (alignment == OF_ALIGN_HORZ_RIGHT) {
					wLine = getWidth();
				}
			}
			float hCursor = font->getTextBounds("|", style, 0, 0).height;

			ofDrawRectangle(baseRect.x + wLine, -hCursor - baseRect.y + baseRect.height*(lines.size() - 1), 4, hCursor);
		}
		else {
			auto indexRect = font->getTextBounds(content.get().substr(0, indexPointer), style, 0, 0);
			ofDrawRectangle(baseRect.x + indexRect.width, 0, 4, getHeight());
		}
	}
	
}

void ofxInterface::TextInput::keyPressed(ofKeyEventArgs & e)
{
	if (isSelected) {
		if (e.key == 356) { // left
			if (indexPointer > 0) {
				indexPointer--;
				string in = "";
				in += content.toString()[indexPointer - 1];
				if (in == "Ã") indexPointer--;
			}
		}
		if (e.key == 358) { // right
			if (indexPointer <= content->size()) {
				indexPointer++;
				string in = "";
				in += content.toString()[indexPointer];
				if (in == "Ã") indexPointer++;
			}
		}
		if (e.key == 8) { // backspace
			if (indexPointer > 0) {
				content = content.toString().erase(indexPointer - 1, 1);
				indexPointer--;
				string in = "";
				in += content.toString()[indexPointer];
				if (in == "Ã") {
					indexPointer--;
					content = content.toString().erase(indexPointer - 1, 1);
				}
			}
		}

		if (e.key == 13) { // return
			if (enableNewline) {
				if (maxChars == 0 || maxChars > content.get().size()) {
					string in = "\n";
					if (indexPointer < content->size()) {
						content = content.toString().insert(indexPointer, in);
					}
					else {
						content += in;
					}
					indexPointer += in.size();
				}
			}
			else {
				string t = content.get();
				ofNotifyEvent(returnPressedEvent, t);
			}

		}

		else if (e.key >= 32 && e.key <= 252) {
			string in = "";
			//only max characters allowed
			if (maxChars == 0 || maxChars > content.get().size())
			{
				// number field only accepts numbers
				if (e.key == 252) // ü
				{
					in += "ü";
				}
				else if (e.key == 220) // Ü
				{
					in += "Ü";
				}
				else if (e.key == 246) // ö
				{
					in += "ö";
				}
				else if (e.key == 214) // Ö
				{
					in += "Ö";
				}
				else if (e.key == 228) // ä
				{
					in += "ä";
				}
				else if (e.key == 196) // ä
				{
					in += "Ä";
				}
				else if (e.key == 223) // ß
				{
					in += "ß";
				}
				else if (e.key == 8364) // €
				{
					in += "€";
				}
				else { in += e.key; }

				if (indexPointer < content->size()) {
					content = content.toString().insert(indexPointer, in);
				}
				else {
					content += in;
				}
				indexPointer += in.size();
			}
		}
		sendContentChanged(content);
	}
}

string ofxInterface::TextInput::getContent()
{
	return content;
}

void ofxInterface::TextInput::setContent(string content_)
{
	if (content.toString() != content_) {
		content = content_;
		indexPointer = content->size();
		sendContentChanged(content);
	}
}

void ofxInterface::TextInput::onSetContent(string & content)
{
	setContent(content);
	sendContentChanged(content);
}

void ofxInterface::TextInput::setSize(float w, float h)
{
	// resize font if external resizing
	if (autoResize) {
		float newFontSize = style.fontSize * (w/getWidth());
		style.fontSize = newFontSize;
		styleInactive.fontSize = newFontSize;
		resizeField();
	}
	
	// proceed font resizing
	Node::setSize(w, h);
}

void ofxInterface::TextInput::setSize(const ofVec2f & s)
{
	setSize(s.x, s.y);
}

void ofxInterface::TextInput::setAlignment(ofAlignHorz alignment_)
{
	alignment = alignment_;
}

void ofxInterface::TextInput::setFontId(string fontId)
{
	style.fontID = fontId;
	styleInactive.fontID = fontId;
	resizeField();
}

void ofxInterface::TextInput::setColor(ofColor main, ofColor inactive)
{
	style.color = main;
	styleInactive.color = inactive;
}

void ofxInterface::TextInput::registerKeyInput(ofEvent<ofKeyEventArgs>& e)
{
	ofAddListener(e, this, &TextInput::keyPressed);
}

void ofxInterface::TextInput::unregisterKeyInput(ofEvent<ofKeyEventArgs> & e)
{
	ofRemoveListener(e, this, &TextInput::keyPressed);
}

void ofxInterface::TextInput::onTouchDown(TouchEvent & event)
{
	setIndexPosition(event.position - getPosition());
}

void ofxInterface::TextInput::onTouchMove(TouchEvent & event)
{
	setIndexPosition(event.position - getPosition());
}

void ofxInterface::TextInput::sendContentChanged(string content)
{
	string send = content;

	if (autoResize) {
		resizeField();
	}

	ofNotifyEvent(contentChangedEvent, send);
}

void ofxInterface::TextInput::setIndexPosition(ofVec2f touch)
{
	// get corresponding cursor pos
	float d = 99999999;
	float iNew = 0;
	if (font->getTextBounds(content.get(), style, 0, 0).width < touch.x){
		iNew = content.get().size();
	} else {
		for (size_t i = 0; i < content.get().size(); i++) {
			float p = font->getTextBounds(content.get().substr(0, i), style, 0, 0).width;
			if (abs(touch.x - p) < d) {
				d = abs(touch.x - p);
				iNew = i;
			}
		}
	}

	indexPointer = iNew;
}

void ofxInterface::TextInput::resizeField()
{
	string text = content.get().size() > 0 ? content : descriptionText;
	auto baseRect = font->getTextBounds("X", style, 0, 0);
	float dy = -baseRect.y - baseRect.height*0.75;

	if (enableNewline) {
		dy = -baseRect.y;
	}

	int w = 0;
	auto lines = ofSplitString(text, "\n");
	for (auto& line : lines) {
		float wt = font->getTextBounds(line, style, 0, 0).width;
		if (wt > w) {
			w = wt;
		}
	}

	

	// use parent function, since class function is to be resizing font as well
	Node::setSize(w + 1, dy + lines.size()*baseRect.height);
}
