#include "tetromino.hpp"

#include "Field.hpp"
#include "engine/screen.hpp"

#include <jngl/all.hpp>
#include <random>

Tetromino::Tetromino(int type, Field& field)
: field_(field),
  shadow(6 * Block::size * jngl::getScaleFactor(), 6 * Block::size * jngl::getScaleFactor()) {
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
	if (field_.getControl().Check(ControlType::Left)) {
		ChangeX(-1);
		if (Collided()) {
			ChangeX(1);
		}
	}
	if (field_.getControl().Check(ControlType::Right)) {
		ChangeX(1);
		if (Collided()) {
			ChangeX(-1);
		}
	}
	if (field_.getControl().Check(ControlType::Rotate)) {
		Rotate(CLOCKWISE);
	}
	if (field_.getControl().Check(ControlType::RotateCounter)) {
		Rotate(COUNTERCLOCKWISE);
	}
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
	{
		const auto context = shadow.use();
		jngl::translate(-jngl::getScreenSize().x / 2., 0);
		jngl::translate(shadow.getSize() / 2);
		jngl::translate(-Block::size / 2, -Block::size / 2);
		shadow.clear();
		jngl::setSpriteColor(0, 0, 0, 10); // 10
		// at y == 310 no tetromino would be visible
		const double STEP_SIZE = 2;
		for (double y = 1; y < 310; y += STEP_SIZE) {
			jngl::translate(0, STEP_SIZE);
			jngl::pushMatrix();
			drawBlocks();
			jngl::popMatrix();
		}
		jngl::setSpriteAlpha(255);
	}
	jngl::pushMatrix();
	field_.Translate(x_ + animationX_, y_ + animationY_);
	assert(!blocks_.empty());
	blocks_.front().setSpriteColor();
	drawBlocks();
	jngl::setSpriteColor(255, 255, 255);
	jngl::popMatrix();
}

void Tetromino::drawShadow() const {
	jngl::pushMatrix();
	field_.Translate(x_ + animationX_, y_ + animationY_);

	jngl::translate(-Block::size * 2.5, -Block::size * 2.5);
	jngl::setSpriteAlpha(50);
	const float width = shadow.getSize().x;
	const float height = shadow.getSize().y;
	const float extensionHeight = 900; // just long enough that you won't see the end
	shadow.drawMesh({
		// top right triangle
		jngl::Vertex{ .x = 0,     .y = 0,      .u = 0, .v = 1 },
		jngl::Vertex{ .x = width, .y = 0,      .u = 1, .v = 1 },
		jngl::Vertex{ .x = width, .y = height, .u = 1, .v = 0 },

		// bottom left triangle
		jngl::Vertex{ .x = 0,     .y = 0,      .u = 0, .v = 1 },
		jngl::Vertex{ .x = 0,     .y = height, .u = 0, .v = 0 },
		jngl::Vertex{ .x = width, .y = height, .u = 1, .v = 0 },

		// Now extend the end of the shadow (v == 0) to the bottom of the field:
		// top right triangle
		jngl::Vertex{ .x = 0,     .y = height + 0,               .u = 0, .v = 0 },
		jngl::Vertex{ .x = width, .y = height + 0,               .u = 1, .v = 0 },
		jngl::Vertex{ .x = width, .y = height + extensionHeight, .u = 1, .v = 0 },

		// bottom left triangle
		jngl::Vertex{ .x = 0,     .y = height + 0,               .u = 0, .v = 0 },
		jngl::Vertex{ .x = 0,     .y = height + extensionHeight, .u = 0, .v = 0 },
		jngl::Vertex{ .x = width, .y = height + extensionHeight, .u = 1, .v = 0 },
	});
	jngl::setSpriteAlpha(255);

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

void Tetromino::drawBlocks() const {
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
}
