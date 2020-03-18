#include "ScrollableContainer.h"


namespace ofxInterface {
	ScrollableContainer::ScrollableContainer():Node()
	{
	}


	ScrollableContainer::~ScrollableContainer()
	{
	}

	void ScrollableContainer::setup(ScrollableContainerSettings s) {
		Node::setup(s);
		setRenderClip(true);
		setTouchClip(true);
		
		scrollableArea = new Node();
		scrollableArea->setName("scrollableArea");
		scrollableArea->setSize(s.sizeScrollableArea);
		addChild(scrollableArea);
		
		bgColor = s.bgColor;
		scrollActiveColor = s.scrollActiveColor;
		scrollInactiveColor = s.scrollInactiveColor;

		ofAddListener(eventChildAdded, this, &ScrollableContainer::onChildAdded);
		ofAddListener(scrollableArea->eventTouchMove, this, &ScrollableContainer::onTouchMove);
		ofAddListener(scrollableArea->eventTouchUp, this, &ScrollableContainer::onTouchUp);
		ofAddListener(scrollableArea->eventTouchDown, this, &ScrollableContainer::onTouchDown);

		edgeAnimation.setCurve(AnimCurve::SWIFT_GOOGLE);
		edgeAnimation.setAnimFinishedLambda([this]() {
			if (animationReady) {
				edgeAnimation.animateTo(finalPoint);
				animationReady = false;
			}
		});
	}


	void ScrollableContainer::draw()
	{
		// animate elements if scrolled on edge
		if (edgeAnimation.isAnimating()) {
			edgeAnimation.update(ofGetLastFrameTime());
			scrollableArea->setPosition(edgeAnimation.getCurrentPosition());
		
		// animate elements right after touchUp
		}
		if (isAutoScroll) {
			currentVelocity *= friction;

			ofVec2f newPos = ofVec2f(scrollableArea->getPosition()) + currentVelocity* ofGetLastFrameTime();

			newPos.x = ofClamp(newPos.x, getWidth() - scrollableArea->getWidth(), 0);
			newPos.y = ofClamp(newPos.y, getHeight() - scrollableArea->getHeight(), 0);

			scrollableArea->setPosition(newPos);

			if (abs(currentVelocity.x) < 1 && abs(currentVelocity.y) < 1) {
				isAutoScroll = false;
			}
			else if( ((newPos.x == 0 || newPos.x == getWidth() - scrollableArea->getWidth()) &&
				(newPos.y == 0 || newPos.y == getHeight() - scrollableArea->getHeight()))) {
				isAutoScroll = false;

				edgeAnimation.setPosition(scrollableArea->getPosition());
				edgeAnimation.setDuration(0.15);

				ofVec2f d;
				d.x = currentVelocity.x < 0 ? -50 : 50;
				d.y = currentVelocity.y < 0 ? -50 : 50;
				if (getWidth() == scrollableArea->getWidth()) {
					d.x = 0;
				}
				if (getHeight() == scrollableArea->getHeight()) {
					d.y = 0;
				}

				edgeAnimation.animateTo(ofVec2f(scrollableArea->getPosition()) + d);
				finalPoint = scrollableArea->getPosition();
				animationReady = false;
			}
		}

		ofSetColor(bgColor);
		ofDrawRectangle(0, 0, getWidth(), getHeight());

		// draw slider
		float wSlider = 6;
		float marginSlider = 20;

		if (scrollableArea->getWidth() > getWidth()) {
			ofVec2f d0 = ofVec2f(marginSlider, getHeight() - marginSlider - wSlider);
			ofSetColor(scrollInactiveColor);
			float width = getWidth() - 3 * marginSlider;
			ofDrawRectangle(d0, width, wSlider);

			ofSetColor(scrollActiveColor);
			float wActive = width * getWidth() / scrollableArea->getWidth();
			if (edgeAnimation.isAnimating()) {
				wActive -= (1.0 - edgeAnimation.getPercentDone()) * 25;
			}
			float pos = (width - wActive) - ofMap(scrollableArea->getPosition().x, getWidth() - scrollableArea->getWidth(), 0, 0, width - wActive, true);
			ofDrawRectangle(d0 + ofVec2f(pos,0), wActive, wSlider);
		}

		if (scrollableArea->getHeight() > getHeight()) {
			ofVec2f d0 = ofVec2f(getWidth() - marginSlider - wSlider, marginSlider);
			ofSetColor(scrollInactiveColor);
			float height = getHeight() - 2 * marginSlider;
			ofDrawRectangle(d0, wSlider, height);

			ofSetColor(scrollActiveColor);
			float hActive = height * getHeight() / scrollableArea->getHeight();
			if (edgeAnimation.isAnimating()) {
				hActive -= (1.0 - edgeAnimation.getPercentDone()) * 25;
			}
			float pos = (height - hActive) - ofMap(scrollableArea->getPosition().y, getHeight() - scrollableArea->getHeight(),0, 0, height-hActive,true);
			ofDrawRectangle(d0 +ofVec2f(0,pos), wSlider, hActive);
		}

	}


