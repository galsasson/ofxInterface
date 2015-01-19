//
//  TouchEvent.h
//
//
//  Created by Gal Sasson on 8/14/14.
//
//

#ifndef __TouchEvent_h__
#define __TouchEvent_h__

#include "ofMain.h"

class ofxUINode;

// touch data we hold per touch ID
class TouchEvent
{
public:
    enum EventType{
        TYPE_UP,
        TYPE_DOWN,
        TYPE_MOVE,
        TYPE_EXIT,
        TYPE_ENTER,
        TYPE_CLICK
    };

	TouchEvent();
	~TouchEvent();
	
	void setReceiver(ofxUINode* comp);

	int id;
	ofxUINode* receiver;			// the touch was first seen above this component
	ofxUINode* lastSeenAbove;		// last node seen below this touch id
	ofVec2f position;				// global position of touch (call toLocal to get the local)
	ofVec2f prevPosition;			// previous global position
	ofVec2f firstPosition;			// global position of first touch down
	ofVec2f velocity;
	ofVec2f prevVelocity;
	ofVec2f velocitySmoothed;
	unsigned long long timestamp;
	unsigned long long prevTimestamp;

	// can be used to attached arbitrary data to this touch
	void *data;

	// for implementing click
	bool bHasMoved;
	void onComponentDestroyed() { receiver = NULL; }

    EventType type;
};

#endif
