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

#include "ofxUINode.h"

class TouchManager
{
public:
	static TouchManager& one()
    {
        static TouchManager instance;
        // Instantiated on first use.
        return instance;
    }

	void setup(ofxUINode* root);
	
	void update(float dt);

	// go over the scene and send the event to the component
	void touchDown(int id, const ofVec2f& p);
	void touchMove(int id, const ofVec2f& p);
	void touchUp(int id, const ofVec2f& p);

	void dispatchTouchDown(int id, const ofVec2f& p);
	void dispatchTouchMove(int id, const ofVec2f& p);
	void dispatchTouchUp(int id, const ofVec2f& p);

	// get top-most component under point
	ofxUINode* getComponentUnder(const ofVec2f &p);
	// returns a list of all components under p (top most first)
	std::list<ofxUINode*> getAllComponentsUnder(const ofVec2f &p);

	void endTouch(int id);

	float velocitySmoothCoeff;
	float touchHasMovedThreshold;
	float touchVelocityDump;
	bool bUpdateDispatch;


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
	ofxUINode *scene;

	void fillComponentsUnder(ofxUINode* root, const ofVec2f &p, std::list<ofxUINode*>& list);

	// touch data per ID
	map<int, TouchEvent*> touches;

	// for update dispatch
	vector<TouchAction> touchQueue;
	ofMutex mutex;

	// singleton stuff
	TouchManager();
    // make sure there are no copies
    TouchManager(TouchManager const&);
    void operator=(TouchManager const&);
	
};

#endif /* defined(__TouchManager__) */
