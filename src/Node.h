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
	struct NodeSettings {
		string name;
		ofVec2f position;
		ofVec2f size;
		bool renderClip = false;
		bool isActive = true;
		float plane = 10;
};

class Node : public ofNode
{
	friend class TouchManager;
	friend class TouchEvent;
	
public:
	virtual ~Node();
	Node();
	Node(const Node& mom);
	virtual Node* clone();

	void setup(NodeSettings settings);

    /******
     * Node Names:
     *
     * If you like naming your components (you should)
     */
	void setName(const std::string& _name) { name = _name; }
	const std::string& getName() const { return name; }
    // search the tree for a node with a specific name, searchDepth of -1 means search all the way down
    Node* getChildWithName(const std::string& searchName, int searchDepth = -1) const;
	Node* getParentWithName(const std::string& searchName, int searchDepth = -1) const;

    /******
     * Adding & Removing childNodes
     */
	void addChild(Node* child, int insertAt=-1, bool bMaintainChildGlobalTransform=false);		// append by default
	Node* removeChild(Node *child, bool bMaintainChildGlobalTransform=false);
	Node* removeChild(int index, bool bMaintainChildGlobalTransform=false);
	int getNumChildren() const { return (int)childNodes.size(); }
	const vector<Node*>& getChildren() const { return childNodes; }
	bool haveChild(Node* child);

	ofEvent<ofxInterface::Node> eventChildRemoved;
	ofEvent<ofxInterface::Node> eventChildAdded;

	/******
	 * Sort children
	 */
	void sortChildren(const function<bool(const Node* a, const Node* b)>& compareFunction);

	// functions to override
	//
	virtual void update(float dt) {};	// please override with update code
	virtual void draw() {};		// please override! draw your object in local space
	virtual void preDraw() {};		// affects group rendering, executed before the draw function
	virtual void postDraw() {};		// affects group rendering, executed after the draw function

	// for debugging
	virtual void drawDebug();	 // debug debugging stuff (will be called by renderDebug)
	virtual void drawBounds();   // overide for debugging if bounds are not rectangular

	/******
	 * for touch event handling
	 *
	 * override to return if a point is inside the node.
	 * default implementation works only if your component is 
	 * rectangular and you set the size.
	 */
	virtual bool contains(const ofVec3f& globalPoint);

	// interaction events (register to these events and you're good to go)
	ofEvent<TouchEvent> eventTouchDown;
	ofEvent<TouchEvent> eventTouchMove;
	ofEvent<TouchEvent> eventTouchUp;
	ofEvent<TouchEvent> eventTouchExit;
	ofEvent<TouchEvent> eventTouchEnter;
	ofEvent<TouchEvent> eventClick;

	// interaction functions (register lambda functions here)
	void setTouchDownFunction(std::function<void(ofxInterface::TouchEvent&)> _func);
	void setTouchMoveFunction(std::function<void(ofxInterface::TouchEvent&)> _func);
	void setTouchUpFunction(std::function<void(ofxInterface::TouchEvent&)> _func);
	void setClickFunction(std::function<void(ofxInterface::TouchEvent&)> _func);


	// use this to enforce only one touch at a time (single touch)
	void setAllowOnlyOneTouch(bool set) { bNodeAllowOneTouch = set; }
	bool isTouched() { return bNodeTouched; }
	int getLastTouchId() { return nodeCurrentTouchId; }


	/******
	 * render will render this component and its subtree.
	 * usually should be called on the root scene object,
	 * but can be used also for offline rendering of any branch of the graph
	 *
	 * render is done as follows:
	 * 1. get list of subtree nodes (only visible ones if forceAll == false)
	 * 2. sort by plane value
	 * 3. call draw from back to front
	 */
	void render(bool forceAll = false);
	void renderDebug(bool forceAll = false);	// same as render but calls drawDebug instead of draw.

	//TODO : description
	void renderDynamic(bool forceAll = false);
	void renderGroups(bool forceAll = false);
    
    /******
     * call the 'update' function of !!visible!! children
     */
	void updateSubtree(float dt, bool forceAll=false);
	void updateSubtreePostOrder(float dt, bool forceAll=false);

	// set this for nodes that are rendered offline and still want to update (without forceall),
	void setUpdateWhenHidden(bool set) { bNodeUpdateWhenHidden = set; }
	bool getUpdateWhenHidden() { return bNodeUpdateWhenHidden; }
	// clip touch means children outside of the node's boundary are disabled
	void setTouchClip(bool set) { bClipTouch = set; }
	bool getTouchClip() { return bClipTouch; }
	// clip render means children outside of the node's boundary will be clipped
	void setRenderClip(bool set) { bClipRender = set; }
	bool getRenderClip() { return bClipRender; }
	bool getGlobalRenderClip();
	ofRectangle getRenderClipRect();
	// if group is activated then all children are rendered afterwards and using the plane of this node (enables group masking)
	void setRenderChildrenInGroup(bool set) {bRenderChildrenInGroup = set;};
	bool getRenderChildrenInGroup() { return bRenderChildrenInGroup; };

