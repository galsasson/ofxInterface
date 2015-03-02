//
//  Node.cpp
//
//
//  Created by Gal Sasson on 7/24/14.
//
//

#include "Node.h"

namespace ofxInterface {

Node::~Node()
{
	if (parent != NULL) {
		((Node*)parent)->removeChild(this);
	}

	if (bSendDestroy) {
		ofNotifyEvent(eventDestroy);
	}

	for (int i=0; i<children.size(); i++) {
		delete children[i];
	}
}

Node::Node()
{
	plane = 0;
	size.set(0, 0);

	bVisible = true;
	bEnabled = true;
	data = NULL;

	bSendDestroy = true;

#ifdef OFXUINODE_DEBUG
	debugBorderColor = ofColor(200, 200, 0);
#endif

#ifdef USE_OFX_HISTORY_PLOT
	historyPlot = NULL;
#endif
}

Node* Node::getNodeWithName(const std::string &searchName, int searchDepth) const
{
    if (searchName == name) {
        return (Node*)this;
    }

    if (searchDepth==0) {
        return NULL;
    }

    for (int i=0; i<children.size(); i++)
    {
        Node* node = children[i]->getNodeWithName(searchName, searchDepth-1);
        if (node != NULL) {
            return node;
        }
    }

    return NULL;
}



void Node::drawDebug()
{
	// draw border
	ofSetLineWidth(1);
	ofNoFill();
	ofSetColor(debugBorderColor);

	drawBounds();
	debugBorderColor = debugBorderColor.lerp(ofColor(ofColor(200, 200, 0)), 0.05);

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

		(*it)->draw();

		ofPopMatrix();
		ofPopStyle();
	}
}



void Node::renderDebug()
{
	// sort scene by z (+z goes outside of the screen), plane is z
	std::list<Node*> sortedNodes;
	std::list<Node*>::iterator it;

	getSubTreeList(sortedNodes);
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
    std::list<Node*> nodes;
    std::list<Node*>::iterator it;
    
    if (forceAll) {
        // get all nodes (visible and invisible)
        getSubTreeList(nodes);
    }
    else {
        // get only visible nodes
        getVisibleSubTreeList(nodes);
    }
    
    for (it = nodes.begin(); it != nodes.end(); it++)
    {
        (*it)->update(dt);
    }
}



void Node::touchDown(int id,  TouchEvent* event)
{
#ifdef OFXUINODE_DEBUG
	debugBorderColor = ofColor(0, 128, 255);
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
#ifdef OFXUINODE_DEBUG
	debugBorderColor.a -= 10;
#endif

	ofNotifyEvent(eventTouchMove, *event);
}

void Node::touchUp(int id,  TouchEvent* event)
{
#ifdef OFXUINODE_DEBUG
	debugBorderColor = ofColor(0, 255, 128);
#endif

#ifdef USE_OFX_HISTORY_PLOT
	if (historyPlot) {
		delete historyPlot;
		historyPlot = NULL;
	}
#endif

	ofNotifyEvent(eventTouchUp, *event);
	if (!event->bHasMoved) {
		// click is considered only when there is not movement
		ofNotifyEvent(eventClick, *event);
	}
}

void Node::touchExit(int id, TouchEvent *event)
{
#ifdef OFXUINODE_DEBUG
	debugBorderColor = ofColor(128, 0, 255);
#endif
	ofNotifyEvent(eventTouchExit, *event);
}

void Node::touchEnter(int id, TouchEvent *event)
{
#ifdef OFXUINODE_DEBUG
	debugBorderColor = ofColor(255, 0, 128);
#endif
	ofNotifyEvent(eventTouchEnter, *event);
}

ofVec3f Node::toLocal(const ofVec3f &screenPoint)
{
	return (ofVec3f)screenPoint*ofNode::getGlobalTransformMatrix().getInverse();
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


void Node::addChild(Node *child, int insertAt)
{
	child->setParent(*this);

	if (insertAt == -1 || insertAt > children.size()) {
		// append
		children.push_back(child);
	}
	else {
		children.insert(children.begin()+insertAt, child);
	}
}

Node* Node::removeChild(Node *child)
{
	for (int i=0; i<children.size(); i++)
	{
		if (children[i] == child) {
			return removeChild(i);
		}
	}

	ofLogWarning("ofxInterface::Node::removeChild", "are you trying to remove a child that does not exist?");
	return NULL;
}

Node* Node::removeChild(int index)
{
	if (index >= children.size()) {
		ofLogWarning("ofxInterface::Node::removeChild", "are you trying to remove a child that does not exist?");
		return NULL;
	}

	Node *child = children[index];
	children.erase(children.begin()+index);
	child->clearParent();
	return child;
}


void Node::getSubTreeList(std::list<Node*>& list)
{
	list.push_back(this);

	for (int i=0; i<children.size(); i++)
	{
		children[i]->getSubTreeList(list);
	}
}

void Node::getVisibleSubTreeList(std::list<Node*>& list)
{
	if (!getVisible()) {
		return;
	}
	
	list.push_back(this);

	for (int i=0; i<children.size(); i++) {
		children[i]->getVisibleSubTreeList(list);
	}
}
    
void Node::getEnabledSubTreeList(std::list<Node *> &list)
{
    if (!getEnabled()) {
        return;
    }
    
    list.push_back(this);
    
    for (int i=0; i<children.size(); i++) {
        children[i]->getEnabledSubTreeList(list);
    }
}

void Node::placeNextTo(const Node &comp, Node::Side side, float margin)
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

} // namespace

