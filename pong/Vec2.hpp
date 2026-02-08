#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>



template <typename T>
class Vec2
{
public:
	T x = 0;
	T y = 0;

	Vec2() = default;


	Vec2(T xin, T yin)
		: x(xin), y(yin)
	{
	}


	friend std::ostream& operator<<(std::ostream& stream, const Vec2<T>& other)
	{
		stream << other.x << ", " << other.y;
		return stream;
	}

	Vec2(const sf::Vector2<T>& vec)
		: x(vec.x), y(vec.y)
	{
	}

	Vec2(const sf::Vector2i& vec)
		: x(T(vec.x)), y(T(vec.y))
	{
	}

	operator sf::Vector2<T>()
	{
		return sf::Vector2<T>(x, y);
	}

	Vec2 operator + (const Vec2& rhs) const
	{
		return Vec2(x + rhs.x, y + rhs.y);
	}
	Vec2 operator - (const Vec2& rhs) const
	{
		return Vec2(x - rhs.x, y - rhs.y);
	}
	Vec2 operator * (const Vec2& rhs) const
	{
		return Vec2(x * rhs.x, y * rhs.y);
	}
	Vec2 operator * (const float rhs) const
	{
		return Vec2(x * rhs, y * rhs);
	}
	Vec2 operator / (const Vec2& rhs) const
	{
		return Vec2(x / rhs.x, y / rhs.y);
	}
	Vec2 operator / (float rhs) const
	{
		return Vec2(x / rhs, y / rhs);
	}
	bool operator == (const Vec2& rhs) const
	{
		if (x == rhs.x && y == rhs.y)
		{
			return true;
		}
		return false;
	}
	bool operator != (const Vec2& rhs) const
	{
		if (x != rhs.x || y != rhs.y)
		{
			return true;
		}
		return false;
	}
	void operator += (const Vec2& rhs)
	{
		x += rhs.x;
		y += rhs.y;
	}
	void operator -= (const Vec2& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
	}
	void operator *= (const Vec2& rhs)
	{
		x *= rhs.x;
		y *= rhs.y;
	}
	void operator /= (const Vec2& rhs)
	{
		x /= rhs.x;
		y /= rhs.y;
	}
	float dist(const Vec2& rhs) const
	{
		Vec2 relPos = { rhs.x - x, rhs.y - y };

		float d = sqrtf(relPos.x * relPos.x + relPos.y * relPos.y);
		return d;
	}
	float lenght() const
	{
		float len = sqrtf(x * x + y * y);
		return len;
	}
	void normalize()
	{
		float len = sqrtf(x * x + y * y);
		if (len != 0)
		{
			x /= len;
			y /= len;
		}
	}
	void print()
	{
		std::cout << "(" << x << ", " << y << ")" << "\n";
	}
};

typedef Vec2<float> Vec2f;

