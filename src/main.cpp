#include "smtp_client.hpp"
#include <fstream>

int main() {
	boost::asio::io_service service;
	boost::asio::ssl::context ssl_context{
	    boost::asio::ssl::context::tlsv12_client};
	smtp_client client{service,
	                   ssl_context,
	                   {"smtp.yandex.com", 465},
	                   {"aidar@fattakhov.su", "password"}};
	std::ifstream image{"image.jpg"};
	std::string data(std::istreambuf_iterator<char>{image},
	                 std::istreambuf_iterator<char>{});
	client.send({"aidar@fattakhov.su",
	             "asschool@yandex.ru",
	             "test",
	             {{"text/plain", "message"}, {"image/jpeg", data}}});
}
