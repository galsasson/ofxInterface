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
#include "VirtualTouch.h"

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
	
	void update(float dt = ofGetLastFrameTime());
	void draw();
	void drawFingerStatus(int x, int y);

	/******
	 * The embedding app should call these to report touches
	 * TouchManager will take it from there
	 */
	void touchDown(int id, const ofVec2f& p);
	void touchMove(int id, const ofVec2f& p);
	void touchUp(int id, const ofVec2f& p);

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

	// parameters that effect velocity smoothing
	float velocitySmoothCoeff;
	float touchVelocityDump;

	/******
	 * Touch Rain: random swipes and taps for testing
	 */
	void startTouchRain(unsigned int maxTaps=1, unsigned int maxSwipes=1);
	void pauseTouchRain();
	void stopTouchRain();
	bool isTouchRainActive() { return bTouchRainActive; }

	/******
	 * Call this function to set the touch rain area.
	 * This is optional, the default is the scene area
	 */
	void setTouchRainArea(const ofRectangle& rect);

private:
	Node *scene;
	bool bUpdateDispatch;

	/******
	 * dispatch touch events to scene
	 */
	// go over the scene and send the event to the component
	void dispatchTouchDown(int id, const ofVec2f& p);
	void dispatchTouchMove(int id, const ofVec2f& p);
	void dispatchTouchUp(int id, const ofVec2f& p);

	// done in post-order right-to-left so list will have the topmost nodes first
	void fillComponentsUnder(Node* root, const ofVec2f &p, std::list<Node*>& list);

	// touch data per ID
	map<int, TouchEvent*> touches;

	/******
	 * for update dispatch
	 * (private things you should not care about
	 */
	enum TouchActionType{ TOUCH_DOWN, TOUCH_MOVE, TOUCH_UP };

	struct TouchAction{
		TouchActionType actionType;
		int id;
		ofVec2f pos;
		TouchAction(TouchActionType a, int _id, ofVec2f p) {
			actionType = a; id = _id; pos = p;
		}
	};
	vector<TouchAction> touchQueue;
	ofMutex mutex;

	void endTouch(int id);


	/******
	 * For Touch Rain implementation
	 */
	bool bTouchRainActive;
	unsigned int maxSwipes;
	unsigned int maxTaps;
	ofRectangle touchRainArea;
	vector<VirtualTouch*> virtualSwipes;
	vector<VirtualTouch*> virtualTaps;
	void updateTouchRain(float dt);
	void createRandomSwipes(unsigned int n);
	void createRandomTaps(unsigned int n);
	ofVec2f getRandomTouchPosition();

	// singleton stuff
	~TouchManager();
	TouchManager();
    // make sure there are no copies
    TouchManager(TouchManager const&);
    void operator=(TouchManager const&);
	
};

}

#endif /* defined(__TouchManager__) */
