#version 150 core

in vec2 texCoord;

uniform sampler2D tex;

out vec4 out_Color;

in float intensity;
//in vec3 LightIntensity;

void main()
{

	out_Color = texture(tex, texCoord) * intensity;
}