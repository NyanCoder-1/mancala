#version 300 es

precision mediump float;
uniform vec4 diffuseColor;

in vec2 uv;
out vec4 outColor;

uniform sampler2D texture0;

void main() {
	outColor = vec4(diffuseColor.rgb, diffuseColor.a * texture(texture0, uv).r);
}
