#version 300 es

precision mediump float;

in vec2 uv;
out vec4 outColor;

uniform sampler2D texture0;

void main() {
	outColor = texture(texture0, uv);
}
