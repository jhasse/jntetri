#include "debugoverlay.hpp"

#include <cmath>
#include <jngl.hpp>

DebugOverlay::DebugOverlay() {
	fps.setTop(1);
	fps.setLeft(0);
}

void DebugOverlay::step() {
	if (jngl::keyPressed(jngl::key::F1)) {
		visible = !visible;
	}
}

void DebugOverlay::draw() const {
	if (visible) {
		fps.setText(std::string("FPS: ") + std::to_string(std::lround(1 / (jngl::getTime() - lastDraw))));
		lastDraw = jngl::getTime();
		jngl::setColor(100, 255, 100, 200);
		jngl::drawRect(-jngl::getScreenSize() / 2.0, { fps.getWidth() + 5, fps.getHeight() });
		jngl::setFontColor(0, 0, 0, 200);
		fps.draw();
	}
}
