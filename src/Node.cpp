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
    
    if (forceAll) {
        // get all nodes (visible and invisible)
        getSubTreeList(nodes);
    }
    else {
        // get only visible nodes
        getVisibleSubTreeList(nodes);
    }
    
    for (Node* node : nodes)
    {
        node->update(dt);
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

ofVec2f Node::toLocal(const ofVec2f &screenPoint)
{
	return (ofVec3f)screenPoint*ofNode::getGlobalTransformMatrix().getInverse();
}

bool Node::contains(const ofVec2f &globalPoint)
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


Node* Node::getChildWithName(const std::string &name, bool deepSearch) const
{
	Node* child = NULL;

	for (int i=0; i<children.size(); i++)
	{
		if (children[i]->getName() == name) {
			return children[i];
		}
		else if (deepSearch) {
			child = children[i]->getChildWithName(name, deepSearch);
		}
	}

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

	for (int i=0; i<children.size(); i++)
	{
		children[i]->getVisibleSubTreeList(list);
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

