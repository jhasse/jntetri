#pragma once

#include "engine/work.hpp"
#include "gui/input.hpp"

#include <memory>

class Button;

class MultiplayerMenu : public Work {
public:
	MultiplayerMenu(bool quickLogin);
	void step();
	void draw() const;
	void OnBack() const;
	void OnLogin();
	std::string GetName() const;
	std::string GetPassword() const;
	void login(const std::string& name, const std::string& password);

private:
	std::shared_ptr<Button> back_;
	std::shared_ptr<Button> login_;
	std::shared_ptr<Input> name_;
	std::shared_ptr<Input> password_;
	bool quickLogin;
};
