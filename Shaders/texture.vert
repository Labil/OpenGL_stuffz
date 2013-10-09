#version 150 core

in vec3 in_Vertex;
in vec3 in_Normals;
in vec2 in_TexCoord0;

//out vec3 LightIntensity;

uniform mat4 modelview;
uniform mat3 NormalMatrix;
uniform mat4 modelviewProjection;

uniform vec4 LightPosition; //Light position in eye coords.
//uniform vec3 Kd;			//Diffuse reflectivity.
//uniform vec3 Ld;			//Diffuse light intensity.

out vec2 texCoord;
out float intensity;

void main()
{
	gl_Position = modelviewProjection * vec4(in_Vertex, 1.0);

	vec3 tnorm = normalize(NormalMatrix * in_Normals);
	vec4 eyeCoords = modelview * vec4(in_Vertex, 1.0);
	vec3 lightDir = normalize(vec3(LightPosition - eyeCoords));

	//LightIntensity = Ld * Kd * max(dot(lightDir, tnorm), 0.0);

	intensity = clamp(dot(tnorm, lightDir), 0, 1);

	texCoord = in_TexCoord0;
}