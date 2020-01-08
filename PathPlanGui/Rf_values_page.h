#pragma once

#include <QMainWindow>
#include "MainWindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include "ui_Rf_values.h"

class Rf_values_page : public QMainWindow
{
	Q_OBJECT

public:
	Rf_values_page(QMainWindow *parent = Q_NULLPTR);
	~Rf_values_page();
private slots:
	void save_rf_v();
	void add();
	void show_rf_v();
	void del();
	void add_new();
private:
	Ui::Rf_values ui;
};
