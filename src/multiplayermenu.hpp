#pragma once

#include "engine/work.hpp"
#include "gui/button.hpp"

class MultiplayerMenu : public Work {
public:
	MultiplayerMenu();
	void Step();
	void Draw() const;
	void OnBack() const;
private:
	Button back_;
};
