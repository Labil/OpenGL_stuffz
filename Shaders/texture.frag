#version 330


in vec2 texCoord;
in vec3 LightDirection_cameraspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;

in float lightVertexDistance;


out vec3 out_Color;

uniform sampler2D tex;

uniform vec3 LightPosition_worldspace;


void main()
{
	
	//Light emmision properties. Can be sent as uniforms instead
	vec3 LightColor = vec3(1,1,1);
	float LightPower = 1.0f;

	//Material properties
	vec3 MaterialDiffuseColor = texture2D(tex, texCoord).rgb;
	vec3 MaterialAmbientColor = vec3(0.1, 0.1, 0.1) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3, 0.3, 0.3);


	//Normal of computed fragment, in camera space
	vec3 n = normalize(Normal_cameraspace);

	//Direction of the light (from the fragment to the light)
	vec3 l = normalize(LightDirection_cameraspace);

	float cosTheta = clamp(dot(n, l), 0.0, 1.0);

	//Eye vector (towards the camera)
	vec3 E = normalize(EyeDirection_cameraspace);
	//Direction in which the triangle reflects the light
	vec3 R = reflect(-l, n);

	//Cosine of the angle between the Eye vector and the reflect vector, clamped to 0.
	//Looking into reflection -> 1, looking elsewhere -> < 1
	float cosAlpha = clamp(dot(E, R), 0.0, 1.0);
	float specular = pow(cosAlpha, 5.0);

	//vec3 color = MaterialDiffuseColor * LightColor * LightPower * (cosTheta + specular);

	//out_Color = vec4(color, 1.0);

	//out_Color = MaterialAmbientColor + MaterialDiffuseColor * LightColor * LightPower * cosTheta / (lightVertexDistance * lightVertexDistance) + MaterialSpecularColor * LightColor * LightPower * specular / (lightVertexDistance * lightVertexDistance);
	out_Color = MaterialAmbientColor + MaterialDiffuseColor * LightColor * LightPower * cosTheta + (MaterialSpecularColor * LightColor * LightPower * specular);
	

}

/*in vec2 texCoord;
in vec3 LightDirection_worldspace;
in vec3 Normal_worldspace;

in vec3 clr;

out vec3 out_Color;

uniform sampler2D tex;



void main()
{
	

	vec3 MaterialDiffuseColor = texture2D(tex, texCoord).rgb;

	vec3 n = normalize(Normal_worldspace);

	vec3 l = normalize(LightDirection_worldspace);

	float cosTheta = max(dot(n, l), 0.0);

	out_Color = clr;

}*/



