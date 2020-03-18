/*
 *  Key.cpp
 *  emptyExample
 *
 *  Created by Brian Eschrich on 23.02.16
 *  Copyright 2017 reddo UG. All rights reserved.
 *
 */

#include "SoftKeyboardKey.h"

namespace ofxInterface {
	 void SoftKeyboardKey::setup(SoftKeyboardKeySettings s)
	 {
		 // init values
		 ModalElement::setup(s);
		 font = s.font;
		 style = s.style;
		 key = s.key;
		 borderRadius = s.borderRadius;
		 borderWidth = s.borderWidth;


		 // draw paths
		 if (borderWidth > 0) {
			 borderPath.setColor(style.color);
			 borderPath.setArcResolution(200);
			 // other path
			 borderPath.moveTo(borderRadius, 0);
			 borderPath.lineTo(getWidth() - borderRadius, 0);
			 borderPath.arc(ofVec2f(getWidth() - borderRadius, borderRadius), borderRadius, borderRadius, 270, 0);
			 borderPath.lineTo(getWidth(),getHeight()-borderRadius);
			 borderPath.arc(ofVec2f(getWidth() - borderRadius, getHeight() - borderRadius), borderRadius, borderRadius, 0, 90);
			 borderPath.lineTo(borderRadius, getHeight());
			 borderPath.arc(ofVec2f(borderRadius, getHeight() - borderRadius), borderRadius, borderRadius, 90, 180);
			 borderPath.lineTo(0, borderRadius);
			 borderPath.arc(ofVec2f(borderRadius, borderRadius), borderRadius, borderRadius, 180, 270);
			 // inner path
			 borderPath.moveTo(borderRadius, borderWidth);
			 borderPath.lineTo(getWidth() - borderRadius, borderWidth);
			 borderPath.arc(ofVec2f(getWidth() - borderRadius, borderRadius), borderRadius - borderWidth, borderRadius -borderWidth, 270, 0);
			 borderPath.lineTo(getWidth() - borderWidth, getHeight() - borderRadius);
			 borderPath.arc(ofVec2f(getWidth() - borderRadius, getHeight() - borderRadius), borderRadius - borderWidth, borderRadius - borderWidth, 0, 90);
			 borderPath.lineTo(borderRadius, getHeight() - borderWidth);
			 borderPath.arc(ofVec2f(borderRadius, getHeight() - borderRadius), borderRadius - borderWidth, borderRadius - borderWidth, 90, 180);
			 borderPath.lineTo(borderWidth, borderRadius);
			 borderPath.arc(ofVec2f(borderRadius, borderRadius), borderRadius - borderWidth, borderRadius - borderWidth, 180, 270);
			 borderPath.close();
		 }

		 // background
		 if (!(colorInactive.a == 0 && colorActive.a == 0 && colorSelected.a == 0)) {
			 fillPath.setUseShapeColor(false);
			 fillPath.setArcResolution(200);
			 fillPath.moveTo(borderRadius, borderWidth);
			 fillPath.lineTo(getWidth() - borderRadius, borderWidth);
			 fillPath.arc(ofVec2f(getWidth() - borderRadius, borderRadius), borderRadius - borderWidth, borderRadius - borderWidth, 270, 0);
			 fillPath.lineTo(getWidth() - borderWidth, getHeight() - borderRadius);
			 fillPath.arc(ofVec2f(getWidth() - borderRadius, getHeight() - borderRadius), borderRadius - borderWidth, borderRadius - borderWidth, 0, 90);
			 fillPath.lineTo(borderRadius, getHeight() - borderWidth);
			 fillPath.arc(ofVec2f(borderRadius, getHeight() - borderRadius), borderRadius - borderWidth, borderRadius - borderWidth, 90, 180);
			 fillPath.lineTo(borderWidth, borderRadius);
			 fillPath.arc(ofVec2f(borderRadius, borderRadius), borderRadius - borderWidth, borderRadius - borderWidth, 180, 270);
			 fillPath.close();
		 }

		 generateText();

		 // listener
		 ofAddListener(eventTouchDown, this, &SoftKeyboardKey::onTouchDown);
		 ofAddListener(eventTouchUp, this, &SoftKeyboardKey::onTouchUp);
	 }
    
    //------------------------------------------------------------------
    void SoftKeyboardKey::draw() {

		borderPath.draw();

		if (isSelected) {
			ofSetColor(colorSelected);
		}
		else if(getEnabled()){
			ofSetColor(colorActive);
		}
		else {
			ofSetColor(colorInactive);
		}

		fillPath.draw();
        
        
        //center text
        ofRectangle r = font->getTextBounds(text, style,0,0);
		// hack to reduce line height to glyph height
		r.y += r.height*0.25;
		r.height *= 0.6;

		font->draw(text,style, (getWidth()-r.width)/2, 0.5*(getHeight() - r.height) - r.y);
		specialKeyPath.draw(specialKeyPos.x, specialKeyPos.y);
        
    }
    
    
    
