#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec3 v_Normal;
out vec3 v_FragPos;
out vec2 v_TexCoord;

void main()
{
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
	v_Normal = a_Normal;
	v_FragPos=vec3(u_Transform*vec4(a_Position,1.0));
	v_TexCoord=a_TexCoord;
}

#type fragment
#version 330 core

layout(location = 0) out vec4 FragColor;

uniform vec4 u_AlbedoColor;
in vec3 v_Normal;
in vec3 v_FragPos;
in vec2 v_TexCoord;
uniform sampler2D u_Texture;

//Phong Lighting model
#define MAX_LIGHT_NUM 10

struct Light
{
	vec3 ambient;
};

uniform vec3 u_LightPos;
uniform Light u_Light;
uniform vec3 u_LightColor;
uniform int u_IsLight;

void main()
{
	vec3 norm=normalize(v_Normal);
	vec3 lightDir=normalize(u_LightPos-v_FragPos);
	float diff=max(dot(norm, lightDir),0.0);
	vec3 diffuse=diff*u_LightColor;
	vec4 result=vec4((u_Light.ambient+diffuse),1.0)*u_AlbedoColor;
	FragColor=texture(u_Texture,v_TexCoord)*result+vec4(u_LightColor*u_IsLight,1.0);
}