//
//  ZoomableView.h
//  shapes
//
//  Created by Gal Sasson on 26/07/2020.
//

#ifndef ZoomableView_h
#define ZoomableView_h

#include "AnimatableNode.h"

namespace ofxInterface
{
	class ZoomableView : public AnimatableNode
	{
	public:
		ZoomableView();
		void zoomOnRect(const ofRectangle& rect, float duration=1);
		void zoomOnPoint(const ofDefaultVec2& p, float scale, float duration=1);
		void update(float dt);
		
	private:
		struct zoomable_t {
			float scale;
			ofDefaultVec2 topLeft;
		} zoomable;
	};
	
	
} // namespace ofxInterface


#endif /* ZoomableView_h */
