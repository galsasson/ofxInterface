/*
 *  SoftKeyboard.cpp
 *  emptyExample
 *
 *  Created by Brian Eschrich on 23.02.16
 *  Copyright 2017 reddo UG. All rights reserved.
 *
 */

#include "SoftKeyboard.h"

namespace ofxInterface {
	void SoftKeyboard::setup(SoftKeyboardSettings s)
	{
		Node::setup(s);
		font = s.font;
		style = s.style;

		bgColor = s.bgColor;
		colorActive = s.colorActive;
		colorInactive = s.colorInactive;
		colorSelected = s.colorSelected;
		borderRadius = s.borderRadius;
		borderWidth = s.borderWidth;
		padding = s.padding;
		margin = s.margin;



		// fixed ratio
		setHeight(getWidth() * 19.4 / 51);

		// create keys
		wKey = (getWidth() - 2 * (margin + borderWidth) - 10*padding) / 11;

		vector< vector<int>> alphaKeys = { 
			{'q','w','e','r','t','z','u','i','o','p',252},
			{'a','s','d','f','g','h','j','k','l',246,228},
			{ 3680,'y','x','c','v','b','n','m',8},
			{ 9997,'@',' ','.',13}
		};

		addSet("alpha", alphaKeys);

		vector< vector<int>> alphaBigKeys = {
			{'Q','W','E','R','T','Z','U','I','O','P',220},
			{'A','S','D','F','G','H','J','K','L',214,196},
			{ 3680,'Y','X','C','V','B','N','M',8},
			{ 9997,'@',' ','.',13}
		};

		addSet("alphaBig", alphaBigKeys);

		vector< vector<int>> numbers = {
			{'1','2','3','4','5','6','7','8','9','0','='},
			{'!','#','$','%','&','\'','*','+','-','/','?'},
			{'^','_','`','{','|','}','~','(',')',',',':' },
			{ 9996,'@',' ',';',13}
		};

		addSet("numbers", numbers);


		setActiveKeyset( "alpha");


		// draw border
		if (borderWidth > 0) {
			borderPath.setColor(style.color);
			borderPath.setArcResolution(200);
			// other path
			borderPath.moveTo(borderRadius, 0);
			borderPath.lineTo(getWidth() - borderRadius, 0);
			borderPath.arc(ofVec2f(getWidth() - borderRadius, borderRadius), borderRadius, borderRadius, 270, 0);
			borderPath.lineTo(getWidth(), getHeight() - borderRadius);
			borderPath.arc(ofVec2f(getWidth() - borderRadius, getHeight() - borderRadius), borderRadius, borderRadius, 0, 90);
			borderPath.lineTo(borderRadius, getHeight());
			borderPath.arc(ofVec2f(borderRadius, getHeight() - borderRadius), borderRadius, borderRadius, 90, 180);
			borderPath.lineTo(0, borderRadius);
			borderPath.arc(ofVec2f(borderRadius, borderRadius), borderRadius, borderRadius, 180, 270);
			// inner path
			borderPath.moveTo(borderRadius, borderWidth);
			borderPath.lineTo(getWidth() - borderRadius, borderWidth);
			borderPath.arc(ofVec2f(getWidth() - borderRadius, borderRadius), borderRadius - borderWidth, borderRadius - borderWidth, 270, 0);
			borderPath.lineTo(getWidth() - borderWidth, getHeight() - borderRadius);
			borderPath.arc(ofVec2f(getWidth() - borderRadius, getHeight() - borderRadius), borderRadius - borderWidth, borderRadius - borderWidth, 0, 90);
			borderPath.lineTo(borderRadius, getHeight() - borderWidth);
			borderPath.arc(ofVec2f(borderRadius, getHeight() - borderRadius), borderRadius - borderWidth, borderRadius - borderWidth, 90, 180);
			borderPath.lineTo(borderWidth, borderRadius);
			borderPath.arc(ofVec2f(borderRadius, borderRadius), borderRadius - borderWidth, borderRadius - borderWidth, 180, 270);
			borderPath.close();
		}
	}
    
