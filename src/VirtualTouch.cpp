//
//  VirtualTouch.cpp
//  modelmath
//
//  Created by Gal Sasson on 4/30/15.
//
//

#include "VirtualTouch.h"
#include "TouchManager.h"

namespace ofxInterface {

VirtualTouch::~VirtualTouch()
{
	if (bInitialized) {
		stop();
	}
}

VirtualTouch::VirtualTouch() :
	fingerTex(NULL),
	bInitialized(false),
	isTap(false)
{

}

int VirtualTouch::setup(const ofVec2f &p, float duration)
{
	isTap = true;
	return setup(p, p, duration);
}

int VirtualTouch::setup(ofNode *src, ofNode *dst, float duration)
{
	return setup(src->getGlobalPosition(), dst->getGlobalPosition(), duration);
}

int VirtualTouch::setup(const ofVec2f &src, const ofVec2f &dst, float duration)
{
	ofPolyline line;
	line.addVertex(ofVec3f(src));
	line.addVertex(ofVec3f(dst));
	return setup(line, duration);
}

int VirtualTouch::setup(const ofPolyline &_path, float _duration)
{
	if (_duration <= 0) {
		ofLogWarning("VitualTouch") << "cannot create VirtualTouch with duration set to 0 or less, fixing with 0.1";
		_duration = 0.1f;
	}

	path = _path;
	duration = _duration;
	bRunning = false;
	bDown = false;
	id = getUniqeTouchId();
	bInitialized = true;

	return id;
}

void VirtualTouch::play()
{
	if (!bInitialized) {
		ofLogWarning("VirtualTouch") << "VirtualTouch not initialized (on call to play)";
		return;
	}

	if (!bDown) {
		time = 0;
		currentPos = path.getVertices()[0];
		TouchManager::one().touchDown(id, currentPos);
		bDown = true;
	}

	bRunning = true;
}

void VirtualTouch::pause()
{
	if (!bInitialized) {
		ofLogWarning("VirtualTouch") << "VirtualTouch not initialized (on call to pause)";
		return;
	}

	bRunning = false;
}

void VirtualTouch::stop(bool fireDoneEvent)
{
	if (!bInitialized) {
		ofLogWarning("VirtualTouch") << "VirtualTouch not initialized (on call to stop)";
		return;
	}

	if (bDown) {
		TouchManager::one().touchUp(id, currentPos);
		bDown = false;
	}

	bRunning = false;

	if (fireDoneEvent) {
		ofNotifyEvent(eventDone, *this, this);
	}
}

void VirtualTouch::setFingerTexture(ofTexture *tex, ofColor c, float rotation, float scale, ofVec2f hotSpotAnchor)
{
	fingerTex = tex;
	fingerTexColor = c;
	fingerTexRotation = rotation;
	fingerTexScale = scale;
	fingerTexHotSpotAnchor = hotSpotAnchor;
}

void VirtualTouch::update(float dt)
{
	if (!bInitialized) {
		ofLogWarning("VirtualTouch") << "VirtualTouch not initialized (on call to update)";
		return;
	}

	if (!bRunning) {
		return;
	}

	// update currentPos and do touch-up if needed

	if (time >= duration) {
		auto & points = path.getVertices();
		currentPos = points[points.size()-1];
		stop(true);		// performs touch-up and fires eventDone
	}
	else {
		// touch move
		if(!isTap){
			currentPos = path.getPointAtPercent(time/duration);
			TouchManager::one().touchMove(id, currentPos);
		}
	}

	time += dt;
}

void VirtualTouch::draw()
{
	if (!fingerTex) {
		return;
	}

	ofPushMatrix();
	ofTranslate(currentPos);
	ofRotate(fingerTexRotation, 0, 0, 1);
	ofScale(fingerTexScale, fingerTexScale, fingerTexScale);
	fingerTex->draw(-fingerTex->getWidth()*fingerTexHotSpotAnchor.x,
					-fingerTex->getHeight()*fingerTexHotSpotAnchor.y);
	ofPopMatrix();
}

int VirtualTouch::getUniqeTouchId()
{
	static int nextId = -100;
	int giveId = nextId;
	nextId = (nextId>INT_MIN)?nextId-1:-100;
	return giveId;
}

}	// namespace