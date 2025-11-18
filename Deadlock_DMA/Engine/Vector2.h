#pragma once

struct Vector2
{
	float x{ 0.0f };
	float y{ 0.0f };

public:
	Vector2 operator-(const Vector2& other) const
	{
		return Vector2{ x - other.x, y - other.y };
	}
	void operator+=(const Vector2& other)
	{
		x += other.x;
		y += other.y;
	}
	void operator*=(const float& other)
	{
		x *= other;
		y *= other;
	}
	bool operator==(const Vector2& other) const
	{
		return (x == other.x) && (y == other.y);
	}
};