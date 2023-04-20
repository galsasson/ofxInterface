#pragma once
#include "ofMain.h"
#include "ofxFontStash2.h"

namespace reddo{
	namespace NodeToJson {
		ofJson styleToJson(ofxFontStash2::Style style);
		ofJson compositionToLog(ofJson sceneDescription, ofJson metaData);
	}
}

