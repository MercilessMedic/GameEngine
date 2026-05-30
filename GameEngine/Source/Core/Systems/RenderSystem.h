#pragma once
#include "Core/Systems/TransformSystem.h"
#include "Scene/EntityManager.h"
#include "Renderer/Renderer.h"

class RenderSystem
{
public:
	
	//Draw every entity based on their transforms.
	void update( EntityManager& em, Renderer& renderer, std::shared_ptr<CubemapTexture> cubemap)
	{
		//Draw the skybox
		if( cubemap )
		{
			renderer.drawSkybox(cubemap);
		}
		int numPointlights = 0;
		//set all the point light component attributes in the shader
		for (auto it = em.pointlightComponents.begin(); it != em.pointlightComponents.end(); it++)
		{
			Entity lightEntity = it->first;
			PointlightComponent& lightComp = it->second;

			//Get the transform of the light entity 
			//for setting it's position in the shader
			auto transIt = em.transforms.find(lightEntity);
			if (transIt == em.transforms.end()) continue;
			glm::vec3 lightPos = transIt->second.position;

			std::string arrIdx = "pointlights[" + to_string(numPointlights) + "]";
			Shaders::PBR->use();
			Shaders::PBR->setVec3(arrIdx + ".position", lightPos);
			Shaders::PBR->setVec3(arrIdx + ".color", lightComp.color);
			Shaders::PBR->setFloat(arrIdx + ".intensity", lightComp.intensity);
			Shaders::PBR->setFloat(arrIdx + ".radius", lightComp.radius);
			numPointlights++;
		}

		Shaders::PBR->setInt("numPointLights", numPointlights);

		
		//Go through Model components and render
		for(auto it = em.modelComponents.begin(); it != em.modelComponents.end(); it++)
		{
			Entity entity = it->first;
			ModelComponent& modelComp = it->second;
			
			auto transIt = em.transforms.find(entity);
			if( transIt == em.transforms.end()){ continue; }
			TransformComponent& transComp = transIt->second;
			
			renderer.draw( *modelComp.model, transComp.modelMatrix );
		}
		//Go through Mesh components and render
		for( auto it = em.meshComponents.begin(); it != em.meshComponents.end(); it++)
		{
			Entity entity = it->first;
			MeshComponent& meshComp = it->second;

			auto transIt = em.transforms.find(entity);
			if (transIt == em.transforms.end()) { continue; }
			TransformComponent& transComp = transIt->second;

			renderer.draw( *meshComp.mesh, transComp.modelMatrix);
		}

	
	}
};