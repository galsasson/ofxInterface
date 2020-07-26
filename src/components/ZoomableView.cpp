//
//  ZoomableView.cpp
//  shapes
//
//  Created by Gal Sasson on 26/07/2020.
//

#include "ZoomableView.h"

namespace ofxInterface
{
	ZoomableView::ZoomableView()
	{
		zoomable.scale = 1;
		zoomable.topLeft = ofDefaultVec2(0,0);
	}
	
	void ZoomableView::zoomOnRect(const ofRectangle &rect, float duration)
	{
		float targetScale = fmin(getWidth() / rect.getWidth(), getHeight() / rect.getHeight());
		ofDefaultVec2 targetTopLeft = ofDefaultVec2(0,0) - (ofDefaultVec2)rect.getTopLeft()*targetScale;
		animateFloat(&zoomable.scale, targetScale, duration);
		animateFloat(&zoomable.topLeft.x, targetTopLeft.x, duration);
		animateFloat(&zoomable.topLeft.y, targetTopLeft.y, duration);
	}
	
	void ZoomableView::zoomOnPoint(const ofDefaultVec2& p, float scale, float duration)
	{
		float targetScale = scale;
		ofDefaultVec2 targetTopLeft = -(p*scale) + 0.5*getSize();
		animateFloat(&zoomable.scale, targetScale, duration);
		animateFloat(&zoomable.topLeft.x, targetTopLeft.x, duration);
		animateFloat(&zoomable.topLeft.y, targetTopLeft.y, duration);
	}
	
	void ZoomableView::update(float dt)
	{
		AnimatableNode::update(dt);
		setScale(zoomable.scale);
		setPosition(zoomable.topLeft);
	}

} // namespace ofxInterface
