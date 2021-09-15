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

uniform samplerCube environmentMap;

const float PI=3.14159265359;

void main()
{
    vec3 N=normalize(v_FragPos);
    vec3 irradiance=vec3(0.0);

    vec3 up=vec3(0.0,1.0,0.0);
    vec3 right=cross(up,N);
    up=cross(N,right);

    float sampleData=0.025;
    float nrSample=0.0;

    for(float phi=0.0;phi<2.0*PI;phi+=sampleData)
    {
        for(float theta=0.0;theta<0.5*PI;theta+=sampleData)
        {
            vec3 tangentSample=vec3(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));
            vec3 sampleVec=tangentSample.x*right+tangentSample.y*up+tangentSample.z*N;

            irradiance+=texture(environmentMap,sampleVec).rgb*cos(theta)*sin(theta);
            nrSample++;
        }
    }
    irradiance=PI*irradiance*(1.0/float(nrSample));
    FragColor=vec4(irradiance,1.0);
}