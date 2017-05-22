//
//  LambdaView.h
//  ofxInterface
//
//  Created by Gal Sasson on 12/17/15.
//
//

#ifndef LambdaView_h
#define LambdaView_h

#include "ofMain.h"
#include "ofxInterface.h"

namespace ofxInterface
{

class LambdaView : public Node
{
public:
	LambdaView(const string& name="");
	void setDrawFunction(std::function<void(void)> _func);
	void setDrawDebugFunction(std::function<void(void)> _func);
	void setUpdateFunction(std::function<void(float)> _func);
	void setTouchDownFunction(std::function<void(ofxInterface::TouchEvent&)> _func);
	void setTouchMoveFunction(std::function<void(ofxInterface::TouchEvent&)> _func);
	void setTouchUpFunction(std::function<void(ofxInterface::TouchEvent&)> _func);
	void setContainsFunction(std::function<bool(const ofVec3f& global)> _func);
	void update(float dt);
	void draw();
	void drawDebug();
	bool contains(const ofVec3f& global);
	
private:
	std::function<void(void)> drawFunc;
	std::function<void(void)> drawDebugFunc = [&](){Node::drawDebug();};
	std::function<void(float)> updateFunc;
	std::function<void(ofxInterface::TouchEvent&)> touchDownFunc;
	std::function<void(ofxInterface::TouchEvent&)> touchMoveFunc;
	std::function<void(ofxInterface::TouchEvent&)> touchUpFunc;
	std::function<bool(const ofVec3f&)> containsFunc;

	void onTouchDown(TouchEvent& event);
	void onTouchMove(TouchEvent& event);
	void onTouchUp(TouchEvent& event);
};

}

#endif /* LambdaView_h */
