#include "SimpleChecker.h"


namespace ofxInterface {
	SimpleChecker::SimpleChecker()
	{
	}


	SimpleChecker::~SimpleChecker()
	{
	}

	void SimpleChecker::setup(ModalElementSettings s)
	{
		s.type = CHECKER;
		ModalElement::setup(s);

		colorFade.setColor(colorInactive);
		colorFade.setDuration(0.10);

		// init shape
		float multX = float(getWidth()) / 32;
		float multY = float(getHeight()) / 32;
		checkerShape.moveTo(2*multX, 6 * multY);
		checkerShape.lineTo(0 * multX, 8 * multY);
		checkerShape.lineTo(7 * multX, 15 * multY);
		checkerShape.lineTo(20 * multX, 2 * multY);
		checkerShape.lineTo(18 * multX, 0 * multY);
		checkerShape.lineTo(7 * multX, 11 * multY);
		checkerShape.lineTo(2 * multX,6 * multY);

		checkerShape.close();
		checkerShape.translate(ofVec2f(6 * multX, 8 * multY));
		checkerShape.setColor(colorSelected);
	}
	void SimpleChecker::draw()
	{
		colorFade.update(ofGetLastFrameTime());
		ofSetColor(colorFade.getCurrentColor());

		ofDrawRectangle(0, 0, getWidth(), getHeight());

		if (isSelected) {
			checkerShape.draw();
		}
		
	}

	void SimpleChecker::onStateChanged(bool & isActive)
	{
		if (isActive) {
			colorFade.animateTo(colorActive);
		}
		else {
			colorFade.animateTo(colorInactive);
		}
	}

}