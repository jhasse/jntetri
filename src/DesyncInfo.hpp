#pragma once

#include "gui/Button.hpp"

#include <jngl/TextLine.hpp>

/// Info box that will be shown in the field when there are desync issues
class DesyncInfo {
public:
	DesyncInfo();

	void step();
	void draw() const;

private:
	jngl::TextLine text;
	Button cancel;
};
