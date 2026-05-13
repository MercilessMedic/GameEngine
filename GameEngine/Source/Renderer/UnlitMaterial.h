#pragma once
#include "Material.h"
#include "Shaders.h"


class UnlitMaterial : public Material
{
public:
    glm::vec3 color{ 1.0f };

    UnlitMaterial() 
    {
        shaderProgram = Shaders::Unlit;
    };

    void bind()
    {
        shaderProgram->setVec3("color", color);
    }
    Shader& getShader() 
    {
        return *shaderProgram;
    };
    glm::vec3& getColor()
    {
        return color;
    }

private:
    std::shared_ptr<Shader> shaderProgram;
}; 