#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include "Entity.h"

typedef std::vector<std::shared_ptr<Entity>> EntityVec; // Todas as entidades
typedef std::map<std::string, EntityVec> EntityMap; // Todas as entidades por tag

class EntityManager
{
	EntityVec m_entities;
	EntityVec m_entitiesToAdd;
	EntityMap m_entityMap;
	size_t    m_totalEntities = 0;

	void removeDeadEntities(EntityVec& vec);
public:
	EntityManager() = default;
	void update();
	std::shared_ptr<Entity> addEntity(const std::string& tag);
	const EntityVec& getEntities() const;
	const EntityVec& getEntites(const std::string& tag);
	const EntityMap& getEntityMap();
};

