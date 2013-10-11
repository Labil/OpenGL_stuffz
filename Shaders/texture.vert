#version 330

in vec3 in_Vertex;
in vec3 in_Normals;
in vec2 in_TexCoord0;


//out vec3 Position_worldspace;
out vec2 texCoord;
out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;
out vec3 Normal_cameraspace;

out float lightVertexDistance;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 LightPosition_worldspace;

void main()
{
	
	gl_Position = projection * view * model * vec4(in_Vertex, 1.0);

	vec3 Position_worldspace = (model * vec4(in_Vertex, 1)).xyz;
	lightVertexDistance = distance(Position_worldspace, LightPosition_worldspace);

	vec3 vertexPosition_cameraspace = (view * model * vec4(in_Vertex, 1)).xyz;
	EyeDirection_cameraspace = vec3(0.0,0.0,0.0) - vertexPosition_cameraspace;

	

	//Vector from the vertex to the light, in camera space. model is omitted because it's identidy
	vec3 LightPosition_cameraspace = (view * vec4(LightPosition_worldspace, 1)).xyz;
	//LightDirection_cameraspace = LightPosition_cameraspace  + EyeDirection_cameraspace;
	//LightDirection_cameraspace = LightPosition_cameraspace  + Position_cameraspace;
	//LightDirection_cameraspace = LightPosition_cameraspace  + vertexPosition_cameraspace; <- denne burde funke
	LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

	//Normal of vertex, in camera space
	Normal_cameraspace = (view * model * vec4(in_Normals, 0.0)).xyz; //Only corret if ModelMatrix does not scale the model. Use its inverse transpose if not.

	texCoord = in_TexCoord0;

}

/*in vec3 in_Vertex;
in vec3 in_Normals;
in vec2 in_TexCoord0;

out vec2 texCoord;

out vec3 LightDirection_worldspace;
out vec3 Normal_worldspace;

out vec3 clr;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 LightPosition_worldspace;

void main()
{
	
	gl_Position = projection * view * model * vec4(in_Vertex, 1.0);

	

	LightDirection_worldspace = LightPosition_worldspace - in_Vertex;
	

	Normal_worldspace = in_Normals;

	texCoord = in_TexCoord0;
	clr = LightPosition_worldspace

}*/


