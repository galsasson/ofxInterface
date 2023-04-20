#include "GradientShader.h"

const ofShader ofxInterface::getGradientShader3Colors()
{
	ofShader shader;

	string vertex = "#version 150\n";
	vertex += STRINGIFY(
		uniform mat4 modelViewProjectionMatrix;

	in vec4 position;

	void main() {
		gl_Position = modelViewProjectionMatrix * position;
	}
	);

	string frag = "#version 150\n";
	frag += "#define SRGB_TO_LINEAR(c) pow((c), vec4(2.2))\n";
	frag += "#define LINEAR_TO_SRGB(c) pow((c), vec4(1.0 / 2.2))\n";
	frag += "#define SRGB(r, g, b, a) SRGB_TO_LINEAR(vec4(float(r), float(g), float(b),float(a)) / 255.0)\n";

	frag += STRINGIFY(
		uniform vec4 color1;
	uniform vec4 color2;
	uniform vec4 color3;

	uniform vec2 begin;
	uniform vec2 end;
	uniform float mid;
	uniform vec2 size;

	out vec4 outputColor;

	// Gradient noise from Jorge Jimenez's presentation:
	// http://www.iryoku.com/next-generation-post-processing-in-call-of-duty-advanced-warfare
	float gradientNoise(vec2 uv)
	{
		const vec3 magic = vec3(0.06711056, 0.00583715, 52.9829189);
		return fract(magic.z * fract(dot(uv, magic.xy)));
	}

	void main() {

		vec4 scolor1 = SRGB(color1.x, color1.y, color1.z, color1.w);
		vec4 scolor2 = SRGB(color2.x, color2.y, color2.z, color2.w);
		vec4 scolor3 = SRGB(color3.x, color3.y, color3.z, color3.w);

		vec2 fragCoord = gl_FragCoord.xy / size;

		// Calculate interpolation factor with vector projection.
		vec2 ba = end - begin;
		float t = dot(fragCoord - begin, ba) / dot(ba, ba);
		// Saturate and apply smoothstep to the factor.
		vec4 color;
		if (t < mid) {
			t = smoothstep(0.0, mid, clamp(t, 0.0, 1.0));
			color = mix(scolor1, scolor2, t);
		}
		else {
			t = smoothstep(mid, 1.0, clamp(t, 0.0, 1.0));
			color = mix(scolor2, scolor3, t);
		}

		// Convert color from linear to sRGB color space (=gamma encode).
		color = LINEAR_TO_SRGB(color);

		// Add gradient noise to reduce banding.
		color += (1.0 / 255.0) * gradientNoise(fragCoord) - (0.5 / 255.0);

		outputColor = vec4(color);
	}
	);

	shader.unload();
	shader.setupShaderFromSource(GL_VERTEX_SHADER, vertex);
	shader.setupShaderFromSource(GL_FRAGMENT_SHADER, frag);
	shader.bindDefaults();
	shader.linkProgram();

	return shader;
}

