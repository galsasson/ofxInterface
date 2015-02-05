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
	data(NULL),
	bHasMoved(false)
{
}

TouchEvent::~TouchEvent()
{
	if (receiver != NULL) {
		ofRemoveListener(receiver->eventDestroy, this, &TouchEvent::onComponentDestroyed);
	}
}

void TouchEvent::setReceiver(Node* comp)
{
	if (receiver != NULL) {
		ofRemoveListener(receiver->eventDestroy, this, &TouchEvent::onComponentDestroyed);
	}

	receiver = comp;
	ofAddListener(receiver->eventDestroy, this, &TouchEvent::onComponentDestroyed);
}

} // namespace