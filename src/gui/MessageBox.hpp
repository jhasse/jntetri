#pragma once

#include "../engine/work.hpp"
#include "Button.hpp"

#include <jngl.hpp>

class MessageBox : public Work {
public:
	MessageBox(const std::string& message, std::shared_ptr<jngl::Work> nextWork);

	void step() override;
	void draw() const override;

private:
	jngl::Text text;
	std::shared_ptr<jngl::Work> nextWork;
};