	// override these functions if you use some other renderer such as NanoVG
	virtual void enableScissor(const ofRectangle& rect) { enableScissor(rect.x, rect.y, rect.width, rect.height); }
	virtual void enableScissor(float x, float y, float w, float h);
	virtual void disableScissor();

	/******
	 * Position
	 */
	virtual void setPosition(float x=0, float y=0, float z=0) { ofNode::setPosition(x, y, z); }
	virtual void setPosition(const ofVec3f& p) { Node::setPosition(p.x, p.y, p.z); }
	virtual void setX(float x) { Node::setPosition(x, getY(), getZ()); }
	virtual void setY(float y) { Node::setPosition(getX(), y, getZ()); }
	virtual void setZ(float z) { Node::setPosition(getX(), getY(), z); }

	/******
	 * hooks into ofNode to get events for position, scale, and orientation changes
	 */
	void onPositionChanged() { ofNotifyEvent(eventPositionChanged, *this); }
	void onScaleChanged() { ofNotifyEvent(eventScaleChanged, *this); }
	void onOrientationChanged() { ofNotifyEvent(eventOrientationChanged, *this); }
	ofEvent<Node> eventPositionChanged;
	ofEvent<Node> eventScaleChanged;
	ofEvent<Node> eventOrientationChanged;
    
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
    virtual ofVec2f getSize() { return size; }
	virtual void setSize(float w, float h) { size.set(w, h); ofNotifyEvent(eventNodeSizeChanged, *this, this); }
	virtual void setSize(const ofVec2f& s) { Node::setSize(s.x, s.y);}
	virtual float getWidth() { return getSize().x; }
	void setWidth(float w) { size.x = w; ofNotifyEvent(eventNodeSizeChanged, *this, this); }
	virtual float getHeight() { return getSize().y; }
	void setHeight(float h) { size.y = h; ofNotifyEvent(eventNodeSizeChanged, *this, this); }
	float getLocalWidth() { return getSize().x * getScale().x; }
	float getLocalHeight() { return getSize().y * getScale().y; }
	float getGlobalHeight() { return getSize().y * getGlobalScale().y; }
	float getGlobalWidth() { return getSize().x * getGlobalScale().x; }

	/* Those are deprecated as their naming is misleading and results greatly depned on who the caller is
	ofVec2f getCenter(){ return ofVec2f(getX() + size.x * 0.5, getY() + size.y * 0.5); }
	ofVec2f getGlobalCenter(){ return toGlobal(getCenter()); }
	*/

	virtual ofRectangle getBounds() { return ofRectangle(getX(), getY(), getWidth(), getHeight()); }
    
    /******
     * resizing events
     *
     * register to get events when settings the size
     */
    ofEvent<Node> eventNodeSizeChanged;
    
    /******
     * translate local and global space points
     */
	virtual ofVec3f toLocal(const ofVec3f& globalPoint);
	virtual ofVec3f toGlobal(const ofVec3f& localPoint);

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
	// setReceivingTouch(false) does not prevent decendents in the tree from receiving touch events
	void setReceivingTouch(bool set) { bReceivingTouch = set; }
	bool getReceivingTouch() { return bReceivingTouch; }
    
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
	// returns a list representation of all childNodes in sub graph
	void getSubTreeList(std::list<Node*>& list, bool returnAllGroupElements = true);
	// returns a list of visible elements in subtree
	void getVisibleSubTreeList(std::list<Node*>& list, bool returnAllGroupElements = true);
    // returns a list of enabled elements in subtree
    void getEnabledSubTreeList(std::list<Node*>& list);
	
	/******
     * Scene sorting
     *
	 * When drawing and interacting with the scene the nodes are sorted
     * from bottom to top.
     * Sorting is by two values:
     * 1. Plane number:
     *    use setPlane to set the plane (default 0 to all).
     * 2. Traversal index:
     *    Traversal in the scene tree where the order is Pre-Order (mrl) for rendering and Post-Order (rlm) for interaction.
     */
	static bool bottomPlaneFirst(const Node* u, const Node* v)
	{
#ifdef OLDSORT
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
#else
		return u->getGlobalPlane() < v->getGlobalPlane();
#endif
	}

	static bool topPlaneFirst(const Node* u, const Node* v)
	{
#ifdef OLDSORT
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
			return v->getGlobalPlane() < u->getGlobalPlane();
		}
#else
		return v->getGlobalPlane() < u->getGlobalPlane();
#endif
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
	void setData(shared_ptr<void> _data) { data = _data; }
	shared_ptr<void> getData() { return data; }

	//check for leaks
	static int getNumNodesAlive();

	enum Side {
		LEFT,
		TOP,
		RIGHT,
		BOTTOM
	};
	void placeNextTo(Node& comp, Node::Side side, float margin=0);


