#include "random.hpp"

#include <boost/random/linear_congruential.hpp>
#include <ctime>

Random::Random() : r_(0) {
	SetSeed(static_cast<int>(time(0)));
}

Random::~Random() {
	if(r_) {
		delete r_;
	}
}

int Random::operator() (int n) {
	int r = static_cast<int>(static_cast<unsigned long long>(n) * (*r_)() / std::numeric_limits<int>::max());
	return r;
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
