#include "debugoverlay.hpp"

#include <jngl/all.hpp>
#include <boost/lexical_cast.hpp>

DebugOverlay::DebugOverlay() {
	fps.setTop(0);
	fps.setLeft(0);
}

void DebugOverlay::step() {
	if (jngl::keyPressed(jngl::key::F1)) {
		visible = !visible;
	}
	if (visible) {
		fpsTemp += jngl::getFPS();
		++fpsCounter;
		if (fpsCounter == 10) {
			fps.setText(std::string("FPS: ") + boost::lexical_cast<std::string>(int(fpsTemp / fpsCounter)));
			fpsCounter = 0;
			fpsTemp = 0;
		}
	}
}

void DebugOverlay::draw() const {
	if (visible) {
		jngl::setColor(100, 255, 100);
		jngl::drawRect(-jngl::getWindowWidth()/2, -jngl::getWindowHeight()/2, fps.getWidth() * jngl::getScaleFactor() + 5, 15);
		fps.draw();
	}
}
