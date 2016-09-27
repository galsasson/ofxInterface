//
//  TouchEvent.cpp
//
//
//  Created by Gal Sasson on 8/14/14.
//
//

#include "TouchEvent.h"

#include "Node.h"

namespace ofxInterface {

TouchEvent::TouchEvent() : 
	id(0),
	receiver(NULL),
	lastSeenAbove(NULL),
	firstPosition(),
	position(),
	prevPosition(),
	velocity(),
	prevVelocity(),
	velocitySmoothed(),
	timestamp(ofGetSystemTimeMicros()),
	prevTimestamp(timestamp),
	data(NULL)
{
}

TouchEvent::~TouchEvent()
{
	if (receiver != NULL) {
		ofRemoveListener(receiver->eventDestroy, this, &TouchEvent::onComponentDestroyed);
	}
}

void TouchEvent::setReceiver(Node* comp, bool reportUpOnPrevious)
{
	if (receiver != NULL) {
		// if we are transferring the receiver to be a different node
		if (comp->bNodeAllowOneTouch && comp->bNodeTouched) {
			ofLogWarning("TouchEvent") << "cannot set receiver to node: "<<this<<", node already touched";
			return;
		}
		if (reportUpOnPrevious) {
			// report touch up on the current (old) receiver node
			ofNotifyEvent(receiver->eventTouchUp, *this, this);
		}
		ofRemoveListener(receiver->eventDestroy, this, &TouchEvent::onComponentDestroyed);
		receiver->bNodeTouched = false;
	}

	// set receiver to new node
	receiver = comp;
	receiver->bNodeTouched = true;
	receiver->nodeCurrentTouchId = id;
	ofAddListener(receiver->eventDestroy, this, &TouchEvent::onComponentDestroyed);
}

} // namespace