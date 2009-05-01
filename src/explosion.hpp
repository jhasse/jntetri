#pragma once

#include "block.hpp"

class Explosion {
public:
	Explosion(const Block&, int numberOfLines);
	void Step();
	void Draw() const;
	bool Finished() const;
private:
	Block block_;
	int countdown_;
	int numberOfLines_;
};
