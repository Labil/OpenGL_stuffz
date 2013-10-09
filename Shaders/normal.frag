#version 150 core

in vec3 color;

out vec4 out_Color;

///in vec3 LightIntensity;
in float intensity;

void main()
{

	out_Color = vec4(color, 1.0) * intensity;
}