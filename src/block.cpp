#include "block.hpp"
#include "engine/screen.hpp"

#include <jngl.hpp>

const int Block::size_ = 60;

Block::Block(int x, int y, RGB color) : x_(x), y_(y), animation_(0), color_(color), flash_(0)
{
}

void Block::Step()
{
	animation_ *= 0.95;
	if(flash_ > 0)
	{
		flash_ *= 0.96;
	}
}

void Block::SetAnimation(double animation)
{
	animation_ = animation;
}

double Block::GetAnimation() const
{
	return animation_;
}

void Block::Draw() const
{
	jngl::SetSpriteColor(static_cast<unsigned char>(color_.GetRed() + flash_),
		                 static_cast<unsigned char>(color_.GetGreen() + flash_),
						 static_cast<unsigned char>(color_.GetBlue() + flash_));
	GetScreen().DrawCentered("block.png", x_ * size_, -(y_ + animation_) * size_);
	jngl::SetSpriteColor(255, 255, 255);
}

void Block::Rotate(Direction direction, double rotationCenterX, double rotationCenterY)
{
	double tempX, tempY;
	double x = x_ - rotationCenterX;
	double y = y_ - rotationCenterY;
	if(direction == CLOCKWISE)
	{
		tempX =  y;
		tempY = -x;
	}
	else
	{
		tempX = -y;
		tempY =  x;
	}
	x_ = static_cast<int>(tempX + rotationCenterX);
	y_ = static_cast<int>(tempY + rotationCenterY);
}

int Block::GetX() const
{
	return x_;
}

int Block::GetY() const
{
	return y_;
}

void Block::SetX(const int x)
{
	x_ = x;
}

void Block::SetY(const int y)
{
	y_ = y;
}

int Block::GetSize() const
{
	return size_;
}

RGB Block::GetColor() const
{
	return color_;
}

void Block::Flash()
{
	flash_ = 50;
}
