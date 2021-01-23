#include "SplitScreen.hpp"

#include "engine/screen.hpp"
#include "Field.hpp"
#include "NetworkControl.hpp"

#include <jngl/matrix.hpp>
#include <jngl/font.hpp>
#include <sstream>

SplitScreen::SplitScreen(std::shared_ptr<ControlBase> opponentControl)
: opponentControl(opponentControl) {
	reset();
}

SplitScreen::~SplitScreen() = default;

void SplitScreen::reset() {
	freezeCountdown = 0;
	const int seed = static_cast<int>(std::time(0));
	field1.reset(new Field(seed, wins1));
	field2.reset(new Field(seed, wins2));
	field2->setControl(new Control{ opponentControl });
	field1->setCheckPause([this]() { return field2->getPause(); });
}

void SplitScreen::step() {
	field1->step();
	field2->step();
	if (freezeCountdown > 0) {
		--freezeCountdown;
		if (freezeCountdown == 1) {
			reset();
		}
	} else {
		const auto penalty = [](const int cleared) {
			switch(cleared) {
				case 2: return 1;
				case 3: return 2;
				case 4: return 4;
			}
			return 0;
		};
		field2->addJunk(penalty(field1->getLinesCleared()));
		field1->addJunk(penalty(field2->getLinesCleared()));
		if (field1->GameOver()) {
			++wins2;
			freezeCountdown = 200;
		}
		if (field2->GameOver()) {
			++wins1;
			freezeCountdown = 200;
		}

		// FIXME: We shouldn't know about NetworkControl here
		if (const auto networkControl = dynamic_cast<NetworkControl*>(opponentControl.get())) {
			networkControl->stepSend(field1->getControl());
		}
	}
}

void SplitScreen::draw() const {
	jngl::pushMatrix();
	jngl::translate(-660, -600);
	field1->draw();
	jngl::translate(1320, 0);
	field2->draw();
	jngl::popMatrix();
	jngl::setFontSize(60);
	jngl::pushMatrix();
	jngl::translate(-180, -500);
	field1->drawNextTetromino();
	jngl::translate(330, 0);
	field2->drawNextTetromino();
	jngl::popMatrix();
	std::stringstream sstream;
	sstream << wins1 << " : " << wins2;
	jngl::setFontSize(140);
	GetScreen().printCentered(sstream.str(), { 0, 0 });
}
