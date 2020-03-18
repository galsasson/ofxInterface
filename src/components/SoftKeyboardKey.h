/*
 *  Key.h
 *  emptyExample
 *
 *  Created by Brian Eschrich on 23.02.16
 *  Copyright 2017 reddo UG. All rights reserved.
 *
 */

#ifndef _SoftKeyboardKey
#define _SoftKeyboardKey

#include "ofMain.h"
#include "ModalElement.h"
#include "ofxFontStash2.h"

namespace ofxInterface {
	struct SoftKeyboardKeySettings : ModalElementSettings {
		shared_ptr<ofxFontStash2::Fonts> font;
		ofxFontStash2::Style style;

		int key = ' ';

		int borderRadius = 4;
		int borderWidth = 1;

	};

    class SoftKeyboardKey : public ModalElement {
        
    public:
		virtual void setup(SoftKeyboardKeySettings s);
		virtual void draw();
        
        void onTouchDown(TouchEvent& event);
        void onTouchUp(TouchEvent& event);
        
        ofEvent<ofKeyEventArgs> 	keyPressed;
        ofEvent<ofKeyEventArgs> 	keyReleased;
     
	protected:
		void generateText();

    private:
        bool bTouched;
        
        int key;

		int borderRadius;
		int borderWidth;

		shared_ptr<ofxFontStash2::Fonts> font;
		ofxFontStash2::Style style;

		ofPath borderPath;
		ofPath fillPath;
		ofPath specialKeyPath;
		ofVec2f specialKeyPos;

		string text = "";
    };
}

#endif
