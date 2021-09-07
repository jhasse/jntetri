#include "DesyncInfo.hpp"

#include "fonts.hpp"

#include <spdlog/spdlog.h>

DesyncInfo::DesyncInfo()
: text(fonts::label(), "no connection"),
  cancel("Disconnect", [this] { spdlog::info("DesyncInfo: Disconnect button clicked."); }) {
	text.setCenter(0, 300);
	cancel.setCenter(0, 500);
}

void DesyncInfo::step() {
	// cancel.step();
}

void DesyncInfo::draw() const {
	text.draw();
	// cancel.draw();
}
