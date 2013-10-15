#version 330

in vec3 color;

in vec3 LightDirection_cameraspace;
in vec3 Normal_cameraspace;
in vec3 EyeDirection_cameraspace;

in float lightVertexDistance;

//uniform vec3 MaterialDiffuseColor;
out vec3 out_Color;
in vec3 N;


void main()
{

	//Light emmision properties. Can be sent as uniforms instead
	vec3 LightColor = vec3(1,1,1);
	float LightPower = 1.5f;

	//Material properties
	vec3 MaterialDiffuseColor = color;
	vec3 MaterialAmbientColor = vec3(0.1, 0.1, 0.1) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(1, 1, 1);


	//Normal of computed fragment, in camera space
	vec3 n = normalize(Normal_cameraspace);

	//Direction of the light (from the fragment to the light)
	vec3 l = normalize(LightDirection_cameraspace);

	float cosTheta = clamp(dot(n, l), 0, 1);

	/*vec3 clr;

	if(cosTheta > 0.95)
		clr = vec3(1.0, 0.5, 0.5);
	else if(cosTheta > 0.5)
		clr = vec3(0.6, 0.3, 0.3);
	else if(cosTheta > 0.25)
		clr = vec3(0.4, 0.2, 0.2);
	else
		clr = vec3(0.2, 0.1, 0.1);*/

	//Eye vector (towards the camera)
	vec3 E = normalize(EyeDirection_cameraspace);
	//Direction in which the triangle reflects the light
	vec3 R = reflect(-l, N);

	//Cosine of the angle between the Eye vector and the reflect vector, clamped to 0.
	//Looking into reflection -> 1, looking elsewhere -> < 1
	float cosAlpha = clamp(dot(E, R), 0.0, 1.0);
	float specular = pow(cosAlpha, 5.0);

	out_Color = MaterialAmbientColor + MaterialDiffuseColor * LightColor * LightPower * cosTheta + (MaterialSpecularColor * LightColor * LightPower * specular);
	//out_Color = clr * cosTheta;
}
