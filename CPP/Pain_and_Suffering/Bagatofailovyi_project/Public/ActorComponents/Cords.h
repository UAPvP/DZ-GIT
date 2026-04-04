#pragma once

#include "ActorComponent.h"
#include "../Struct/Vector2D.h"

class Cords : public ActorComponent
{
	Vector2D position;

public:
	Cords(Actor* owner, const Vector2D& position) : ActorComponent(owner)
	{
		this->position = position;
	}

	Vector2D getPosition() const
	{
		return position;
	}

	void setPosition(const Vector2D& position)
	{
		this->position = position;
	}

	bool hasSamePosition(const Cords& object) const
	{
		return position.x == object.position.x && position.y == object.position.y;
	}

	bool isNear(const Cords& object) const
	{
		int dx = position.x - object.position.x;
		int dy = position.y - object.position.y;

		return dx * dx + dy * dy <= 1;
	}
};
