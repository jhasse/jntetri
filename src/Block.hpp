#pragma once

#include <jngl.hpp>

enum Direction {
	CLOCKWISE,
	COUNTERCLOCKWISE
};

class Block {
public:
	Block(int x, int y, jngl::Color color);
	void step();

	void setSpriteColor() const;
	void draw(jngl::Mat3 modelview) const;

	int getX() const;
	int getY() const;
	void setX(int);
	void setY(int);
	void rotate(Direction, double rotationCenterX, double rotationCenterY);
	void setAnimationX(double);
	void setAnimationY(double);
	double getAnimationX() const;
	double getAnimationY() const;

	/// Returns width/height in dp
	int getSize() const;

	jngl::Color getColor() const;
	void flash();

	/// width/height in dp
	const static int size;
private:
	int x, y;
	double animationX = 0;
	double animationY = 0;
	jngl::Color color;
	double flashAnimation;
	jngl::Sprite sprite{ "gfx/block" };
};
