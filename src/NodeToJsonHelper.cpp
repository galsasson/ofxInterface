#include "NodeToJsonHelper.h"

namespace reddo {
	namespace NodeToJson {
		ofJson styleToJson(ofxFontStash2::Style style)
		{
			auto ret = ofJson();
			switch (style.alignmentV) {
			case NVG_ALIGN_TOP:
				ret["alignmentV"] = "top";
				break;
			case NVG_ALIGN_MIDDLE:
				ret["alignmentV"] = "middle";
				break;
			case NVG_ALIGN_BOTTOM:
				ret["alignmentV"] = "bottom";
				break;
			case NVG_ALIGN_BASELINE:
				ret["alignmentV"] = "baseline";
				break;
			}
			ret["blur"] = style.blur;
			ret["color"] = vector<int>{ style.color.r,style.color.g,style.color.b,style.color.a };
			ret["fontId"] = style.fontID;
			ret["fontSize"] = style.fontSize;
			ret["lineHeightMult"] = style.lineHeightMult;
			ret["spacing"] = style.spacing;
			return ret;
		}
		ofJson compositionToLog(ofJson sceneDescription, ofJson metaData)
		{
			ofJson ret = ofJson();
			if (!metaData["background"].is_null()) {
				auto path = ofSplitString(metaData["background"].get<string>(), "../").back();
				ret["background"] = path;
			}
			if (!metaData["overlay"].is_null()) {
				auto path = ofSplitString(metaData["overlay"].get<string>(), "../").back();
				ret["overlay"] = path;
			}
			if (!metaData["isPrint"].is_null()) {
				ret["isPrint"] = metaData["isPrint"].get<bool>();
			}
			if (!metaData["sharing"].is_null()) {
				ret["sharing"] = metaData["sharing"].get<string>();
			}
			if (!metaData["itemId"].is_null()) {
				ret["itemId"] = metaData["itemId"].get<string>();
			}
			if (!metaData["furtherUse"].is_null()) {
				ret["furtherUse"] = metaData["furtherUse"].get<bool>();
			}

			ofJson icons = ofJson();
			ofJson texts = ofJson();
			
			for (auto& objects : sceneDescription["children"][0]["children"]) {
				if (objects["name"] == "objects" && !objects["children"].is_null()) {
					for (auto& elem : objects["children"]) {
						if (elem["nodeType"] == "TextureNode") {
							auto path = ofSplitString(elem["customData"]["file"].get<string>(), "../").back();
							icons.push_back(path);
						}
						else if (elem["nodeType"] == "TextInputOfTTF") {
							ofJson text = ofJson();
							text["fontId"] = ofSplitString(elem["style"].get<string>(), "/").back();
							text["text"] = elem["text"];
							texts.push_back(text);
						}

					}
				}
			}
			if (!icons.is_null())
			{
				ret["icons"] = icons;
			}
			if (!texts.is_null()) {
				ret["texts"] = texts;
			}
			return ret;
		}
	}
}