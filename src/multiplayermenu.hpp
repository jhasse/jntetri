#pragma once

#include "engine/work.hpp"
#include "gui/button.hpp"
#include "gui/input.hpp"

class MultiplayerMenu : public Work {
public:
	MultiplayerMenu();
	void Step();
	void Draw() const;
	void OnBack() const;
	void OnLogin();
	std::string GetName() const;
	std::string GetPassword() const;
private:
	Button back_;
	Button login_;
	Input name_;
	Input password_;
};
