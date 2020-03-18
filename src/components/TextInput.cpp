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

	// listener
	ofAddListener(eventTouchDown, this, &TextInput::onTouchDown);
	ofAddListener(eventTouchMove, this, &TextInput::onTouchMove);
}

void ofxInterface::TextInput::draw()
{
	auto baseRect = font->getTextBounds("X", style, 0, 0);
	// draw content
	if (content.get().size() == 0) {
		font->draw(descriptionText, styleInactive, 0, -baseRect.y);
	}else {
		font->draw(content, style, 0, -baseRect.y);
	}

	// draw index pointer
	ofSetColor(colorActive);
	auto indexRect = font->getTextBounds(content.get().substr(0, indexPointer), style, 0, 0);
	ofDrawRectangle(indexRect.x + indexRect.width, 0, 4, indexRect.height);
}

void ofxInterface::TextInput::keyPressed(ofKeyEventArgs & e)
{
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
