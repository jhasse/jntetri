#pragma once

#include "engine/rgb.hpp"

enum Direction {
	CLOCKWISE,
	COUNTERCLOCKWISE
};

class Block {
public:
	Block(int x, int y, RGB color);
	void Step();
	void Draw() const;
	int GetX() const;
	int GetY() const;
	void SetX(int);
	void SetY(int);
	void Rotate(Direction, double rotationCenterX, double rotationCenterY);
	void SetAnimation(double);
	double GetAnimation() const;
	int GetSize() const;
	RGB GetColor() const;
	void Flash();
private:
	bool exists_;
	int x_, y_;
	const static int size_;
	double animation_;
	RGB color_;
	double flash_;
};
