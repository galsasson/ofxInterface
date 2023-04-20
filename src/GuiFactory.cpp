#include "GuiFactory.h"

namespace ofxInterface
{
		GuiFactory::GuiFactory() {
		}


		GuiFactory::~GuiFactory() {
		}
		
		void GuiFactory::setup(ofJson config, shared_ptr<ofxAssetManager> assets_) {
			assets = assets_;

			// init colors
			if (!config["colors"].is_null()) {
				for (auto& color : config["colors"]) {
					colors.insert(pair<string, ofColor>(color["id"].get<string>(), colorFromJson(color["color"],colors)));


				}
			}else {
				ofLogError("GuiFactory::setup", "no font colors defined, gui elements will use predefined ones");
			}

			// init font styles

			// disable logging each font that is loaded
			auto currentLogLevel = ofGetLogLevel();
			ofSetLogLevel(OF_LOG_ERROR);

			float fontScale = 1.5;
			if (!config["fontScale"].is_null()) {
				fontScale = config["fontScale"].get<float>();
			}
			if (!config["fontStyles"].is_null()) {
				for (auto& font : config["fontStyles"]) {
					ofxFontStash2::Style style;
					style.fontID = font["fontId"].get<string>();
					style.fontSize = font["fontSize"].get<int>()*fontScale;
					style.color = colors[font["color"].get<string>()];
					if (!font["lineHeight"].is_null()) style.lineHeightMult = font["lineHeight"].get<float>();
					if (!font["spacing"].is_null()) style.spacing = font["spacing"].get<float>();

					assets->getFonts()->addStyle(font["name"].get<string>(), style);
				}
			} else {
				ofLogError("GuiFactory::setup", "no font styles defined, gui elements using fonts won't be available" );
			}
			ofSetLogLevel(currentLogLevel);

			// init elements
			if (!config["elements"].is_null()) {
				styles = config["elements"];
			}else {
				ofLogVerbose("GuiFactory::setup", "no elements defined");
			}
			

			// register creation functions
			registerCreationFunction("node", [this](ofJson config, ofJson style) {return this->getNode(config, style); });
			registerCreationFunction("canvas", [this](ofJson config, ofJson style) {return this->getNode(config, style); });
			registerCreationFunction("label", [this](ofJson config, ofJson style) {return this->getLabel(config, style); });
			registerCreationFunction("textField", [this](ofJson config, ofJson style) {return this->getTextField(config, style); });
			registerCreationFunction("colorPanel", [this](ofJson config, ofJson style) {return this->getColorPanel(config, style); });
			registerCreationFunction("scrollableContainer", [this](ofJson config, ofJson style) {return this->getScrollableContainer(config, style); });
			registerCreationFunction("texture", [this](ofJson config, ofJson style) {return this->getTextureNode(config, style); });
			registerCreationFunction("modalElement", [this](ofJson config, ofJson style) {return this->getModal(config, style); });
			registerCreationFunction("simpleChecker", [this](ofJson config, ofJson style) {return this->getSimpleChecker(config, style); });
			registerCreationFunction("softKeyboard", [this](ofJson config, ofJson style) {return this->getSoftKeyboard(config, style); });
			registerCreationFunction("textInput", [this](ofJson config, ofJson style) {return this->getTextInput(config, style); });
			registerCreationFunction("slider", [this](ofJson config, ofJson style) {return this->getSlider(config, style); });
		}

		///\brief create all GUI elements from json and add them to parent node
		void GuiFactory::createElements(Node * parent, ofJson config) {
			for (auto& e : config["elements"]) {
				auto elem = getElement(e);
				if (elem != nullptr) {
					parent->addChild(elem);
					// create child elements
					if (!e["elements"].is_null() && e["elements"].is_array()) {
						createElements(elem, e);
					}
				}
			}
		}

