#version 330 core

out vec4 color;
in vec3 vertexColor;

void main() {
	color.rg = vertexColor.rg;
	color.b = 0.5f;
	color.w = 1.0f;
}
