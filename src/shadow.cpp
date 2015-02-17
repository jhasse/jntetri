#include "shadow.hpp"
#include "engine/screen.hpp"
#include "block.hpp"

Shadow::Shadow(int x, int y, const Block* block) : x(x), y(y), block(block) {
}

void Shadow::draw() const {
	double tmpY = y;
	if (block) {
		tmpY += block->getAnimationY();
	}
	GetScreen().DrawCentered("shadow", x * Block::size, -tmpY * Block::size);
}