const ofShader ofxInterface::getGradientCircularMaskShader()
{
	ofShader shader;

	string vertex = "#version 150\n";
	vertex += STRINGIFY(
		uniform mat4 modelViewProjectionMatrix;

	in vec4 position;

	void main() {
		gl_Position = modelViewProjectionMatrix * position;
	}
	);

	string frag = "#version 150\n";
	frag += "#define SRGB_TO_LINEAR(c) pow((c), vec4(2.2))\n";
	frag += "#define LINEAR_TO_SRGB(c) pow((c), vec4(1.0 / 2.2))\n";
	frag += "#define SRGB(r, g, b, a) SRGB_TO_LINEAR(vec4(float(r), float(g), float(b),float(a)) / 255.0)\n";

	frag += STRINGIFY(
		uniform vec4 color1;
	uniform vec4 color2;
	uniform vec4 color3;

	uniform float radius;
	uniform vec2 mouse;
	uniform vec2 begin;
	uniform vec2 end;
	uniform float mid;
	uniform vec2 size;

	out vec4 outputColor;

	// Gradient noise from Jorge Jimenez's presentation:
	// http://www.iryoku.com/next-generation-post-processing-in-call-of-duty-advanced-warfare
	float gradientNoise(vec2 uv)
	{
		const vec3 magic = vec3(0.06711056, 0.00583715, 52.9829189);
		return fract(magic.z * fract(dot(uv, magic.xy)));
	}

	void main() {

		vec4 scolor1 = SRGB(color1.x, color1.y, color1.z, color1.w);
		vec4 scolor2 = SRGB(color2.x, color2.y, color2.z, color2.w);
		vec4 scolor3 = SRGB(color3.x, color3.y, color3.z, color3.w);

		vec2 fragCoord = gl_FragCoord.xy / size;

		// Calculate interpolation factor with vector projection.
		vec2 ba = end - begin;
		float t = dot(fragCoord - begin, ba) / dot(ba, ba);
		// Saturate and apply smoothstep to the factor.
		vec4 color;
		if (t < mid) {
			t = smoothstep(0.0, mid, clamp(t, 0.0, 1.0));
			color = mix(scolor1, scolor2, t);
		}
		else {
			t = smoothstep(mid, 1.0, clamp(t, 0.0, 1.0));
			color = mix(scolor2, scolor3, t);
		}

		// Convert color from linear to sRGB color space (=gamma encode).
		color = LINEAR_TO_SRGB(color);

		// Add gradient noise to reduce banding.
		color += (1.0 / 255.0) * gradientNoise(fragCoord) - (0.5 / 255.0);

		// mask out shader
		if (distance(gl_FragCoord.xy, mouse) > radius) {
			color = vec4(0, 0, 0, 0);
		}

		outputColor = vec4(color);
	}
	);

	shader.unload();
	shader.setupShaderFromSource(GL_VERTEX_SHADER, vertex);
	shader.setupShaderFromSource(GL_FRAGMENT_SHADER, frag);
	shader.bindDefaults();
	shader.linkProgram();

	return shader;
}

const ofShader ofxInterface::getGradientShader2Colors()
{
	ofShader shader;

	string vertex = "#version 150\n";
	vertex += STRINGIFY(
		uniform mat4 modelViewProjectionMatrix;

	in vec4 position;

	void main() {
		gl_Position = modelViewProjectionMatrix * position;
	}
	);

	string frag = "#version 150\n";
	frag += "#define SRGB_TO_LINEAR(c) pow((c), vec4(2.2))\n";
	frag += "#define LINEAR_TO_SRGB(c) pow((c), vec4(1.0 / 2.2))\n";
	frag += "#define SRGB(r, g, b, a) SRGB_TO_LINEAR(vec4(float(r), float(g), float(b),float(a)) / 255.0)\n";

	frag += STRINGIFY(
		uniform vec4 color1;
	uniform vec4 color2;

	uniform vec2 begin;
	uniform vec2 end;
	uniform vec2 size;

	out vec4 outputColor;

	// Gradient noise from Jorge Jimenez's presentation:
	// http://www.iryoku.com/next-generation-post-processing-in-call-of-duty-advanced-warfare
	float gradientNoise(vec2 uv)
	{
		const vec3 magic = vec3(0.06711056, 0.00583715, 52.9829189);
		return fract(magic.z * fract(dot(uv, magic.xy)));
	}

	void main() {

		vec4 scolor1 = SRGB(color1.x, color1.y, color1.z, color1.w);
		vec4 scolor2 = SRGB(color2.x, color2.y, color2.z, color2.w);

		vec2 fragCoord = gl_FragCoord.xy / size;

		// Calculate interpolation factor with vector projection.
		vec2 ba = end - begin;
		float t = dot(fragCoord - begin, ba) / dot(ba, ba);
		// Saturate and apply smoothstep to the factor.
		vec4 color;

		t = smoothstep(0.0, 1.0, clamp(t, 0.0, 1.0));
		color = mix(scolor1, scolor2, t);
		


		// Convert color from linear to sRGB color space (=gamma encode).
		color = LINEAR_TO_SRGB(color);

		// Add gradient noise to reduce banding.
		color += (1.0 / 255.0) * gradientNoise(fragCoord) - (0.5 / 255.0);

		outputColor = vec4(color);
	}
	);

	shader.unload();
	shader.setupShaderFromSource(GL_VERTEX_SHADER, vertex);
	shader.setupShaderFromSource(GL_FRAGMENT_SHADER, frag);
	shader.bindDefaults();
	shader.linkProgram();

	return shader;
}

const ofVec4f ofxInterface::colorToVec4(ofColor c)
{
	return ofVec4f(c.r, c.g, c.b, c.a);
}