    void SoftKeyboardKey::onTouchDown(TouchEvent &event)
    {
        
            ofKeyEventArgs out(ofKeyEventArgs::Pressed, key);

			ofNotifyEvent(keyPressed, out);
   
        
        bTouched = true;
    }
    
    void SoftKeyboardKey::onTouchUp(TouchEvent &event)
    {
            ofKeyEventArgs out(ofKeyEventArgs::Released, key);
            ofNotifyEvent(keyReleased, out);
        
        bTouched = false;
    }
	void SoftKeyboardKey::generateText()
	{

		float glyphHeight = font->getTextBounds("X", style, 0, 0).height*0.6;
		float glyphCurveRadius = borderRadius * 0.5;
		float glyphStroke = 1.0;

		if (key == 252) {
			text = "ü";
		}
		else if (key == 220) {
			text = "Ü";
		}
		else if (key == 228) {
			text = "ä";
		}
		else if (key == 196) {
			text = "Ä";
		}
		else if (key == 246) {
			text = "ö";
		}
		else if (key == 214) {
			text = "Ö";
		}
		else if (key == 223) {
			text = "ß";
		}
		else if (key == 8364) {
			text = "€";
		}
		else if (key == 127) {
			text = "b";
		}
		else if (key == 13) {
			text = "Return";
		}
		else if (key == 8) { // backspace
			text = "";
			specialKeyPath.setColor(style.color);
			specialKeyPath.setArcResolution(50);
			// other path
			specialKeyPath.moveTo(glyphHeight *24.0/18.0 - glyphCurveRadius, 0);
			specialKeyPath.arc(ofVec2f(glyphHeight *24.0 / 18.0 - glyphCurveRadius, glyphCurveRadius), glyphCurveRadius, glyphCurveRadius, 270, 0); // rightTop
			specialKeyPath.lineTo(glyphHeight *24.0 / 18.0, glyphHeight - glyphCurveRadius);
			specialKeyPath.arc(ofVec2f(glyphHeight *24.0 / 18.0 - glyphCurveRadius, glyphHeight - glyphCurveRadius), glyphCurveRadius, glyphCurveRadius, 0, 90); // rightBottom
			specialKeyPath.lineTo(glyphHeight *9.0 / 18.0, glyphHeight);
			specialKeyPath.arc(ofVec2f(glyphHeight *9.0 / 18.0 - glyphCurveRadius, glyphHeight - glyphCurveRadius), glyphCurveRadius, glyphCurveRadius, 90, 135); // leftBottom
			specialKeyPath.lineTo(glyphCurveRadius * 0.5, 9.0/18.0*glyphHeight + glyphCurveRadius);
			specialKeyPath.arc(ofVec2f(glyphCurveRadius, 9.0 / 18.0*glyphHeight), glyphCurveRadius, glyphCurveRadius, 135, 225); // left
			specialKeyPath.lineTo(9.0 / 18.0*glyphHeight - 1.924*glyphCurveRadius,  glyphCurveRadius * 0.383);
			specialKeyPath.arc(ofVec2f(9.0 / 18.0*glyphHeight - glyphCurveRadius, glyphCurveRadius), glyphCurveRadius, glyphCurveRadius, 225, 270); // leftTop
			specialKeyPath.close();

			// inner path
			specialKeyPath.moveTo(glyphHeight *24.0 / 18.0 - glyphCurveRadius, glyphStroke);
			specialKeyPath.arc(ofVec2f(glyphHeight *24.0 / 18.0 - glyphCurveRadius, glyphCurveRadius), glyphCurveRadius - glyphStroke, glyphCurveRadius - glyphStroke, 270, 0); // rightTop
			specialKeyPath.lineTo(glyphHeight *24.0 / 18.0 - glyphStroke, glyphHeight - glyphCurveRadius);
			specialKeyPath.arc(ofVec2f(glyphHeight *24.0 / 18.0 - glyphCurveRadius, glyphHeight - glyphCurveRadius), glyphCurveRadius - glyphStroke, glyphCurveRadius - glyphStroke, 0, 90); // rightBottom
			specialKeyPath.lineTo(glyphHeight *9.0 / 18.0, glyphHeight - glyphStroke);
			specialKeyPath.arc(ofVec2f(glyphHeight *9.0 / 18.0 - glyphCurveRadius, glyphHeight - glyphCurveRadius), glyphCurveRadius - glyphStroke, glyphCurveRadius - glyphStroke, 90, 135); // leftBottom
			specialKeyPath.lineTo(glyphCurveRadius * 0.5 + glyphStroke*0.924, 9.0 / 18.0*glyphHeight + glyphCurveRadius);
			specialKeyPath.arc(ofVec2f(glyphCurveRadius, 9.0 / 18.0*glyphHeight), glyphCurveRadius - glyphStroke, glyphCurveRadius - glyphStroke, 135, 225); // left
			specialKeyPath.lineTo(9.0 / 18.0*glyphHeight - 1.924*glyphCurveRadius, glyphCurveRadius * 0.383 + glyphStroke);
			specialKeyPath.arc(ofVec2f(9.0 / 18.0*glyphHeight - glyphCurveRadius, glyphCurveRadius), glyphCurveRadius - glyphStroke, glyphCurveRadius - glyphStroke, 225, 270); // leftTop
			specialKeyPath.close();
			
			// x
			float glyphStrokeD = sqrt(glyphStroke*glyphStroke * 2);
			specialKeyPath.moveTo(glyphHeight *15.0 / 18.0 - glyphStrokeD, glyphHeight *9.0 / 18.0);
			specialKeyPath.lineTo(glyphHeight *12.0 / 18.0 - glyphStroke * 0.383, glyphHeight *12.0 / 18.0 -glyphStroke * 0.924);
			specialKeyPath.arcNegative(ofVec2f(glyphHeight *12.0 / 18.0, glyphHeight *12.0 / 18.0), glyphStroke, glyphStroke, 225, 45); // leftBottom
			specialKeyPath.lineTo(glyphHeight *15.0 / 18.0, glyphHeight *9.0 / 18.0 + glyphStrokeD);
			specialKeyPath.lineTo(glyphHeight *18.0 / 18.0 - glyphStroke * 0.383, glyphHeight *12.0 / 18.0 + glyphStroke * 0.924);
			specialKeyPath.arcNegative(ofVec2f(glyphHeight *18.0 / 18.0, glyphHeight *12.0 / 18.0), glyphStroke, glyphStroke, 135, 315); // rightBottom
			specialKeyPath.lineTo(glyphHeight *15.0 / 18.0 + glyphStrokeD, glyphHeight *9.0 / 18.0);
			specialKeyPath.lineTo(glyphHeight *18.0 / 18.0 + glyphStroke * 0.383, glyphHeight *6.0 / 18.0 +glyphStroke * 0.924);
			specialKeyPath.arcNegative(ofVec2f(glyphHeight *18.0 / 18.0, glyphHeight *6.0 / 18.0), glyphStroke, glyphStroke, 45, 225); // rightTop
			specialKeyPath.lineTo(glyphHeight *15.0 / 18.0 , glyphHeight *9.0 / 18.0 - glyphStrokeD);
			specialKeyPath.lineTo(glyphHeight *12.0 / 18.0 + glyphStroke * 0.383, glyphHeight *6.0 / 18.0 - glyphStroke * 0.924);
			specialKeyPath.arcNegative(ofVec2f(glyphHeight *12.0 / 18.0 , glyphHeight *6.0 / 18.0), glyphStroke, glyphStroke, 315, 135); // leftTop
			specialKeyPath.close();
			
			float wGlyph = glyphHeight * 12 / 9.0;
			specialKeyPos = ofVec2f(0.5*(getWidth() - wGlyph), 0.5*(getHeight() - glyphHeight));
		}
		else if (key == 769) {
			text = "strg";
		}
		else if (key == 770) {
			text = "strg";
		}
		else if (key == 1281) {
			text = "alt";
		}
		else if (key == 1282) {
			text = "alt";
		}
		else if (key == 3680 || key == 3681) {//shift
			text = "";
			
			specialKeyPath.setColor(style.color);
			specialKeyPath.setArcResolution(50);
			// other path
			specialKeyPath.moveTo(glyphCurveRadius, 11.0/18.0*glyphHeight);
			specialKeyPath.lineTo(4.0 / 18.0*glyphHeight, 11.0 / 18.0*glyphHeight);
			specialKeyPath.lineTo(4.0 / 18.0*glyphHeight, glyphHeight - glyphCurveRadius);
			specialKeyPath.arcNegative(ofVec2f(4.0 / 18.0*glyphHeight + glyphCurveRadius, glyphHeight - glyphCurveRadius), glyphCurveRadius, glyphCurveRadius, 180, 90);
			specialKeyPath.lineTo(16.0 / 18.0*glyphHeight - glyphCurveRadius, glyphHeight);
			specialKeyPath.arcNegative(ofVec2f(16.0 / 18.0*glyphHeight - glyphCurveRadius, glyphHeight - glyphCurveRadius), glyphCurveRadius, glyphCurveRadius, 90, 0);
			specialKeyPath.lineTo(16.0 / 18.0*glyphHeight, 11.0 / 18.0*glyphHeight);
			specialKeyPath.lineTo(20.0 / 18.0*glyphHeight - glyphCurveRadius, 11.0 / 18.0*glyphHeight);
			specialKeyPath.arcNegative(ofVec2f(20.0 / 18.0*glyphHeight - glyphCurveRadius, 11.0 / 18.0*glyphHeight - glyphCurveRadius), glyphCurveRadius, glyphCurveRadius, 90, 315); // right
			specialKeyPath.lineTo(10.0 / 18.0*glyphHeight + glyphCurveRadius*1.5, glyphCurveRadius);
			specialKeyPath.arcNegative(ofVec2f(10.0 / 18.0*glyphHeight, glyphCurveRadius), glyphCurveRadius, glyphCurveRadius, 315, 225); // top
			specialKeyPath.lineTo((1.0 - 0.383)*glyphCurveRadius, 11.0 / 18.0*glyphHeight - glyphCurveRadius * 1.924);
			specialKeyPath.arcNegative(ofVec2f(glyphCurveRadius, 11.0 / 18.0*glyphHeight - glyphCurveRadius), glyphCurveRadius, glyphCurveRadius, 225, 90); // left

			// inner path
			specialKeyPath.moveTo(glyphCurveRadius, 11.0 / 18.0*glyphHeight - glyphStroke);
			specialKeyPath.lineTo(4.0 / 18.0*glyphHeight + glyphStroke, 11.0 / 18.0*glyphHeight- glyphStroke);
			specialKeyPath.lineTo(4.0 / 18.0*glyphHeight + glyphStroke, glyphHeight - glyphCurveRadius);
			specialKeyPath.arcNegative(ofVec2f(4.0 / 18.0*glyphHeight + glyphCurveRadius, glyphHeight - glyphCurveRadius), glyphCurveRadius- glyphStroke, glyphCurveRadius - glyphStroke, 180, 90);
			specialKeyPath.lineTo(16.0 / 18.0*glyphHeight - glyphCurveRadius, glyphHeight - glyphStroke);
			specialKeyPath.arcNegative(ofVec2f(16.0 / 18.0*glyphHeight - glyphCurveRadius, glyphHeight - glyphCurveRadius), glyphCurveRadius - glyphStroke, glyphCurveRadius - glyphStroke, 90, 0);
			specialKeyPath.lineTo(16.0 / 18.0*glyphHeight - glyphStroke, 11.0 / 18.0*glyphHeight - glyphStroke);
			specialKeyPath.lineTo(20.0 / 18.0*glyphHeight - glyphCurveRadius, 11.0 / 18.0*glyphHeight - glyphStroke);
			specialKeyPath.arcNegative(ofVec2f(20.0 / 18.0*glyphHeight - glyphCurveRadius, 11.0 / 18.0*glyphHeight - glyphCurveRadius), glyphCurveRadius - glyphStroke, glyphCurveRadius - glyphStroke, 90, 315); // right
			specialKeyPath.lineTo(10.0 / 18.0*glyphHeight + glyphCurveRadius * 1.5 - glyphStroke, glyphCurveRadius);
			specialKeyPath.arcNegative(ofVec2f(10.0 / 18.0*glyphHeight, glyphCurveRadius), glyphCurveRadius - glyphStroke, glyphCurveRadius - glyphStroke, 315, 225); // top
			specialKeyPath.lineTo((1.0 - 0.383)*glyphCurveRadius + glyphStroke, 11.0 / 18.0*glyphHeight - glyphCurveRadius * 1.924);
			specialKeyPath.arcNegative(ofVec2f(glyphCurveRadius, 11.0 / 18.0*glyphHeight - glyphCurveRadius), glyphCurveRadius - glyphStroke, glyphCurveRadius - glyphStroke, 225, 90); // left
			borderPath.close();

			float wGlyph = glyphHeight * 10 / 9.0;
			specialKeyPos = ofVec2f(0.5*(getWidth() - wGlyph), 0.5*(getHeight() - glyphHeight));
			
		}
		else if (key == 356) { //left
			text = "<-";
		}
		else if (key == 358) { //right
			text = "->";
		}
		else if (key == 9997) { //switch keypad
			text = "123";
		}
		else if (key == 9996) { //switch keys
			text = "abc";
		}
		else {
			text += key;
		}
	}
}
