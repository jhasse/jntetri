#include "tetromino.hpp"
#include "field.hpp"
#include "engine/screen.hpp"
#include "engine/random.hpp"

#include <jngl.hpp>

Tetromino::Tetromino(int type, Field& field)
	: field_(field)
{
	assert(0 <= type && type < 7);
	switch(type)
	{
		case 0:{
			RGB color(200, 200, 0);
			blocks_.push_back(Block(-1, 1, color)); // XX
			blocks_.push_back(Block( 0, 1, color)); // XX
			blocks_.push_back(Block(-1, 0, color));
			blocks_.push_back(Block( 0, 0, color));
			rotationCenterX_.push_back(-0.5);
			rotationCenterY_.push_back( 0.5);
		break;}
		case 1:{
			RGB color(200, 0, 0);
			blocks_.push_back(Block(-1, 0, color)); // 1234
			blocks_.push_back(Block( 0, 0, color));
			blocks_.push_back(Block( 1, 0, color));
			blocks_.push_back(Block( 2, 0, color));
			rotationCenterX_.push_back( 0);
			rotationCenterY_.push_back( 0);
			rotationCenterX_.push_back( 0.5);
			rotationCenterY_.push_back(-0.5);
		break;}
		case 2:{
			RGB color(0, 200, 200);
			blocks_.push_back(Block( 0, 1, color)); //  1
			blocks_.push_back(Block(-1, 0, color)); // 234
			blocks_.push_back(Block( 0, 0, color));
			blocks_.push_back(Block( 1, 0, color));
			rotationCenterX_.push_back(-0.5);
			rotationCenterY_.push_back( 0.5);
			rotationCenterX_.push_back( 0);
			rotationCenterY_.push_back( 0);
		break;}
		case 3:{
			RGB color(0, 0, 200);
			blocks_.push_back(Block( 0,  1, color)); //  1
			blocks_.push_back(Block( 0,  0, color)); //  2
			blocks_.push_back(Block( 0, -1, color)); // 43
			blocks_.push_back(Block(-1, -1, color));
			rotationCenterX_.push_back(0);
			rotationCenterY_.push_back(0);
			rotationCenterX_.push_back(-0.5);
			rotationCenterY_.push_back( 0.5);
		break;}
		case 4:{
			RGB color(200, 100, 0);
			blocks_.push_back(Block(-1,  1, color)); // 12
			blocks_.push_back(Block( 0,  1, color)); //  3
			blocks_.push_back(Block( 0,  0, color)); //  4
			blocks_.push_back(Block( 0, -1, color));
			rotationCenterX_.push_back(0);
			rotationCenterY_.push_back(0);
			rotationCenterX_.push_back(-0.5);
			rotationCenterY_.push_back( 0.5);
		break;}
		case 5:{
			RGB color(200, 0, 200);
			blocks_.push_back(Block( 0, 1, color)); //  12
			blocks_.push_back(Block( 1, 1, color)); // 34
			blocks_.push_back(Block(-1, 0, color));
			blocks_.push_back(Block( 0, 0, color));
			rotationCenterX_.push_back(-0.5);
			rotationCenterY_.push_back( 0.5);
			rotationCenterX_.push_back( 0);
			rotationCenterY_.push_back( 0);
		break;}
		case 6:{
			RGB color(0, 200, 0);
			blocks_.push_back(Block(-1, 1, color)); // 12
			blocks_.push_back(Block( 0, 1, color)); //  34
			blocks_.push_back(Block( 0, 0, color));
			blocks_.push_back(Block( 1, 0, color));
			rotationCenterX_.push_back(-0.5);
			rotationCenterY_.push_back( 0.5);
			rotationCenterX_.push_back( 0);
			rotationCenterY_.push_back( 0);
		break;}
	}
	int numberOfRotations = Random::Handle()(4);
	for(int i = 0; i < numberOfRotations; ++i)
	{
		Rotate(CLOCKWISE);
	}
	animationX_ = 0;
	animationY_ = 0;
	rotation_ = 0;
	x_ = 5;
	y_ = 17;
}

