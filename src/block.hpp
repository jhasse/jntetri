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
	void setAnimation(double);
	double getAnimation() const;
	int getSize() const;
	jngl::Color getColor() const;
	void flash();

	const static int size;
private:
	int x, y;
	double animation;
	jngl::Color color;
	double flashAnimation;
};
