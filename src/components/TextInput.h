#pragma once

#include "ModalElement.h"
#include "ofxFontStash2.h"


namespace ofxInterface {
	struct TextInputSettings : public ModalElementSettings {
		shared_ptr<ofxFontStash2::Fonts> font;
		ofxFontStash2::Style style;

		int maxChars = 30;
		string descriptionText;
	};

	class TextInput : public ModalElement
	{
	public:
		void setup(TextInputSettings settings);

		virtual void draw() override;
		virtual void keyPressed(ofKeyEventArgs & e);
		string getContent();
		void setContent(string content);
		void onSetContent(string& content);

		ofEvent<string> contentChangedEvent;

		void registerKeyInput(ofEvent<ofKeyEventArgs> & e);
		void unregisterKeyInput(ofEvent<ofKeyEventArgs> & e);

		template<class ListenerClass, typename ListenerMethod>
		void addListener(ListenerClass * listener, ListenerMethod method) {
			content.addListener(listener, method);
		}

		template<class ListenerClass, typename ListenerMethod>
		void removeListener(ListenerClass * listener, ListenerMethod method) {
			content.removeListener(listener, method);
		}

		void onTouchDown(TouchEvent& event);
		void onTouchMove(TouchEvent& event);

	protected:
		void sendContentChanged(string content);
		void setIndexPosition(ofVec2f touch);

		shared_ptr<ofxFontStash2::Fonts> font;
		ofxFontStash2::Style style;
		ofxFontStash2::Style styleInactive;

		ofParameter<string> content;
		string descriptionText;
		int indexPointer;

		bool isNumberField;
		int maxChars;

	private:
		
	};

}