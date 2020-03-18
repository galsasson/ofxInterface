#pragma once
#include "ModalElement.h"
#include "ofxAnimatable.h"
#include "ofxAnimatableOfColor.h"

namespace ofxInterface {

	class SimpleChecker : public ModalElement
	{
	public:
		SimpleChecker();
		~SimpleChecker();

		void setup(ModalElementSettings s);
		virtual void draw();

		void onStateChanged(bool& isActive);

	protected:
	private:
		ofxAnimatableOfColor colorFade;
		int currentAni = 0;

		ofPath checkerShape;
			 
	};
}
