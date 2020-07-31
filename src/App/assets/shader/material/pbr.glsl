#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in vec3 a_Tangent;
layout(location = 4) in vec3 a_Bitangent;

struct Material
{
    vec3 color;
    sampler2D albedoMap;
    sampler2D normalMap;
    sampler2D metallicMap;
    sampler2D roughnessMap;
    sampler2D aoMap;
    sampler2D displacementMap;
    float metallic;
    float roughness;
};

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;
uniform float u_HeightScale;
uniform Material u_Material;

out vec2 v_TexCoords;
out vec3 v_Normal;
out vec3 v_FragPos;

void main()
{    
    float height=texture(u_Material.displacementMap,a_TexCoord).r;
    height=u_HeightScale*height;

    v_Normal=mat3(transpose(inverse(u_Transform)))*a_Normal;
    v_FragPos=vec3(u_Transform*vec4(a_Position,1.0));
    v_TexCoords=a_TexCoord;
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position+a_Normal*height, 1.0);
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
    sampler2D albedoMap;
    sampler2D normalMap;
    sampler2D metallicMap;
    sampler2D roughnessMap;
    sampler2D aoMap;
    sampler2D displacementMap;
    float metallic;
    float roughness;
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
uniform int u_HasNormalMap;

// IBL
uniform samplerCube u_irradianceMap;
uniform samplerCube u_prefilterMap;
uniform sampler2D u_brdfLUT;

const float PI=3.14159265359;

vec3 getNormalFromMap()
{
    vec3 tangentNormal=texture(u_Material.normalMap,v_TexCoords).xyz*2.0-1.0;

    vec3 Q1=dFdx(v_FragPos);
    vec3 Q2=dFdy(v_FragPos);
    vec2 st1=dFdx(v_TexCoords);
    vec2 st2=dFdx(v_TexCoords);

    vec3 N=normalize(v_Normal);
    vec3 T=normalize(Q1*st2.t-Q2*st1.t);
    vec3 B=-normalize(cross(N,T));
    mat3 TBN=mat3(T,B,N);

    return normalize(TBN*tangentNormal);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a=roughness*roughness;
    float a2=a*a;
    float NdotH=max(dot(N,H),0.0);
    float NdotH2=NdotH*NdotH;

    float nom=a2;
    float denom=(NdotH2*(a2-1.0)+1.0);
    denom=PI*denom*denom;

    return nom/denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float nom=NdotV;
    float denom=NdotV*(1.0-roughness)+roughness;

    return nom/denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV=max(dot(N,V),0.0);
    float NdotL=max(dot(N,L),0.0);
    float ggx1=GeometrySchlickGGX(NdotV,roughness);
    float ggx2=GeometrySchlickGGX(NdotL,roughness);
    return ggx1*ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0+(1.0-F0)*pow(1.0-cosTheta,5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0+(max(vec3(1.0-roughness),F0)*pow(1.0-cosTheta,5.0));
}

vec3 CalculateLight(vec3 albedo, float metallic, float roughness, float ao, vec3 N, vec3 V, vec3 R, vec3 F0, Light light)
{
    vec3 L=normalize(light.position-v_FragPos);
    vec3 H=normalize(V+L);
    vec3 radiance=light.color*light.intensity;

    // cook-torrance BRDF
    float NDF=DistributionGGX(N,H,roughness);
    float G=GeometrySmith(N,V,L,roughness);
    vec3 F=fresnelSchlick(max(dot(H,V),0.0),F0);

    vec3 nominator=NDF*G*F;
    float denominator=4*max(dot(N,V),0.0)*max(dot(N,L),0.0)+0.001;
    vec3 specular=nominator/denominator;

    vec3 kS=F;
    vec3 kD=vec3(1.0)-kS;
    kD*=1.0-metallic;

    float NdotL=max(dot(N,L),0.0);
    return (kD*albedo/PI+specular)*radiance*NdotL;
}

vec3 CalculatePointLight(vec3 albedo, float metallic, float roughness, float ao, vec3 N, vec3 V, vec3 R, vec3 F0, PointLight light)
{
    vec3 L=normalize(light.position-v_FragPos);
    vec3 H=normalize(V+L);
    float dist=length(light.position-v_FragPos);
    float attenuation=1.0/(light.constant+light.linear*dist+light.quadratic*(dist*dist));
    vec3 radiance=light.color*light.intensity*attenuation;

    // cook-torrance BRDF
    float NDF=DistributionGGX(N,H,roughness);
    float G=GeometrySmith(N,V,L,roughness);
    vec3 F=fresnelSchlick(max(dot(H,V),0.0),F0);

    vec3 nominator=NDF*G*F;
    float denominator=4*max(dot(N,V),0.0)*max(dot(N,L),0.0)+0.001;
    vec3 specular=nominator/denominator;

    vec3 kS=F;
    vec3 kD=vec3(1.0)-kS;
    kD*=1.0-metallic;

    float NdotL=max(dot(N,L),0.0);
    return (kD*albedo/PI+specular)*radiance*NdotL;
}

vec3 CalculateDirLight(vec3 albedo, float metallic, float roughness, float ao, vec3 N, vec3 V, vec3 R, vec3 F0, DirLight light)
{
    vec3 L=normalize(light.direction);
    vec3 H=normalize(V+L);
    vec3 radiance=light.color*light.intensity;

    // cook-torrance BRDF
    float NDF=DistributionGGX(N,H,roughness);
    float G=GeometrySmith(N,V,L,roughness);
    vec3 F=fresnelSchlick(max(dot(H,V),0.0),F0);

    vec3 nominator=NDF*G*F;
    float denominator=4*max(dot(N,V),0.0)*max(dot(N,L),0.0)+0.001;
    vec3 specular=nominator/denominator;

    vec3 kS=F;
    vec3 kD=vec3(1.0)-kS;
    kD*=1.0-metallic;

    float NdotL=max(dot(N,L),0.0);
    return (kD*albedo/PI+specular)*radiance*NdotL;
}

vec3 CalculateSpotLight(vec3 albedo, float metallic, float roughness, float ao, vec3 N, vec3 V, vec3 R, vec3 F0, SpotLight light)
{
    vec3 L=normalize(light.position-v_FragPos);
    vec3 H=normalize(V+L);
    float theta=dot(L,normalize(light.direction));
    float epsilon=light.cutoff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);   
    vec3 radiance=light.color*light.intensity*intensity;

    // cook-torrance BRDF
    float NDF=DistributionGGX(N,H,roughness);
    float G=GeometrySmith(N,V,L,roughness);
    vec3 F=fresnelSchlick(max(dot(H,V),0.0),F0);

    vec3 nominator=NDF*G*F;
    float denominator=4*max(dot(N,V),0.0)*max(dot(N,L),0.0)+0.001;
    vec3 specular=nominator/denominator;

    vec3 kS=F;
    vec3 kD=vec3(1.0)-kS;
    kD*=1.0-metallic;

    float NdotL=max(dot(N,L),0.0);
    return (kD*albedo/PI+specular)*radiance*NdotL;
}

