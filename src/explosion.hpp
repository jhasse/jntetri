#pragma once

#include "block.hpp"

#include <jngl/sprite.hpp>

class Explosion {
public:
	Explosion(const Block&, int numberOfLines);
	void Step();
	void Draw() const;
	bool isFinished() const;
private:
	Block block_;
	int countdown_;
	int numberOfLines_;
	jngl::Sprite sprite;
};
