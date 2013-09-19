#version 150 core

in vec3 in_Vertex;
in vec2 in_TexCoord0;

uniform mat4 modelviewProjection;

out vec2 texCoord;

void main()
{
	gl_Position = modelviewProjection * vec4(in_Vertex, 1.0);

	texCoord = in_TexCoord0;
}