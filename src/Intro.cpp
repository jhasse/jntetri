#include "Intro.hpp"

#include "engine/procedure.hpp"
#include "engine/options.hpp"
#include "menu.hpp"
#include "engine/fade.hpp"
#include "engine/screen.hpp"
#include "constants.hpp"

#include <jngl/all.hpp>

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
	if ((jngl::mousePressed() || jngl::keyPressed(jngl::key::Any)) && finished_) {
		jngl::setWork(std::make_shared<Fade>(std::make_shared<Menu>()));
		// jngl::setWork(std::make_shared<Fade>(std::make_shared<ReplayPlayer>("test3.jtr")));
	}
}
