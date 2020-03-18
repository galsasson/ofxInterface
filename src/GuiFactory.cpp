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
					colors.insert(pair<string, ofColor>(color["id"].get<string>(), colorFromJson(color["color"])));
				}
			}else {
				ofLogError("GuiFactory::setup", "no font colors defined, gui elements will use predefined ones");
			}

			// init font styles
			if (!config["fontStyles"].is_null()) {
				for (auto& font : config["fontStyles"]) {
					ofxFontStash2::Style style;
					style.fontID = font["fontId"].get<string>();
					style.fontSize = font["fontSize"].get<int>() * 1.5;
					style.color = colors[font["color"].get<string>()];
					if (!font["lineHeight"].is_null()) style.lineHeightMult = font["lineHeight"].get<float>();

					assets->getFonts()->addStyle(font["name"].get<string>(), style);
				}
			} else {
				ofLogError("GuiFactory::setup", "no font styles defined, gui elements using fonts won't be available" );
			}

			// init elements
			if (!config["elements"].is_null()) {
				styles = config["elements"];
			}else {
				ofLogVerbose("GuiFactory::setup", "no elements defined");
			}
			

			// register creation functions
			registerCreationFunction("node", [this](ofJson config, ofJson style) {return this->getNode(config, style); });
			registerCreationFunction("label", [this](ofJson config, ofJson style) {return this->getLabel(config, style); });
			registerCreationFunction("textField", [this](ofJson config, ofJson style) {return this->getTextField(config, style); });
			registerCreationFunction("colorPanel", [this](ofJson config, ofJson style) {return this->getColorPanel(config, style); });
			registerCreationFunction("scrollableContainer", [this](ofJson config, ofJson style) {return this->getScrollableContainer(config, style); });
			registerCreationFunction("texture", [this](ofJson config, ofJson style) {return this->getTextureNode(config, style); });
			registerCreationFunction("modalElement", [this](ofJson config, ofJson style) {return this->getModal(config, style); });
			registerCreationFunction("simpleChecker", [this](ofJson config, ofJson style) {return this->getSimpleChecker(config, style); });
			registerCreationFunction("softKeyboard", [this](ofJson config, ofJson style) {return this->getSoftKeyboard(config, style); });
			registerCreationFunction("textInput", [this](ofJson config, ofJson style) {return this->getTextInput(config, style); });
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
			ofJson styleConfig = getStyle(config["class"].get<string>());

			if (styleConfig["id"] != "#error") {
				string elemClass = styleConfig["class"].get<string>();
				DEBUG("create element : " << config["id"].get<string>() << "  ( " << elemClass << " -> " << styleConfig["id"].get<string>() << " )");
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
					s.alignment = ofAlignHorz::OF_ALIGN_HORZ_CENTER;
				} else if (getValue<string>("alignment", config, style) == "right") {
					s.alignment = ofAlignHorz::OF_ALIGN_HORZ_RIGHT;
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
					s.alignment = ofAlignHorz::OF_ALIGN_HORZ_LEFT;
				} else if (al == "right") {
					s.alignment = ofAlignHorz::OF_ALIGN_HORZ_RIGHT;
				} else if (al == "center") {
					s.alignment = ofAlignHorz::OF_ALIGN_HORZ_CENTER;
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
			readNodeSettings(s,config,style);
			TextureNode* tex = new TextureNode();

			if (hasValue("texture", config, style)) {
				s.texture = assets->getTexture(getValue<string>("texture", config, style));
				if (!hasValue("size", config, style)) {
					s.size = ofVec2f(assets->getTexture(getValue<string>("texture", config, style)).getWidth(),
						assets->getTexture(getValue<string>("texture", config, style)).getHeight());
				}
			}
			if (hasValue("tint", config, style)) {
				s.tinting = colors[getValue<string>("tint", config, style)];
			}

			if (hasValue("blendmode", config, style)) {
				string al = getValue<string>("blendmode", config, style);
				if (al == "alpha") {
					s.blendmode = OF_BLENDMODE_ALPHA;
				} else if (al == "none") {
					s.blendmode = OF_BLENDMODE_DISABLED;
				} else if (al == "add") {
					s.blendmode = OF_BLENDMODE_ADD;
				} else if (al == "multiply") {
					s.blendmode = OF_BLENDMODE_MULTIPLY;
				} else if (al == "screen") {
					s.blendmode = OF_BLENDMODE_SCREEN;
				} else if (al == "subtract") {
					s.blendmode = OF_BLENDMODE_SUBTRACT;
				}
			}
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

			ret->setup(settings);
			return ret;
		}

		Node * GuiFactory::getTextInput(ofJson config, ofJson style)
		{
			TextInputSettings settings;
			readModalElementSettings(settings,config,style);

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

			TextInput* t = new TextInput();
			t->setup(settings);
			return t;
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
				settings.bgColor = colors[getValue<string>("color", config, style)];
			}
			if (hasValue("strokeColor", config, style)) {
				settings.strokeColor = colors[getValue<string>("strokeColor", config, style)];
			}
		}

		ofColor GuiFactory::colorFromJson(ofJson val) {
			switch (val.size()) {
			case 1:
				// interprete string
				if (val.is_string()) {

					if (ofIsStringInString(val.get<string>(), ".")) {
						ofParameter<ofFloatColor> pTemp;
						pTemp.fromString(val);
						return pTemp.get();
					} else {
						ofParameter<ofColor> pTemp;
						pTemp.fromString(val);
						return pTemp.get();
					}

				}// interprete array
				else
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