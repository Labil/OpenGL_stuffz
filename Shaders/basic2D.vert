#version 150 core

in vec2 in_Vertex; //OpenGL vet at variabler med ordet "in" foran kommer fra array VertexAttrib

in vec3 in_Color;

uniform mat4 projection;
uniform mat4 modelview;

out vec3 color;


void main()
{
	gl_Position = vec4(in_Vertex, 0.0, 1.0) * projection * modelview;


	color = in_Color;
}