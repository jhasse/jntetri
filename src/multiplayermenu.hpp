#pragma once

#include "engine/work.hpp"
#include "gui/button.hpp"
#include "gui/input.hpp"

#include <boost/shared_ptr.hpp>

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
	boost::shared_ptr<Button> back_;
	boost::shared_ptr<Button> login_;
	boost::shared_ptr<Input> name_;
	boost::shared_ptr<Input> password_;
};
