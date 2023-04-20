#pragma once

#include "ofMain.h"

#define STRINGIFY(A) #A

namespace ofxInterface {
		const ofShader getGradientShader3Colors();
		const ofShader getGradientShader2Colors();
		const ofShader getGradientCircularMaskShader();
		const ofVec4f colorToVec4(ofColor c);
}