    //------------------------------------------------------------------
    void SoftKeyboard::draw() {
		borderPath.draw();
        
    }

	void SoftKeyboard::setActiveKeyset(string id)
	{
		for (auto& keySet : keySets) {
			if (keySet.first == id) {
				keySet.second->activate();
				activeKeySet = keySet.second;
			}
			else {
				keySet.second->deactivate();
			}
		}
	}
	
	void SoftKeyboard::addSet(string keySet, vector<vector<int>> keys)
	{
		keySets.insert(make_pair(keySet, new Node()));
		addChild(keySets[keySet]);
		keySets[keySet]->setName(keySet);

		int py = borderWidth + margin;
		for (auto& line : keys) {
			addLine(keySets[keySet], line, py);
			py += wKey + padding;
		}
	}

	void SoftKeyboard::addLine(Node* keySet, vector<int> keys, vector<float> widthKeys, int y)
	{
		int wLine = margin* (widthKeys.size()-1);
		for (auto& w : widthKeys) {
			wLine += wKey * w;
		}

		float x = margin + borderWidth;
		int i = 0;
		for (auto& key : keys) {
			float w =  widthKeys[i];

			addKey(keySet, key, x, y, w, wKey);

			x += w + padding;
			++i;
		}
	}

	void SoftKeyboard::addLine(Node* keySet, vector<int> keys, int y){
		map<int, float> keyWidths;
		keyWidths[' '] = wKey * 5 + padding * 4;
		keyWidths['@'] = (3 * wKey + padding) *0.5;
		keyWidths[9997] = (3 * wKey + padding) *0.5; // abc
		keyWidths[9996] = (3 * wKey + padding) *0.5; // 123
		keyWidths[3680] = wKey * 2 + padding; // shift
		keyWidths[8] = wKey * 2 + padding; // backspace
		keyWidths[13] = wKey * 2 + padding; // return

		vector<float> widthKeys;

        for (auto key : keys) {
			if (keyWidths.find(key) != keyWidths.end()) {
				widthKeys.push_back(keyWidths[key]);
			}else {
				widthKeys.push_back(wKey);
			}
        }
		addLine(keySet, keys, widthKeys, y);
    }
    
    void SoftKeyboard::addKey(Node* keySet, int keyValue, int x, int y, int w, int h){

        SoftKeyboardKey* key = new SoftKeyboardKey();
		SoftKeyboardKeySettings s;

		s.name = "key_" + ofToString(keyValue);
		s.position = ofVec2f(x, y);
		s.size = ofVec2f(w, h);
		s.borderWidth = borderWidth * 0.25;
		s.borderRadius = borderRadius * 0.5;
		s.colorActive = colorActive;
		s.colorInactive = colorInactive;
		s.colorSelected = colorSelected;
		s.font = font;
		s.key = keyValue;
		s.style = style;

		key->setup(s);

        keySet->addChild(key);
        
        ofAddListener(key->keyPressed, this, &SoftKeyboard::onKeyPressed);
        ofAddListener(key->keyReleased, this, &SoftKeyboard::onKeyReleased);
    }
    
    void SoftKeyboard::onKeyPressed(ofKeyEventArgs &event)
    {
		if (activeKeySet->getName() == "alphaBig") {
			setActiveKeyset("alpha");
		}
        else if(activeKeySet->getName() == "alpha" && (event.key == 3680 || event.key == 3681)){
				setActiveKeyset("alphaBig");
        }
		else if (event.key == 9996) {
			setActiveKeyset("alpha");
		}
		else if (event.key == 9997) {
			setActiveKeyset("numbers");
		}
        ofNotifyEvent(keyPressed, event);
    }
    
    void SoftKeyboard::onKeyReleased(ofKeyEventArgs &event)
    {
        ofNotifyEvent(keyReleased, event);
    }
    
}