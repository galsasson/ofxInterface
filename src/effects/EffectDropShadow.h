#pragma once

#include "ofMain.h"
#include "ofShader.h"
#include "EffectDropShadowShader.h"

namespace ofxInterface
{

	struct EffectDropShadowSettings {
		ofColor color = ofColor(0);
		ofVec2f position = ofVec2f(0);
		float size = 0;
		bool isDynamic = false; // todo;
	};

	class EffectDropShadow
	{
	public:
		EffectDropShadow();
		~EffectDropShadow();

		void setup(EffectDropShadowSettings s);
		void update(ofFbo in);
		ofFbo getShadow();

	private:
		void checkAllocated(ofFbo& fbo, int width, int height);
		void generateShadow(ofFbo in, ofFbo buffer, ofFbo& out, EffectDropShadowSettings settings);

		ofShader shaderShadow;
		EffectDropShadowShader shaderDef;

		ofFbo fboBuffer;
		ofFbo fboShadow;
		EffectDropShadowSettings settings;
	};

}