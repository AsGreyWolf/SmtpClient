#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <fstream>
#include <memory>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow() override;

private:
	std::unique_ptr<Ui::MainWindow> ui;
};

#endif /* end of include guard: MAINWINDOW_HPP */
