#include "ReplayPlayer.hpp"

#include "Game.hpp"
#include "ReplayControl.hpp"
#include "engine/options.hpp"

#include <jngl/input.hpp>

ReplayPlayer::ReplayPlayer(const std::string& filename) : fin_(filename.c_str()) {
	assert(fin_);
	int seed, startLevel, startJunk, gameType;
	fin_ >> seed >> startLevel >> startJunk >> gameType;
	oldStartLevel = getOptions().startLevel;
	oldStartJunk = getOptions().startJunks;
	getOptions().startLevel = startLevel;
	getOptions().startJunks = startJunk;
	game_ = std::make_shared<Game>(static_cast<GameType>(gameType), seed, true);
	game_->GetField().setControl(new Control{std::make_shared<ReplayControl>(fin_)});
}

ReplayPlayer::~ReplayPlayer() {
	getOptions().startLevel = oldStartLevel;
	getOptions().startJunks = oldStartJunk;
}

void ReplayPlayer::step() {
	if (jngl::keyDown('f')) {
		for (int i = 0; i < 19; ++i) {
			game_->step();
		}
	}
	if (jngl::keyDown('s')) {
		for (int i = 0; i < 60; ++i) {
			game_->step();
		}
	}
	if (jngl::keyPressed('e')) {
		while (!game_->gameOver()) {
			game_->step();
		}
	}
	game_->step();
}

void ReplayPlayer::draw() const {
	game_->draw();
}
