#ifndef SMTP_CLIENT
#define SMTP_CLIENT

#include "email.hpp"
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <string>

struct bad_response : std::logic_error {
	bad_response(const std::string &line) : std::logic_error{line} {}
};
struct host_info {
	std::string address;
	uint16_t port;
};
struct auth_info {
	std::string login;
	std::string password;
};

class smtp_client {
	boost::asio::ssl::stream<boost::asio::ip::tcp::socket> sock_;
	boost::asio::streambuf inbuf_;
	boost::asio::streambuf outbuf_;

public:
	smtp_client(boost::asio::io_service &service,
	            boost::asio::ssl::context &context, const host_info &host,
	            const auth_info &auth);
	void send(const email &to_send);

private:
	std::string next_line(int expected = 250);
	std::string write(const std::string &msg, int expected = 250);
};

#endif /* end of include guard: SMTP_CLIENT */
