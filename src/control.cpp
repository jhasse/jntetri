#include "control.hpp"

#include <jngl.hpp>

Control::~Control()
{
}

bool KeyboardControl::Drop()
{
	return (jngl::KeyPressed(jngl::key::Space) || jngl::KeyPressed(jngl::key::Return));
}

bool KeyboardControl::Down()
{
	return jngl::KeyDown(jngl::key::Down);
}

bool KeyboardControl::Left()
{
	return jngl::KeyPressed(jngl::key::Left);
}

bool KeyboardControl::Right()
{
	return jngl::KeyPressed(jngl::key::Right);
}

bool KeyboardControl::Rotate()
{
	return jngl::KeyPressed(jngl::key::Up);
}

bool KeyboardControl::RotateCounter()
{
	return jngl::KeyPressed(jngl::key::ControlR);
}
