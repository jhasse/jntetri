#include "MessageBox.hpp"

#include "../fonts.hpp"

#include <jngl.hpp>

MessageBox::MessageBox(const std::string& message, std::shared_ptr<jngl::Work> nextWork)
: text(message), nextWork(std::move(nextWork)) {
	auto ok = std::make_shared<Button>("OK", [this]() { jngl::setWork(this->nextWork); });
	ok->setCenter(0, 100);
	addWidget(std::move(ok));
	text.setFont(fonts::label());
	text.setCenter(0, -100);
}

void MessageBox::step() {
	StepWidgets();
}

void MessageBox::draw() const {
	nextWork->draw();
	jngl::drawRect(jngl::modelview().translate(-jngl::getScreenSize() / 2), jngl::getScreenSize(),
	               jngl::Rgba::u8(255, 255, 255, 220));
	jngl::setFontColor(0, 0, 0);
	text.draw();
	DrawWidgets();
}
