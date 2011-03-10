#pragma once

#include <boost/noncopyable.hpp>

namespace boost {
	class rand48;
}

class Random : boost::noncopyable {
public:
	Random();
	~Random();
	int operator() (int);
	int GetSeed() const;
	void SetSeed(int);
private:
	boost::rand48* r_;
	int seed_;
};

int RandomNumber(int);
