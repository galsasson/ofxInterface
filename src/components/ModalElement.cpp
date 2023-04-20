#include "ModalElement.h"


namespace ofxInterface {
	ModalElement::ModalElement()
	{
		ofAddListener(eventClick, this, &ModalElement::onClicked, 100);
		ofAddListener(eventTouchDown, this, &ModalElement::onTouchDown, 100);
		ofAddListener(eventTouchUp, this, &ModalElement::onTouchUp, 100);
		isSelected.addListener(this, &ModalElement::onStateChanged);
	}

	ModalElement::ModalElement(const ModalElement & mom) : Node(mom)
	{
		colorActive = mom.colorActive;
		colorSelected = mom.colorSelected;
		colorInactive = mom.colorInactive;
		type = mom.type;
		isSelected = mom.isSelected.get();
		isSelected.addListener(this, &ModalElement::onStateChanged);
		ofAddListener(eventClick, this, &ModalElement::onClicked, 100);
		ofAddListener(eventTouchDown, this, &ModalElement::onTouchDown, 100);
		ofAddListener(eventTouchUp, this, &ModalElement::onTouchUp, 100);
	}


	ModalElement::~ModalElement()
	{
	}

	Node * ModalElement::clone()
	{
		return new  ModalElement(*this);
	}

	void ModalElement::setup(ModalElementSettings s)
	{
		Node::setup(s);
		type = s.type;
		colorActive = s.colorActive;
		colorInactive = s.colorInactive;
		colorSelected = s.colorSelected;
		
		
	}

	void ModalElement::draw()
	{
		//if (type == ofxInterface::BUTTON && !isTouched() && isSelected) {
		//	isSelected = false;
		//}

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

	ofJson ModalElement::getNodeJson()
	{
		auto ret = Node::getNodeJson();
		ret["nodeType"] = "ModalElement";
		
		switch (type) {
		case BUTTON:
			ret["modalType"] = "Button";
			break;
		case CHECKER:
			ret["modalType"] = "Checker";
			break;
		case RADIO:
			ret["modalType"] = "Radio";
			break;
		}
		ret["colorActive"] = vector<int>{ colorActive.r,colorActive.g,colorActive.b,colorActive.a };
		ret["colorSelected"] = vector<int>{ colorSelected.r,colorSelected.g,colorSelected.b,colorSelected.a };
		ret["colorInactive"] = vector<int>{ colorInactive.r,colorInactive.g,colorInactive.b,colorInactive.a };
		
		return ret;
	}

}