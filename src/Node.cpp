//
//  Node.cpp
//
//
//  Created by Gal Sasson on 7/24/14.
//
//

#include "Node.h"

namespace ofxInterface {

#ifdef OFXUINODE_DEBUG
ofColor Node::defaultNodeColor = ofColor(200, 200, 0);
ofColor Node::touchDownNodeColor = ofColor(0, 128, 255);
ofColor Node::touchUpNodeColor = ofColor(0, 255, 128);
ofColor Node::touchExitNodeColor = ofColor(128, 0, 255);
ofColor Node::touchEnterNodeColor = ofColor(255, 0, 128);
#endif

static int numInterfaceNodes = 0;

Node::~Node()
{
	numInterfaceNodes--;
	if (parent != nullptr) {
		((Node*)parent)->removeChild(this);
	}

	if (bSendDestroy) {
		ofNotifyEvent(eventDestroy);
	}

	while (!childNodes.empty()) {
		delete removeChild(0);
	}
}

Node::Node()
{
	numInterfaceNodes++;
	plane = 0;
	size.set(0, 0);

	bVisible = true;
	bEnabled = true;
	bClipTouch = false;
	bClipRender = false;
	bReceivingTouch = true;
	data = nullptr;

	bSendDestroy = true;
	bNodeAllowOneTouch = false;
	bNodeUpdateWhenHidden = false;
	bNodeTouched = false;
	nodeCurrentTouchId = -1;	// not relevant
	data = shared_ptr<ofJson>(new ofJson());
	dataIsJson = true;
#ifdef OLDSORT
    sameDepthOffset = ofRandom(0, 1);
#endif

#ifdef OFXUINODE_DEBUG
	debugBorderColor = defaultNodeColor;
#endif

#ifdef USE_OFX_HISTORY_PLOT
	historyPlot = nullptr;
#endif

	ofAddListener(eventTouchDown, this, &Node::onTouchDown);
	ofAddListener(eventTouchMove, this, &Node::onTouchMove);
	ofAddListener(eventTouchUp, this, &Node::onTouchUp);
	ofAddListener(eventClick, this, &Node::onClick);
}
	
