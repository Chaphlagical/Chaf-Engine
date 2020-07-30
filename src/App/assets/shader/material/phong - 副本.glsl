#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Bitangent;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 v_TexCoords;
out vec3 v_Normal;
out vec3 v_FragPos;

void main()
{    
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
    v_TexCoords=a_TexCoord;
    v_Normal=mat3(transpose(inverse(u_Transform)))*a_Normal;
    v_FragPos=vec3(u_Transform*vec4(a_Position,1.0));
}

#type fragment
#version 330 core

layout(location = 0) out vec4 FragColor;

in vec2 v_TexCoords;
in vec3 v_Normal;
in vec3 v_FragPos;

struct Material
{
    vec3 color;
    sampler2D diffuse;
    sampler2D specular;
    sampler2D normalMap;
    float shininess;
};

struct Light
{
    vec3 color;
    vec3 position;
    float intensity;
};

struct DirLight
{
    vec3 color;
    vec3 direction;
    float intensity;
};

struct PointLight
{
    vec3 color;
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    float intensity;
};

struct SpotLight
{
    vec3 color;
    vec3 position;
    vec3 direction;
    float cutoff;
    float outerCutOff;
    float intensity;
};

#define MAX_LIGHT_NUM 10

uniform Material u_Material;
uniform Light u_Light[MAX_LIGHT_NUM];
uniform DirLight u_DirLight[MAX_LIGHT_NUM];
uniform PointLight u_PointLight[MAX_LIGHT_NUM];
uniform SpotLight u_SpotLight[MAX_LIGHT_NUM];

uniform int BasicNum;
uniform int DirLightNum;
uniform int PointLightNum;
uniform int SpotLightNum;
uniform vec3 u_ViewPos;
float Ambient=0.1;


//  Calculate normal light
vec3 CalculateLight(Light light)
{
    //  environment light
    vec3 ambient=Ambient*light.color*vec3(texture(u_Material.diffuse,v_TexCoords));

    //  diffuse light
    vec3 norm=normalize(v_Normal);
    vec3 lightDir=normalize(light.position-v_FragPos);
    float diff=max(dot(norm,lightDir),0.0);
    vec3 diffuse=vec3(texture(u_Material.diffuse,v_TexCoords))*light.color*diff;

    //  specular light
    vec3 viewDir=normalize(u_ViewPos-v_FragPos);
    vec3 reflectDir=reflect(-lightDir,norm);
    float spec=pow(max(dot(viewDir,reflectDir),0.0),u_Material.shininess);
    vec3 specular=vec3(texture(u_Material.specular,v_TexCoords))*spec*light.color;

    return (ambient+diffuse+specular)*light.intensity;
}

//  Calculate directional light
vec3 CalculateDirLight(DirLight light)
{
    //  environment light
    vec3 ambient=Ambient*light.color*vec3(texture(u_Material.diffuse,v_TexCoords));

    //  diffuse light
    vec3 norm=normalize(v_Normal);
    vec3 lightDir=normalize(-light.direction);
    float diff=max(dot(norm,lightDir),0.0);
    vec3 diffuse=vec3(texture(u_Material.diffuse,v_TexCoords))*light.color*diff;

    //  specular light
    vec3 viewDir=normalize(u_ViewPos-v_FragPos);
    vec3 reflectDir=reflect(-lightDir,norm);
    float spec=pow(max(dot(viewDir,reflectDir),0.0),u_Material.shininess);
    vec3 specular=vec3(texture(u_Material.specular,v_TexCoords))*spec*light.color;

    return (ambient+diffuse+specular)*light.intensity;
}

//  Calculate point light
vec3 CalculatePointLight(PointLight light)
{
    //  environment light
    vec3 ambient=Ambient*light.color*vec3(texture(u_Material.diffuse,v_TexCoords));

    //  diffuse light
    vec3 norm=normalize(v_Normal);
    vec3 lightDir=normalize(light.position-v_FragPos);
    float diff=max(dot(norm,lightDir),0.0);
    vec3 diffuse=vec3(texture(u_Material.diffuse,v_TexCoords))*light.color*diff;

    //  specular light
    vec3 viewDir=normalize(u_ViewPos-v_FragPos);
    vec3 reflectDir=reflect(-lightDir,norm);
    float spec=pow(max(dot(viewDir,reflectDir),0.0),u_Material.shininess);
    vec3 specular=vec3(texture(u_Material.specular,v_TexCoords))*spec*light.color;

    //  attenuation factor
    float dist=length(light.position-v_FragPos);
    float attenuation=1.0/(light.constant+light.linear*dist+light.quadratic*(dist*dist));
    
    return (ambient+diffuse+specular)*attenuation*light.intensity;
}

//  Calculate Spot light
vec3 CalculateSpotLight(SpotLight light)
{
    //  environment light
    vec3 ambient=Ambient*light.color*vec3(texture(u_Material.diffuse,v_TexCoords));

    //  diffuse light
    vec3 norm=normalize(v_Normal);
    vec3 lightDir=normalize(light.position-v_FragPos);
    float diff=max(dot(norm,lightDir),0.0);

    float theta=dot(lightDir,normalize(light.direction));
    float epsilon=light.cutoff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);   

    vec3 diffuse=vec3(texture(u_Material.diffuse,v_TexCoords))*light.color*diff;
    //  specular light
    vec3 viewDir=normalize(u_ViewPos-v_FragPos);
    vec3 reflectDir=reflect(-lightDir,norm);
    float spec=pow(max(dot(viewDir,reflectDir),0.0),u_Material.shininess);
    vec3 specular=vec3(texture(u_Material.specular,v_TexCoords))*spec*light.color;
    return (ambient+diffuse+specular)*light.intensity*intensity;

}

void main()
{
    vec3 result=vec3(0.0);
    for(int i=0;i<BasicNum;i++)
    {
        result+=CalculateLight(u_Light[i]);
    }
    
    for(int i=0;i<DirLightNum;i++)
    {
        result+=CalculateDirLight(u_DirLight[i]);
    }
    
    for(int i=0;i<PointLightNum;i++)
    {
        result+=CalculatePointLight(u_PointLight[i]);
    }
    
    for(int i=0;i<SpotLightNum;i++)
    {
        result+=CalculateSpotLight(u_SpotLight[i]);
    }
    FragColor=vec4(result,1.0);
}