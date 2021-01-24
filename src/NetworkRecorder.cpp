#include "NetworkRecorder.hpp"

#include "NetworkControl.hpp"

#include <cassert>
#include <initializer_list>

NetworkRecorder::NetworkRecorder(std::initializer_list<std::shared_ptr<ControlBase>> l,
                                 std::shared_ptr<NetworkControl> networkControl)
: Control(l), networkControl(std::move(networkControl)) {
}

bool NetworkRecorder::step() {
	bool success = Control::step();
	assert(success);
	networkControl->stepSend(*this);
	return true;
}
