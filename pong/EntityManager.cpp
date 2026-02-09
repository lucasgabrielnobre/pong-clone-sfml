#include "EntityManager.h"

void EntityManager::removeDeadEntities(EntityVec& vec)
{
	/* 
	Não é possível apenas deletar por causa de Iterator Ivalidation,
	que acontece quando se deleta algo dentro de um vetor enquanto você está
	iterando sobre esse vetor.

	Portanto, a estratégia é percorrer pelas entidades salvando
	todas que devem ser deletadas, e em seguidas remover elas
	na ordem do último para o primeiro (pois de primeiro para o último tem 
	que levar em conta os índices, mais trabalho)
	*/
	std::vector<size_t> entitiesToRemove;
	size_t index = 0;
	for (auto& e : vec)
	{
		if (!e->isAlive())
		{
			entitiesToRemove.push_back(index);
		}
		index++;
	}


	for (int i = (int)entitiesToRemove.size() - 1; i >= 0; i--)
	{
		vec.erase(vec.begin() + entitiesToRemove[(size_t)i]);
	}
}

void EntityManager::update()
{
	for (std::shared_ptr<Entity> e : m_entitiesToAdd)
	{
		m_entities.push_back(e);
		m_entityMap[e->tag()].push_back(e);
	}
	m_entitiesToAdd.clear();

	removeDeadEntities(m_entities);

	for (auto& [tag, entityVec] : m_entityMap)
	{
		removeDeadEntities(entityVec);
	}
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));
	m_entities.push_back(entity);
	return entity;
}

const EntityVec& EntityManager::getEntities() const
{
	return m_entities;
}

const EntityVec& EntityManager::getEntites(const std::string& tag)
{
	return m_entityMap[tag];
}

const EntityMap& EntityManager::getEntityMap()
{
	return m_entityMap;
}
