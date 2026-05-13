#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 WorldPos;
in mat3 TBN;

#define MAX_TEXTURE_COUNT 8

struct Material
{
    sampler2D texture_diffuse[MAX_TEXTURE_COUNT];  // adjust max number if needed
    sampler2D texture_specular[MAX_TEXTURE_COUNT];
    sampler2D texture_normal;
    sampler2D texture_height;
    float shininess;
    bool hasNormalMap;
    int numDiffuse;
    int numSpecular;
};

struct Light
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};



uniform Material material;
uniform Light light;

uniform vec3 viewPos;

out vec4 FragColor;



void main()
{
     vec3 norm;
     if( material.hasNormalMap ) 
     { 
       norm = texture(material.texture_normal, TexCoord).rgb; 
       norm = normalize(norm * 2.0 - 1.0); 
       norm = normalize(TBN * norm); 
     } 
     else 
     { 
       norm = normalize(Normal); 
     }

    vec3 lightDir = normalize(light.position - WorldPos);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 diffuseColor = vec3(texture(material.texture_diffuse[0], TexCoord));
    vec3 specularColor = vec3(texture(material.texture_specular[0], TexCoord));

    vec3 diffuse = light.diffuse * diff * diffuseColor;
    vec3 ambient = light.ambient * diffuseColor;

    vec3 viewDir = normalize(viewPos - WorldPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * specularColor;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
    //FragColor = vec4(texture( material.texture_diffuse[0], TexCoord).rgb, 1.0);
	//FragColor = vec4(normalize(norm) * 0.5 + 0.5, 1.0);

}
