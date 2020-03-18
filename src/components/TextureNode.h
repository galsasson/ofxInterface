#pragma once
#include "Node.h"

namespace ofxInterface {
	struct TextureNodeSettings : NodeSettings {
		ofTexture texture;
		ofColor tinting = ofColor(255);
		float alpha = 255;
		ofScaleMode scaleMode = OF_SCALEMODE_FILL;
		ofBlendMode blendmode = OF_BLENDMODE_ALPHA;
	};

	class TextureNode :
		public Node
	{
	public:
		TextureNode();
		~TextureNode();

		void setup(ofTexture& texture);
		void setup(TextureNodeSettings settings);

        virtual void draw();

		void setTexture(ofTexture& texture_);
		ofTexture getTexture();
        
		ofScaleMode getScaleMode();
		void setScaleMode(ofScaleMode mode);

		float getAlpha();
		void setAlpha(float alpha);
		
		ofColor getTinting();
		void setTinting(ofColor color);

		bool isHardShadowblend();
		void setHardShadowBlend(bool isHardShadowBlend);
		void setBlendMode(ofBlendMode blendmode);

		ofEvent<ofTexture> eventTextureChanged;

		ofJson getNodeJson() override;

	protected:
		ofTexture texture;
		float alpha;

    private:
		ofColor tinting = ofColor(255);
		ofScaleMode scaleMode = OF_SCALEMODE_FILL;
		bool hardShadowBlend = false;
		ofBlendMode blendmode = OF_BLENDMODE_ALPHA;
	};
}