	ofEvent<void> eventDestroy;			// send this event in the destructor


	string print(int depth=0) const;

	template <class ListenerClass, typename ArgType>
	void addEventListener(const string& eventName, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(ArgType&), bool recursive=false)
	{
		ofEvent<ArgType>* event = getEventForName<ArgType>(eventName);
		if (event == NULL) {
			ofLogError("Node") << "no event named '"<<eventName<<"'";
			return;
		}

#if (OF_VERSION_MINOR < 9) || defined(OF_USES_POCO)
		(*event) -= Poco::priorityDelegate(listener, listenerMethod, OF_EVENT_ORDER_AFTER_APP);
		(*event) += Poco::priorityDelegate(listener, listenerMethod, OF_EVENT_ORDER_AFTER_APP);
#else
		event->remove(listener, listenerMethod, OF_EVENT_ORDER_AFTER_APP);
		event->add(listener, listenerMethod, OF_EVENT_ORDER_AFTER_APP);
#endif

		if (recursive) {
			for (Node* n : childNodes) {
				n->addEventListener(eventName, listener, listenerMethod, recursive);
			}
		}
	}

	template <class ListenerClass, typename ArgType>
	void removeEventListener(const string& eventName, ListenerClass  * listener, void (ListenerClass::*listenerMethod)(ArgType&), bool recursive=false)
	{
		ofEvent<ArgType>* event = getEventForName<ArgType>(eventName);
		if (event == NULL) {
			ofLogError("Node") << "no event named '"<<eventName<<"'";
			return;
		}

#if (OF_VERSION_MINOR < 9) || defined(OF_USES_POCO)
		(*event) -= Poco::priorityDelegate(listener, listenerMethod, OF_EVENT_ORDER_AFTER_APP);
#else
		event->remove(listener, listenerMethod, OF_EVENT_ORDER_AFTER_APP);
#endif

		if (recursive) {
			for (Node* n : childNodes) {
				n->removeEventListener(eventName, listener, listenerMethod, recursive);
			}
		}
	}

	ofVec3f getTranslationTo(Node* node);
	float getAngleTo(Node* node);

	/******
	 * Exporting
	 *
	 * export the node as json
	 *
	 */

	ofJson getSceneDescription();
	ofJson getSceneDescription(vector<string> attributes, bool onlyActiveNodes = false);
	virtual ofJson getNodeJson();
	string listActiveNodes(int depth = 0);

protected:
	ofJson filterSceneDescription(ofJson desc, vector<string> attributes,bool onlyActiveNodes);

	std::string name;
	ofVec2f size;

	bool bVisible;
	bool bEnabled;
	bool bReceivingTouch;
	bool bClipRender;
	bool bClipTouch;
	bool bRenderChildrenInGroup = false;

	vector<Node*> childNodes;

	// plane (instead of z)
	// small value means back, large value is front
	float plane;

    // hold custom data
	shared_ptr<void> data;


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
	template<typename ArgType>
	ofEvent<ArgType>* getEventForName(const string& eventName)
	{
		if (eventName == "touchdown") {
			return &eventTouchDown;
		}
		else if (eventName == "touchmove") {
			return &eventTouchMove;
		}
		else if (eventName == "touchup") {
			return &eventTouchUp;
		}
		else {
			return NULL;
		}
	}

	// lambda functions here
	std::function<void(ofxInterface::TouchEvent&)> touchDownFunc;
	std::function<void(ofxInterface::TouchEvent&)> touchMoveFunc;
	std::function<void(ofxInterface::TouchEvent&)> touchUpFunc;
	std::function<void(ofxInterface::TouchEvent&)> clickFunc;

	void onTouchDown(TouchEvent& event);
	void onTouchMove(TouchEvent& event);
	void onTouchUp(TouchEvent& event);
	void onClick(TouchEvent& event);

	bool bSendDestroy;
	bool bNodeAllowOneTouch;
	bool bNodeUpdateWhenHidden;
	bool bNodeTouched;
	int nodeCurrentTouchId;

	bool isClickAllowed = true;

#ifdef OLD_SORT
    /******
     * sameDepthOffset:
     * resolve sorting-by-depth ambiguity for nodes on the same plane and tree depth.
     * it is set to a random number between 0 - 1 in the constructor.
     */
    float sameDepthOffset;
#endif

    /******
     * TouchManager will call these on touch events
     * from there functions we dispatch the touch events above
     */
    void touchDown(int id, TouchEvent* event);
    void touchMove(int id,  TouchEvent* event);
    void touchUp(int id,  TouchEvent* event);
    void touchExit(int id, TouchEvent* event);
    void touchEnter(int id, TouchEvent* event);


	//helper functions for export json
	const ofJson toJson(glm::vec3 val);
	const ofJson toJson(glm::vec2 val);
	const ofJson toJson(glm::quat val);
};

}




#endif /* defined(__osxLingo__GNode__) */
