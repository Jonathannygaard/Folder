#version 330 core

out vec4 FragColor;

in vec3 Color;
in vec3 Normal;
in vec3 FragPos;

void main(){
	float AmbStrength = 0.5;
	vec3 LightColor = vec3(1,1,1);
	vec3 Ambient = LightColor * AmbStrength;

	vec3 Normals = normalize(Normal);

	vec3 LightPos = vec3(0,5,0);
	vec3 LightDir = normalize(LightPos - FragPos);

	float Diff = max(dot(Normals, LightDir), 0.0);
	vec3 Diffuse = Diff * LightColor;
	
	FragColor = vec4((Ambient + Diffuse) * Color, 1.0);
}