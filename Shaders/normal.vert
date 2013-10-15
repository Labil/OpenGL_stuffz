#version 330

in vec3 in_Vertex;
in vec3 in_Color;
in vec3 in_Normals;

out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;
out vec3 Normal_cameraspace;

out float lightVertexDistance;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 LightPosition_worldspace;

out vec3 N;

out vec3 color;

void main()
{
	gl_Position = projection * view * model * vec4(in_Vertex, 1.0);

	vec3 Position_worldspace = (model * vec4(in_Vertex, 1)).xyz;
	lightVertexDistance = distance(Position_worldspace, LightPosition_worldspace);

	vec3 vertexPosition_cameraspace = (view * model * vec4(in_Vertex, 1)).xyz;
	EyeDirection_cameraspace = vec3(0.0,0.0,0.0) - vertexPosition_cameraspace;

	vec3 LightPosition_cameraspace = (view * vec4(LightPosition_worldspace, 1)).xyz;
	LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

	Normal_cameraspace = (view * model * vec4(in_Normals, 0.0)).xyz; //Only corret if ModelMatrix does not scale the model. Use its inverse transpose if not.

	N = normalize(Normal_cameraspace);

	color = in_Color;
}
