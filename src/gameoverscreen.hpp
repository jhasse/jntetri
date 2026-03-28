#pragma once

#include "engine/work.hpp"
#include "gui/input.hpp"
#include "highscore.hpp"

namespace jngl {
class Work;
} // namespace jngl

class Game;

class GameOverScreen : public Work {
public:
	explicit GameOverScreen(Game*);
	void step() override;
	void draw() const override;
	void onQuitEvent() override;

private:
	bool isHighscore() const;
	Game* const game_;
	int blink_;
	Data data_;
	Highscore highscore_;
	std::shared_ptr<Input> input_;
	std::shared_ptr<jngl::Work> work_;
};
