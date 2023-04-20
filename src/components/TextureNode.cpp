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
		auto ret = new TextureNode(*this);
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
		horizontalAlign = settings.horizontalAlign;
		verticalAlign = settings.verticalAlign;
	}
    
    void TextureNode::draw()
    {
        if (texture.isAllocated()) {

			if (blendmode == OF_BLENDMODE_ALPHA) {				
				glEnable(GL_BLEND);
				glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
				glBlendFuncSeparate(GL_ONE, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
				//glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
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
				texCoordst.scaleTo(dimensionsT, OF_ASPECT_RATIO_KEEP, horizontalAlign, verticalAlign);
				//texCoordst.scaleTo(dimensionsT, OF_SCALEMODE_FIT);
				dimensions = texCoordst;
				break;
			case OF_SCALEMODE_FILL:
				//dimensionsT.scaleTo(texCoords, OF_SCALEMODE_FIT);
				dimensionsT.scaleTo(texCoords, OF_ASPECT_RATIO_KEEP, horizontalAlign, verticalAlign);
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

	ofAlignVert TextureNode::getVerticalAlign()
	{
		return verticalAlign;
	}

	void TextureNode::setVerticalAlign(ofAlignVert align)
	{
		verticalAlign = align;
	}

	ofAlignHorz TextureNode::getHorizontalAlign()
	{
		return horizontalAlign;
	}

	void TextureNode::setHorizontalAlign(ofAlignHorz align)
	{
		horizontalAlign = align;
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