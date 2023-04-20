#include "EffectDropShadow.h"


namespace ofxInterface
{
	EffectDropShadow::EffectDropShadow()
	{
		//shaderShadow.load("shaders/blur");
		if (ofIsGLProgrammableRenderer()) {
			ofLogVerbose("EffectDropShadow.loadShaders()") << "Programmable renderer detected.";
			ofLogVerbose("EffectDropShadow.loadShaders()") << "Loading shader......";
			shaderShadow.setupShaderFromSource(GL_VERTEX_SHADER, shaderDef.vertex);
			shaderShadow.setupShaderFromSource(GL_FRAGMENT_SHADER, shaderDef.fragment);
			shaderShadow.bindDefaults();
			shaderShadow.linkProgram();
		}
		else {
			ofLogError("EffectDropShadow", "DropShadow only available for glsl >= ver.3");
		}
	}


	EffectDropShadow::~EffectDropShadow()
	{
	}

	void EffectDropShadow::setup(EffectDropShadowSettings s)
	{
		settings = s;
	}

	void EffectDropShadow::update(ofFbo in)
	{
		// only update if size > 0
		if (settings.size > 0) {
			int wFbo = in.getWidth() + settings.size * 2;
			int hFbo = in.getHeight() + settings.size * 2;

			// allocate new fbo
			if (!fboBuffer.isAllocated() ||
				(settings.isDynamic &&
					(wFbo != fboBuffer.getWidth() ||
						hFbo != fboBuffer.getHeight())
					)
				) {
				fboBuffer.allocate(wFbo, hFbo);
				fboShadow.allocate(wFbo, hFbo);

				generateShadow(in,fboBuffer, fboShadow, settings);
			}

			// update filter
			if ((settings.isDynamic &&
				(wFbo != fboBuffer.getWidth() ||
					hFbo != fboBuffer.getHeight())
				)
				) {
				generateShadow(in,fboBuffer, fboShadow, settings);
			}
		}
	}

	ofFbo EffectDropShadow::getShadow()
	{
		return fboShadow;
	}


	void EffectDropShadow::checkAllocated(ofFbo& fbo, int width, int height)
	{
		if (!fbo.isAllocated() || fbo.getWidth() != width || fbo.getHeight() != height) {
			fbo.allocate(width, height);
		}
	}
	void EffectDropShadow::generateShadow(ofFbo in, ofFbo buffer, ofFbo& out, EffectDropShadowSettings settings)
	{

		ofFloatColor c = ofFloatColor(settings.color);

		// create buffer fbo
		buffer.begin();
		ofClear(0, 0);
		ofSetColor(255);
		in.draw(settings.size, settings.size);
		buffer.end();

		// apply filter
		out.begin();
		shaderShadow.begin();
		shaderShadow.setUniform1i("kernelSize", 40);
		shaderShadow.setUniform4f("shadowColor", ofVec4f(c.r, c.g, c.b, c.a));
		shaderShadow.setUniform2f("offset", ofVec2f(settings.size/20));

		buffer.draw(0, 0);
		shaderShadow.end();
		out.end();

	}
}