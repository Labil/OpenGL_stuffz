#version 150 core

in vec3 color;

uniform vec3 clr;

out vec4 out_Color;

void main()
{
	out_Color = vec4(clr, 1.0);
	
}