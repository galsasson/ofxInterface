#pragma once
#include "Node.h"
#include "ofxFontStash2.h"
#include "ofxAnimatableOfPoint.h"
#include "ofxEasing.h"
#include "ColorPanel.h"

namespace ofxInterface {
    
	struct ScrollableContainerSettings : NodeSettings{
		ofVec2f sizeScrollableArea;
		float plane = 0; // option to set scroll option above elements

		ofColor bgColor = ofColor(0, 0);
		ofColor scrollActiveColor = ofColor(255);
		ofColor scrollInactiveColor = ofColor(128);

	};

	class ScrollableContainer :
		public Node
	{
	public:
		ScrollableContainer();
		~ScrollableContainer();

		virtual void setup(ScrollableContainerSettings s);

        virtual void draw() override;

		Node* getScrollableArea();
		//void addChildToScrollableArea(Node* child);

		virtual void onTouchDown(TouchEvent & event);
		virtual void onTouchUp(TouchEvent & event);
		virtual void onTouchMove(TouchEvent & event);
		
		void onScrollAreaSizeChanged(ofxInterface::Node & n);

		void onChildAdded(ofxInterface::Node& n);
		void onChildRemoved(ofxInterface::Node& n);
		void fitSizeToItems();

	protected:
		Node* containerRemoveChild(Node *child, bool bMaintainChildGlobalTransform = false);
		Node* containerRemoveChild(int index, bool bMaintainChildGlobalTransform = false);
		void updateChildItemSize(Node* child, int& w, int& h);
		void updateChildListeners(Node* child);

		void updateSlider();

    private:
		Node* scrollableArea;
		Node* dragArea;
		map<string, ColorPanel*> slider;
		ofVec2f pStartDragCursor;
		ofVec2f pStartDrag;
		ofVec2f baseSize;

		ofColor bgColor;
		ofColor scrollActiveColor;
		ofColor scrollInactiveColor;

		/// \brief rollout animation on touch up
		ofxAnimatableOfPoint edgeAnimation;
		ofVec2f currentVelocity;
		ofVec2f finalPoint;
		bool animationReady = false;
		const float friction = 0.94;
		bool isAutoScroll = false;

		/// \brief clamp of velocity animation sliding
		const int maxVelocity = 1000;

		/// \brief in s
		const float maxAnimationLength = 2.3f;

		
		
	};
}

