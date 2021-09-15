#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec4 o_Position;

void main()
{
	o_Position = vec4(a_Position, 1.0);
	gl_Position=vec4(a_TexCoord,0.0,1.0);
	// o_Position = vec4(a_Position, 1.0);
	// gl_Position=vec4(a_TexCoord,0.0,1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 FragColor;

in vec4 o_Position;

void main()
{
	FragColor=o_Position;
	// FragColor=o_Position;
}