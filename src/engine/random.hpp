#pragma once

#include <ctime>
#include <boost/noncopyable.hpp>
#include <boost/random/linear_congruential.hpp>

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
