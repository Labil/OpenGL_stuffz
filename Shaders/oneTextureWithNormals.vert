#version 150 core

in vec3 in_Vertex;
in vec3 in_Color;
in vec2 in_TexCoord0;
in vec3 in_Normals;

uniform mat4 modelviewProjection;
uniform vec3 LightPosition_worldspace;

out vec3 normal;
out vec2 texCoord;

void main()
{
	gl_Position = modelviewProjection * vec4(in_Vertex, 1.0);

	normal = in_Normals;
	texCoord = in_TexCoord0;
}