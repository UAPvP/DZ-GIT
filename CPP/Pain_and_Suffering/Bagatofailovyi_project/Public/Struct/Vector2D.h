#pragma once
struct Vector2D
{
	int x;
	int y;
	Vector2D() : x(0), y(0) {}
	Vector2D(int x, int y) : x(x), y(y) {}
	void AddX(int x) { this->x += x; }
	void AddY(int y) { this->y += y; }
	void DecreaseX(int x) { this->x -= x; }
	void DecreaseY(int y) { this->y -= y; }
};
