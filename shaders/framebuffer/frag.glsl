#version 430

in vec2 texCoords;
out vec4 FragColor;

uniform sampler2D screenTexture;

const float offset = 1.0 / 300.0;

const vec2 mid = vec2(1280 / 2, 720 / 2);

const vec2 offsets[9] = vec2[](
	vec2(-offset, offset), // top-left
	vec2(0.0f, offset), // top-center
	vec2(offset, offset), // top-right
	vec2(-offset, 0.0f),   // center-left
	vec2(0.0f, 0.0f),   // center-center
	vec2(offset, 0.0f),   // center-right
	vec2(-offset, -offset), // bottom-left
	vec2(0.0f, -offset), // bottom-center
	vec2(offset, -offset)  // bottom-right    
	);

void main() {
	/*float kernel[9] = float[](
		1.0 / 16, 2.0 / 16, 1.0 / 16,
		2.0 / 16, 4.0 / 16, 2.0 / 16,
		1.0 / 16, 2.0 / 16, 1.0 / 16
	);
	vec3 sampleTex[9];
	for (int i = 0; i < 9; i++) {
		sampleTex[i] = vec3(texture(screenTexture, texCoords + offsets[i]));
	}
	vec3 col = vec3(0.0);
	for (int i = 0; i < 9; i++) {
		col += sampleTex[i] * kernel[i];
	}
	FragColor = vec4(col, 1.0);*/
	float dist = length(gl_FragCoord.xy - mid);

	FragColor = texture(screenTexture, texCoords)
		+ vec4(texture(screenTexture, texCoords + 0.02 * (texCoords - vec2(0.5))).r, 0.0, 0.0, 1.0)
		+ vec4(0.0, texture(screenTexture, texCoords - 0.02 * (texCoords - vec2(0.5))).g, 0.0, 1.0)
		+ vec4(mod(5.2 * texCoords, 0.09), mod(2.3 * texCoords.x, 0.1), 1.0);
}