#include "smtp_client.hpp"
#include "utils.hpp"
#include <algorithm>
#include <iostream>
#include <regex>

const std::regex re_halfresponse{R"R((\d{3})-(.*))R"};
const std::regex re_response{R"R((\d{3})\s(.*))R"};
std::string smtp_client::next_line(int expected) {
	std::string result;
	std::smatch match;
	std::istream is{&inbuf_};
	do {
		result = "";
		size_t read_cnt = boost::asio::read_until(sock_, inbuf_, "\r\n");
		if (read_cnt < 5)
			throw bad_response{"Invalid format"};
		std::copy_n(std::istreambuf_iterator<char>{is}, read_cnt - 2,
		            std::back_inserter(result));
		is.ignore(3);
		std::cout << result << std::endl;
	} while (std::regex_match(result, re_halfresponse));
	if (!std::regex_match(result, match, re_response))
		throw bad_response{"Invalid format " + result};
	int status = stoi(match[0]);
	if (status != expected)
		throw bad_response{result};
	return result;
}
std::string smtp_client::write(const std::string &msg, int expected) {
	std::ostream os{&outbuf_};
	os << msg << "\r\n";
	if (boost::asio::write(sock_, outbuf_) != msg.size() + 2)
		throw std::logic_error{"Unable to write"};

	return next_line(expected);
}

smtp_client::smtp_client(boost::asio::io_service &service,
                         boost::asio::ssl::context &context,
                         const host_info &host, const auth_info &auth)
    : sock_{service, context} {
	boost::asio::ip::tcp::resolver rslv{service};
	auto endpoints = rslv.resolve(host.address, std::to_string(host.port));
	boost::system::error_code ec;
	boost::asio::connect(sock_.lowest_layer(), endpoints, ec);
	if (ec)
		throw std::logic_error{"Unable to connect"};
	sock_.handshake(boost::asio::ssl::stream_base::client, ec);
	if (ec)
		throw std::logic_error{"Invalid handshake"};
	next_line(220);
	write("EHLO " + auth.login);
	write("AUTH LOGIN", 334);
	write(encode64(auth.login), 334);
	write(encode64(auth.password), 235);
}

void smtp_client::send(const email &to_send) {
	write("MAIL FROM: " + to_send.from);
	for (auto &to : to_send.to)
		write("RCPT TO: " + to);
	write("DATA", 354);
	std::string boundary = "__abacabadabacabaeabacabadabacaba__sfdssnglk";
	std::string email = "From: " + to_send.from + "\r\n";
	for (auto &to : to_send.to)
		email += "To: " + to + "\r\n";
	email += "Subject: " + to_send.subject +
	         "\r\n"
	         "MIME-Version: 1.0\r\n"
	         "Content-Type: multipart/mixed; "
	         "boundary=" +
	         boundary +
	         "\r\n"
	         "\r\n";
	for (auto &&part : to_send.parts) {
		email += "--" + boundary +
		         "\r\n"
		         "Content-Type: " +
		         part.mime_type +
		         "\r\n"
		         "Content-Transfer-Encoding: base64\r\n\r\n" +
		         part.data + "\r\n";
	}
	email += "--" + boundary + "--\r\n.";
	write(email);
}
