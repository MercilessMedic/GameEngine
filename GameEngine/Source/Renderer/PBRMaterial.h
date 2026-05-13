
#include "Material.h"
#include "Texture.h"
#include "Shaders.h"

class PBRMaterial : public Material
{
public:
    PBRMaterial()  
    {
        shaderProgram = Shaders::PBR;

    }

    //Bind the material and set all uniforms for the shader
    void bind() override
    {
        if (!shaderProgram) return;

        if (albedoMap)
        {
            albedoMap->bind(0);
            shaderProgram->setInt("material.albedoMap", 0);
            shaderProgram->setBool("material.hasAlbedoMap", true);
        }
        else
        {
            shaderProgram->setBool("material.hasAlbedoMap", false);
            shaderProgram->setVec3("material.albedoColor", albedoColor);
        }

        // Normal map
        if (normalMap)
        {
            normalMap->bind(1);
            shaderProgram->setInt("material.normalMap", 1);
            shaderProgram->setBool("material.hasNormalMap", true);
        }
        else
        {
            shaderProgram->setBool("material.hasNormalMap", false);
        }

        if (metallicMap)
        {
            metallicMap->bind(2);
            shaderProgram->setInt("material.metallicMap", 2);
            shaderProgram->setBool("material.hasMetallicMap", true);
        }
        else
        {
            shaderProgram->setBool("material.hasMetallicMap", false);
            shaderProgram->setFloat("material.metallic", metallic);
        }
        if (roughnessMap)
        {
            roughnessMap->bind(3);
            shaderProgram->setInt("material.roughnessMap", 3);
            shaderProgram->setBool("material.hasRoughnessMap", true);
        }
        else
        {
            shaderProgram->setBool("material.hasRoughnessMap", false);
            shaderProgram->setFloat("material.roughness", roughness);
        }
        if (aoMap)
        {
            aoMap->bind(4);
            shaderProgram->setInt("material.aoMap", 4);
            shaderProgram->setBool("material.hasAOMap", true);
        }
        else
        {
            shaderProgram->setBool("material.hasAOMap", false);
            shaderProgram->setFloat("material.ao", ao);
        }
    }

    Shader& getShader() override
    {
        return *shaderProgram;
    }

    std::shared_ptr<Texture> albedoMap;
    std::shared_ptr<Texture> normalMap;
    std::shared_ptr<Texture> metallicMap;
    std::shared_ptr<Texture> roughnessMap;
    std::shared_ptr<Texture> aoMap;

    glm::vec3 albedoColor = glm::vec3(1.0f);
    float metallic = 0.0f;
    float roughness = 0.5f;
    float ao = 1.0f;

private:
    std::shared_ptr<Shader> shaderProgram;
};