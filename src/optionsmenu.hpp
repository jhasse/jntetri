#pragma once

#include "engine/work.hpp"
#include "gui/button.hpp"
#include "gui/chooser.hpp"

#include <vector>

class OptionsMenu : public Work {
public:
	OptionsMenu();
	void Step();
	void Draw() const;
	void OnBack() const;
private:
	std::vector<Button> buttons_;
	Chooser startLevel_;
	Chooser startJunks_;
};
