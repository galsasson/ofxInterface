//
//  AnimatableNode.h
//  maccabi-wall
//
//  Created by Gal Sasson on 7/24/17.
//
//

#ifndef AnimatableNode_h
#define AnimatableNode_h

#include "ofMain.h"
#include "ofxInterface.h"

namespace ofxInterface {

class Curve
{
	// http://st-on-it.blogspot.com/2011/05/calculating-cubic-bezier-function.html
	class Bezier
	{
	public:
		Bezier(float cp1x=0, float cp1y=0, float cp2x=0, float cp2y=0) {
			cx = 3 * cp1x;
			bx = 3 * (cp2x - cp1x) - cx;
			ax = 1 - cx - bx;
			
			cy = 3 * cp1y;
			by = 3 * (cp2y - cp1y) - cy;
			ay = 1 - cy - by;
		};

		float y_for_x(float x) {
			return y_for_t(t_for_x(x));
		}

	private:
		float cx,bx,ax;
		float cy,by,ay;
		
		float x_for_t(float t) { return t * (cx + t * (bx + t * ax)); }
		float x_for_t_der(float t) { return cx + t * (2*bx + 3*ax * t); }
		float y_for_t(float t) { return t * (cy + t * (by + t * ay)); }
		// using Newton's method to aproximate the parametric value of x for t
		float t_for_x(float x) {
			float t=x;
			float z;
			int i=0;
			while (i<5) {
				z = x_for_t(t) - x;
				if (abs(z) < 1e-3) break; // if already got close enough
				t = t - z/x_for_t_der(t);
				i++;
			}
			return t;
		};
	};
	
public:
	enum Type {
		EASE_IN_EASE_OUT = 0,
		EASE_IN,
		EASE_OUT,
		LINEAR,
		TANH,
		SINH,
		SQUARE,
		QUADRATIC_EASE_IN,
		QUADRATIC_EASE_OUT,
		CUBIC_BEZIER_PARAM,
	} type;
	Bezier bezier;
	Curve(Type t = EASE_IN_EASE_OUT,float p0=0, float p1=0, float p2=0, float p3=0) {
		type = t;
		if (type==CUBIC_BEZIER_PARAM) {
			bezier = Bezier(p0, p1, p2, p3);
		}
	}
	Curve(float p[4]) : Curve(CUBIC_BEZIER_PARAM, p[0], p[1], p[2], p[3]) {}
	float calcAt(float t) {
		switch ( type ) {
				
			case EASE_IN_EASE_OUT: return(0.5f - 0.5f * cosf( PI * t ));
			case EASE_IN: return 1.0f + sinf( HALF_PI * t - HALF_PI);
			case EASE_OUT: return sinf( HALF_PI * t );
			case LINEAR: return t;
			case TANH: return 0.5f + 0.5f * tanh( 2.0f * PI * t - PI ) * 1.00374187319732; break;
			case SINH: return 0.5f + 0.23482f * sinh( 3.0f * t - 1.5f );
			case SQUARE: return (t < 0.5f) ? 0.0f : 1.0f;
			case QUADRATIC_EASE_IN: return t * t;
			case QUADRATIC_EASE_OUT: return 1.0f - (t - 1.0f) * (t - 1.0f);
			case CUBIC_BEZIER_PARAM: return bezier.y_for_x(t);
		}
		
		return t;
	}
};

class AnimatableNode : public Node
{
public:
	virtual ~AnimatableNode() {};
    AnimatableNode();
	void animatePositionTo(const ofVec3f& p, float duration, Curve curve = Curve(Curve::Type::EASE_IN_EASE_OUT), float delay=0, const std::function<void(void)>& onEndFunc=[](){});
	void stopPositionAnimation();
    void animateColor(ofColor* c, const ofColor& target, float duration, Curve curve = Curve(Curve::Type::EASE_IN_EASE_OUT), float delay=0, const std::function<void(void)>& onEndFunc=[](){});
	void animateFloat(float* val, float target, float duration, Curve curve=Curve(Curve::Type::EASE_IN_EASE_OUT), float delay=0, const std::function<void(void)>& onEndFunc=[](){});
    virtual void update(float dt) override;
    
    ofColor nodeColor;
    ofColor nodeBGColor;
    
private:
    struct posanim_t {
        ofVec3f source;
        ofVec3f target;
        float duration;
        Curve curve;
        float delay;
        float progress;
        bool bActive;
		std::function<void(void)> onEnd;
    };
    
    struct coloranim_t {
        ofColor* color;
        ofColor source;
        ofColor target;
        float duration;
        Curve curve;
        float delay;
        float progress;
		std::function<void(void)> onEnd;
    };

    struct floatanim_t {
        float* value;
        float source;
        float target;
        float duration;
        Curve curve;
        float delay;
        float progress;
		std::function<void(void)> onEnd;
    };

    posanim_t nodePositionAnimation;
    map<ofColor*, coloranim_t> nodeColorAnimations;
    map<float*, floatanim_t> nodeFloatAnimations;
};

}
#endif /* AnimatableNode_h */
