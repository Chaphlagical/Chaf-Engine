#type vertex
#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 v_FragPos;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    v_FragPos = aPos;
    gl_Position =  projection * view * vec4(v_FragPos, 1.0);
}

#type fragment
#version 330 core
out vec4 FragColor;
in vec3 v_FragPos;

uniform sampler2D equirectangularMap;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{		
    vec2 uv = SampleSphericalMap(normalize(v_FragPos));
    vec3 color = texture(equirectangularMap, uv).rgb;
    
    FragColor = vec4(color, 1.0);
}

