#pragma once
#include "Components.h"
#include <tuple>

class EntityManager;


// Vai ser usado para verificar se essa entidade tem ou não cada um desses componentes
using ComponentTuple = std::tuple<
	CTransform,
	CCollision,
	CCircle,
	CRectangle,
	CInput,
	CScore
>;

class Entity
{
	friend class EntityManager; // pode acessar a parte privada de EntityManager
	ComponentTuple m_components;
	bool           m_alive = true;
	std::string    m_tag     = "default";
	int            m_id	   = 0;
public:
	Entity() = default;
	Entity(size_t id, const std::string& tag);
	~Entity();

	template <typename T>
	T& get() const; // const -> não muda o contéudo da Classe
	template <typename T, typename... TArgs>
	// ... : 0 ou vários tipos de argumentos
	T& add(TArgs&&... mArgs);
	/*
			&&    : evita cópias(recebe tanto como lvalue quanto como rvalue)
			mArgs : os argumentos em si
	*/
	size_t id() const; 
	bool isAlive() const;
	void destroy();
	const std::string& tag() const;
};
