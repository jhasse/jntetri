#include "random.hpp"

Random::Random() : r_(0) {
	SetSeed(static_cast<int>(time(0)));
}

Random::~Random() {
	if(r_) {
		delete r_;
	}
}

int Random::operator() (int n) {
	return (*r_)()%n;
}

int Random::GetSeed() const {
	return seed_;
}

void Random::SetSeed(int s) {
	seed_ = s;
	if(r_) {
		delete r_;
	}
	r_ = new boost::rand48(seed_);
}

int RandomNumber(int n) {
	static Random random;
	return random(n);
}
