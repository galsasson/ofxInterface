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

}