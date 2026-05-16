#pragma once
#include "Core/Systems/TransformSystem.h"
#include "Scene/EntityManager.h"
#include "Renderer/Renderer.h"

class RenderSystem
{
public:
	void update( EntityManager& em, Renderer& renderer)
	{
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