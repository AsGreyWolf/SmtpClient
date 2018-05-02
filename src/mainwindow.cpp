#include "mainwindow.hpp"

#include "smtp_client.hpp"
#include "ui_untitled.h"
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : ui{std::make_unique<Ui::MainWindow>()} {
	ui->setupUi(this);
	connect(ui->sendButton, &QPushButton::clicked, [this, ui = ui.get()] {
		boost::asio::io_service service;
		boost::asio::ssl::context ssl_context{
		    boost::asio::ssl::context::tlsv12_client};
		smtp_client client{
		    service,
		    ssl_context,
		    {ui->hostEdit->text().toStdString(), ui->portEdit->value()},
		    {ui->loginEdit->text().toStdString(),
		     ui->passwordEdit->text().toStdString()}};
		std::ifstream image{"image.jpg"};
		std::string data(std::istreambuf_iterator<char>{image},
		                 std::istreambuf_iterator<char>{});
		auto qtTo = ui->toEdit->text().split(QRegExp(R"R(,\s*)R"));
		std::vector<std::string> to;
		to.reserve(qtTo.size());
		transform(qtTo.begin(), qtTo.end(), std::back_inserter(to),
		          [](const QString &s) { return s.toStdString(); });
		client.send(
		    {ui->fromEdit->text().toStdString(),
		     to,
		     ui->subjectEdit->text().toStdString(),
		     {{"text/html; charset=UTF-8", ui->messageEdit->toHtml().toStdString()},
		      {"image/jpeg", data}}});
	});
}
MainWindow::~MainWindow() {}
