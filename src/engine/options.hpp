#pragma once

#include "singleton.hpp"

#include <boost/serialization/map.hpp>
#include <boost/serialization/variant.hpp>

class Options : public Singleton<Options> {
public:
	Options();
	void Save() const;
	
	template<class T>
	T Get(const std::string& name) const {
		return boost::get<T>(values_.at(name));
	}
	template<class T>
	void Set(const std::string& name, const T& value) {
		values_[name] = value;
	}
	template<class T>
	void SetFallback(const std::string& name, const T& fallback) {
		auto it = values_.find(name);
		if(it == values_.end()) {
			Set(name, fallback);
		}
	}
private:
	std::map<std::string, boost::variant<int, std::string> > values_;
	std::string filename_;

    friend class boost::serialization::access;
    template<class archive>
    void serialize(archive& ar, const unsigned int version)
    {
        using boost::serialization::make_nvp;
		ar & make_nvp("values", values_);
    }
};

Options& GetOptions(); // Easier access to the singleton