		Node * GuiFactory::getElement(ofJson config) {
			
			// usinf "node" if no class specified
			string className;
			if (config["class"].is_null()) {
				className = "node";
			}
			else {
				className = config["class"].get<string>();
			}
			ofJson styleConfig = getStyle(className);

			if (styleConfig["id"] != "#error") {
				string elemClass = styleConfig["class"].get<string>();
				ofLogVerbose() <<"create element : " << config["id"].get<string>() << "  ( " << elemClass << " -> " << styleConfig["id"].get<string>() << " )";
				Node* ret = nullptr;
				if (creationTable.find(elemClass) != creationTable.end()) {
					ret = (this->creationTable[elemClass])(config, styleConfig);
				} else {
					ofLogError("GuiFactory", "Can't create element \"" + config["id"].get<string>() + "\" : no definition for class \"" + elemClass + "\" available.");
					return nullptr;
				}
				return ret;
			}
			return nullptr;
		}
		Node * GuiFactory::getNode(ofJson config, ofJson style) {
			NodeSettings s;
			readNodeSettings(s,config,style);
			Node* n = new Node();
			n->setup(s);
			return n;
		}
		// --------------------------------------------------------------------------------------------------------------
		Node * GuiFactory::getLabel(ofJson config, ofJson style) {

			LabelSettings s;
			readNodeSettings(s, config, style);

			s.font = assets->getFonts();
			if (hasValue("font", config, style)) {
				s.style = assets->getFonts()->getStyle(getValue<string>("font", config, style));
			}

			if (hasValue("text", config, style)) {
				s.text = getValue<string>("text", config, style);
			}

			if (hasValue("alignment", config, style)) {
				if (getValue<string>("alignment", config, style) == "center") {
					s.horzAlignment = ofAlignHorz::OF_ALIGN_HORZ_CENTER;
				} else if (getValue<string>("alignment", config, style) == "right") {
					s.horzAlignment = ofAlignHorz::OF_ALIGN_HORZ_RIGHT;
				}
			}

			if (hasValue("shadow", config, style)) {
				s.isDropshadow = true;
				if (hasValue("size", config["shadow"], style["shadow"])) {
					s.shadowSize = getValue<float>("size", config["shadow"], style["shadow"]);
				}
				if (hasValue("x", config["shadow"], style["shadow"])) {
					s.shadowPos.x = getValue<float>("x", config["shadow"], style["shadow"]);
				}
				if (hasValue("y", config["shadow"], style["shadow"])) {
					s.shadowPos.y = getValue<float>("y", config["shadow"], style["shadow"]);
				}
				if (hasValue("color", config["shadow"], style["shadow"])) {

					s.shadowColor = colors[getValue<string>("color", config["shadow"], style["shadow"])];
				}
			}

			Label* label = new Label();
			label->setup(s);

			return label;
		}

		Node * GuiFactory::getTextField(ofJson config, ofJson style) {
			TextFieldSettings s;
			readNodeSettings(s, config, style);

			s.font = assets->getFonts();

			if (!config["text"].is_null()) {
				if (config["text"].is_array()) {
					for (auto elem : config["text"]) {
						s.text += elem.get<string>();
					}
				} else {
					s.text = config["text"].get<string>();
				}
			}
			if (hasValue("alignment", config, style)) {
				string al = getValue<string>("alignment", config, style);
				if (al == "left") {
					s.horzAlignment = ofAlignHorz::OF_ALIGN_HORZ_LEFT;
				} else if (al == "right") {
					s.horzAlignment = ofAlignHorz::OF_ALIGN_HORZ_RIGHT;
				} else if (al == "center") {
					s.horzAlignment = ofAlignHorz::OF_ALIGN_HORZ_CENTER;
				}
			}
			

			TextField* textField = new TextField();
			textField->setup(s);
			return textField;
		}

		Node * GuiFactory::getColorPanel(ofJson config, ofJson style) {
			ColorPanelSettings s;
			readColorpanelSettings(s, config, style);

			ColorPanel* p = new ColorPanel();
			p->setup(s);
			return p;

		}

		

		Node * GuiFactory::getScrollableContainer(ofJson config, ofJson style) {
			ScrollableContainerSettings s;
			readScrollableContainerSettings(s, config, style);
			ScrollableContainer * p = new ScrollableContainer();
			p->setup(s);
			return p;
		}

		Node * GuiFactory::getTextureNode(ofJson config, ofJson style) {
			TextureNodeSettings s;
			readTextureSettings(s,config,style);
			TextureNode* tex = new TextureNode();
			tex->setup(s);
			return tex;
		}

		Node * GuiFactory::getModal(ofJson config, ofJson style) {
			ModalElementSettings s;
			readModalElementSettings(s, config, style);
			ModalElement* ret = new ModalElement();
			ret->setup(s);
			return ret;
		}

		Node * GuiFactory::getSimpleChecker(ofJson config, ofJson style) {
			ModalElementSettings s;
			readModalElementSettings(s, config, style);
			
			SimpleChecker* ret = new SimpleChecker();
			ret->setup(s);
			return ret;
		}

