#pragma once
#include "Shader.h"

class Material
{
public:
	virtual void bind() = 0;
	virtual Shader& getShader() = 0;
	virtual std::shared_ptr<Texture> getTexture(TextureType type) const
	{
		return nullptr; 
	}	
	virtual ~Material() = default;
};