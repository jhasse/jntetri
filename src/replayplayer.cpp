#include "replayplayer.hpp"
#include "replaycontrol.hpp"
#include "game.hpp"

ReplayPlayer::ReplayPlayer(const std::string& filename) : fin_(filename.c_str()) {
	assert(fin_);
	int seed;
	fin_ >> seed;
	game_.reset(new Game(NORMAL, seed));
	game_->GetField().SetControl(new ReplayControl(fin_));
}

void ReplayPlayer::Step() {
	game_->Step();
}

void ReplayPlayer::Draw() const {
	game_->Draw();
}
