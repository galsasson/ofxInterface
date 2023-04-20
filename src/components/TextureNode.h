#pragma once
#include "Node.h"

namespace ofxInterface {
	struct TextureNodeSettings : NodeSettings {
		ofTexture texture;
		ofColor tinting = ofColor(255);
		float alpha = 255;
		ofScaleMode scaleMode = OF_SCALEMODE_FILL;
		ofBlendMode blendmode = OF_BLENDMODE_ALPHA;
		ofAlignHorz horizontalAlign = OF_ALIGN_HORZ_CENTER;
		ofAlignVert verticalAlign = OF_ALIGN_VERT_CENTER;
	};

	class TextureNode :
		public Node
	{
	public:
		TextureNode();
		~TextureNode();
		virtual Node* clone() override;

		void setup(ofTexture& texture);
		void setup(TextureNodeSettings settings);

		virtual void draw() override;

		void setTexture(ofTexture& texture_);
		ofTexture getTexture();
        
		ofScaleMode getScaleMode();
		void setScaleMode(ofScaleMode mode);

		float getAlpha();
		void setAlpha(float alpha);
		
		ofColor getTinting();
		void setTinting(ofColor color);

		ofAlignVert getVerticalAlign();
		void setVerticalAlign(ofAlignVert align);
		
		ofAlignHorz getHorizontalAlign();
		void setHorizontalAlign(ofAlignHorz align);

		void setBlendMode(ofBlendMode blendmode);

		ofEvent<ofTexture> eventTextureChanged;

		ofJson getNodeJson() override;

	protected:
		ofTexture texture;
		float alpha;

    private:
		ofColor tinting = ofColor(255);
		ofScaleMode scaleMode = OF_SCALEMODE_FILL;
		ofBlendMode blendmode = OF_BLENDMODE_ALPHA;

		ofAlignHorz horizontalAlign = OF_ALIGN_HORZ_CENTER;
		ofAlignVert verticalAlign = OF_ALIGN_VERT_CENTER;
	};
}