		Node * GuiFactory::getSoftKeyboard(ofJson config, ofJson style)
		{
			SoftKeyboardSettings settings;
			readNodeSettings(settings, config, style);

			SoftKeyboard* ret = new SoftKeyboard();

			settings.font = assets->getFonts();
			if (hasValue("font", config, style)) {
				settings.style = assets->getFonts()->getStyle(getValue<string>("font", config, style));
			}
			if (hasValue("bgColor", config, style)) {
				settings.bgColor = colors[getValue<string>("bgColor", config, style)];
			}
			if (hasValue("colorActive", config, style)) {
				settings.colorActive = colors[getValue<string>("colorActive", config, style)];
			}
			if (hasValue("colorInactive", config, style)) {
				settings.colorInactive = colors[getValue<string>("colorInactive", config, style)];
			}
			if (hasValue("colorSelected", config, style)) {
				settings.colorSelected = colors[getValue<string>("colorSelected", config, style)];
			}

			if (hasValue("borderRadius", config, style)) {
				settings.borderRadius = getValue<int>("borderRadius", config, style);
			}
			if (hasValue("borderWidth", config, style)) {
				settings.borderWidth = getValue<int>("borderWidth", config, style);
			}
			if (hasValue("margin", config, style)) {
				settings.margin = getValue<int>("margin", config, style);
			}
			if (hasValue("padding", config, style)) {
				settings.padding = getValue<int>("padding", config, style);
			}
			if (hasValue("layout", config, style)) {
				settings.layout = getValue<string>("layout", config, style);
			}
			if (hasValue("textureKeys", config, style)) {
				ofJson mapping;
				if (config["textureKeys"] != nullptr) {
					mapping = config["textureKeys"];
				}
				else if (style["textureKeys"] != nullptr) {
					mapping = style["textureKeys"];
				}

				for (auto& entry:mapping.items())
				{
					settings.textureKeys.insert(make_pair(ofToInt(entry.key()), assets->getTexture(entry.value().get<string>())));
				}
			}

			ret->setup(settings);
			return ret;
		}

		Node * GuiFactory::getTextInput(ofJson config, ofJson style)
		{
			TextInputSettings settings;
			readTextInputSettings(settings, config, style);
			

			TextInput* t = new TextInput();
			t->setup(settings);
			return t;
		}

		Node * GuiFactory::getSlider(ofJson config, ofJson style)
		{
			SliderSettings settings; 
			readNodeSettings(settings, config, style);

			if (hasValue("direction", config, style)) {
				string val = ofToUpper(getValue<string>("direction", config, style));
				if (val == "HORIZONTAL") {
					settings.direction = HORIZONTAL;
				}
				else if (val == "VERTICAL") {
					settings.direction = VERTICAL;
				}
			}
			if (hasValue("colorActive", config, style)) {
				settings.colorActive = colors[getValue<string>("colorActive", config, style)];
			}
			if (hasValue("colorInactive", config, style)) {
				settings.colorInactive = colors[getValue<string>("colorInactive", config, style)];
			}
			if (hasValue("colorSelected", config, style)) {
				settings.colorSelected = colors[getValue<string>("colorSelected", config, style)];
			}

			if (hasValue("lineWidth", config, style)) {
				settings.lineWidth = getValue<int>("lineWidth", config, style);
			}

			Slider* ret = new Slider();
			ret->setup(settings);
			return ret;
		}

		ofJson GuiFactory::getTemplate(string templateId)
		{
			for (auto& elem:styles){
				if (elem["id"].get<string>() == templateId) {
					return elem;
				}
			}
			return ofJson{ {"error","style not found"} };
		}

		ofJson GuiFactory::getStyle(string id) {
			for (auto& s : styles) {
				if (s["id"] == id) {
					return s;
				}
			}
			if (id != "canvas") {
				ofLogError("GuiFactory::getStyle", "style <" + id + "> not found");
			}

			return ofJson({ {"id","#error"} });
		}
		void GuiFactory::readNodeSettings(NodeSettings & settings, ofJson config, ofJson style) {
			readEffectSettings(settings, config, style);
			settings.name = config["id"].get<string>();
			if (hasValue("position", config, style)) {
				settings.position = vec2fFromJson(getValue<ofJson>("position", config, style));
			}
			if (hasValue("size", config, style)) {
				settings.size = vec2fFromJson(getValue<ofJson>("size", config, style));
			}
			if (hasValue("plane", config, style)) {
				settings.plane = getValue<float>("plane", config, style);
			}
			if (hasValue("renderClip", config, style)) {
				settings.renderClip = getValue<bool>("plane", config, style);
			}
			if (hasValue("active", config, style)) {
				settings.isActive = getValue<bool>("active", config, style);
			}
		}

