#pragma once

#include "engine/rgb.hpp"

enum Direction {
	CLOCKWISE,
	COUNTERCLOCKWISE
};

class Block {
public:
	Block(int x, int y, RGB color);
	void step();
	void draw() const;
	int getX() const;
	int getY() const;
	void setX(int);
	void setY(int);
	void rotate(Direction, double rotationCenterX, double rotationCenterY);
	void setAnimation(double);
	double getAnimation() const;
	int getSize() const;
	RGB getColor() const;
	void flash();
	
	const static int size;
private:
	bool exists;
	int x, y;
	double animation;
	RGB color;
	double flashAnimation;
};