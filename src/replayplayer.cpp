#include "replayplayer.hpp"
#include "replaycontrol.hpp"
#include "game.hpp"
#include "engine/options.hpp"

#include <jngl/input.hpp>

ReplayPlayer::ReplayPlayer(const std::string& filename) : fin_(filename.c_str()) {
	assert(fin_);
	int seed, startLevel, startJunk;
	fin_ >> seed >> startLevel >> startJunk;
	oldStartLevel = GetOptions().Get<int>("startLevel");
	oldStartJunk = GetOptions().Get<int>("startJunks");
	GetOptions().Set("startLevel", startLevel);
	GetOptions().Set("startJunks", startJunk);
	game_.reset(new Game(NORMAL, seed));
	game_->GetField().SetControl(new Control{std::make_shared<ReplayControl>(fin_)});
}

ReplayPlayer::~ReplayPlayer() {
	GetOptions().Set("startLevel", oldStartLevel);
	GetOptions().Set("startJunks", oldStartJunk);
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
