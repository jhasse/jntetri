#pragma once

#include "engine/work.hpp"
#include "gui/button.hpp"
#include "gui/chooser.hpp"

#include <vector>

class OptionsMenu : public Work {
public:
	OptionsMenu();
	void step();
	void draw() const;
	void OnBack() const;
private:
	std::shared_ptr<Chooser> startLevel_;
	std::shared_ptr<Chooser> startJunks_;
	std::shared_ptr<Button> back_;
};
