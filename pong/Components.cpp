#include "Components.h"


CTransform::CTransform(const Vec2f& p, const Vec2f& v, float a)
	: pos(p), velocity(v), angle(a)
{
}

CRectangle::CRectangle(Vec2f size, const sf::Color& fill, const sf::Color& outline, float thickness) 
	: shape(size)
{
	shape.setFillColor(fill);
	shape.setOutlineColor(outline);
	shape.setOutlineThickness(thickness);
	shape.setOrigin( size / 2.0f );
}

CCircle::CCircle(float radius, size_t points, const sf::Color& fill, const sf::Color& outline, float thickness)
	: shape(radius, points)
{
	shape.setFillColor(fill);
	shape.setOutlineColor(outline);
	shape.setOutlineThickness(thickness);
	shape.setOrigin({ radius / 2.0f, radius / 2.0f });
}
