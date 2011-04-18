#pragma once

#include "engine/work.hpp"
#include "game.hpp"
#include "gui/input.hpp"
#include "highscore.hpp"

#include <boost/shared_ptr.hpp>

class GameOverScreen : public Work {
public:
	GameOverScreen(Game*);
	void Step();
	void Draw() const;
	void QuitEvent();
private:
	bool IsHighscore() const;
	Game* const game_;
	int blink_;
	Data data_;
	Highscore highscore_;
	boost::shared_ptr<Input> input_;
	boost::shared_ptr<Work> work_;
};
