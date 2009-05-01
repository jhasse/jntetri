#include "singleton.hpp"

#include <boost/random/linear_congruential.hpp>

class Random : public Singleton<Random> {
public:
	Random()
	{
		r.seed(static_cast<int>(time(0)));
	}
	int operator() (int n)
	{
		return static_cast<int>(
			static_cast<unsigned long long>(n) * r() / std::numeric_limits<int>::max()
		);
	}
private:
	boost::rand48 r;
};
