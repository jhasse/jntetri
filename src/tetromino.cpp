#include "tetromino.hpp"
#include "field.hpp"
#include "engine/screen.hpp"

#include <jngl/all.hpp>
#include <random>

Tetromino::Tetromino(int type, Field& field)
: field_(field), recreateShadow(true) {
	assert(0 <= type && type < 7);
	switch(type) {
		case 0:{
			jngl::Color color(200, 200, 0);
			blocks_.push_back(Block(-1, 1, color)); // XX
			blocks_.push_back(Block( 0, 1, color)); // XX
			blocks_.push_back(Block(-1, 0, color));
			blocks_.push_back(Block( 0, 0, color));
			rotationCenterX_.push_back(-0.5);
			rotationCenterY_.push_back( 0.5);
		break;}
		case 1:{
			jngl::Color color(200, 0, 0);
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
			jngl::Color color(0, 200, 200);
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
			jngl::Color color(0, 0, 200);
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
			jngl::Color color(200, 100, 0);
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
			jngl::Color color(200, 0, 200);
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
			jngl::Color color(0, 200, 0);
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

	int numberOfRotations = std::uniform_int_distribution<int>(0,3)(field_.getRandom());
	for (int i = 0; i < numberOfRotations; ++i) {
		Rotate(CLOCKWISE);
	}
	animationX_ = animationY_ = rotation_ = 0; // Clear animation set by Rotate(CLOCKWISE)
	x_ = 5;
	y_ = 17;
}

void Tetromino::Step() {
	animationX_ *= 0.7;
	animationY_ *= 0.8;
	rotation_ *= 0.8;
	if (field_.getControl().Check(control::Left)) {
		ChangeX(-1);
		if (Collided()) {
			ChangeX(1);
		} else {
			recreateShadow = true;
		}
	}
	if (field_.getControl().Check(control::Right)) {
		ChangeX(1);
		if (Collided()) {
			ChangeX(-1);
		} else {
			recreateShadow = true;
		}
	}
	if (field_.getControl().Check(control::Rotate)) {
		Rotate(CLOCKWISE);
		recreateShadow = true;
	}
	if (field_.getControl().Check(control::RotateCounter)) {
		Rotate(COUNTERCLOCKWISE);
		recreateShadow = true;
	}
	if (recreateShadow) {
		int minX = 0;
		int maxX = 0;
		auto end = blocks_.end();
		for (auto it = blocks_.begin(); it != end; ++it) {
			if (it->getX() > maxX) {
				maxX = it->getX();
			}
			if (it->getX() < minX) {
				minX = it->getX();
			}
		}
		field_.clearShadows();
		for (int i = minX; i <= maxX; ++i) {
			int tmpX = x_ + i;
			int tmpY = y_;
			while (!field_.checkCollision(tmpX, tmpY)) {
				--tmpY;
			}
			field_.addShadow(tmpX, tmpY+1);
		}
		recreateShadow = false;
	}
}

void Tetromino::updateShadow() {
}

void Tetromino::SetX(const int x) {
	x_ = x;
}

void Tetromino::ChangeX(int c) {
	x_ += c;
	animationX_ -= c;
}

void Tetromino::ChangeY(int c) {
	y_ += c;
	animationY_ -= c;
}

void Tetromino::Rotate(const Direction direction) {
	bool collidedBeforeRotation = Collided();
	auto end = blocks_.end();
	for (auto it = blocks_.begin(); it != end; ++it) {
		if (direction == CLOCKWISE) {
			it->rotate(direction, rotationCenterX_.front(), rotationCenterY_.front());
		} else {
			it->rotate(direction, rotationCenterX_.back(), rotationCenterY_.back());
		}
	}
	rotationCenterX_.push_back(rotationCenterX_.front());
	rotationCenterX_.pop_front();
	rotationCenterY_.push_back(rotationCenterY_.front());
	rotationCenterY_.pop_front();
	if (direction == CLOCKWISE) {
		rotation_ -= 90;
	} else {
		rotation_ += 90;
	}
	if (!collidedBeforeRotation && Collided()) {
		ChangeX(1);
		if (Collided()) {
			ChangeX(-2);
			if (Collided()) {
				ChangeX(3);
				if (Collided()) {
					ChangeX(-4);
					if (Collided()) {
						ChangeX(2);
						// Undo rotation
						if (direction == CLOCKWISE) {
							Rotate(COUNTERCLOCKWISE);
						} else {
							Rotate(CLOCKWISE);
						}
					}
				}
			}
		}
	}
}

void Tetromino::AttachToField() {
	auto end = blocks_.end();
	for (auto it = blocks_.begin(); it != end; ++it) {
		it->setX(it->getX() + x_);
		it->setY(it->getY() + y_);
		it->setAnimationX(animationX_);
		it->setAnimationY(animationY_);
		it->flash();
		field_.AddBlock(*it);
	}
}

bool Tetromino::MoveDown() {
	--y_;
	if (Collided()) {
		++y_;
		return false;
	}
	animationY_ += 1;
	return true;
}

void Tetromino::moveUp(int amount) {
	y_ += amount;
	animationY_ -= amount;
}

void Tetromino::Draw() const {
	jngl::pushMatrix();
	field_.Translate(x_ + animationX_, y_ + animationY_);

	const int blockSize = field_.getBlockSize();
	if (rotation_ < 0) { // Clockwise
		jngl::translate( rotationCenterX_.back() * blockSize, -rotationCenterY_.back() * blockSize);
		jngl::rotate(rotation_);
		jngl::translate(-rotationCenterX_.back() * blockSize,  rotationCenterY_.back() * blockSize);
	} else {
		jngl::translate( rotationCenterX_.front() * blockSize, -rotationCenterY_.front() * blockSize);
		jngl::rotate(rotation_);
		jngl::translate(-rotationCenterX_.front() * blockSize,  rotationCenterY_.front() * blockSize);
	}

	for (auto& block : blocks_) {
		block.draw();
	}
	jngl::popMatrix();
}

bool Tetromino::Collided() const {
	auto end = blocks_.end();
	for (auto it = blocks_.begin(); it != end; ++it) {
		if (field_.checkCollision(x_ + it->getX(), y_ + it->getY())) {
			return true;
		}
	}
	return false;
}

void Tetromino::drop() {
	while (MoveDown());
	animationX_ = animationY_ = 0;
}