		void GuiFactory::readModalElementSettings(ModalElementSettings & settings, ofJson config, ofJson style) {
			readNodeSettings(settings, config, style);
			if (hasValue("type", config, style)) {
				string val = getValue<string>("type", config, style);
				if (val == "button") {
					settings.type = BUTTON;
				}else if (val == "checker") {
					settings.type = CHECKER;
				} else if (val == "radio") {
					settings.type = RADIO;
				}
			}
			if (hasValue("colorActive", config, style)) {
				settings.colorActive = colors[getValue<string>("colorActive", config, style)];
			}
			if (hasValue("colorInactive", config, style)) {
				settings.colorInactive = colors[getValue<string>("colorInactive", config, style)];
			}
			if (hasValue("colorSelected", config, style)) {
				settings.colorSelected = colors[getValue<string>("colorSelected", config, style)];
			}
		}

		void GuiFactory::readScrollableContainerSettings(ScrollableContainerSettings & settings, ofJson config, ofJson style) {
			readNodeSettings(settings, config, style);
			if (hasValue("sizeScrollableArea", config, style)) {
				settings.sizeScrollableArea = vec2fFromJson(getValue<ofJson>("sizeScrollableArea", config, style));
			} else {
				settings.sizeScrollableArea = settings.size;
			}
			if (hasValue("bgColor", config, style)) {
				settings.bgColor = colors[getValue<string>("bgColor", config, style)];
			}
			if (hasValue("scrollActiveColor", config, style)) {
				settings.scrollActiveColor = colors[getValue<string>("scrollActiveColor", config, style)];
			}
			if (hasValue("scrollInactiveColor", config, style)) {
				settings.scrollInactiveColor = colors[getValue<string>("scrollInactiveColor", config, style)];
			}
		}

		void GuiFactory::readColorpanelSettings(ColorPanelSettings & settings, ofJson config, ofJson style) {
			readNodeSettings(settings, config, style);

			if (hasValue("strokeWidth", config, style)) {
				settings.strokeWidth = getValue<float>("strokeWidth", config, style);
			}
			if (hasValue("drawBackground", config, style)) {
				settings.bDrawBackground = getValue<bool>("drawBackground", config, style);
			}
			if (hasValue("borderRadius", config, style)) {
				settings.borderRadius = getValue<float>("borderRadius", config, style);
			}
			if (hasValue("color", config, style)) {
				if (getValueType("color", config, style) == "array") {
					auto cs = config["color"] == nullptr ? style["color"] : config["color"];
					settings.colors.clear();
					for (auto& c : cs) {
						settings.colors.push_back(colors[c.get<string>()]);
					}
				}
				else {
					settings.colors.front() = colors[getValue<string>("color", config, style)];
				}
			}
			if (hasValue("strokeColor", config, style)) {
				settings.strokeColor = colors[getValue<string>("strokeColor", config, style)];
			}
			if (hasValue("gradientDirection", config, style)) {
				settings.gradientDirection = getValue<float>("gradientDirection", config, style);
			}
			if (hasValue("gradientColorPositions", config, style)) {
				auto gs = config["gradientColorPositions"] == nullptr ? style["gradientColorPositions"] : config["gradientColorPositions"];
				settings.gradientColorPositions.clear();
				for (auto& g : gs) {
					settings.colors.push_back(g.get<float>());
				}
			}


			vector<float> gradientColorPositions;
		}

