#pragma once

#include "Vec2.hpp"

class Component
{
public:
	bool exits = false;
};

class CTransform : public Component
{
public:
	Vec2f pos = { 0.0f, 0.0f };
	Vec2f velocity = { 0.0f, 0.0f };
	float angle = 0;
	CTransform() = default;
	CTransform(const Vec2f& p, const Vec2f& v, float a); // usar const reference para não copiar o objeto inteiro
};

class CRectangle : public Component
{
public:
	sf::RectangleShape shape;
	CRectangle() = default;
	CRectangle(Vec2f size, const sf::Color& fill, const sf::Color& outline, float thickness);
};
class CCircle: public Component
{
public:
	sf::CircleShape shape;
	CCircle() = default;
	CCircle(float radius, size_t points,const sf::Color& fill, const sf::Color& outline, float thickness);
};
class CCollision : public Component
{
public:
	Vec2f hitBox;
};

class CInput : public Component
{
public:
	bool up = false;
	bool down = false;
};
