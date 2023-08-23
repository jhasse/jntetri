#include "Game.hpp"

#include "engine/screen.hpp"
#include "engine/options.hpp"
#include "engine/procedure.hpp"
#include "gameoverscreen.hpp"
#include "pausemenu.hpp"
#include "ReplayRecorder.hpp"

#include <jngl.hpp>
#include <sstream>
#include <iomanip>
#include <boost/lexical_cast.hpp>

Game::Game(const GameType type, const int seed, const bool replay)
: field_(seed, getOptions().startLevel, getOptions().startJunks), type_(type),
  nextPosition_(field_.GetNextPosition()), oldNextPosition_(nextPosition_), rotateScreen_(false),
  rotateDegree_(0) {
	if (!replay) { // Don't record when we're playing back a replay
		replayRecorder = std::make_unique<ReplayRecorder>(field_, type);
	}
	jngl::setMouseVisible(false);
}

Game::~Game() {
	jngl::setMouseVisible(true);
}

void Game::SetRotateScreen(bool rotateScreen) {
	rotateScreen_ = rotateScreen;
}

void Game::step() {
	StepToRotateScreen();
	if (field_.GameOver()) {
		pauseTime_ = jngl::getTime();
		jngl::setWork(std::make_shared<GameOverScreen>(this));
	} else {
		field_.SetPause(false);
		field_.step();
		if (replayRecorder) {
			replayRecorder->Step();
		}
		if (type_ == GameType::FIFTYLINES && field_.GetLines() >= 50) {
			field_.setGameOver(true);
		}
	}
	nextPosition_ = field_.GetNextPosition();
	oldNextPosition_ = (nextPosition_ - oldNextPosition_) * 0.01 + oldNextPosition_;
	if (pauseTime_ > 0.0001 && !field_.GameOver()) {
		pauseTime_ = 0;
	}
	if (jngl::keyPressed('p') || jngl::keyPressed(jngl::key::Escape)) {
		onQuitEvent(); // Pause
	}
}

void Game::onQuitEvent() {
	static double lastPauseTime = 0;
	if (jngl::getTime() - lastPauseTime > 1) { // Don't allow pausing the game more then one time per second
		lastPauseTime = pauseTime_ = jngl::getTime();
		field_.SetPause(true);
		jngl::setWork(std::make_shared<PauseMenu>(shared_from_this()));
		jngl::cancelQuit();
	}
}

void Game::DrawTime(const int x, const int y) const {
	jngl::print("Time: ", x, y);
	double time = GetTime();
	int minutes = int(time / 60);
	int seconds = int(time - minutes * 60);
	std::stringstream sstream;
	sstream.fill('0');
	sstream << minutes << ":" << std::setw(2) << seconds;
	jngl::print(sstream.str(), 450, y + 100);
}

void Game::StepToRotateScreen() {
	if (rotateScreen_ && rotateDegree_ < 90) {
		rotateDegree_ += (90 - rotateDegree_) * 0.05;
	} else {
		rotateDegree_ *= 0.95;
	}
}

void Game::draw() const {
	jngl::pushMatrix();
	jngl::rotate(rotateDegree_);
	jngl::scale(1 + rotateDegree_ / 270);

	field_.draw();
	if (!rotateScreen_) {
		jngl::setFontColor(0, 0, 0);
		jngl::setFontSize(60);
		jngl::pushMatrix();
		jngl::translate(-600, oldNextPosition_);
		field_.drawNextTetromino();
		jngl::popMatrix();
		if (type_ == GameType::FIFTYLINES) {
			DrawTime(450, -500);
		} else {
			jngl::print("Score: ", 450, -500);
			jngl::print(std::to_string(field_.GetScore()), 450, -400);
			DrawTime(450, 220);
		}
		jngl::print("Level: ", 450, -260);
		jngl::print(std::to_string(field_.GetLevel()), 450, -160);
		jngl::print("Lines: ", 450, -20);
		jngl::print(std::to_string(field_.GetLines()), 450, 80);
	}
	jngl::popMatrix();
}

Field& Game::GetField() {
	return field_;
}

double Game::GetTime() const {
	return field_.getSecondsPlayed();
}

bool Game::GameOverAnimationFinished() const {
	return field_.GameOverAnimationFinished();
}

GameType Game::getType() const {
	return type_;
}

bool Game::gameOver() const {
	return field_.GameOver();
}

bool Game::isReplay() const {
	return replayRecorder == nullptr;
}
