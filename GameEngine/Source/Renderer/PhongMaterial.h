#include "Material.h"
#include "Texture.h"
#include "Shaders.h"
class PhongMaterial : public Material
{
public:
	PhongMaterial()
	{
		shaderProgram = Shaders::Phong;
        Shininess = 32.0;
	}
    void bind() override
    {
        int diffuseCount = 0;
        int specularCount = 0;
        bool hasNormalMap = false;
        
        // Zero-initialize all sampler slots first
        for (int i = 0; i < 8; i++)
        {
            shaderProgram->setInt("material.texture_diffuse[" + std::to_string(i) + "]", 0);
            shaderProgram->setInt("material.texture_specular[" + std::to_string(i) + "]", 0);
        }
        
        int unit = 0; // global texture unit counter
        for (int i = 0; i < textures.size(); i++)
        {
            textures[i]->bind(unit);

            std::string uniformName;
            TextureType type = textures[i]->getType();
            if (type == TextureType::DIFFUSE)
            { 
                uniformName = "material.texture_diffuse[" + std::to_string(diffuseCount) + "]";
                diffuseCount++; 
            }
            else if (type == TextureType::SPECULAR) 
            {
                uniformName = "material.texture_specular[" + std::to_string(specularCount) + "]";
                specularCount++;
            }
            else if (type == TextureType::NORMAL) 
            {
                uniformName = "material.texture_normal";
                hasNormalMap = true;
            }
            else if (type == TextureType::HEIGHT) 
            {
                uniformName = "material.texture_height";
            }

            shaderProgram->setInt(uniformName, unit); // use unit, not i
            unit++;
        }

        shaderProgram->setBool("material.hasNormalMap", hasNormalMap);
        shaderProgram->setInt("material.numDiffuse", diffuseCount);
        shaderProgram->setInt("material.numSpecular", specularCount);

        // upload shininess
        shaderProgram->setFloat("material.shininess", Shininess);
    }

	void setTextures(const std::vector<std::shared_ptr<Texture>>& tex)
	{
		textures = tex;
	}

	const std::vector<std::shared_ptr<Texture>>& getTextures() const
	{
		return textures;
	}
	
    float getShininess()
    {
        return Shininess;
    }

    void setShininess(float shininess)
    {
        Shininess = shininess;
    }

	Shader& getShader() override
	{
		return *shaderProgram;
	}
private:
	std::shared_ptr<Shader> shaderProgram;	
	std::vector<std::shared_ptr<Texture>> textures;
	float Shininess;
};