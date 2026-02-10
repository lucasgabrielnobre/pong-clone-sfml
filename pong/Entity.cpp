#include "Entity.h"

Entity::Entity(size_t id, const std::string& tag)
	: m_id(id), m_tag(tag)
{
}

Entity::~Entity()
{
	std::cout << m_id << ": " << m_tag << std::endl;
}

size_t Entity::id() const
{
	return size_t(m_id);
}

bool Entity::isAlive() const
{
	return m_alive;
}

void Entity::destroy()
{
	m_alive = false;
}

const std::string& Entity::tag() const
{
	return m_tag;
}
/*
template<typename T>
inline T& Entity::get() const
{
	return std::get<T>(m_components);
}
*/
/*
template<typename T, typename ...TArgs> // T é o componente e TArgs são os argumentos
inline T& Entity::add(TArgs && ...mArgs)
{
	auto& component = get<T>();

	component = T(std::forward<TArgs>(mArgs)...);
	
	O foward é usado para passar valores sem perder 
	sua natureza de valor(lvalue e rvalue).

	component = T(mArgs...);
	

	component.exits = true;

	return component;
	}
*/