	Node::Node(const Node& mom) : ofNode(mom)
{
	name = mom.name;
	plane = mom.plane;
	size = mom.size;
	bVisible = mom.bVisible;
	bEnabled = mom.bEnabled;
	bClipTouch = mom.bClipTouch;
	bClipRender = mom.bClipRender;
	bReceivingTouch = mom.bReceivingTouch;
	setData(mom.data);
	bSendDestroy = mom.bSendDestroy;
	bNodeAllowOneTouch = mom.bNodeAllowOneTouch;
	bNodeUpdateWhenHidden = mom.bNodeUpdateWhenHidden;
	bNodeTouched = mom.bNodeTouched;
	nodeCurrentTouchId = -1;
	data = mom.data;
	dataIsJson = mom.dataIsJson;
#ifdef OLDSORT
	sameDepthOffset = ofRandom(0, 1);
#endif
	
#ifdef OFXUINODE_DEBUG
	debugBorderColor = mom.debugBorderColor;
#endif
	
#ifdef USE_OFX_HISTORY_PLOT
	historyPlot = nullptr;
#endif
	
	// copy children
	for (auto& c: mom.getChildren()) {
		addChild(c->clone());
	}
	ofAddListener(eventTouchDown, this, &Node::onTouchDown);
	ofAddListener(eventTouchMove, this, &Node::onTouchMove);
	ofAddListener(eventTouchUp, this, &Node::onTouchUp);
}
	
Node* Node::clone()
{
	auto ret = new Node(*this);
	
	/*for (auto& child : getChildren())
	{
		ret->addChild(child->clone());
	}*/
	return ret;
}

void Node::setup(NodeSettings settings) {
	setName(settings.name);
	setPosition(settings.position);
	setSize(settings.size);
	setPlane(settings.plane);
	setRenderClip(settings.renderClip);
	if (settings.isActive) {
		activate();
	}
	else {
		deactivate();
	}

	if (settings.effects.dropShadow.size > 0) {
		effectDropShadow.setup(settings.effects.dropShadow);
	}
	effectSettings = settings.effects;
}

int Node::getNumNodesAlive(){
	return numInterfaceNodes;
}

Node* Node::getChildWithName(const std::string &searchName, int searchDepth) const
{
    if (searchName == name) {
        return (Node*)this;
    }

    if (searchDepth==0) {
        return nullptr;
    }

    for (int i=0; i<childNodes.size(); i++)
    {
        Node* node = childNodes[i]->getChildWithName(searchName, searchDepth-1);
        if (node != nullptr) {
            return node;
        }
    }

    return nullptr;
}

vector<Node*> Node::getChildrenWithName(const std::string& searchName, int searchDepth) const
{
	vector<Node*> ret;

	if (searchName == name) {
		ret.push_back((Node*)this);
	}

	if (searchDepth == 0) {
		return ret;
	}

	for (int i = 0; i < childNodes.size(); i++)
	{
		vector<Node*> retC = childNodes[i]->getChildrenWithName(searchName, searchDepth - 1);
		if (retC.size() > 0) {
			ret.insert(ret.end(), retC.begin(), retC.end());
		}
	}

	return ret;
}

Node* Node::getParentWithName(const std::string &searchName, int searchDepth) const
{
	if (searchName == name) {
		return (Node*)this;
	}

	if (searchDepth==0) {
		return nullptr;
	}

	if (parent == nullptr) {
		return nullptr;
	}

	return ((Node*)parent)->getParentWithName(searchName, searchDepth-1);
}

void Node::drawDebug()
{
	// draw border
	ofSetLineWidth(1);
	ofNoFill();
	ofSetColor(debugBorderColor);

	drawBounds();
	#ifdef OFXUINODE_DEBUG
	debugBorderColor = debugBorderColor.lerp(defaultNodeColor, 0.05);
	#endif

	stringstream ss;
	ss<<name<<" (plane: "<<getGlobalPlane()<<((bVisible)?", visible":"")<<")";
	ofDrawBitmapString(ss.str(), 0, -3);

#ifdef USE_OFX_HISTORY_PLOT
	if (historyPlot != nullptr) {
		ofFill();
		historyPlot->draw(0, getHeight(), 100, 50);
	}
#endif
}

void Node::drawBounds()
{
	ofDrawRectangle(0, 0, getWidth(), getHeight());
}

void Node::render(bool forceAll)
{
	//not yet finally tested
	renderDynamic(forceAll);
	/*
	std::list<Node*> sortedNodes;
	std::list<Node*>::iterator it;

	if (forceAll) {
		// get all nodes (visible and invisible)
		getSubTreeList(sortedNodes);
	}
	else {
		// get only visible nodes
		getVisibleSubTreeList(sortedNodes);
	}

	// sort scene by z (+z goes outside of the screen), plane is z
	sortedNodes.sort(Node::bottomPlaneFirst);

	for (it=sortedNodes.begin(); it!=sortedNodes.end(); it++)
	{
		ofPushStyle();
		ofPushMatrix();
		ofMultMatrix((*it)->getGlobalTransformMatrix());
		// use anchor

		if ((*it)->getGlobalRenderClip()) {
			(*it)->enableScissor((*it)->getRenderClipRect());
		}

		(*it)->draw();

		if ((*it)->getGlobalRenderClip()) {
			(*it)->disableScissor();
		}

		ofPopMatrix();
		ofPopStyle();
	}
	*/
}

void Node::renderDebug(bool forceAll)
{
	// sort scene by z (+z goes outside of the screen), plane is z
	std::list<Node*> sortedNodes;
	std::list<Node*>::iterator it;

	if (forceAll) {
		// get all nodes (visible and invisible)
		getSubTreeList(sortedNodes);
	}
	else {
		// get only visible nodes
		getVisibleSubTreeList(sortedNodes);
	}

	// sort scene by z (+z goes outside of the screen), plane is z
	sortedNodes.sort(Node::bottomPlaneFirst);

	for (it=sortedNodes.begin(); it!=sortedNodes.end(); it++)
	{
		ofPushStyle();
		ofPushMatrix();
		ofMultMatrix((*it)->getGlobalTransformMatrix());
		// use anchor

		(*it)->drawDebug();

		ofPopMatrix();
		ofPopStyle();
	}
}

void Node::renderDynamic(bool forceAll)
{
	std::list<Node*> sortedNodes;
	std::list<Node*>::iterator it;

	if (forceAll) {
		// get all nodes (visible and invisible)
		getSubTreeList(sortedNodes);
	}
	else {
		// get only visible nodes
		getVisibleSubTreeList(sortedNodes);
	}

	// sort scene by z (+z goes outside of the screen), plane is z
	sortedNodes.sort(Node::bottomPlaneFirst);

	for (it = sortedNodes.begin(); it != sortedNodes.end(); it++)
	{
		ofPushStyle();
		ofPushMatrix();
		ofMultMatrix((*it)->getGlobalTransformMatrix());
		// use anchor

		if ((*it)->getGlobalRenderClip()) {
			(*it)->enableScissor((*it)->getRenderClipRect());
		}
		if ((*it)->getRenderChildrenInGroup()) {
			(*it)->renderGroups();
		}
		else {

			(*it)->drawDropShadow();
			(*it)->draw();
		}

		if ((*it)->getGlobalRenderClip()) {
			(*it)->disableScissor();
		}

		ofPopMatrix();
		ofPopStyle();
	}
}

void Node::renderGroups(bool forceAll)
{
	for (auto& child : childNodes) {
		if (forceAll || child->getVisible()) {
			
			//if (child->getName() != "") cout << child->getName() << "  predraw " <<endl;
			child->preDraw();
			//ofPushStyle();
			ofPushMatrix();
			ofMultMatrix(child->getGlobalTransformMatrix());
			// use anchor

			if (child->getGlobalRenderClip()) {
				child->enableScissor(child->getRenderClipRect());
			}
			//if (child->getName() != "") cout << child->getName() << "  draw " << endl;
			child->draw();
			if (child->getGlobalRenderClip()) {
				child->disableScissor();
			}

			ofPopMatrix();
			//ofPopStyle();
			child->renderGroups();
			//if (child->getName() != "") cout << child->getName() << "  postDraw" << endl;
			child->postDraw();
		}
	}
}

void Node::updateSubtree(float dt, bool forceAll)
{
	update(dt);

    std::vector<Node*>::iterator it;
    
    for (it = childNodes.begin(); it != childNodes.end(); it++)
    {
		if (forceAll) {
			(*it)->updateSubtree(dt, forceAll);
		}
		else {
			if ((*it)->getVisible() || (*it)->bNodeUpdateWhenHidden) {
				(*it)->updateSubtree(dt, forceAll);
			}
		}
    }
}

void Node::updateSubtreePostOrder(float dt, bool forceAll)
{
	std::vector<Node*>::iterator it;

	for (it = childNodes.begin(); it != childNodes.end(); it++)
	{
		if (forceAll) {
			(*it)->updateSubtreePostOrder(dt, forceAll);
		}
		else {
			if ((*it)->getVisible() || (*it)->bNodeUpdateWhenHidden) {
				(*it)->updateSubtreePostOrder(dt, forceAll);
			}
		}
	}

	update(dt);
}

bool Node::getGlobalRenderClip()
{
	if (getParent() != nullptr) {
		return bClipRender || ((Node*)getParent())->getGlobalRenderClip();
	}
	else {
		return bClipRender;
	}
}

ofRectangle Node::getRenderClipRect()
{
	ofRectangle parentRect;
	if (getParent() != nullptr) {
		parentRect = ((Node*)getParent())->getRenderClipRect();
	}
	ofRectangle rect;
	if (bClipRender) {
		ofVec2f pos = getGlobalPosition();

		rect = ofRectangle(pos.x, pos.y, getGlobalWidth(), getGlobalHeight());
	}

	if (parentRect.width == 0 || parentRect.height == 0) {
		if (rect.width == 0 || rect.height == 0) {
			return ofRectangle();
		}
		else {
			return rect;
		}
	}
	else {
		if (rect.width == 0 || rect.height == 0) {
			return parentRect;
		}
		else {
			return parentRect.getIntersection(rect);
		}
	}
}

void Node::enableScissor(float x, float y, float w, float h)
{
	ofVec2f local = toLocal(ofVec2f(x, y));
	ofVec2f localScale = ofVec2f(w, h)*getGlobalScale();
	glEnable(GL_SCISSOR_TEST);
	//glScissor(local.x, local.y, localScale.x, localScale.y);
	// openGL uses an inverted y - axis, so we need to invert the y value
	float dy = ofGetWindowHeight()-h -y;
	
	glScissor(x,  dy, localScale.x, localScale.y);
}

void Node::disableScissor()
{
	glDisable(GL_SCISSOR_TEST);
}

void Node::setCenteredH()
{
    if (parent == nullptr) {
        return;
    }

    setX((((Node*)parent)->getLocalWidth() - getLocalWidth())/2);
}

void Node::setCenteredV()
{
    if (parent == nullptr) {
        return;
    }

    setY((((Node*)parent)->getLocalHeight() - getLocalHeight())/2);
}

void Node::setCentered()
{
    if (parent == nullptr) {
        return;
    }

    setPosition((((Node*)parent)->getLocalWidth() - getLocalWidth())/2,
                (((Node*)parent)->getLocalHeight() - getLocalHeight())/2);
}

void Node::drawDropShadow()
{
	if (effectSettings.dropShadow.size > 0) {
		ofRectangle drawingRect = ofRectangle(
			-effectSettings.dropShadow.size + effectSettings.dropShadow.position.x,
			-effectSettings.dropShadow.size + effectSettings.dropShadow.position.y,
			getWidth() + effectSettings.dropShadow.size * 2,
			getHeight() + effectSettings.dropShadow.size * 2
		);

		if (!fboDropShadow.isAllocated()) {
			fboDropShadow.allocate(
				getWidth() ,//+ effectSettings.dropShadow.size * 2,
				getHeight() //+ effectSettings.dropShadow.size * 2
			);

			fboDropShadow.begin();
			ofClear(0, 0);
			
			draw();
			fboDropShadow.end();
			effectDropShadow.update(fboDropShadow);

		}

		//ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_MULTIPLY);
		effectDropShadow.update(fboDropShadow);
		effectDropShadow.getShadow().draw(drawingRect);
		ofEnableAlphaBlending();
	}
}

void Node::onTouchDown(TouchEvent & event) {
	if (touchDownFunc) {
		touchDownFunc(event);
	}
}

void Node::onTouchMove(TouchEvent & event) {
	if (touchMoveFunc) {
		touchMoveFunc(event);
	}
}

void Node::onTouchUp(TouchEvent & event) {
	if (touchUpFunc) {
		touchUpFunc(event);
	}
}

void Node::onClick(TouchEvent & event) {
	if (clickFunc) {
		clickFunc(event);
	}
}

void Node::touchDown(int id,  TouchEvent* event)
{
	if (bNodeAllowOneTouch && bNodeTouched && nodeCurrentTouchId!=id) {
		return;
	}

	bNodeTouched = true;
	nodeCurrentTouchId = id;

#ifdef OFXUINODE_DEBUG
	debugBorderColor = touchDownNodeColor;
#endif

#ifdef USE_OFX_HISTORY_PLOT
	historyPlot = new ofxHistoryPlot(&event->velocitySmoothed.y, "currentFrameRate auto updated", 500, true);
	historyPlot->setBackgroundColor(ofColor(0, 0, 0, 255));
	historyPlot->setColor( ofColor(0,255,0) );
	historyPlot->setShowNumericalInfo(false);
//	historyPlot->setRespectBorders(true);
	historyPlot->setLineWidth(1);
#endif
	ofNotifyEvent(eventTouchDown, *event);
}

void Node::touchMove(int id,  TouchEvent* event)
{
	if (bNodeAllowOneTouch && id != nodeCurrentTouchId) {
		return;
	}
	
	if (event->velocitySmoothed.length() > 200) {
		isClickAllowed = false;
	}

#ifdef OFXUINODE_DEBUG
	debugBorderColor.a -= 10;
#endif

	ofNotifyEvent(eventTouchMove, *event);
}

void Node::touchUp(int id,  TouchEvent* event)
{
	if (bNodeAllowOneTouch && id != nodeCurrentTouchId) {
		return;
	}

#ifdef OFXUINODE_DEBUG
	debugBorderColor = touchUpNodeColor;
#endif

#ifdef USE_OFX_HISTORY_PLOT
	if (historyPlot) {
		delete historyPlot;
		historyPlot = nullptr;
	}
#endif

	ofNotifyEvent(eventTouchUp, *event);
	if (contains(event->position) && isClickAllowed) {
		ofNotifyEvent(eventClick, *event);
	}

	bNodeTouched = false;
	isClickAllowed = true;
}

void Node::touchExit(int id, TouchEvent *event)
{
	if (bNodeAllowOneTouch && id != nodeCurrentTouchId) {
		return;
	}

#ifdef OFXUINODE_DEBUG
	debugBorderColor = touchExitNodeColor;
#endif
	ofNotifyEvent(eventTouchExit, *event);
}

void Node::touchEnter(int id, TouchEvent *event)
{
	if (bNodeAllowOneTouch && id != nodeCurrentTouchId) {
		return;
	}

#ifdef OFXUINODE_DEBUG
	debugBorderColor = touchEnterNodeColor;
#endif
	ofNotifyEvent(eventTouchEnter, *event);
}

const ofJson Node::toJson(glm::vec3 val)
{
	ofJson ret;
	ret.push_back(val.x);
	ret.push_back(val.y);
	ret.push_back(val.z);
	return ret;
}

const ofJson Node::toJson(glm::vec2 val)
{
	ofJson ret;
	ret.push_back(val.x);
	ret.push_back(val.y);
	return ret;
}

const ofJson Node::toJson(glm::quat val)
{
	ofJson ret;
	ret.push_back(val.w);
	ret.push_back(val.x);
	ret.push_back(val.y);
	ret.push_back(val.z);
	return ofJson();
}

ofVec3f Node::toLocal(const ofVec3f &screenPoint)
{
	#ifdef GLM_SWIZZLE //this version of OF is using GLM for math ops
	return (ofVec3f)screenPoint * glm::inverse(ofNode::getGlobalTransformMatrix());
	#else
	//return (ofVec3f)screenPoint*ofNode::getGlobalTransformMatrix().getInverse();
	return (ofVec3f)screenPoint * glm::inverse(ofNode::getGlobalTransformMatrix());
	#endif
}
    
ofVec3f Node::toGlobal(const ofVec3f &localPoint)
{
    return (ofVec3f)localPoint*ofNode::getGlobalTransformMatrix();
}

void Node::setVisible(bool visible)
{
    bool bSendAppear = visible && !bVisible;
    bool bSendDisappear = !visible && bVisible;
    std::list<Node*> nodes;
    std::list<Node*>::iterator it;
    
    if (bSendDisappear) {
        // get visible nodes DEFORE we make outselves invisible
        getVisibleSubTreeList(nodes);
    }
    
    bVisible = visible;
    
    // notify appear events
    if (bSendAppear) {
        // get visible nodes AFTER we make ourselves visible
        getVisibleSubTreeList(nodes);
        for (it = nodes.begin(); it != nodes.end(); it++) {
            ofNotifyEvent((*it)->eventNodeDidAppear, *(*it), this);
        }
    }
    
    // send disappear events
    if (bSendDisappear) {
        for (it = nodes.begin(); it != nodes.end(); it++) {
            ofNotifyEvent((*it)->eventNodeDidDisappear, *(*it), this);
        }
    }
}

bool Node::getVisibleGlobally() const
{
    if (parent == nullptr) {
        return bVisible;
    }
    else {
        return bVisible && ((Node*)parent)->getVisibleGlobally();
    }
}

void Node::setEnabled(bool enable)
{
    bool bSendEnabled = enable && !bEnabled;
    bool bSendDisabled = !enable && bEnabled;
    std::list<Node*> nodes;
    std::list<Node*>::iterator it;
    
    if (bSendDisabled) {
        // get visible nodes DEFORE we make outselves invisible
        getEnabledSubTreeList(nodes);
    }
    
    bEnabled = enable;
    
    // notify appear events
    if (bSendEnabled) {
        // get visible nodes AFTER we make ourselves visible
        getEnabledSubTreeList(nodes);
        for (it = nodes.begin(); it != nodes.end(); it++) {
            ofNotifyEvent((*it)->eventNodeWasEnabled, *(*it), this);
        }
    }
    
    // send disappear events
    if (bSendDisabled) {
        for (it = nodes.begin(); it != nodes.end(); it++) {
            ofNotifyEvent((*it)->eventNodeWasDisabled, *(*it), this);
        }
    }
}

bool Node::getEnabledGlobally() const
{
    if (parent == nullptr) {
        return bEnabled;
    }
    else {
        return bEnabled && ((Node*)parent)->getEnabledGlobally();
    }
}
    
bool Node::contains(const ofVec3f &globalPoint)
{
	ofVec2f local = toLocal(globalPoint);

	if (local.x < 0 ||
		local.y < 0 ||
		local.x > size.x ||
		local.y > size.y) {
		return false;
	}

	return true;
}

void Node::setTouchDownFunction(std::function<void(ofxInterface::TouchEvent&)> _func) {
	touchDownFunc = _func;
}

void Node::setTouchMoveFunction(std::function<void(ofxInterface::TouchEvent&)> _func) {
	touchMoveFunc = _func;
}

void Node::setTouchUpFunction(std::function<void(ofxInterface::TouchEvent&)> _func) {
	touchUpFunc = _func;
}

void Node::setClickFunction(std::function<void(ofxInterface::TouchEvent&)> _func) {
	clickFunc = _func;
}

void Node::addChild(Node *child, int insertAt, bool bMaintainChildGlobalTransform)
{
	child->setParent(*this, bMaintainChildGlobalTransform);

	if (insertAt == -1 || insertAt > childNodes.size()) {
		// append
		childNodes.push_back(child);
	}
	else {
		childNodes.insert(childNodes.begin()+insertAt, child);
	}

	ofNotifyEvent(eventChildAdded, *child, this);
}

Node* Node::removeChild(Node *child, bool bMaintainChildGlobalTransform)
{
	for (int i=0; i<childNodes.size(); i++)
	{
		if (childNodes[i] == child) {
			return removeChild(i, bMaintainChildGlobalTransform);
		}
	}

	ofLogWarning("ofxInterface::Node::removeChild", "are you trying to remove a child that does not exist?");
	return nullptr;
}

Node* Node::removeChild(int index, bool bMaintainChildGlobalTransform)
{
	if (index >= childNodes.size()) {
		ofLogWarning("ofxInterface::Node::removeChild", "are you trying to remove a child that does not exist?");
		return nullptr;
	}

	Node *child = childNodes[index];
	childNodes.erase(childNodes.begin()+index);
	child->clearParent(bMaintainChildGlobalTransform);
	ofNotifyEvent(eventChildRemoved, *child, this);
	return child;
}

bool Node::haveChild(ofxInterface::Node *child)
{
	return find(childNodes.begin(), childNodes.end(), child) != childNodes.end();
}

void Node::sortChildren(const function<bool(const Node* a, const Node* b)>& compareFunction)
{
	std::sort(childNodes.begin(), childNodes.end(), compareFunction);
}

void Node::getSubTreeList(std::list<Node*>& list, bool returnAllGroupElements)
{
	list.push_back(this);

	if (!bRenderChildrenInGroup) {
		for (int i = 0; i < childNodes.size(); i++)
		{
			childNodes[i]->getSubTreeList(list);
		}
	}
	
}

void Node::getVisibleSubTreeList(std::list<Node*>& list, bool returnAllGroupElements)
{
	if (!getVisible()) {
		return;
	}
	
	list.push_back(this);

	if (!bRenderChildrenInGroup) {
		for (int i = 0; i < childNodes.size(); i++) {
			childNodes[i]->getVisibleSubTreeList(list);
		}
	}
}
    
void Node::getEnabledSubTreeList(std::list<Node *> &list)
{
    if (!getEnabled()) {
        return;
    }
    
    list.push_back(this);
    
    for (int i=0; i<childNodes.size(); i++) {
        childNodes[i]->getEnabledSubTreeList(list);
    }
}

void Node::placeNextTo(Node &comp, Node::Side side, float margin)
{
	switch (side)
	{
		case Node::LEFT:
			setPosition(comp.getPosition().x - getWidth() - margin,
						comp.getPosition().y,
						getPosition().z);
			break;

		case Node::TOP:
			setPosition(comp.getPosition().x,
						comp.getPosition().y - getHeight() - margin,
						getPosition().z);
			break;

		case Node::RIGHT:
			setPosition(comp.getPosition().x + comp.getWidth() + margin,
						comp.getPosition().y,
						getPosition().z);
			break;

		case Node::BOTTOM:
			setPosition(comp.getPosition().x,
						comp.getPosition().y + comp.getHeight() + margin,
						getPosition().z);
			break;
	}
}

string Node::printItem(int depth) const
{
	stringstream ss;

	for (int i=0; i<depth; i++) {
		ss << "--------";
	}
	ss << depth<<"-   "<<getName()<<endl;

	for (Node* node : childNodes) {
		ss << node->printItem(depth+1);
	}

	return ss.str();
}

ofVec3f Node::getTranslationTo(ofxInterface::Node *node)
{
	return toLocal(node->getGlobalPosition());
}

float Node::getAngleTo(ofxInterface::Node *node)
{
	ofVec3f translate = getTranslationTo(node);
	return atan2(translate.y, translate.x)*180/PI;
}

ofJson Node::getSceneDescription()
{
	ofJson ret;
	ret = getNodeJson();
	ofJson jChild;
	for (auto& child:getChildren()){
		jChild.push_back(child->getSceneDescription());
	}
	ret["children"] = jChild;
	return ret;
}

ofJson Node::getSceneDescription(vector<string> attributes,bool onlyActiveNodes)
{
	return filterSceneDescription(getSceneDescription(), attributes ,  onlyActiveNodes);
}

ofJson Node::getNodeJson()
{
	ofJson ret;
	ret["nodeType"] = "Node";
	ret["name"] = getName();
	ret["size"] = toJson(getSize());
	ret["position"] = toJson(getPosition());
	ret["orientation"] = toJson(getOrientationQuat());
	ret["scale"] = toJson(getScale());
	ret["isVisible"] = getVisible();
	ret["isEnabled"] = getEnabled();
	ret["isReceivingTouch"] = getReceivingTouch();
	ret["plane"] = getPlane();
	if (dataIsJson) {
		auto t = static_pointer_cast<ofJson>(data);
		ret["customData"] = *t;
	}
	if (childNodes.size() > 0) {
		ret["children"] = ofJson::array();
		for (auto& child : childNodes) {
			ret["children"].push_back(child->getNodeJson());
		}
	}
	
	return ret;
}

ofJson Node::getNodeJsonFiltered(set<string> keys)
{
	return filterJsonDescription(getNodeJson(),keys);
}


string Node::listActiveNodes(int depth)
{
	string ret;
	if (getEnabled()) {
		for (size_t i = 0; i < depth; i++){
			ret += " ";
		}
		ret += name + "\n";
		for (auto n : getChildren()) {
			ret += n->listActiveNodes(depth+1);
		}
	}
	return ret;
}

EffectSettings Node::getEffectSettings()
{
	return effectSettings;
}

ofJson Node::filterSceneDescription(ofJson desc, vector<string> attributes, bool onlyActiveNodes)
{
	ofJson ret;
	if (onlyActiveNodes == false || (onlyActiveNodes == true && getEnabled()) ) {
		for (auto& attr : attributes) {
			if (desc[attr] != nullptr) ret[attr] = desc[attr];
		}

		if (desc["children"] != nullptr && desc["children"].size() > 0) {
			for (auto& child : desc["children"]) {
				ret["children"].push_back(filterSceneDescription(child, attributes, onlyActiveNodes));
			}
		}
	}
	return ret;
}

ofJson Node::filterJsonDescription(ofJson desc, set<string> keys)
{
	ofJson ret = ofJson();
	bool hasChild = false;
	for (ofJson::iterator it = desc.begin(); it != desc.end(); ++it) {
		if (keys.find(it.key()) != keys.end()) {
			if (it.key() == "children") {
				hasChild = true;
			}
			else {
				ret[it.key()] = it.value();
			}
		}
	}

	if (hasChild) {
		ret["children"] = ofJson::array();
		for (auto& child : desc["children"]) {
			ret["children"].push_back(filterJsonDescription(child, keys));
		}
	}

	return ret;
}

} // namespace