	Node * ScrollableContainer::getScrollableArea() {
		return scrollableArea;
	}

	void ScrollableContainer::addChildToScrollableArea(Node * child) {
		// enable clipping for children
		child->setRenderClip(true);
		scrollableArea->addChild(child);
	}

	void ScrollableContainer::onTouchDown(TouchEvent & event) {
		edgeAnimation.reset();
	}

	void ScrollableContainer::onTouchUp(TouchEvent & event) {
		// use the event velocity to simulate an impulse
		float dMaxImpulse = 1000;
		ofVec3f velocityModifier;

		velocityModifier.x = ofxeasing::map_clamp(abs(event.velocitySmoothed.x), 100, maxVelocity, 0, dMaxImpulse, &ofxeasing::exp::easeOut);
		if (event.velocitySmoothed.x < 0) {
			velocityModifier.x *= -1;
		}
		velocityModifier.y = ofxeasing::map_clamp(abs(event.velocitySmoothed.y), 100, maxVelocity, 0, dMaxImpulse, &ofxeasing::exp::easeOut);
		if (event.velocitySmoothed.y < 0) {
			velocityModifier.y *= -1;
		}
		
		ofVec2f newPos = scrollableArea->getPosition() + velocityModifier;

		newPos.x = ofClamp(newPos.x, getWidth() - scrollableArea->getWidth(), 0);
		newPos.y = ofClamp(newPos.y, getHeight() - scrollableArea->getHeight(), 0);

		currentVelocity = velocityModifier;
		isAutoScroll = true;

	}

	void ScrollableContainer::onTouchMove(TouchEvent & event) {

		// calculate movement frome last event
		ofVec3f d = event.position - event.prevPosition;
		ofVec3f newPos = scrollableArea->getPosition() + d;
		
		// clamp the position
		newPos.x = ofClamp(newPos.x, getWidth() - scrollableArea->getWidth(),0);
		newPos.y = ofClamp(newPos.y, getHeight() - scrollableArea->getHeight(),0);

		scrollableArea->setPosition(newPos);
	}

	void ScrollableContainer::onScrollAreaSizeChanged(ofxInterface::Node & n) {
		dragArea->setSize(n.getSize());
	}

	///\brief moves child to scrollable Area
	void ScrollableContainer::onChildAdded(ofxInterface::Node & n) {
		containerRemoveChild(&n);
		scrollableArea->addChild(&n);
		ofAddListener(n.eventTouchMove, this, &ScrollableContainer::onTouchMove);
		ofAddListener(n.eventTouchUp, this, &ScrollableContainer::onTouchUp);
		ofAddListener(n.eventTouchDown, this, &ScrollableContainer::onTouchDown);
	}

	Node * ScrollableContainer::containerRemoveChild(Node * child, bool bMaintainChildGlobalTransform)
	{
		for (int i = 0; i < childNodes.size(); i++)
		{
			if (childNodes[i] == child) {
				return containerRemoveChild(i, bMaintainChildGlobalTransform);
			}
		}

		ofLogWarning("ofxInterface::Node::removeChild", "are you trying to remove a child that does not exist?");
		return NULL;
	}

	Node * ScrollableContainer::containerRemoveChild(int index, bool bMaintainChildGlobalTransform)
	{
		if (index >= childNodes.size()) {
			ofLogWarning("ofxInterface::Node::removeChild", "are you trying to remove a child that does not exist?");
			return NULL;
		}

		Node *child = childNodes[index];
		childNodes.erase(childNodes.begin() + index);
		child->clearParent(bMaintainChildGlobalTransform);
		//ofNotifyEvent(eventChildRemoved, *child, this);
		return child;
	}


}