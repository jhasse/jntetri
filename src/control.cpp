#include "control.hpp"

#include <jngl.hpp>

Control::~Control()
{
}

bool KeyboardControl::Drop()
{
	return jngl::KeyPressed(jngl::key::Space) || jngl::KeyPressed(jngl::key::Return) || jngl::KeyPressed(jngl::key::WizX);
}

bool KeyboardControl::Down()
{
	return jngl::KeyDown(jngl::key::Down) || jngl::KeyDown(jngl::key::WizDown);
}

bool KeyboardControl::Left()
{
	return jngl::KeyPressed(jngl::key::Left) || jngl::KeyPressed(jngl::key::WizLeft);
}

bool KeyboardControl::Right()
{
	return jngl::KeyPressed(jngl::key::Right) || jngl::KeyPressed(jngl::key::WizRight);
}

bool KeyboardControl::Rotate()
{
	return jngl::KeyPressed(jngl::key::Up) || jngl::KeyPressed(jngl::key::WizUp) || jngl::KeyPressed(jngl::key::WizB);
}

bool KeyboardControl::RotateCounter()
{
	return jngl::KeyPressed(jngl::key::ControlR) || jngl::KeyPressed(jngl::key::WizA);
}

bool WizControl::Drop()
{
	return jngl::KeyPressed(jngl::key::WizX);
}

bool WizControl::Down()
{
	return jngl::KeyDown(jngl::key::WizDown);
}

bool WizControl::Left()
{
	return jngl::KeyPressed(jngl::key::WizLeft);
}

bool WizControl::Right()
{
	return jngl::KeyPressed(jngl::key::WizRight);
}

bool WizControl::Rotate()
{
	return jngl::KeyPressed(jngl::key::WizUp) || jngl::KeyPressed(jngl::key::WizB);
}

bool WizControl::RotateCounter()
{
	return jngl::KeyPressed(jngl::key::WizA);
}

bool WizControlRotated::Drop()
{
	return jngl::KeyPressed(jngl::key::WizA);
}

bool WizControlRotated::Down()
{
	return jngl::KeyDown(jngl::key::WizLeft);
}

bool WizControlRotated::Left()
{
	return jngl::KeyPressed(jngl::key::WizUp);
}

bool WizControlRotated::Right()
{
	return jngl::KeyPressed(jngl::key::WizDown);
}

bool WizControlRotated::Rotate()
{
	return jngl::KeyPressed(jngl::key::WizRight) || jngl::KeyPressed(jngl::key::WizX);
}

bool WizControlRotated::RotateCounter()
{
	return jngl::KeyPressed(jngl::key::WizY);
}