		void GuiFactory::readTextureSettings(TextureNodeSettings & settings, ofJson config, ofJson style)
		{
			readNodeSettings(settings, config, style);

			if (hasValue("texture", config, style)) {
				settings.texture = assets->getTexture(getValue<string>("texture", config, style));
				if (!hasValue("size", config, style)) {
					settings.size = ofVec2f(assets->getTexture(getValue<string>("texture", config, style)).getWidth(),
						assets->getTexture(getValue<string>("texture", config, style)).getHeight());
				}
			}
			if (hasValue("tint", config, style)) {
				settings.tinting = colors[getValue<string>("tint", config, style)];
			}
			if (hasValue("scaleMode", config, style)) {
				string al = getValue<string>("scaleMode", config, style);
				if (al == "fill") {
					settings.scaleMode = OF_SCALEMODE_FILL;
				}
				else if (al == "fit") {
					settings.scaleMode = OF_SCALEMODE_FIT;
				}
				else if (al == "center") {
					settings.scaleMode = OF_SCALEMODE_CENTER;
				}
				else if (al == "stretch") {
					settings.scaleMode = OF_SCALEMODE_STRETCH_TO_FILL;
				}
			}

			if (hasValue("verticalAlign", config, style)) {
				string al = getValue<string>("verticalAlign", config, style);
				if (al == "top") {
					settings.verticalAlign = OF_ALIGN_VERT_TOP;
				}
				else if (al == "bottom") {
					settings.verticalAlign = OF_ALIGN_VERT_BOTTOM;
				}
				else if (al == "center") {
					settings.verticalAlign = OF_ALIGN_VERT_CENTER;
				}
			}

			if (hasValue("horizontalAlign", config, style)) {
				string al = getValue<string>("horizontalAlign", config, style);
				if (al == "left") {
					settings.horizontalAlign = OF_ALIGN_HORZ_LEFT;
				}
				else if (al == "right") {
					settings.horizontalAlign = OF_ALIGN_HORZ_RIGHT;
				}
				else if (al == "center") {
					settings.horizontalAlign = OF_ALIGN_HORZ_CENTER;
				}
			}

			if (hasValue("blendmode", config, style)) {
				string al = getValue<string>("blendmode", config, style);
				if (al == "alpha") {
					settings.blendmode = OF_BLENDMODE_ALPHA;
				}
				else if (al == "none") {
					settings.blendmode = OF_BLENDMODE_DISABLED;
				}
				else if (al == "add") {
					settings.blendmode = OF_BLENDMODE_ADD;
				}
				else if (al == "multiply") {
					settings.blendmode = OF_BLENDMODE_MULTIPLY;
				}
				else if (al == "screen") {
					settings.blendmode = OF_BLENDMODE_SCREEN;
				}
				else if (al == "subtract") {
					settings.blendmode = OF_BLENDMODE_SUBTRACT;
				}
			}
		}

		void GuiFactory::readTextInputSettings(TextInputSettings& settings, ofJson config, ofJson style)
		{
			readModalElementSettings(settings, config, style);

			settings.font = assets->getFonts();
			if (hasValue("font", config, style)) {
				settings.style = assets->getFonts()->getStyle(getValue<string>("font", config, style));
			}

			if (hasValue("maxChars", config, style)) {
				settings.maxChars = getValue<int>("maxChars", config, style);
			}
			if (hasValue("description", config, style)) {
				settings.descriptionText = getValue<string>("description", config, style);
			}
			if (hasValue("enableNewline", config, style)) {
				settings.enableNewline = getValue<bool>("enableNewline", config, style);
			}

			if (hasValue("alignment", config, style)) {
				if (getValue<string>("alignment", config, style) == "center") {
					settings.horzAlignment = ofAlignHorz::OF_ALIGN_HORZ_CENTER;
				}
				else if (getValue<string>("alignment", config, style) == "right") {
					settings.horzAlignment = ofAlignHorz::OF_ALIGN_HORZ_RIGHT;
				}
			}

			if (hasValue("autoResize", config, style)) {
				settings.autoResize = getValue<bool>("autoResize", config, style);
			}
		}

		void GuiFactory::readEffectSettings(NodeSettings& settings, ofJson config, ofJson style)
		{
			if (hasValue("nodeEffects", config, style)) {
				if (hasValue("shadow", config["nodeEffects"], style["nodeEffects"])) {
					if (hasValue("color", config["nodeEffects"]["shadow"], style["nodeEffects"]["shadow"])) {
						settings.effects.dropShadow.color = colors[getValue<string>("color", config["nodeEffects"]["shadow"], style["nodeEffects"]["shadow"])];
					}
					if (hasValue("size", config["nodeEffects"]["shadow"], style["nodeEffects"]["shadow"])) {
						settings.effects.dropShadow.size = getValue<int>("size", config["nodeEffects"]["shadow"], style["nodeEffects"]["shadow"]);
					}
					if (hasValue("position", config["nodeEffects"]["shadow"], style["nodeEffects"]["shadow"])) {
						settings.effects.dropShadow.position = vec2fFromJson(getValue<ofJson>("position", config["nodeEffects"]["shadow"], style["nodeEffects"]["shadow"]));
					}

				}
			}

		}

