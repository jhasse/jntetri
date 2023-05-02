#include "Intro.hpp"

#include "constants.hpp"
#include "engine/fade.hpp"
#include "engine/options.hpp"
#include "engine/procedure.hpp"
#include "engine/screen.hpp"
#include "fonts.hpp"
#include "Game.hpp"
#include "menu.hpp"
#include "multiplayermenu.hpp"

#include <jngl.hpp>

Intro::Intro() : blink_(0), finished_(false) {
}

void Intro::draw() const {
	jngl::setFontSize(250);
	jngl::setFontColor(50, 50, 50);
	GetScreen().printCentered("JNTETRI", { 0, -250 });
	jngl::setFontSize(60);

	jngl::pushMatrix();
	jngl::translate(0, 150);
	float percentage;
	if (resizeGraphics_.isFinished(percentage)) {
		finished_ = true;
		jngl::setFontColor(0, 0, 0, blink_ > 255 ? 510 - blink_ : blink_);
		fonts::label(); // TODO: find a better way to load all fonts
		GetScreen().printCentered("Press any key to continue", { 0, 0 });
	} else {
		GetScreen().printCentered(std::to_string(int(percentage)) + " %", { 0, 0 });
	}
	jngl::popMatrix();

	jngl::setFontColor(100, 100, 100, 255);
	jngl::setFontSize(35);
	GetScreen().printCentered(programVersion, { 0, 360 });
	GetScreen().printCentered("https://bixense.itch.io/jntetri", { 0, 430 });
	GetScreen().printCentered("Copyright 2009-2019 bixense.com", { 0, 500 });
}

void Intro::step() {
	blink_ += 4;
	if (blink_ > 2 * 255) {
		blink_ = 0;
	}
#ifdef NDEBUG
	bool anyKeyPressed = jngl::mousePressed() || jngl::keyPressed(jngl::key::Any);
#else
	bool anyKeyPressed = true;
#endif
	if (anyKeyPressed && finished_) {
		bool login = false;
		for (const auto& arg : jngl::getArgs()) {
			if (arg == "--login") {
				login = true;
			}
			if (login) {
				getOptions().lastLoginName = arg;
			}
		}
		if (login) {
			jngl::setWork<MultiplayerMenu>();
		} else {
			jngl::setWork(std::make_shared<Fade>(std::make_shared<Menu>()));
		}
		// jngl::setWork(std::make_shared<Fade>(std::make_shared<ReplayPlayer>("test3.jtr")));
		// jngl::setWork(std::make_shared<Game>(GameType::NORMAL, static_cast<int>(time(0)),
		// false));
	}
}
