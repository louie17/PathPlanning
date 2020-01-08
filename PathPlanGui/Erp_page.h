#pragma once

#include <QWidget>
#include <QPushbutton>
#include "MainWindow.h"
#include <QMainWindow>
#include <QVBoxLayout>
#include <QMessageBox>
#include "ui_Erp.h"

class Erp_page : public QMainWindow
{
	Q_OBJECT

public:
	Erp_page(QMainWindow *parent = Q_NULLPTR);
	~Erp_page();
private slots:
	void show_erp();
	void save();
	//void add();

	//void del();

	void add_new_erp();

private:
	Ui::Erp ui;
};
