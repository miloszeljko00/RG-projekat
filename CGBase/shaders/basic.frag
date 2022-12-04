#version 330 core

out vec4 FragColour;

in vertex_out {
	vec3 FragColour;
} FragmentIn;

uniform float offset;

void main() {
	FragColour = vec4(FragmentIn.FragColour * offset, 1.0f);
}