#include "replayplayer.hpp"
#include "replaycontrol.hpp"
#include "game.hpp"
#include "engine/options.hpp"

ReplayPlayer::ReplayPlayer(const std::string& filename) : fin_(filename.c_str()) {
	assert(fin_);
	int seed, startLevel, startJunk;
	fin_ >> seed >> startLevel >> startJunk;
	oldStartLevel = GetOptions().Get<int>("startLevel");
	oldStartJunk = GetOptions().Get<int>("startJunk");
	GetOptions().Set("startLevel", startLevel);
	GetOptions().Set("startJunk", startJunk);
	game_.reset(new Game(NORMAL, seed));
	game_->GetField().SetControl(new ReplayControl(fin_));
}

ReplayPlayer::~ReplayPlayer() {
	GetOptions().Set("startLevel", oldStartLevel);
	GetOptions().Set("startJunk", oldStartJunk);	
}

void ReplayPlayer::Step() {
	game_->Step();
}

void ReplayPlayer::Draw() const {
	game_->Draw();
}
