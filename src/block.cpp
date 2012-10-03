#include "block.hpp"
#include "engine/screen.hpp"

#include <jngl/sprite.hpp>

const int Block::size = 60;

Block::Block(int x, int y, RGB color) : x(x), y(y), animation(0), color(color), flashAnimation(0)
{
}

void Block::step()
{
	animation *= 0.95;
	if(flashAnimation > 0)
	{
		flashAnimation *= 0.96;
	}
}

void Block::setAnimation(double a) {
	animation = a;
}

double Block::getAnimation() const
{
	return animation;
}

void Block::draw() const
{
	jngl::setSpriteColor(static_cast<unsigned char>(color.GetRed() + flashAnimation),
		                 static_cast<unsigned char>(color.GetGreen() + flashAnimation),
						 static_cast<unsigned char>(color.GetBlue() + flashAnimation));
	GetScreen().DrawCentered("block.png", x * size, -(y + animation) * size);
	jngl::setSpriteColor(255, 255, 255);
}

void Block::rotate(Direction direction, double rotationCenterX, double rotationCenterY)
{
	double tempX, tempY;
	double xRot = x - rotationCenterX;
	double yRot = y - rotationCenterY;
	if(direction == CLOCKWISE)
	{
		tempX =  yRot;
		tempY = -xRot;
	}
	else
	{
		tempX = -yRot;
		tempY =  xRot;
	}
	x = static_cast<int>(tempX + rotationCenterX);
	y = static_cast<int>(tempY + rotationCenterY);
}

int Block::getX() const {
	return x;
}

int Block::getY() const {
	return y;
}

void Block::setX(const int x) {
	this->x = x;
}

void Block::setY(const int y) {
	this->y = y;
}

int Block::getSize() const {
	return size;
}

RGB Block::getColor() const {
	return color;
}

void Block::flash() {
	flashAnimation = 50;
}