#include "ModalElement.h"


namespace ofxInterface {
	ModalElement::ModalElement()
	{
	}


	ModalElement::~ModalElement()
	{
	}

	void ModalElement::setup(ModalElementSettings s)
	{
		Node::setup(s);
		type = s.type;
		colorActive = s.colorActive;
		colorInactive = s.colorInactive;
		colorSelected = s.colorSelected;

		ofAddListener(eventClick, this, &ModalElement::onClicked,100);
		ofAddListener(eventTouchDown, this, &ModalElement::onTouchDown, 100);
		ofAddListener(eventTouchUp, this, &ModalElement::onTouchUp, 100);
		isSelected.addListener(this, &ModalElement::onStateChanged);
	}

	void ModalElement::draw()
	{
		if (getEnabled()) {
			ofSetColor(colorActive);
			ofDrawRectangle(0, 0, getWidth(), getHeight());
			if (isSelected) {
				ofSetColor(colorSelected);
				ofDrawRectangle(2, 2, getWidth()-4, getHeight()-4);
			}
		} else {
			ofSetColor(colorInactive);
			ofDrawRectangle(0, 0, getWidth(), getHeight());
		}

		
		
	}
	void ModalElement::onClicked(TouchEvent & e)
	{
		switch (type) {
		case ofxInterface::BUTTON:
			break;
		case ofxInterface::CHECKER:
			isSelected = !isSelected;
			break;
		case ofxInterface::RADIO:
			if (!isSelected) {
				isSelected = true;
			}
			break;
		default:
			break;
		}
		
	}

	void ModalElement::onTouchDown(TouchEvent & e) {
		if (type == BUTTON) {
			isSelected = true;
		}
	}

	void ModalElement::onTouchUp(TouchEvent & e) {
		if (type == BUTTON) {
			isSelected = false;
		}
	}

	void ModalElement::setModalType(ModalType type_) {
		type = type_;
	}

	ModalType ModalElement::getModalType() {
		return type;
	}

}