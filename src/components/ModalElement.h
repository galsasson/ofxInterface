#pragma once
#include "Node.h"

namespace ofxInterface {
	enum ModalType {
		BUTTON,
		CHECKER,
		RADIO
	};

	struct ModalElementSettings : NodeSettings{
		ModalType type = BUTTON;
		ofColor colorActive = ofColor(200);
		ofColor colorSelected = ofColor(100);
		ofColor colorInactive = ofColor(50);
	};

	class ModalElement : public Node
	{
	public:
		ModalElement();
		ModalElement(const ModalElement& mom);
		~ModalElement();

		virtual Node* clone() override;

		void setup(ModalElementSettings s);
		virtual void draw();

		void onClicked(TouchEvent& e);
		void onTouchDown(TouchEvent& e);
		void onTouchUp(TouchEvent& e);
		virtual void onStateChanged(bool& isActive) {};
		ofParameter<bool> isSelected = false;

		void setModalType(ModalType type);
		ModalType getModalType();

		ofColor colorActive;
		ofColor colorSelected;
		ofColor colorInactive;

		ofJson getNodeJson() override;
		 
	private:
		ModalType type;
		
	};
}
