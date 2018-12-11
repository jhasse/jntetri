#include "gameoverscreen.hpp"
#include "engine/procedure.hpp"
#include "engine/screen.hpp"
#include "engine/fade.hpp"
#include "engine/options.hpp"
#include "menu.hpp"
#include "replayplayer.hpp"

#include <jngl/all.hpp>

GameOverScreen::GameOverScreen(Game* game) : game_(game), blink_(0), highscore_(game_->getType()),
input_(new Input(-160, 200)), work_(jngl::getWork()) {
	data_.score = game_->GetField().GetScore();
	data_.time = game_->GetTime();
	input_->setText(getOptions().lastHighscoreName);
	input_->setMaxWidth(400);
	addWidget(input_);
}

bool GameOverScreen::isHighscore() const {
	return !game_->isReplay() && highscore_.isHighscore(data_) &&
	       (game_->getType() == GameType::NORMAL || game_->GetField().GetLines() >= 50);
}

void GameOverScreen::step() {
	game_->GetField().step(); // Show GameOver animation
	game_->StepToRotateScreen();
	if (game_->GameOverAnimationFinished()) {
		if (isHighscore()) {
			StepWidgets();
			if (jngl::keyPressed(jngl::key::Return)) {
				data_.name = input_->getText();
				getOptions().lastHighscoreName = data_.name;
				highscore_.Add(data_);
				highscore_.save();
				auto menu = std::make_shared<Menu>();
				menu->BlinkHighscore(data_);
				jngl::setWork(std::make_shared<Fade>(std::move(menu)));
			}
		} else {
			blink_ += 4;
			if (blink_ > 2 * 255) {
				blink_ = 0;
			}
			if (jngl::mousePressed() || jngl::keyPressed(jngl::key::Any)) {
				jngl::setWork(std::make_shared<Fade>(std::make_shared<Menu>()));
			}
		}
	} else if (jngl::keyPressed(jngl::key::Escape)) {
		while (!game_->GameOverAnimationFinished()) {
			game_->GetField().step();
		}
	}
}

void GameOverScreen::onQuitEvent() {
	while (!game_->GameOverAnimationFinished()) {
		game_->GetField().step();
	}
	if (!isHighscore()) {
		jngl::quit();
	}
}

void GameOverScreen::draw() const {
	game_->draw();
	jngl::setFontSize(80);
	jngl::setFontColor(0, 0, 0);
	GetScreen().PrintCentered("GAMEOVER", 0, -100);
	if (game_->GameOverAnimationFinished()) {
		jngl::setFontSize(50);
		if (isHighscore()) {
			jngl::setFontColor(0, 0, 0);
			GetScreen().PrintCentered("You entered the top!", 0, 50);
			GetScreen().PrintCentered("Enter your name:", 0, 130);
			DrawWidgets();
		} else {
			jngl::setFontColor(0, 0, 0, blink_ > 255 ? 510 - blink_ : blink_);
			GetScreen().PrintCentered("Press any key", 0, 100);
		}
	}
}
