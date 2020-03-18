#pragma once
#include "ofMain.h"
#include "ofxAssetManager.h"

#include "Label.h"
#include "TextField.h"
#include "ColorPanel.h"
#include "ScrollableContainer.h"
#include "TextureNode.h"
#include "SimpleChecker.h"
#include "SoftKeyboard.h"
#include "TextInput.h"


#define DEBUG(X) if(isDebug) ofLogNotice() << X 


///\brief defines a function for creating gui elements
///
/// since it looks a little overcomplicated (maybe it is) i try to explain it
/// the GuiFactory should be subclassed and therefore you need to register creation functions 
/// for your own gui elements. Every function is set in the form of:
/// ofxInterface::Node*(ofJson, ofJson)
/// to register function from subclasses you need to use a std::function template
///
/// typedef does not work with std::function thats why we used a struct
/// https://stackoverflow.com/questions/27002263/c11-typedef-stdfunction-and-argument-on-itself
struct creationFunction : std::function < ofxInterface::Node*(ofJson, ofJson)> {
	using std::function < ofxInterface::Node*(ofJson, ofJson)>::function;
};

namespace ofxInterface
{
		class GuiFactory
		{
		public:
			GuiFactory();
			~GuiFactory();

			virtual void setup(ofJson config, shared_ptr<ofxAssetManager> assets);
			
			void createElements(Node* parent, ofJson config);
			Node* getElement(ofJson config);

			Node* getNode(ofJson config, ofJson style);
			Node* getLabel(ofJson config, ofJson style);
			Node* getTextField(ofJson config, ofJson style);
			Node* getColorPanel(ofJson config, ofJson style);
			Node* getScrollableContainer(ofJson config, ofJson style);
			Node* getTextureNode(ofJson config, ofJson style);
			Node* getModal(ofJson config, ofJson style);
			Node* getSimpleChecker(ofJson config, ofJson style);
			Node* getSoftKeyboard(ofJson config, ofJson style);
			Node* getTextInput(ofJson config, ofJson style);
			

		protected:
			// -- Helper functions -- //
			ofJson getStyle(string id);

			bool hasValue(string valueName, ofJson config, ofJson style);
			static ofColor colorFromJson(ofJson val);
			static ofVec2f vec2fFromJson(ofJson val);
			static ofVec3f vec3fFromJson(ofJson val);

			void addStyle(ofJson style, ofJson& styleConfig, bool overwrite = false);

			template <class ValueType>
			ValueType getValue(string valueName, ofJson config, ofJson style);

			// -- function registration -- //
			void registerCreationFunction(string name, creationFunction  fct);

			// -- class functions --//
			void readNodeSettings(NodeSettings& settings, ofJson config, ofJson style);
			void readModalElementSettings(ModalElementSettings& settings, ofJson config, ofJson style);
			void readScrollableContainerSettings(ScrollableContainerSettings& settings, ofJson config, ofJson style);
			void readColorpanelSettings(ColorPanelSettings& settings, ofJson config, ofJson style);

			// -- variables -- //
			shared_ptr<ofxAssetManager> assets;
			map<string, ofColor> colors;
			bool isDebug = true;

			

		private:
			
			ofJson styles;
			std::map<string, creationFunction> creationTable;

		};

		template<class ValueType>
		inline ValueType GuiFactory::getValue(string valueName, ofJson config, ofJson style) {
			ValueType ret;
			if (config[valueName] != nullptr) {
				ret = config[valueName].get<ValueType>();
			}else if (style[valueName] != nullptr) {
				ret = style[valueName].get<ValueType>();
			}
			return ret;
		}
}
