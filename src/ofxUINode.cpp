//
//  ofxUINode.cpp
//
//
//  Created by Gal Sasson on 7/24/14.
//
//

#include "ofxUINode.h"

ofxUINode::~ofxUINode()
{
	ofNotifyEvent(eventDestroy);

	for (int i=0; i<children.size(); i++) {
		delete children[i];
	}
}

ofxUINode::ofxUINode()
{
	plane = 0;
	size.set(0, 0);

	bVisible = true;
	bEnabled = true;
	data = NULL;

#ifdef OFXUINODE_DEBUG
	debugBorderColor = ofColor(200, 200, 0);
	historyPlot = NULL;
#endif
}

void ofxUINode::drawDebug()
{
#ifdef OFXUINODE_DEBUG
	// draw border
	ofSetLineWidth(1);
	ofNoFill();
	ofSetColor(debugBorderColor);

	ofDrawRectangle(0, 0, getWidth(), getHeight());
	debugBorderColor = debugBorderColor.lerp(ofColor(ofColor(200, 200, 0)), 0.05);

	// draw anchor
	ofSetColor(255, 0, 0);
	ofVec2f anchorPixels = anchor*size;
	ofLine(anchorPixels.x-2, anchorPixels.y, anchorPixels.x+2, anchorPixels.y);
	ofLine(anchorPixels.x, anchorPixels.y-2, anchorPixels.x, anchorPixels.y+2);

	if (historyPlot != NULL) {
		ofFill();
		historyPlot->draw(0, getHeight(), 100, 50);
	}

	ofSetColor(0, 0, 0, 50);
	ofFill();
	ofDrawRectangle(0, -12, 200, 12);
	ofSetColor(255, 0, 0);
	stringstream ss;
	ss<<name<<": "<<getGlobalPlane()<<"-"<<bVisible;
	ofDrawBitmapString(ss.str(), 0, 0);
#endif
}


void ofxUINode::render(bool forceAll)
{
//	ofPushMatrix();
//	ofMultMatrix(getGlobalTransformMatrix().getInverse());

	std::list<ofxUINode*> sortedNodes;
	std::list<ofxUINode*>::iterator it;

	if (forceAll) {
		// get all nodes (visible and invisible)
		getSubTreeList(sortedNodes);
	}
	else {
		// get only visible nodes
		getVisibleSubTreeList(sortedNodes);
	}

	// sort scene by z (+z goes outside of the screen), plane is z
	sortedNodes.sort(ofxUINode::bottomPlaneFirst);

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

//	ofPopMatrix();
}



void ofxUINode::renderDebug()
{
//	ofPushMatrix();
//	ofMultMatrix(getGlobalTransformMatrix().getInverse());

	// sort scene by z (+z goes outside of the screen), plane is z
	std::list<ofxUINode*> sortedNodes;
	std::list<ofxUINode*>::iterator it;

	getSubTreeList(sortedNodes);
	sortedNodes.sort(ofxUINode::bottomPlaneFirst);

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

//	ofPopMatrix();
}





void ofxUINode::touchDown(int id,  TouchEvent* event)
{
#ifdef OFXUINODE_DEBUG
	debugBorderColor = ofColor(0, 128, 255);

	historyPlot = new ofxHistoryPlot(&event->velocitySmoothed.y, "currentFrameRate auto updated", 500, true);
	historyPlot->setBackgroundColor(ofColor(0, 0, 0, 255));
	historyPlot->setColor( ofColor(0,255,0) );
	historyPlot->setShowNumericalInfo(false);
//	historyPlot->setRespectBorders(true);
	historyPlot->setLineWidth(1);
#endif
	ofNotifyEvent(eventTouchDown, *event);
}

void ofxUINode::touchMove(int id,  TouchEvent* event)
{
#ifdef OFXUINODE_DEBUG
	debugBorderColor = ofColor(255, 128, 0);
#endif

	ofNotifyEvent(eventTouchMove, *event);
}

void ofxUINode::touchUp(int id,  TouchEvent* event)
{
#ifdef OFXUINODE_DEBUG
	debugBorderColor = ofColor(0, 255, 128);
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

void ofxUINode::touchExit(int id, TouchEvent *event)
{
#ifdef OFXUINODE_DEBUG
	debugBorderColor = ofColor(128, 0, 255);
#endif
	ofNotifyEvent(eventTouchExit, *event);
}

void ofxUINode::touchEnter(int id, TouchEvent *event)
{
#ifdef OFXUINODE_DEBUG
	debugBorderColor = ofColor(255, 0, 128);
#endif
	ofNotifyEvent(eventTouchEnter, *event);
}

ofVec2f ofxUINode::toLocal(const ofVec2f &screenPoint)
{
	return (ofVec3f)screenPoint*ofNode::getGlobalTransformMatrix().getInverse();
}

bool ofxUINode::contains(const ofVec2f &p)
{
	ofVec2f local = toLocal(p);

	if (local.x < 0 ||
		local.y < 0 ||
		local.x > size.x ||
		local.y > size.y) {
		return false;
	}

	return true;
}



void ofxUINode::addChild(ofxUINode *child, int insertAt)
{
	child->setParent(*this);

	if (insertAt == -1) {
		// append
		children.push_back(child);
	}
	else {
		children.insert(children.begin()+insertAt, child);
	}
}

ofxUINode* ofxUINode::removeChild(ofxUINode *child)
{
	for (int i=0; i<children.size(); i++)
	{
		if (children[i] == child) {
			return removeChild(i);
		}
	}

	ofLogWarning("ofxUINode", "are you trying to remove a child that does not exist?");
	return NULL;
}

ofxUINode* ofxUINode::removeChild(int index)
{
	if (index >= children.size()) {
		ofLogWarning("ofxUINode", "are you trying to remove a child that does not exist?");
		return NULL;
	}

	ofxUINode *child = children[index];
	children.erase(children.begin()+index);
	child->clearParent();
	return child;
}


ofxUINode* ofxUINode::getChildWithName(const std::string &name, bool deepSearch)
{
	ofxUINode* child = NULL;

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

void ofxUINode::getSubTreeList(std::list<ofxUINode*>& list)
{
	list.push_back(this);

	for (int i=0; i<children.size(); i++)
	{
		children[i]->getSubTreeList(list);
	}
}

void ofxUINode::getVisibleSubTreeList(std::list<ofxUINode*>& list)
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

void ofxUINode::placeNextTo(const ofxUINode &comp, ofxUINode::Side side, float margin)
{
	switch (side)
	{
		case ofxUINode::LEFT:
			setPosition(comp.getPosition().x - getWidth() - margin,
						comp.getPosition().y,
						getPosition().z);
			break;

		case ofxUINode::TOP:
			setPosition(comp.getPosition().x,
						comp.getPosition().y - getHeight() - margin,
						getPosition().z);
			break;

		case ofxUINode::RIGHT:
			setPosition(comp.getPosition().x + comp.getWidth() + margin,
						comp.getPosition().y,
						getPosition().z);
			break;

		case ofxUINode::BOTTOM:
			setPosition(comp.getPosition().x,
						comp.getPosition().y + comp.getHeight() + margin,
						getPosition().z);
			break;
	}
}

