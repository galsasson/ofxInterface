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

    /******
     * Node Names:
     *
     * If you like naming your components (you should)
     */
	void setName(const std::string& _name) { name = _name; }
	const std::string& getName() { return name; }
    // search the tree for a node with a specific name, searchDepth of -1 means search all the way down
    Node* getNodeWithName(const std::string& searchName, int searchDepth = -1) const;

    /******
     * Adding & Removing children
     */
	void addChild(Node* child, int insertAt = -1);		// append by default
	Node* removeChild(Node *child);
	Node* removeChild(int index);
	int getNumChildren() const { return (int)children.size(); }
	const vector<Node*>& getChildren() const { return children; }


	// functions to override
	//
	virtual void update(float dt) {}	// please override with update code
	virtual void draw() {};		// please override! draw your object in local space

	// for debugging
	virtual void drawDebug();	 // debug debugging stuff (will be called by renderDebug)
	virtual void drawBounds();   // overide for debugging if bounds are not rectangular

	/******
	 * for touch event handling
	 *
	 * override to return if a point is inside the node.
	 * default implementation works only if your component is 
	 * rectangular.
	 */
	virtual bool contains(const ofVec3f& globalPoint);

	// interaction events (register to these events and you're good to go)
	ofEvent<TouchEvent> eventTouchDown;
	ofEvent<TouchEvent> eventTouchMove;
	ofEvent<TouchEvent> eventTouchUp;
	ofEvent<TouchEvent> eventTouchExit;
	ofEvent<TouchEvent> eventTouchEnter;
	ofEvent<TouchEvent> eventClick;


	/******
	 * render will render this component and its subtree.
	 * usually should be called on the root scene object,
	 * but can be used also for offline rendering of any branch of the graph
	 *
	 * render is done as follows:
	 * 1. get list of subtree nodes (only visible ones if forceAll = false)
	 * 2. sort by plane value
	 * 3. call draw from back to front
	 */
	void render(bool forceAll = false);
	void renderDebug(bool forceAll = false);	// same as render but calls drawDebug instead of draw.
    
    /******
     * call the 'update' function of visible children
     */
    void updateSubtree(float dt, bool forceAll=false);
	void updateSubtreePostOrder(float dt, bool forceAll=false);

	void setPosition(float x=0, float y=0, float z=0) { ofNode::setPosition(x, y, z); }
	void setPosition(const ofVec3f& p) { ofNode::setPosition(p); }
	void setX(float x) { setPosition(x, getY(), getZ()); }
	void setY(float y) { setPosition(getX(), y, getZ()); }
	void setZ(float z) { setPosition(getX(), getY(), z); }
    
    /******
     * For convenience:
     *
     * Center component inside parent
     */
    void setCenteredH();
    void setCenteredV();
    void setCentered();

    
	// when rendering, nodes will be sorted by plane number (float)
	float getPlane() const { return plane; }
	void setPlane(float _plane) { plane = _plane; }
	float getGlobalPlane() const { return (parent == NULL)?plane:((Node*)parent)->getGlobalPlane()+plane;}

    /******
     * Node Size
     *
     * A node have a width and a height.
     * The size is being used in the 'contains' function which tells the TouchManager
     * if a point falls inside the component. The contains function can be overriden
     * to support non rectangular nodes.
     * The size is also being used by the drawBounds function, so you should override
     * that one too if your node is not rectangular, or ignores width and height.
     */
    ofVec2f getSize() const { return size; }
	void setSize(float w, float h) { size.set(w, h); ofNotifyEvent(eventNodeSizeChanged, *this, this); }
	void setSize(const ofVec2f& s) { size.set(s); ofNotifyEvent(eventNodeSizeChanged, *this, this); }
	float getWidth() const { return size.x; }
	void setWidth(float w) { size.x = w; ofNotifyEvent(eventNodeSizeChanged, *this, this); }
	float getHeight() const { return size.y; }
	void setHeight(float h) { size.y = h; ofNotifyEvent(eventNodeSizeChanged, *this, this); }
	float getGlobalHeight() const { return size.y * getGlobalScale().y; }
	float getGlobalWidth() const { return size.x * getGlobalScale().x; }
    
    /******
     * resizing events
     *
     * register to get events when settings the size
     */
    ofEvent<Node> eventNodeSizeChanged;
    
    /******
     * translate local and global space points
     */
	ofVec3f toLocal(const ofVec3f& screenPoint);
    ofVec3f toGlobal(const ofVec3f& localPoint);

    /******
     * Visible & Enabled:
     *
     * truely visible node will draw and update every frame
     * truely enabled node will respond to touch events
     *
     * truely visible: this node and all its ancestors are visible
     * truely enabled: this node and all its ancestors and enabled
     */
    void setVisible(bool visible);
    bool getVisible() const { return bVisible; }
    bool getVisibleGlobally() const; // this node and all ancestors and visible (truely visible)
    void setEnabled(bool enable);
    bool getEnabled() const { return bEnabled; }
    bool getEnabledGlobally() const; // this node and all ancestors and enabled (truely enabled)
    
    /******
     * visiblity & enable events
     *
     * will be notified when the node becomes visible / hidden, enabled / disabled
     */
    ofEvent<ofxInterface::Node> eventNodeDidAppear;
    ofEvent<ofxInterface::Node> eventNodeDidDisappear;
    ofEvent<ofxInterface::Node> eventNodeWasEnabled;
    ofEvent<ofxInterface::Node> eventNodeWasDisabled;

	void activate() { setVisible(true); setEnabled(true); }
	void deactivate() { setVisible(false); setEnabled(false); }

	/******
     * returns the component's depth in the scene-graph (how many ancestors).
     */
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
	// returns a list of visible elements in subtree
	void getVisibleSubTreeList(std::list<Node*>& list);
    // returns a list of enabled elements in subtree
    void getEnabledSubTreeList(std::list<Node*>& list);
	
	/******
     * Scene sorting
     *
     * When drawing and interacting with the scene the nodes are sorted 
     * from bottom to top.
     * Sorting is achieved using three values:
     * 1. Plane number:
     *    Is a float that you can set using setPlane(float).
     * 2. Depth in scene:
     *    If the plane number of two nodes is the same, we sort by the 
     *    depth in the scene-tree.
     * 3. Same Depth Offset:
     *    Is used to resolve a case where 1 and 2 are the same and we want 
     *    consistency between drawing and interaction.
     */
	static bool bottomPlaneFirst(const Node* u, const Node* v)
	{
		if (u->getGlobalPlane() == v->getGlobalPlane()) {
            // same plane
            if (u->getDepthInScene() == v->getDepthInScene()) {
                // same plane and depth in scene, resolve with sameDepthOffset
                return u->sameDepthOffset < v->sameDepthOffset;
            }
            else {
                return u->getDepthInScene() < v->getDepthInScene();
            }
		}
		else {
			return u->getGlobalPlane() < v->getGlobalPlane();
		}
	}

	static bool topPlaneFirst(const Node* u, const Node* v)
	{
		if (u->getGlobalPlane() == v->getGlobalPlane()) {
            // same plane
            if (u->getDepthInScene() == v->getDepthInScene()) {
                // same plane and depth in scene, resolve with sameDepthOffset
                return u->sameDepthOffset > v->sameDepthOffset;
            }
            else {
                return u->getDepthInScene() > v->getDepthInScene();
            }
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

    // hold custom data
	void* data;


	// debug stuff
	ofColor debugBorderColor;
#ifdef USE_OFX_HISTORY_PLOT
	ofxHistoryPlot* historyPlot;
#endif

	#ifdef OFXUINODE_DEBUG
	static ofColor defaultNodeColor;
	static ofColor touchDownNodeColor;
	static ofColor touchUpNodeColor;
	static ofColor touchExitNodeColor;
	static ofColor touchEnterNodeColor;
	#endif

private:
	bool bSendDestroy;
    
    /******
     * sameDepthOffset:
     * resolve sorting-by-depth ambiguity for nodes on the same plane and tree depth.
     * it is set to a random number between 0 - 1 in the constructor.
     */
    float sameDepthOffset;
    
    
    /******
     * TouchManager will call these on touch events
     * from there functions we dispatch the touch events above
     */
    void touchDown(int id, TouchEvent* event);
    void touchMove(int id,  TouchEvent* event);
    void touchUp(int id,  TouchEvent* event);
    void touchExit(int id, TouchEvent* event);
    void touchEnter(int id, TouchEvent* event);
};

}

#endif /* defined(__osxLingo__GNode__) */
