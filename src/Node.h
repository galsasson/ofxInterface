//
//  Node.h
//
//
//  Created by Gal Sasson on 7/24/14.
//
//

#ifndef __Node__
#define __Node__

#include <iostream>
#include "ofMain.h"
#include "TouchEvent.h"

// you can comment this to save a little (very little)
#define OFXUINODE_DEBUG

#ifdef USE_OFX_HISTORY_PLOT
  #include "ofxHistoryPlot.h"
#endif


namespace ofxInterface
{

class Node : public ofNode
{
	friend class TouchManager;
	
public:
	virtual ~Node();
	Node();

	// if you like naming your components (you should)
	void setName(const std::string& _name) { name = _name; }
	const std::string& getName() { return name; }

	// scene-graph adding/removing children stuff
	void addChild(Node* child, int insertAt = -1);		// append by default
	Node* removeChild(Node *child);
	Node* removeChild(int index);
	Node* getChildWithName(const std::string& name, bool deepSearch = false) const;
	int getNumChildren() const { return children.size(); }
	const vector<Node*>& getChildren() const { return children; }


	// functions to override
	//
	virtual void update(float dt) {}	// please override with update code
	virtual void draw() {};		// please override! draw your object in local space

	// for debugging
	virtual void drawDebug();	 // debug debugging stuff (will be called by renderDebug)
	virtual void drawBounds();   // ovveride for debugging if bounds are not rectangular

	// for touch event handling
	//
	// override if your component is not rectangular
	virtual bool contains(const ofVec2f& p);

	// interaction events (register to these events and you're good to go)
	ofEvent<TouchEvent> eventTouchDown;
	ofEvent<TouchEvent> eventTouchMove;
	ofEvent<TouchEvent> eventTouchUp;
	ofEvent<TouchEvent> eventTouchExit;
	ofEvent<TouchEvent> eventTouchEnter;
	ofEvent<TouchEvent> eventClick;


	// render will render this component and its subtree.
	// usually should be called on the root scene object,
	// but can be used also for offline rendering of any branch of the graph
	//
	// render is done as follows:
	// 1. get list of subtree nodes (only visible ones if forceAll = false)
	// 2. sort by plane value
	// 3. call draw from back to front
	void render(bool forceAll = false);
	void renderDebug();	// same as render but calls drawDebug instead of draw.

	void setPosition(float x=0, float y=0, float z=0) { ofNode::setPosition(x, y, z); }
	void setPosition(const ofVec3f& p) { ofNode::setPosition(p); }

	// when rendering, nodes will be sorted by plane number (its a float)
	float getPlane() const { return plane; }
	void setPlane(float _plane) { plane = _plane; }
	float getGlobalPlane() const { return (parent == NULL)?plane:((Node*)parent)->getGlobalPlane()+plane;}

	// set your size!
	// this is important for the default contains function
	ofVec2f getSize() const { return size; }
	void setSize(float w, float h) { size.set(w, h); }
	void setSize(const ofVec2f& s) { size.set(s); }
	float getWidth() const { return size.x; }
	void setWidth(float w) { size.x = w; }
	float getHeight() const { return size.y; }
	void setHeight(float h) { size.y = h; }
	float getGlobalHeight() const { return size.y * getGlobalScale().y; }
	float getGlobalWidth() const { return size.x * getGlobalScale().x; }

	// TouchManager will call these on touch events
	void touchDown(int id, TouchEvent* event);
	void touchMove(int id,  TouchEvent* event);
	void touchUp(int id,  TouchEvent* event);
	void touchExit(int id, TouchEvent* event);
	void touchEnter(int id, TouchEvent* event);

	ofVec2f toLocal(const ofVec2f& screenPoint);

	void setVisible(bool visible) { bVisible = visible; }
	bool getVisible() const { return bVisible; }
	void setEnabled(bool enable) { bEnabled = enable; }
	bool getEnabled() const { return bEnabled; }

	void activate() { setVisible(true); setEnabled(true); }
	void deactivate() { setVisible(false); setEnabled(false); }

	// returns the component's depth in the scene-graph (how many ancestors).
	int getDepthInScene() const
	{
		if (parent != NULL) {
			return 1+((Node*)getParent())->getDepthInScene();
		}
		else {
			return 0;
		}
	}
	// returns a list representation of all children in sub graph
	void getSubTreeList(std::list<Node*>& list);
	// returns a list of visible elements in subgraph
	void getVisibleSubTreeList(std::list<Node*>& list);
	
	// sort components by z
	static bool bottomPlaneFirst(const Node* u, const Node* v)
	{
		if (u->getGlobalPlane() == v->getGlobalPlane()) {
			return u->getDepthInScene() < v->getDepthInScene();
		}
		else {
			return u->getGlobalPlane() < v->getGlobalPlane();
		}
	}

	static bool topPlaneFirst(const Node* u, const Node* v)
	{
		if (u->getGlobalPlane() == v->getGlobalPlane()) {
			return u->getDepthInScene() > v->getDepthInScene();
		}
		else {
			return u->getGlobalPlane() > v->getGlobalPlane();
		}
	}

	// sort by depth in scene
	static bool rootToTop(const Node* u, const Node* v)
	{
		return u->getDepthInScene() < v->getDepthInScene();
	}

	static bool TopToRoot(const Node* u, const Node* v)
	{
		return u->getDepthInScene() > v->getDepthInScene();
	}


	// for dragging
	// return true to accept drop for this object type
	virtual bool acceptsNode(Node* node) { return false; }

	// remove ('cut') the node because it have been dropped in another component.
	// this is called before calling pasteNode on the receiving component (do not delete the object here)
	virtual void cutNode(Node* node) {};

	// add the dropped node to the component (this is the place to delete the compoenent if needed)
	virtual void pasteNode(Node* node) {};

	// If no component accepted the node, then it will be returned to its origin
	// by calling takeNodeBack
	virtual void takeNodeBack(Node* node) {};


	// use this to hold any data
	void setData(void* _data) { data = _data; }
	void* getData() { return data; }



	enum Side {
		LEFT,
		TOP,
		RIGHT,
		BOTTOM
	};
	void placeNextTo(const Node& comp, Node::Side side, float margin=0);


	ofEvent<void> eventDestroy;			// send this event in the destructor

protected:
	std::string name;
	ofVec2f size;

	bool bVisible;
	bool bEnabled;
	vector<Node*> children;

	// plane (instead of z)
	// small value means back, large value is front
	float plane;

	void* data;


	// debug stuff
	ofColor debugBorderColor;
#ifdef USE_OFX_HISTORY_PLOT
	ofxHistoryPlot* historyPlot;
#endif

private:
	bool bSendDestroy;

};

}

#endif /* defined(__osxLingo__GNode__) */