		ofColor GuiFactory::colorFromJson(ofJson val, map<string, ofColor> colors) {
			if (val.is_string()) { // read ofParam
				if (ofIsStringInString(val.get<string>(), ".")) {
					ofParameter<ofFloatColor> pTemp;
					pTemp.fromString(val);
					return pTemp.get();
				}
				else { 
					ofParameter<ofColor> pTemp;
					pTemp.fromString(val);
					return pTemp.get();
				}
			}
			else if (val.is_array()) { // read as rgba array
				switch (val.size()) {
				case 1:
					return ofColor(val[0].get<int>());
					break;
				case 2:
					return ofColor(val[0], val[0], val[0], val[1]);
					break;
				case 3:
					return ofColor(val[0], val[1], val[2]);
					break;
				case 4:
					return ofColor(val[0], val[1], val[2], val[3]);
					break;
				default:
					return ofColor();
					break;
				}
			}
			else if(val.is_object())
			{
				ofColor base;
				if (val["base"].is_null()) {
					ofLogError("no base Color defined");
				}
				else {
					if (colors.find(val["base"].get<string>()) != colors.end()) {
						base = colors[val["base"].get<string>()];
					}
					if (!val["r"].is_null()) {
						base.r = val["r"].get<int>();
					}
					if (!val["g"].is_null()) {
						base.g = val["g"].get<int>();
					}
					if (!val["b"].is_null()) {
						base.b = val["b"].get<int>();
					}
					if (!val["a"].is_null()) {
						base.a = val["a"].get<int>();
					}
					
				}
				
				return base;
			}
			return ofColor();
		}

		ofVec2f GuiFactory::vec2fFromJson(ofJson val) {
			switch (val.size()) {
			case 1:
				// interprete string
				if (val.is_string()) {
					ofParameter<ofVec2f> pTemp;
					pTemp.fromString(val);
					return pTemp.get();
				}// interprete array
				else
					return ofVec2f(val[0].get<int>());

				break;
			case 2:
				return ofVec2f(val[0], val[1]);
				break;
			default:
				return ofVec2f();
				break;
			}
		}

		ofVec3f GuiFactory::vec3fFromJson(ofJson val) {
			switch (val.size()) {
			case 1:
				// interprete string
				if (val.is_string()) {
					ofParameter<ofVec3f> pTemp;
					pTemp.fromString(val);
					return pTemp.get();
				}// interprete array
				else
					return ofVec3f(val[0].get<int>());

				break;
			case 2:
				return ofVec3f(val[0], val[1]);
				break;
			case 3:
				return ofVec3f(val[0], val[1], val[2]);
				break;
			default:
				return ofVec2f();
				break;
			}
		}

		void GuiFactory::addStyle(ofJson style, ofJson& styleConfig, bool overwrite)
		{
			vector<ofJson> styleList;
			bool hasStyle = false;
			for (auto& elem : styleConfig) {
				if (elem["id"] == style["id"]) {
					hasStyle = true;
					if (overwrite) {
						styleList.push_back(style);
					} else {
						styleList.push_back(elem);
					}
				} else {
					styleList.push_back(elem);
				}
			}
			if (!hasStyle) {
				styleList.push_back(style);
			}
			styleConfig = styleList;
		}

		bool GuiFactory::hasValue(string valueName, ofJson config, ofJson style) {
			if (!config[valueName].is_null()) {
				return true;
			}
			if (!style[valueName].is_null()) {
				return true;
			}
			return false;
	}
		string GuiFactory::getValueType(string valueName, ofJson config, ofJson style)
		{
			string ret = "null";
			if (config[valueName] != nullptr) {
				ret = config[valueName].type_name();
			}
			else if (style[valueName] != nullptr) {
				ret = style[valueName].type_name();
			}
			return ret;
		}
		///\brief register a creation function
		///
		/// since it is not self-explanatory, here you have an example (using a lamda function)
		/// registerCreationFunction("a functionname", [this](ofJson config, ofJson style) {return this->myFunction(config, style); })
		///
		/// if someone finds a way to achive this easier, please edit ;)
		void GuiFactory::registerCreationFunction(string name, creationFunction  fct) {
			// register function in table
			creationTable[name] = fct;

			// create base style for factory
			addStyle(ofJson{ {"id",name },{"class",name} }, styles);
		}

	
}
