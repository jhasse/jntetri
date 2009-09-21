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
	boost::shared_ptr<Chooser> startLevel_;
	boost::shared_ptr<Chooser> startJunks_;
	boost::shared_ptr<Button> back_;
};
