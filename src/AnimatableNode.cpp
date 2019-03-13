//
//  AnimatableNode.cpp
//  maccabi-wall
//
//  Created by Gal Sasson on 7/24/17.
//
//

#include "AnimatableNode.h"

namespace ofxInterface {

AnimatableNode::AnimatableNode()
{
    nodePositionAnimation.bActive = false;
}

void AnimatableNode::animatePositionTo(const ofVec3f& pos, float duration, Curve curve, float delay, const std::function<void(void)>& onEndFunc)
{
    nodePositionAnimation.source = getPosition();
    nodePositionAnimation.target = pos;
    nodePositionAnimation.duration = duration;
    nodePositionAnimation.curve = curve;
    nodePositionAnimation.delay = delay;
    nodePositionAnimation.progress = 0;
    nodePositionAnimation.bActive = true;
	nodePositionAnimation.onEnd = onEndFunc;
}

void AnimatableNode::stopPositionAnimation()
{
	nodePositionAnimation.bActive = false;
}

void AnimatableNode::animateColor(ofColor *c, const ofColor& target, float duration, Curve curve, float delay, const std::function<void(void)>& onEndFunc)
{
	nodeColorAnimations[c].color = c;
    nodeColorAnimations[c].source = *c;
    nodeColorAnimations[c].target = target;
    nodeColorAnimations[c].duration = duration;
    nodeColorAnimations[c].curve = curve;
    nodeColorAnimations[c].delay = delay;
    nodeColorAnimations[c].progress = 0;
	nodeColorAnimations[c].onEnd = onEndFunc;
}

void AnimatableNode::animateFloat(float* val, float target, float duration, Curve curve, float delay, const std::function<void(void)>& onEndFunc)
{
    nodeFloatAnimations[val].value = val;
    nodeFloatAnimations[val].source = *val;
    nodeFloatAnimations[val].target = target;
    nodeFloatAnimations[val].duration = duration;
    nodeFloatAnimations[val].curve = curve;
    nodeFloatAnimations[val].delay = delay;
    nodeFloatAnimations[val].progress = 0;
	nodeFloatAnimations[val].onEnd = onEndFunc;
}

void AnimatableNode::update(float dt)
{
    // Update position animation
    if (nodePositionAnimation.bActive) {
        nodePositionAnimation.progress += dt;
        float timeProg = ofClamp(nodePositionAnimation.progress-nodePositionAnimation.delay, 0, nodePositionAnimation.duration) / nodePositionAnimation.duration;
		float prog = ofClamp(nodePositionAnimation.curve.calcAt(timeProg), 0, 1);
        setPosition(nodePositionAnimation.source.getInterpolated(nodePositionAnimation.target, prog));
        if (nodePositionAnimation.progress >= nodePositionAnimation.delay+nodePositionAnimation.duration) {
			nodePositionAnimation.bActive = false;
			function<void(void)> f = nodePositionAnimation.onEnd;
			f();
        }
    }
    
    // Update color animation
	vector<ofColor*> rmcolor;
	for (auto& anim: nodeColorAnimations) {
        anim.second.progress += dt;
        float timeProg = ofClamp(anim.second.progress-anim.second.delay, 0, anim.second.duration)/anim.second.duration;
		float prog = ofClamp(anim.second.curve.calcAt(timeProg), 0, 1);
        *anim.second.color = anim.second.source.getLerped(anim.second.target, prog);
        if (anim.second.progress >= anim.second.delay+anim.second.duration) {
			rmcolor.push_back(anim.first);
			function<void(void)> f = anim.second.onEnd;
			f();
        }
    }
	// go over again and remove completed animations
	for (auto* c: rmcolor) {
		if (nodeColorAnimations[c].progress >= nodeColorAnimations[c].delay+nodeColorAnimations[c].duration) {
			nodeColorAnimations.erase(c);
		}
	}
    
    // Update float animation
	vector<float*> rmfloat;
	for (auto& anim: nodeFloatAnimations) {
        anim.second.progress += dt;
        float timeProg = ofClamp(anim.second.progress-anim.second.delay, 0, anim.second.duration)/anim.second.duration;
		float prog = ofClamp(anim.second.curve.calcAt(timeProg), 0, 1);
        *anim.second.value = ofMap(prog, 0, 1, anim.second.source, anim.second.target, true);
        if (anim.second.progress >= anim.second.delay+anim.second.duration) {
			rmfloat.push_back(anim.first);
			function<void(void)> f = anim.second.onEnd;
			f();
        }
    }
	// go over again and remove completed animations
	for (auto* f: rmfloat) {
		if (nodeFloatAnimations[f].progress >= nodeFloatAnimations[f].delay+nodeFloatAnimations[f].duration) {
			nodeFloatAnimations.erase(f);
		}
	}
}
	
}
