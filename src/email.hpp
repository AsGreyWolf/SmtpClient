#ifndef EMAIL_HPP
#define EMAIL_HPP

#include "utils.hpp"
#include <string>
#include <vector>

struct email_part {
	std::string mime_type;
	std::string data;
	email_part(std::string mime_type_, const std::string &a)
	    : mime_type(std::move(mime_type_)), data(encode64(a)) {}
};

struct email {
	std::string from;
	std::string to;
	std::string subject;
	std::vector<email_part> parts;
};

#endif /* end of include guard: EMAIL_HPP */
