//
//  TouchManager.h
//
//
//  Created by Gal Sasson on 7/28/14.
//
//

#ifndef __TouchManager__
#define __TouchManager__

#include <iostream>
#include "ofMain.h"

#include "Node.h"

namespace ofxInterface
{

class TouchManager
{
public:
	static TouchManager& one()
    {
        static TouchManager instance;
        // Instantiated on first use.
        return instance;
    }

	void setup(Node* root, bool dispatchOnUpdate = false);
	
	void update(float dt = 1.0f / ofGetFrameRate());

	/******
	 * touch events
	 */
	// go over the scene and send the event to the component
	void touchDown(int id, const ofVec2f& p);
	void touchMove(int id, const ofVec2f& p);
	void touchUp(int id, const ofVec2f& p);

	void dispatchTouchDown(int id, const ofVec2f& p);
	void dispatchTouchMove(int id, const ofVec2f& p);
	void dispatchTouchUp(int id, const ofVec2f& p);


	/******
	 * Registering to the events below will give you all the events
	 * in the system, use carefully
	 */
	ofEvent<TouchEvent> eventEveryTouchDown;
	ofEvent<TouchEvent> eventEveryTouchMove;
	ofEvent<TouchEvent> eventEveryTouchUp;

	// get top-most component under point
	Node* getComponentUnder(const ofVec2f &p);
	// returns a list of all components under p (top most first)
	std::list<Node*> getAllComponentsUnder(const ofVec2f &p);

	void endTouch(int id);

	float velocitySmoothCoeff;
	float touchHasMovedThreshold;
	float touchVelocityDump;

	enum TouchActionType{ TOUCH_DOWN, TOUCH_MOVE, TOUCH_UP };

    struct TouchAction{
        TouchActionType actionType;
        int id;
        ofVec2f pos;
        TouchAction(TouchActionType a, int _id, ofVec2f p) {
            actionType = a; id = _id; pos = p;
        }
    };

private:
	Node *scene;
	bool bUpdateDispatch;


	void fillComponentsUnder(Node* root, const ofVec2f &p, std::list<Node*>& list);

	// touch data per ID
	map<int, TouchEvent*> touches;

	// for update dispatch
	vector<TouchAction> touchQueue;
	ofMutex mutex;

	// singleton stuff
	~TouchManager();
	TouchManager();
    // make sure there are no copies
    TouchManager(TouchManager const&);
    void operator=(TouchManager const&);
	
};

}

#endif /* defined(__TouchManager__) */
