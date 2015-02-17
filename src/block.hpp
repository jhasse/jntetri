#pragma once

#include <jngl/color.hpp>

enum Direction {
	CLOCKWISE,
	COUNTERCLOCKWISE
};

class Block {
public:
	Block(int x, int y, jngl::Color color);
	void step();
	void draw() const;
	int getX() const;
	int getY() const;
	void setX(int);
	void setY(int);
	void rotate(Direction, double rotationCenterX, double rotationCenterY);
	void setAnimationX(double);
	void setAnimationY(double);
	double getAnimationX() const;
	double getAnimationY() const;
	int getSize() const;
	jngl::Color getColor() const;
	void flash();
	
	const static int size;
private:
	bool exists;
	int x, y;
	double animationX = 0;
	double animationY = 0;
	jngl::Color color;
	double flashAnimation;
};