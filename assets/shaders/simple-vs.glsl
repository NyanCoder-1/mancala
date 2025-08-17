#version 300 es

uniform mat4 matModel;
uniform mat4 matViewProjection;

in vec3 inPos;
in vec4 inColor;
out vec4 color;

void main() {
	gl_Position = vec4(inPos, 1.0) * matModel * matViewProjection;
	color = inColor;
}
