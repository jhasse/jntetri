#pragma once

#include "engine/work.hpp"
#include "game.hpp"
#include "gui/input.hpp"
#include "highscore.hpp"

#include <boost/shared_ptr.hpp>

namespace jngl {
	class Work;
}

class GameOverScreen : public Work {
public:
	GameOverScreen(Game*);
	void step();
	void draw() const;
	void onQuitEvent();
private:
	bool IsHighscore() const;
	Game* const game_;
	int blink_;
	Data data_;
	Highscore highscore_;
	std::shared_ptr<Input> input_;
	std::shared_ptr<jngl::Work> work_;
};
