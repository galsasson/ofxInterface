#pragma once
#include "Node.h"

namespace ofxInterface {
	enum Direction {
		HORIZONTAL,
		VERTICAL
	};

	struct SliderSettings : NodeSettings{
		Direction direction = HORIZONTAL;
		ofColor colorActive = ofColor(200);
		ofColor colorSelected = ofColor(100);
		ofColor colorInactive = ofColor(50);
		int lineWidth = 6;
	};

	class Slider : public Node
	{
	public:
		Slider();
		Slider(const Slider& mom);
		~Slider();

		virtual Node* clone() override;

		void setup(SliderSettings s);
		virtual void draw();

		void onTouchDown(TouchEvent& e);
		//void onTouchUp(TouchEvent& e);
		void onTouchMove(TouchEvent& e);

		void setDirection(Direction type);
		Direction getDirection();

		ofColor colorActive;
		ofColor colorSelected;
		ofColor colorInactive;

		ofParameter<float> value;
		 
	private:
		Direction direction;
		int lineWidth;
	};
}
