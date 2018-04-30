#ifndef UTILS_HPP
#define UTILS_HPP

#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/transform_width.hpp>

inline std::string decode64(const std::string &val) {
	using namespace boost::archive::iterators;
	using It =
	    transform_width<binary_from_base64<std::string::const_iterator>, 8, 6>;
	return std::string(It{val.begin()}, It{val.end()});
}

inline std::string encode64(const std::string &val) {
	using namespace boost::archive::iterators;
	using It =
	    base64_from_binary<transform_width<std::string::const_iterator, 6, 8>>;
	auto tmp = std::string(It{val.begin()}, It{val.end()});
	return tmp.append((3 - val.size() % 3) % 3, '=');
}

#endif /* end of include guard: UTILS_HPP */
