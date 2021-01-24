#pragma once

#include "control.hpp"

class NetworkControl;

class NetworkRecorder : public Control {
public:
	NetworkRecorder(std::initializer_list<std::shared_ptr<ControlBase>>,
	                std::shared_ptr<NetworkControl> networkControl);

	bool step() override;

private:
	std::shared_ptr<NetworkControl> networkControl;
};