void main()
{
    vec3 albedo=pow(texture(u_Material.albedoMap,v_TexCoords).rgb*u_Material.color,vec3(2.2));
    float metallic=texture(u_Material.metallicMap,v_TexCoords).r*u_Material.metallic;
    float roughness=texture(u_Material.roughnessMap,v_TexCoords).r*u_Material.roughness;
    float ao=texture(u_Material.aoMap,v_TexCoords).r;


    vec3 N=vec3(0.0);
    if(u_HasNormalMap>0)
        N=getNormalFromMap();
    else N=normalize(v_Normal);
    vec3 V=normalize(u_ViewPos-v_FragPos);
    vec3 R=reflect(-V,N);

    vec3 F0=vec3(0.0);
    F0=mix(F0,albedo,metallic);

    //reflactance equation
    vec3 Lo=vec3(0.0);
    
    for(int i=0;i<BasicNum;i++)
    {
        Lo+=CalculateLight(albedo, metallic, roughness, ao, N, V, R, F0, u_Light[i]);
    }

    for(int i=0;i<DirLightNum;i++)
    {
        Lo+=CalculateDirLight(albedo, metallic, roughness, ao, N, V, R, F0, u_DirLight[i]);
    }
    
    for(int i=0;i<PointLightNum;i++)
    {
        Lo+=CalculatePointLight(albedo, metallic, roughness, ao, N, V, R, F0, u_PointLight[i]);
    }
    
    for(int i=0;i<SpotLightNum;i++)
    {
        Lo+=CalculateSpotLight(albedo, metallic, roughness, ao, N, V, R, F0, u_SpotLight[i]);
    }
    
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

    vec3 kS=F;
    vec3 kD=1.0-kS;
    kD*=1.0-metallic;
    vec3 irradiance=texture(u_irradianceMap,N).rgb;
    vec3 diffuse=irradiance*albedo;

    const float MAX_REFLECTION_LOD=4.0;
    vec3 prefilteredColor=textureLod(u_prefilterMap,R,roughness*MAX_REFLECTION_LOD).rgb;
    vec2 brdf=texture(u_brdfLUT,vec2(max(dot(N,V),0.0),roughness)).rg;
    vec3 specular=prefilteredColor*(F*brdf.x+brdf.y);

    vec3 ambient=(kD*diffuse+specular)*ao;
    
    //vec3 ambient=vec3(0.03)*albedo*ao;

    vec3 color=(ambient+Lo);
    color=color/(color+vec3(1.0));
    color=pow(color,vec3(1.0/2.2));

    FragColor=vec4(color,1.0);
}