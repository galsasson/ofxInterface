#include "Slider.h"


namespace ofxInterface {
	Slider::Slider()
	{
		ofAddListener(eventTouchMove, this, &Slider::onTouchMove, 100);
		ofAddListener(eventTouchDown, this, &Slider::onTouchDown, 100);
		//ofAddListener(eventTouchUp, this, &Slider::onTouchUp, 100);
		value.set("value", 0, 0, 1);
	}

	Slider::Slider(const Slider & mom) : Node(mom)
	{
		colorActive = mom.colorActive;
		colorSelected = mom.colorSelected;
		lineWidth = mom.lineWidth;
		colorInactive = mom.colorInactive;
		direction = mom.direction;
		value = mom.value;
		ofAddListener(eventTouchMove, this, &Slider::onTouchMove, 100);
		ofAddListener(eventTouchDown, this, &Slider::onTouchDown, 100);
	}


	Slider::~Slider()
	{
	}

	Node * Slider::clone()
	{
		return new  Slider(*this);
	}

	void Slider::setup(SliderSettings s)
	{
		Node::setup(s);
		colorActive = s.colorActive;
		colorSelected = s.colorSelected;
		lineWidth = s.lineWidth;
		colorInactive = s.colorInactive;
		direction = s.direction;
	}

	void Slider::draw()
	{
		float lActive;
		float lInactive;

		ofRectangle rActive;
		ofRectangle rInactive;

		if (direction == HORIZONTAL) {
			lActive = ofMap(value, value.getMin(), value.getMax(), 0, getWidth(), true);
			lInactive = getWidth() - lActive;
			rActive = ofRectangle(0, 0.5*(getHeight() - lineWidth), lActive, lineWidth);
			rInactive = ofRectangle(lActive, 0.5*(getHeight() - lineWidth), lInactive, lineWidth);
		}else {
			lActive = ofMap(value, value.getMin(), value.getMax(), 0, getHeight(), true);
			lInactive = getHeight() - lActive;
			rActive = ofRectangle(0.5*(getWidth() - lineWidth),0,  lineWidth, lActive );
			rInactive = ofRectangle(0.5*(getWidth() - lineWidth),lActive, lineWidth,lInactive);
		}

		ofSetColor(colorSelected);
		ofDrawRectangle(rActive);
		ofSetColor(colorInactive);
		ofDrawRectangle(rInactive);
		ofSetColor(colorActive);
		ofDrawCircle(rInactive.x, rInactive.y, 0.5*getHeight());
	}

	void Slider::onTouchDown(TouchEvent & e) {
		if (direction == HORIZONTAL) {
			value = ofMap(toLocal(e.position).x, 0, getWidth(), value.getMin(), value.getMax(), true);
		}
		else {
			value = ofMap(toLocal(e.position).y, 0, getHeight(), value.getMin(), value.getMax(), true);
		}
		
	}

	void Slider::onTouchMove(TouchEvent & e)
	{
		if (direction == HORIZONTAL) {
			value = ofMap(toLocal(e.position).x, 0, getWidth(), value.getMin(), value.getMax(), true);
		}
		else {
			value = ofMap(toLocal(e.position).y, 0, getHeight(), value.getMin(), value.getMax(), true);
		}
	}

	void Slider::setDirection(Direction type)
	{
		direction = type;
	}

	Direction Slider::getDirection()
	{
		return direction;
	}

	/*void Slider::onTouchUp(TouchEvent & e) {
		if (type == BUTTON) {
			isSelected = false;
		}
	}*/


}