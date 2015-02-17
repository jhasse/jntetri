#include "block.hpp"
#include "engine/screen.hpp"

#include <jngl/sprite.hpp>

const int Block::size = 60;

Block::Block(int x, int y, jngl::Color color) : x(x), y(y), color(color), flashAnimation(0) {
}

void Block::step() {
	animationX *= 0.7;
	animationY *= 0.95;
	if (flashAnimation > 0) {
		flashAnimation *= 0.96;
	}
}

void Block::setAnimationY(double a) {
	animationY = a;
}

double Block::getAnimationY() const {
	return animationY;
}

void Block::setAnimationX(double a) {
	animationX = a;
}

double Block::getAnimationX() const {
	return animationX;
}

void Block::draw() const {
	jngl::setSpriteColor(static_cast<unsigned char>(color.getRed() + flashAnimation),
		                 static_cast<unsigned char>(color.getGreen() + flashAnimation),
						 static_cast<unsigned char>(color.getBlue() + flashAnimation));
	GetScreen().DrawCentered("block", (x + animationX) * size, -(y + animationY) * size);
	jngl::setSpriteColor(255, 255, 255);
}

void Block::rotate(Direction direction, double rotationCenterX, double rotationCenterY) {
	double tempX, tempY;
	double xRot = x - rotationCenterX;
	double yRot = y - rotationCenterY;
	if (direction == CLOCKWISE) {
		tempX =  yRot;
		tempY = -xRot;
	} else {
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

jngl::Color Block::getColor() const {
	return color;
}

void Block::flash() {
	flashAnimation = 50;
}