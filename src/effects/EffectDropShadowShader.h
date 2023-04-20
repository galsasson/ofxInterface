#pragma once

#define STRINGIFY(A) #A

#include "ofMain.h"

class EffectDropShadowShader {
public: string vertex, fragment;

	EffectDropShadowShader() {
		  vertex = "#version 150\n";
		  vertex += STRINGIFY(
			  uniform mat4 projectionMatrix;
		  uniform mat4 modelViewMatrix;
		  uniform mat4 modelViewProjectionMatrix;

		  in vec4  position;
		  in vec2  texcoord;

		  out vec2 texCoordVarying;

		  void main()
		  {
			  texCoordVarying = texcoord;
			  gl_Position = modelViewProjectionMatrix * position;
		  }
		  );

		  // === Base mask ============================================================================
		  fragment = "#version 150\n";
		  fragment += STRINGIFY(
			  // Input params
			  uniform sampler2DRect tex0;
		  uniform int kernelSize;
		  uniform vec2 offset;
		  uniform vec4 shadowColor;

		  in vec2 texCoordVarying;
		  out vec4 outputColor;

		  void main()
		  {
			  vec4 accumColor = vec4(0.);


			  vec2 st = texCoordVarying;
			  float kernelSizef = float(kernelSize);


			  float accumWeight = 0.;
			  const float k = .15915494; // 1 / (2*PI)
			  float kernelSize2 = kernelSizef * kernelSizef;

			  for (int j = 0; j < kernelSize; j++) {
				  float y = -.5 * (kernelSizef - 1.) + float(j);
				  for (int i = 0; i < kernelSize; i++) {
					  //if(distance(st, st-vec2(j,i))<=kernelSizef ){ // radial filter
					  float x = -.5 * (kernelSizef - 1.) + float(i);
					  float weight = (k / kernelSizef) * exp(-(x * x + y * y) / (2. * kernelSize2));
					  accumColor += weight * vec4(shadowColor.r, shadowColor.g, shadowColor.b, texture(tex0, st + vec2(x, y) * offset).a);
					  accumWeight += weight;
					  //  }


				  }
			  }
			  accumColor.a*= shadowColor.a;
			  //outputColor =  shadowColor;
			  outputColor = accumColor / accumWeight;
			  //outputColor = texture(tex0, st);



		  }
		  );

	  }
};