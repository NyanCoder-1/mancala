#version 300 es

uniform mat4 matModel;
uniform mat4 matViewProjection;
uniform vec2 uvShift;

in vec3 inPos;
in vec2 inUv;
out vec2 uv;

void main() {
	gl_Position = vec4(inPos, 1.0) * matModel * matViewProjection;
	uv = inUv + uvShift;
}