void Tetromino::Step()
{
	animationY_ *= 0.8;
	animationX_ *= 0.7;
	rotation_ *= 0.8;
	if(jngl::KeyPressed(jngl::key::Left))
	{
		ChangeX(-1);
		if(Collided())
		{
			ChangeX(1);
		}
	}
	if(jngl::KeyPressed(jngl::key::Right))
	{
		ChangeX(1);
		if(Collided())
		{
			ChangeX(-1);
		}
	}
	if(jngl::KeyPressed(jngl::key::Up))
	{
		Rotate(CLOCKWISE);
	}
	if(jngl::KeyPressed(jngl::key::ControlR))
	{
		Rotate(COUNTERCLOCKWISE);
	}
}

void Tetromino::ChangeX(int c)
{
	x_ += c;
	animationX_ -= c;
}

void Tetromino::ChangeY(int c)
{
	y_ += c;
	animationY_ -= c;
}

void Tetromino::Rotate(const Direction direction)
{
	bool collidedBeforeRotation = Collided();
	std::vector<Block>::iterator end = blocks_.end();
	for(std::vector<Block>::iterator it = blocks_.begin(); it != end; ++it)
	{
		if(direction == CLOCKWISE)
		{
			it->Rotate(direction, rotationCenterX_.front(), rotationCenterY_.front());
		}
		else
		{
			it->Rotate(direction, rotationCenterX_.back(), rotationCenterY_.back());
		}
	}
	if(direction == CLOCKWISE)
	{
		rotationCenterX_.push_back(rotationCenterX_.front());
		rotationCenterX_.pop_front();
		rotationCenterY_.push_back(rotationCenterY_.front());
		rotationCenterY_.pop_front();
	}
	else
	{
		rotationCenterX_.push_back(rotationCenterX_.front());
		rotationCenterX_.pop_front();
		rotationCenterY_.push_back(rotationCenterY_.front());
		rotationCenterY_.pop_front();
	}
	if(direction == CLOCKWISE)
	{
		rotation_ -= 90;
	}
	else
	{
		rotation_ += 90;
	}
	if(!collidedBeforeRotation && Collided())
	{
		ChangeX(1);
		if(Collided())
		{
			ChangeX(-2);
			if(Collided())
			{
				ChangeX(3);
				if(Collided())
				{
					ChangeX(-4);
					if(Collided())
					{
						ChangeX(2);
						// Undo rotation
						if(direction == CLOCKWISE)
						{
							Rotate(COUNTERCLOCKWISE);
						}
						else
						{
							Rotate(CLOCKWISE);
						}
					}
				}
			}
		}
	}
}

void Tetromino::AttachToField()
{
	std::vector<Block>::iterator end = blocks_.end();
	for(std::vector<Block>::iterator it = blocks_.begin(); it != end; ++it)
	{
		it->SetX(it->GetX() + x_);
		it->SetY(it->GetY() + y_);
		it->Flash();
		field_.AddBlock(*it);
	}
}

bool Tetromino::MoveDown()
{
	--y_;
	if(Collided())
	{
		++y_;
		return false;
	}
	animationY_ += 1;
	return true;
}

void Tetromino::Draw() const
{
	jngl::PushMatrix();
	field_.Translate(x_ + animationX_, y_ + animationY_);

	if(rotation_ < 0) // Clockwise
	{
		GetScreen().Translate( rotationCenterX_.back() * field_.GetBlockSize(), -rotationCenterY_.back() * field_.GetBlockSize());
		jngl::Rotate(rotation_);
		GetScreen().Translate(-rotationCenterX_.back() * field_.GetBlockSize(),  rotationCenterY_.back() * field_.GetBlockSize());
	}
	else
	{
		GetScreen().Translate( rotationCenterX_.front() * field_.GetBlockSize(), -rotationCenterY_.front() * field_.GetBlockSize());
		jngl::Rotate(rotation_);
		GetScreen().Translate(-rotationCenterX_.front() * field_.GetBlockSize(),  rotationCenterY_.front() * field_.GetBlockSize());
	}

	std::vector<Block>::const_iterator end = blocks_.end();
	for(std::vector<Block>::const_iterator it = blocks_.begin(); it != end; ++it)
	{
		it->Draw();
	}
	jngl::PopMatrix();
}

bool Tetromino::Collided() const
{
	std::vector<Block>::const_iterator end = blocks_.end();
	for(std::vector<Block>::const_iterator it = blocks_.begin(); it != end; ++it)
	{
		if(field_.CheckCollision(x_ + it->GetX(), y_ + it->GetY()))
		{
			return true;
		}
	}
	return false;
}
