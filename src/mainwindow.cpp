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
		client.send({ui->fromEdit->text().toStdString(),
		             ui->toEdit->text().toStdString(),
		             ui->subjectEdit->text().toStdString(),
		             {{"text/html", ui->messageEdit->toHtml().toStdString()},
		              {"image/jpeg", data}}});
	});
}
MainWindow::~MainWindow() {}
