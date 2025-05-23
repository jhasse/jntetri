#pragma once

#include "Block.hpp"

#include <jngl/Sprite.hpp>

class Explosion {
public:
	Explosion(const Block&, int numberOfLines);
	void Step();
	void draw(jngl::Mat3 modelview) const;
	bool isFinished() const;
private:
	Block block_;
	int countdown_;
	int numberOfLines_;
	jngl::Sprite sprite;
};
