#include "TextureNode.h"


namespace ofxInterface {
	TextureNode::TextureNode():Node()
	{
	}


	TextureNode::~TextureNode()
	{
	}

	Node * TextureNode::clone()
	{
		auto ret = new Node(*this);
		return ret;
	}

	void TextureNode::setup(ofTexture& texture)
	{
		setTexture(texture);
		setSize(texture.getWidth(), texture.getHeight());
	}

	void TextureNode::setup(TextureNodeSettings settings) {
		Node::setup(settings);
		if (settings.size == ofVec2f(0, 0)) {
			setup(settings.texture);
		} else {
			texture = settings.texture;
		}
		scaleMode = settings.scaleMode;
		blendmode = settings.blendmode;
		tinting = settings.tinting;
	}
    
    void TextureNode::draw()
    {
        if (texture.isAllocated()) {

			if (hardShadowBlend) {
				glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			} else {
				setBlendMode(blendmode);
			}
			
			if (alpha<255)
			{
				ofSetColor(255, alpha);
			}
			
			ofRectangle dimensions{ 0,0,getWidth(),getHeight() };
			ofRectangle texCoords{ 0,0,texture.getWidth(),texture.getHeight() };

			ofRectangle dimensionsT = dimensions;
			ofRectangle texCoordst = texCoords;
			switch (scaleMode) {
			case OF_SCALEMODE_FIT:
				texCoordst.scaleTo(dimensionsT, OF_SCALEMODE_FIT);
				dimensions = texCoordst;
				break;
			case OF_SCALEMODE_FILL:
				dimensionsT.scaleTo(texCoords, OF_SCALEMODE_FIT);
				texCoords = dimensionsT;
				break;
			case OF_SCALEMODE_CENTER:
				break;
			case OF_SCALEMODE_STRETCH_TO_FILL:
				break;
			default:
				break;
			}

			ofSetColor(tinting);
			texture.drawSubsection(dimensions, texCoords);
        }

    }

	void TextureNode::setTexture(ofTexture& texture_)
	{
		texture = texture_;
		ofNotifyEvent(eventTextureChanged,texture);
	}

	ofTexture TextureNode::getTexture()
	{
		return texture;
	}

	float TextureNode::getAlpha() {
		return alpha;
	}

	void TextureNode::setAlpha(float alpha_)
	{
		alpha = alpha_;
	}

	void TextureNode::setTinting(ofColor color)
	{
		tinting = color;
	}

	ofScaleMode TextureNode::getScaleMode() {
		return scaleMode;
	}

	void TextureNode::setScaleMode(ofScaleMode mode) {
		scaleMode = mode;
	}
    
	ofColor TextureNode::getTinting() {
		return tinting;
	}

	bool TextureNode::isHardShadowblend() {
		return hardShadowBlend;
	}

	void TextureNode::setHardShadowBlend(bool isHardShadowBlend_) {
		
		hardShadowBlend = isHardShadowBlend_;
	}

	void TextureNode::setBlendMode(ofBlendMode blendmode_) {
		blendmode = blendmode_;
	}

	ofJson TextureNode::getNodeJson() {
		auto ret = Node::getNodeJson();
		ret["nodeType"] = "TextureNode";
		ret["tint"] = vector<int>{ tinting.r,tinting.g,tinting.b,tinting.a };
		ret["alpha"] = alpha;
		ret["scaleMode"] = "fill";
		return ret;
	}

}