#pragma once

#include "engine/work.hpp"
#include "engine/socket.hpp"

#include <boost/shared_ptr.hpp>

class Lobby : public Work {
public:
	Lobby();
	void Step();
	void Draw() const;
private:
	boost::shared_ptr<Socket> socket_;
};
