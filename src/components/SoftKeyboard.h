/*
 *  SoftKeyboard.h
 *  emptyExample
 *
 *  Created by Brian Eschrich on 23.02.16
 *  Copyright 2017 reddo UG. All rights reserved.
 *
 */

#ifndef _SoftKeyboard
#define _SoftKeyboard

#include "ofMain.h"
#include "SoftKeyboardKey.h"

namespace ofxInterface {
	struct SoftKeyboardSettings : NodeSettings {
		shared_ptr<ofxFontStash2::Fonts> font;
		ofxFontStash2::Style style;

		ofColor bgColor = ofColor(0, 0);
		ofColor colorActive = ofColor(255);
		ofColor colorInactive = ofColor(128);
		ofColor colorSelected = ofColor(128);

		int borderRadius = 8;
		int borderWidth = 4;
		int margin = 12;
		int padding = 8;

		string layout = "de";

		map<int, ofTexture> textureKeys;

	};


class SoftKeyboard : public Node{
	
public:
	virtual void setup(SoftKeyboardSettings s);
    virtual void draw();
    
	void setActiveKeyset(string id);

    void onKeyPressed(ofKeyEventArgs& event);
    void onKeyReleased(ofKeyEventArgs& event);
    
    ofEvent<ofKeyEventArgs> 	keyPressed;
    ofEvent<ofKeyEventArgs> 	keyReleased;
    
protected:
	void addSet(string keySet, vector<vector<int>> keys);
	void addLine(Node* keySet, vector<int> keys, vector<float> widthKeys, int y);
    void addLine(Node* keySet, vector<int> keys, int y);
    void addKey(Node* keySet, int keyValue, int x, int y, int w, int h);

	map<string, vector< vector<int>> > getKeyLayout(string layout);

	map<string,Node*> keySets;
	Node* activeKeySet;

	shared_ptr<ofxFontStash2::Fonts> font;
	ofxFontStash2::Style style;

	ofColor bgColor;
	ofColor colorActive;
	ofColor colorInactive;
	ofColor colorSelected;
	int borderRadius;

	int wKey;
	int hKey;
	int margin;
	int padding;
	int borderWidth;

	ofPath borderPath;
	map<int, ofTexture> textureKeys;

private:
    
   
	
};
}

#endif
