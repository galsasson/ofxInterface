//
//  LambdaView.cpp
//  ofxInterface
//
//  Created by Gal Sasson on 12/17/15.
//
//

#include "LambdaView.h"

namespace ofxInterface
{

LambdaView::LambdaView(const string& name)
{
	setName(name);
	ofAddListener(eventTouchDown, this, &LambdaView::onTouchDown);
	ofAddListener(eventTouchMove, this, &LambdaView::onTouchMove);
	ofAddListener(eventTouchUp, this, &LambdaView::onTouchUp);
}

void LambdaView::setDrawFunction(std::function<void(void)> _func)
{
	drawFunc = _func;
}

void LambdaView::setDrawDebugFunction(std::function<void(void)> _func)
{
	drawDebugFunc = _func;
}

void LambdaView::setUpdateFunction(std::function<void(float)> _func)
{
	updateFunc = _func;
}

void LambdaView::setTouchDownFunction(std::function<void(ofxInterface::TouchEvent&)> _func)
{
	touchDownFunc = _func;
}

void LambdaView::setTouchMoveFunction(std::function<void(ofxInterface::TouchEvent&)> _func)
{
	touchMoveFunc = _func;
}

void LambdaView::setTouchUpFunction(std::function<void(ofxInterface::TouchEvent&)> _func)
{
	touchUpFunc = _func;
}

void LambdaView::setContainsFunction(std::function<bool(const ofVec3f& global)> _func)
{
	containsFunc = _func;
}

void LambdaView::draw()
{
	if (drawFunc) {
		drawFunc();
	}
}

void LambdaView::drawDebug()
{
	if (drawDebugFunc) {
		drawDebugFunc();
	}
}

void LambdaView::update(float dt)
{
	if (updateFunc) {
		updateFunc(dt);
	}
}

bool LambdaView::contains(const ofVec3f& global)
{
	if (containsFunc) {
		return containsFunc(global);
	}
	else {
		return Node::contains(global);
	}
}

void LambdaView::onTouchDown(TouchEvent& event)
{
	if (touchDownFunc) {
		touchDownFunc(event);
	}
}

void LambdaView::onTouchMove(TouchEvent& event)
{
	if (touchMoveFunc) {
		touchMoveFunc(event);
	}
}

void LambdaView::onTouchUp(TouchEvent& event)
{
	if (touchUpFunc) {
		touchUpFunc(event);
	}
}

}