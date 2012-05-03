#pragma once

#include "block.hpp"

#include <vector>
#include <deque>

class Field;

class Tetromino {
public:
	Tetromino(int type, Field&);
	void Step();
	void Draw() const;
	bool Collided() const;
	bool MoveDown();
	void AttachToField();
	void Rotate(Direction);
	void RandomRotate();
	void SetX(int);
private:
	void ChangeX(int);
	void ChangeY(int);
	void updateShadow();
	
	std::vector<Block> blocks_;
	Field& field_;
	int x_, y_;
	double animationX_;
	double animationY_;
	double rotation_;
	std::deque<int> positionTweaksX_; // These values help correctly rotate the tetromino because
	std::deque<int> positionTweaksY_; // in nearly all Tetris versions it isn't a real rotation.
	std::deque<double> rotationCenterX_;
	std::deque<double> rotationCenterY_;
	bool recreateShadow;
};
