//
//  TouchManager.cpp
//
//
//  Created by Gal Sasson on 7/28/14.
//
//

#include "TouchManager.h"
//#include "ofxRemoteUIServer.h"
#include "TouchEvent.h"

TouchManager::TouchManager()
{
	velocitySmoothCoeff = 0.25f;
	touchVelocityDump = 0.6f;
	touchHasMovedThreshold = 50;
	bUpdateDispatch = false;
}

void TouchManager::setup(ofxUINode *root, bool dispatchOnUpdate)
{
	scene = root;
	bUpdateDispatch = dispatchOnUpdate;
}

void TouchManager::update(float dt)
{

	// smooth velocity of touches
	map<int, TouchEvent*>::iterator it = touches.begin();
	for (;it!=touches.end(); it++)
	{
		(*it).second->velocitySmoothed *= touchVelocityDump;
	}

	if (bUpdateDispatch) {
		// dispatch all touches in the queue
		mutex.lock();
		vector<TouchAction> queue = touchQueue;
		touchQueue.clear();
		mutex.unlock();

		for (int i=0; i<queue.size(); i++) {
			TouchAction* t = &queue[i];
			switch (t->actionType) {
			case TOUCH_DOWN:
				dispatchTouchDown(t->id, t->pos);
				break;
			case TOUCH_MOVE:
				dispatchTouchMove(t->id, t->pos);
				break;
			case TOUCH_UP:
				dispatchTouchUp(t->id, t->pos);
				break;
			}
		}
	}
}

void TouchManager::touchDown(int id, const ofVec2f& p)
{
	if (bUpdateDispatch) {
		// queue touch to be dispatched in update
		TouchAction t(TOUCH_DOWN, id, p);
		mutex.lock();
		touchQueue.push_back(t);
		mutex.unlock();
	}
	else {
		dispatchTouchDown(id, p);
	}
}

void TouchManager::touchMove(int id, const ofVec2f& p)
{
	if (bUpdateDispatch) {
		// queue touch to be dispatched in update
		TouchAction t(TOUCH_MOVE, id, p);
		mutex.lock();
		touchQueue.push_back(t);
		mutex.unlock();
	}
	else {
		dispatchTouchMove(id, p);
	}
}

void TouchManager::touchUp(int id, const ofVec2f& p)
{
	if (bUpdateDispatch) {
		// queue touch to be dispatched in update
		TouchAction t(TOUCH_UP, id, p);
		mutex.lock();
		touchQueue.push_back(t);
		mutex.unlock();
	}
	else {
		dispatchTouchUp(id, p);
	}
}


void TouchManager::dispatchTouchDown(int id, const ofVec2f& p)
{
	TouchEvent *event = new TouchEvent();
	event->id = id;
	event->firstPosition = event->position = event->prevPosition = p;
	touches[id] = event;

	ofxUINode *node = getComponentUnder(p);
	if (node == NULL) {
		ofLogError("TouchManager","could not find node for touchDown");
		return;
	}

	event->setReceiver(node);
	event->lastSeenAbove = node;
    event->type = TouchEvent::TYPE_DOWN;
	return node->touchDown(id, event);
}

void TouchManager::dispatchTouchMove(int id, const ofVec2f &p)
{
	// update touch info
	TouchEvent* event = touches[id];

	// discard if same position as the previous touch
	if (event->prevPosition == p) {
		return;
	}

	if ((event->firstPosition - p).length() > touchHasMovedThreshold)
	{
		event->bHasMoved = true;
	}
	event->prevPosition = event->position;
	event->prevVelocity = event->velocity;
	event->prevTimestamp = event->timestamp;
	event->position = p;
	event->timestamp = ofGetSystemTimeMicros();

	// there is a problem with calculating the velocity when delta time is too small
	// sometimes we get events with delta time of 0 or 1 ms
	if (event->timestamp - event->prevTimestamp > 5000) {
		event->velocity = (p - event->prevPosition)*1000000 / (event->timestamp-event->prevTimestamp);
		event->velocitySmoothed += (event->velocity-event->velocitySmoothed)*velocitySmoothCoeff;
	}


	if (event->receiver == NULL) {
		// the original component of this touch was destroyed
		// ignore this touch
		ofLogVerbose("TouchManager","ignoring touch id: %d",id);
		return;
	}

	// find node below touch point
	ofxUINode *node = getComponentUnder(p);
	if (node == NULL) {
		ofLogError("TouchManager","could not find node for touchMove");
		return;
	}

	if (node != event->receiver) {
		// we are outside of this touch original component
		if (event->lastSeenAbove == event->receiver) {
			// touch moved to another component
			// send touchExit event to the original component
            event->type = TouchEvent::TYPE_EXIT;
			event->receiver->touchExit(id, event);
		}
		else {
			// send touch move the the original component (firstSeenAbove)
            event->type = TouchEvent::TYPE_MOVE;
			event->receiver->touchMove(id, event);
		}

		// update last seen above
		event->lastSeenAbove = node;
	}
	else {
		// we are above the original component
		if (event->lastSeenAbove != event->receiver) {
			// touch just moved back to the original component
			// send touch enter when we return
            event->type = TouchEvent::TYPE_ENTER;
			node->touchEnter(id, event);
		}
		else {
            event->type = TouchEvent::TYPE_MOVE;
			node->touchMove(id, event);
		}

		event->lastSeenAbove = node;
	}
}

void TouchManager::dispatchTouchUp(int id, const ofVec2f &p)
{
	// update touch info
	TouchEvent* event = touches[id];
	event->prevPosition = event->position;
	event->prevVelocity = event->velocity;
	event->prevTimestamp = event->timestamp;
	event->position = p;
	event->timestamp = ofGetSystemTimeMicros();

	// there is a problem with calculating the velocity when delta time is too small
	// sometimes we get events with delta time of 0 or 1 ms
	if (event->timestamp - event->prevTimestamp > 5000) {
		event->velocity = (p - event->prevPosition)*1000000 / (event->timestamp-event->prevTimestamp);
		event->velocitySmoothed += (event->velocity-event->velocitySmoothed)*velocitySmoothCoeff;
	}

	ofxUINode* node = getComponentUnder(p);
	if (node == NULL) {
		ofLogError("TouchManager","could not find node for touchUp");
		endTouch(id);
		return;
	}

	if (event->receiver == NULL) {
		ofLogNotice("TouchManager","this touch does not belong to any component");
		endTouch(id);
		return;
	}

    event->type = TouchEvent::TYPE_UP;
	event->receiver->touchUp(id, event);
	endTouch(id);
}

void TouchManager::endTouch(int id)
{
	TouchEvent* te = touches[id];
	touches.erase(id);
	delete te;
}

ofxUINode* TouchManager::getComponentUnder(const ofVec2f &p)
{
	if (!scene) {
		return NULL;
	}

	std::list<ofxUINode*> compList = getAllComponentsUnder(p);
	if (compList.size() == 0) {
		return scene;
	}

	return *compList.begin();
}

std::list<ofxUINode*> TouchManager::getAllComponentsUnder(const ofVec2f &p)
{
	std::list<ofxUINode*> list;
	fillComponentsUnder(scene, p, list);
	list.sort(ofxUINode::topPlaneFirst);
	return list;
}

void TouchManager::fillComponentsUnder(ofxUINode* root, const ofVec2f &p, std::list<ofxUINode *> &list)
{
	if (root == NULL || !root->getEnabled()) {
		return;
	}

	if (root->contains(p)) {
		list.push_back(root);
	}

	for (int i=0; i<root->children.size(); i++) {
		fillComponentsUnder(root->children[i], p, list);
	}
}





