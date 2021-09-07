#include "SplitScreen.hpp"

#include "engine/fade.hpp"
#include "engine/screen.hpp"
#include "Field.hpp"
#include "NetworkControl.hpp"
#include "NetworkRecorder.hpp"
#include "lobby.hpp"

#include <jngl/matrix.hpp>
#include <jngl/font.hpp>
#include <memory>
#include <sstream>

SplitScreen::SplitScreen(std::shared_ptr<ControlBase> opponentControl)
: opponentControl(opponentControl) {
	reset();
}

SplitScreen::~SplitScreen() = default;

void SplitScreen::reset() {
	freezeCountdown = 0;
	// const int seed = static_cast<int>(std::time(0));
	const int seed = 123; // FIXME: Exchange seed
	field1.reset(new Field(seed, wins1));
	// FIXME: We shouldn't know about NetworkControl here
	if (const auto networkControl = std::dynamic_pointer_cast<NetworkControl>(opponentControl)) {
		field1->setControl(new NetworkRecorder(
		    { std::make_shared<KeyboardControl>(), std::make_shared<GamepadControl>(0) },
		    networkControl));
		field1->onUserQuit([networkControl]() {
			networkControl->sendQuit();
			jngl::setWork<Fade>(std::make_shared<Lobby>(networkControl->getSocket()));
		});
	}
	field2.reset(new Field(seed, wins2));
	field2->setControl(new Control{ opponentControl });
	field1->setCheckDesync([this]() { return field2->desync(); });
}

void SplitScreen::step() {
	field1->step();
	field2->step();
	if (freezeCountdown > 0) {
		--freezeCountdown;
		if (freezeCountdown == 1) {
			if (const auto networkControl =
			        std::dynamic_pointer_cast<NetworkControl>(opponentControl)) {
				jngl::setWork<Lobby>(networkControl->getSocket());
			} else {
				reset();
			}
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
			if (const auto networkControl =
			        std::dynamic_pointer_cast<NetworkControl>(opponentControl)) {
				networkControl->sendQuit();
			}
		}
		if (field2->GameOver()) {
			++wins1;
			freezeCountdown = 200;
			if (const auto networkControl =
			        std::dynamic_pointer_cast<NetworkControl>(opponentControl)) {
				networkControl->sendQuit();
			}
		}
	}
}

void SplitScreen::draw() const {
	jngl::pushMatrix();
	jngl::translate(-660, 0);
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
