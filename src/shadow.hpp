#pragma once

class Block;

class Shadow {
public:
	Shadow(int x, int y, const Block*);
	void draw() const;
private:
	int x;
	int y;
	const Block* block;
};