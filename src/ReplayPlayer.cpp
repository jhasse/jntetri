#include "ReplayPlayer.hpp"

#include "Game.hpp"
#include "ReplayControl.hpp"
#include "engine/Options.hpp"

#include <jngl/input.hpp>

ReplayPlayer::ReplayPlayer(const std::string& filename)
: fin_([&]() {
	  auto content = jngl::readConfig(filename);
	  if (content.empty()) {
		  throw std::runtime_error("Could not open replay file " + filename);
	  }
	  jngl::info("Loaded replay file {}, ({} KB)", filename, content.size() / 1024.);
	  return content;
  }()) {
	if (!fin_) {
		throw std::runtime_error("Could not open replay file " + filename);
	}
	int seed, startLevel, startJunk, gameType;
	fin_ >> seed >> startLevel >> startJunk >> gameType;
	if (!fin_) {
		throw std::runtime_error("Error while reading replay header of " + filename);
	}
	oldStartLevel = getOptions().startLevel;
	oldStartJunk = getOptions().startJunks;
	getOptions().startLevel = startLevel;
	getOptions().startJunks = startJunk;
	game_ = std::make_shared<Game>(static_cast<GameType>(gameType), seed, true);
	game_->GetField().setControl(new Control{ std::make_shared<ReplayControl>(fin_) });
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
