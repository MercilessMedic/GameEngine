#pragma once

#include "EntityManager.h"
#include "Renderer/Renderer.h"
#include "Core/Systems/RenderSystem.h"
#include "Core/Systems/TransformSystem.h"
class Scene
{
public:
	
	void update( Renderer &renderer )
	{
		transformSystem.update( entityManager );
		renderSystem.update( entityManager, renderer);
	}
	
	Entity createEntity()
	{
		return entityManager.createEntity();
	}

	void addTransform( Entity e, const TransformComponent& transform )
	{
		entityManager.addTransformComponent( e, transform );
	}

	void addModel( Entity e, const ModelComponent& modelComp )
	{
		entityManager.addModelComponent( e, modelComp );
	}

	void addMesh( Entity e, const MeshComponent& meshComp )
	{
		entityManager.addMeshComponent(e, meshComp);
	}

	TransformComponent* getTransformComp( Entity e )
	{
		return entityManager.getTransformComponent( e );
	}

	ModelComponent* getModelComp( Entity e )
	{
		return entityManager.getModelComponent(e);
	}

	MeshComponent* getMeshComp( Entity e)
	{
		return entityManager.getMeshComponent(e);
	}

	EntityManager& getEntityManager()
	{
		return entityManager;
	}

private:
	EntityManager entityManager;
	RenderSystem renderSystem;
	TransformSystem transformSystem;

};