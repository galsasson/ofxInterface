//
//  VirtualTouch.h
//  modelmath
//
//  Created by Gal Sasson on 4/30/15.
//
//

#ifndef __modelmath__VirtualTouch__
#define __modelmath__VirtualTouch__

#include <stdio.h>
#include "ofMain.h"
#include "Node.h"

namespace ofxInterface {

class VirtualTouch
{
public:

	~VirtualTouch();
	VirtualTouch();

	/******
	 * Setup the path and duration of the touch
	 *
	 * returns the touch id
	 */
	int setup(const ofVec2f& p, float duration);						// use for tap
	int setup(const ofVec2f& src, const ofVec2f& dst, float duration);	// src to dst swipe
	int setup(ofNode* src, ofNode* dst, float duration);				// node to node swipe
	int setup(const ofPolyline& path, float duration);					// arbitrary path swipe


	/******
	 * Call these to actually start/stop the touch
	 */
	void play();
	void pause();
	void stop(bool fireDoneEvent=false);


	/******
	 * Call these to change the touch parameters
	 */
	float getDuration() { return duration; }
	void setDuration(float sec) { duration = sec; }
	ofPolyline getPath() { return path; }
	void setPath(const ofPolyline& newPath) { path = newPath; }


	/******
	 * Set this to draw a finger texture with this touch
	 * hotSpotAnchor is (0-1, 0-1)
	 */
	void setFingerTexture(ofTexture* tex, ofColor c=ofColor::white, float rotation=0.0f, float scale=1.0f, ofVec2f hotSpotAnchor=ofVec2f());

	int getId() { return id; }
	bool isDown() { return bDown; }
	bool isRunning() { return bRunning; }

	void update(float dt);
	void draw();

	ofEvent<VirtualTouch> eventDone;

private:
	bool bInitialized;
	int id;
	ofPolyline path;
	ofVec2f currentPos;
	float duration;

	bool isTap;

	float time;
	bool bRunning;
	bool bDown;

	ofTexture* fingerTex;
	ofColor fingerTexColor;
	float fingerTexRotation;
	float fingerTexScale;
	ofVec2f fingerTexHotSpotAnchor;

	// return uniqe touch id ranging (-100 - INT_MIN) and wrapping back
	int getUniqeTouchId();
};

}	// namespace

#endif /* defined(__modelmath__VirtualTouch__) */
