//
//  ofxUINode.h
//
//
//  Created by Gal Sasson on 7/24/14.
//
//

#ifndef __ofxUINode__
#define __ofxUINode__

#include <iostream>
#include "ofMain.h"
#include "TouchEvent.h"

#ifdef OFXUINODE_DEBUG
  #include "ofxHistoryPlot.h"
#endif


class ofxUINode : public ofNode
{
	friend class TouchManager;
	
public:
	virtual ~ofxUINode();
	ofxUINode();
	virtual void update(float dt) {}	// please override!
	virtual void draw() {};		// please override! draw object content in local space
	virtual void drawDebug();	// debug drawing in global space (implementation should transform the matrix)
	virtual void drawBounds() {};  // draw the bounds of the object in local space

	// render will render this component and its subtree.
	// usually should be called on the root scene object,
	// but can be used also for offline rendering of every node in the graph
	//
	// render is done as follows:
	// 1. get list of subtree nodes (only visible ones if forceAll = false)
	// 2. sort by plane value
	// 3. call draw from back to front
	void render(bool forceAll = false);
	void renderDebug();	// same as render but calls drawDebug instead of draw.

	// convenience for 2D
	void setPosition(float x=0, float y=0, float z=0) { ofNode::setPosition(x, y, z); }
	void setPosition(const ofVec3f& p) { ofNode::setPosition(p); }

	// when rendering, nodes will be sorted by plane number
	float getPlane() const { return plane; }
	void setPlane(float _plane) { plane = _plane; }
	float getGlobalPlane() const { return (parent == NULL)?plane:((ofxUINode*)parent)->getGlobalPlane()+plane;}

	const ofVec2f& getSize() const { return size; }
	void setSize(float w, float h) { size.set(w, h); }
	void setSize(const ofVec2f& s) { size.set(s); }
	float getWidth() const { return size.x; }
	void setWidth(float w) { size.x = w; }
	float getHeight() const { return size.y; }
	void setHeight(float h) { size.y = h; }

	float getGlobalHeight() const { return size.y * getGlobalScale().y; }
	float getGlobalWidth() const { return size.x * getGlobalScale().x; }

	// TouchListener
	virtual void touchDown(int id, TouchEvent* event);
	virtual void touchMove(int id,  TouchEvent* event);
	virtual void touchUp(int id,  TouchEvent* event);
	virtual void touchExit(int id, TouchEvent* event);
	virtual void touchEnter(int id, TouchEvent* event);

	ofVec2f toLocal(const ofVec2f& screenPoint);
	virtual bool contains(const ofVec2f& p);

	void setVisible(bool visible) { bVisible = visible; }
	bool getVisible() const { return bVisible; }
	void setEnabled(bool enable) { bEnabled = enable; }
	bool getEnabled() const { return bEnabled; }

	void activate() { setVisible(true); setEnabled(true); }
	void deactivate() { setVisible(false); setEnabled(false); }

	int getNumChildren() { return children.size(); }
	const vector<ofxUINode*>& getChildren() { return children; }
	void addChild(ofxUINode* child, int insertAt = -1);
	ofxUINode* removeChild(ofxUINode *child);
	ofxUINode* removeChild(int index);
	ofxUINode* getChildWithName(const std::string& name, bool deepSearch = false);

	// returns the component's depth in the scene-graph (how many ancestors).
	int getDepthInScene() const
	{
		if (parent != NULL) {
			return 1+((ofxUINode*)getParent())->getDepthInScene();
		}
		else {
			return 0;
		}
	}
	// returns a list representation of all children in sub graph
	void getSubTreeList(std::list<ofxUINode*>& list);
	// returns a list of visible elements in subgraph
	void getVisibleSubTreeList(std::list<ofxUINode*>& list);
	
	// sort components by z
	static bool bottomPlaneFirst(const ofxUINode* u, const ofxUINode* v)
	{
		if (u->getGlobalPlane() == v->getGlobalPlane()) {
			return u->getDepthInScene() < v->getDepthInScene();
		}
		else {
			return u->getGlobalPlane() < v->getGlobalPlane();
		}
	}

	static bool topPlaneFirst(const ofxUINode* u, const ofxUINode* v)
	{
		if (u->getGlobalPlane() == v->getGlobalPlane()) {
			return u->getDepthInScene() > v->getDepthInScene();
		}
		else {
			return u->getGlobalPlane() > v->getGlobalPlane();
		}
	}

	// sort by depth in scene
	static bool rootToTop(const ofxUINode* u, const ofxUINode* v)
	{
		return u->getDepthInScene() < v->getDepthInScene();
	}

	static bool TopToRoot(const ofxUINode* u, const ofxUINode* v)
	{
		return u->getDepthInScene() > v->getDepthInScene();
	}


	// for dragging
	// return true to accept drop for this object type
	virtual bool acceptsNode(ofxUINode* node) { return false; }

	// remove ('cut') the node because it have been dropped in another component.
	// this is called before calling pasteNode on the receiving component (do not delete the object here)
	virtual void cutNode(ofxUINode* node) {};

	// add the dropped node to the component (this is the place to delete the compoenent if needed)
	virtual void pasteNode(ofxUINode* node) {};

	// If no component accepted the node, then it will be returned to its origin
	// by calling takeNodeBack
	virtual void takeNodeBack(ofxUINode* node) {};

	virtual void setName(const std::string& _name) { name = _name; }
	virtual const std::string& getName() { return name; }

	void setData(void* _data) { data = _data; }
	void* getData() { return data; }

	enum Side {
		LEFT,
		TOP,
		RIGHT,
		BOTTOM
	};
	void placeNextTo(const ofxUINode& comp, ofxUINode::Side side, float margin=0);


	// events
	ofEvent<TouchEvent> eventTouchDown;
	ofEvent<TouchEvent> eventTouchMove;
	ofEvent<TouchEvent> eventTouchUp;
	ofEvent<TouchEvent> eventTouchExit;
	ofEvent<TouchEvent> eventTouchEnter;
	ofEvent<TouchEvent> eventClick;

	
	ofEvent<void> eventDestroy;


	// HACKISH stuff
	virtual void pop(bool animate) {};

protected:
	std::string name;
	ofVec2f size;

	bool bVisible;
	bool bEnabled;
	vector<ofxUINode*> children;

	// plane (instead of z)
	// small value means back, large value is front
	float plane;

	void* data;


	// debug stuff
#ifdef GNODE_DEBUG
	ofColor debugBorderColor;
	ofxHistoryPlot* historyPlot;
#endif



};

#endif /* defined(__osxLingo__GNode__) */
