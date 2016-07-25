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
	if (parent != NULL) {
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
	data = NULL;

	bSendDestroy = true;
	bNodeAllowOneTouch = false;
	bNodeUpdateWhenHidden = false;
	bNodeTouched = false;
	nodeCurrentTouchId = -1;	// not relevant
#ifdef OLDSORT
    sameDepthOffset = ofRandom(0, 1);
#endif

#ifdef OFXUINODE_DEBUG
	debugBorderColor = defaultNodeColor;
#endif

#ifdef USE_OFX_HISTORY_PLOT
	historyPlot = NULL;
#endif
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
        return NULL;
    }

    for (int i=0; i<childNodes.size(); i++)
    {
        Node* node = childNodes[i]->getChildWithName(searchName, searchDepth-1);
        if (node != NULL) {
            return node;
        }
    }

    return NULL;
}

Node* Node::getParentWithName(const std::string &searchName, int searchDepth) const
{
	if (searchName == name) {
		return (Node*)this;
	}

	if (searchDepth==0) {
		return NULL;
	}

	if (parent == NULL) {
		return NULL;
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
	if (historyPlot != NULL) {
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
	if (getParent() != NULL) {
		return bClipRender || ((Node*)getParent())->getGlobalRenderClip();
	}
	else {
		return bClipRender;
	}
}

ofRectangle Node::getRenderClipRect()
{
	ofRectangle parentRect;
	if (getParent() != NULL) {
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
	glScissor(local.x, local.y, localScale.x, localScale.y);
}

void Node::disableScissor()
{
	glDisable(GL_SCISSOR_TEST);
}

void Node::setCenteredH()
{
    if (parent == NULL) {
        return;
    }

    setX((((Node*)parent)->getLocalWidth() - getLocalWidth())/2);
}

void Node::setCenteredV()
{
    if (parent == NULL) {
        return;
    }

    setY((((Node*)parent)->getLocalHeight() - getLocalHeight())/2);
}

void Node::setCentered()
{
    if (parent == NULL) {
        return;
    }

    setPosition((((Node*)parent)->getLocalWidth() - getLocalWidth())/2,
                (((Node*)parent)->getLocalHeight() - getLocalHeight())/2);
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
		historyPlot = NULL;
	}
#endif

	ofNotifyEvent(eventTouchUp, *event);
	if (contains(event->position)) {
		ofNotifyEvent(eventClick, *event);
	}

	bNodeTouched = false;
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

ofVec3f Node::toLocal(const ofVec3f &screenPoint)
{
	#ifdef GLM_SWIZZLE //this version of OF is using GLM for math ops
	return (ofVec3f)screenPoint * glm::inverse(ofNode::getGlobalTransformMatrix());
	#else
	return (ofVec3f)screenPoint*ofNode::getGlobalTransformMatrix().inverse();
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
    if (parent == NULL) {
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
    if (parent == NULL) {
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
	return NULL;
}

Node* Node::removeChild(int index, bool bMaintainChildGlobalTransform)
{
	if (index >= childNodes.size()) {
		ofLogWarning("ofxInterface::Node::removeChild", "are you trying to remove a child that does not exist?");
		return NULL;
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

void Node::getSubTreeList(std::list<Node*>& list)
{
	list.push_back(this);

	for (int i=0; i<childNodes.size(); i++)
	{
		childNodes[i]->getSubTreeList(list);
	}
}

void Node::getVisibleSubTreeList(std::list<Node*>& list)
{
	if (!getVisible()) {
		return;
	}
	
	list.push_back(this);

	for (int i=0; i<childNodes.size(); i++) {
		childNodes[i]->getVisibleSubTreeList(list);
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

string Node::print(int depth) const
{
	stringstream ss;

	for (int i=0; i<depth; i++) {
		ss << "--------";
	}
	ss << depth<<"-   "<<getName()<<endl;

	for (Node* node : childNodes) {
		ss << node->print(depth+1);
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

} // namespace

